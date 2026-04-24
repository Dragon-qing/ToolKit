#include "renametoolservice.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QHash>
#include <QPointer>
#include <QRegularExpression>
#include <QDateTime>

#include "common.h"
#include "datadef.h"
#include "threadmanager.h"
#include "tklogger.h"

RenameToolService::RenameToolService(QObject* parent) : QObject(parent), m_running(false) {}

RenameToolService::~RenameToolService() {}

void RenameToolService::SetTask(const RenameTaskDTO& task) {
    m_task = task;
    m_lastError.clear();
}

RenameTaskDTO RenameToolService::GetTask() const {
    return m_task;
}

void RenameToolService::Run()
{
    if (m_task.files.isEmpty()) {
        SetError(TR("没有文件需要重命名"));
        emit ErrorSignal(m_lastError);
        return;
    }
    else if (m_task.rules.isEmpty()) {
        SetError(TR("没有定义重命名规则"));
        emit ErrorSignal(m_lastError);
        return;
    }
    
    if (m_task.previewMode) {
        PreviewAsync();
    } else {
        ExecuteAsync();
    }
}

void RenameToolService::PreviewAsync() {
    QPointer<RenameToolService> safeThis = this;
    ThreadManager::Instance().cpuPool().start(new ThreadPoolTask([safeThis]() {
        if (!safeThis) {
            return;
        }
        QVector<PreviewItem> previewItems = safeThis->BuildPreviewItems();
        QVector<RenamePreviewDTO> previewList;
        previewList.reserve(previewItems.size());

        bool hasInvalid = false;
        for (const PreviewItem& item : previewItems) {
            previewList.push_back(item.preview);
            hasInvalid = hasInvalid || !item.preview.valid;
        }

        const QString lastError = safeThis->GetLastError();
        // 通过 invokeMethod 切换回主线程发出信号
        QMetaObject::invokeMethod(safeThis, [safeThis, previewList, hasInvalid, lastError]() {
            if (!safeThis) return;

            if (hasInvalid && !lastError.isEmpty()) {
                emit safeThis->ErrorSignal(lastError);
            }
            emit safeThis->PreviewReadySignal(previewList);
        });
    }));
}

void RenameToolService::ExecuteAsync() {
    if (m_running) {  // 判断当前是否有任务在执行
        SetError(TR("重命名任务正在执行中"));
        emit ErrorSignal(m_lastError);
        return;
    }

    m_running = true;                             // 标志置为ture，表示任务正在执行
    QPointer<RenameToolService> safeThis = this;  // 使用QPointer确保在异步操作中对象的安全访问
    ThreadManager::Instance().cpuPool().start(new ThreadPoolTask([safeThis]() {
        if (!safeThis) {
            return;
        }

        QVector<PreviewItem> previewItems = safeThis->BuildPreviewItems();
        QVector<RenameResultDTO> resultList;
        resultList.reserve(previewItems.size());

        bool hasInvalid = false;
        for (const PreviewItem& item : previewItems) {
            if (!item.preview.valid) {
                hasInvalid = true;
                break;
            }
        }
        QString lastError = safeThis->GetLastError();
        if (hasInvalid) {
            safeThis->SetError(lastError.isEmpty() ? TR("预览校验失败，任务已终止") : lastError);
            lastError = safeThis->GetLastError();
            for (const PreviewItem& item : previewItems) {
                RenameResultDTO result;
                result.originalPath = item.sourcePath;
                result.newPath = item.targetPath;
                result.success = false;
                result.errorMessage = item.preview.valid ? lastError : item.preview.errorMessage;
                resultList.push_back(result);
            }
            safeThis->m_running = false;
            QMetaObject::invokeMethod(safeThis, [safeThis, resultList = std::move(resultList),
                                                 lastError]() {  // 切换回主线程发送信号
                if (!safeThis) return;
                emit safeThis->ErrorSignal(lastError);
                emit safeThis->RenameFinishedSignal(resultList);
            });
            return;
        }

        const int total = previewItems.size();
        bool aborted = false;
        for (int i = 0; i < total; ++i) {
            const PreviewItem& item = previewItems.at(i);
            RenameResultDTO result;
            result.originalPath = item.sourcePath;
            result.newPath = item.targetPath;
            result.success = false;

            if (aborted) {
                result.errorMessage = TR("任务已终止");
                resultList.push_back(result);
                QMetaObject::invokeMethod(safeThis, [safeThis, i, total]() {
                    if (!safeThis) return;
                    emit safeThis->ProgressValueChangedSignal(i + 1, total);
                });
                continue;
            }

            QString normalizedSource = QDir::cleanPath(item.sourcePath);
            QString normalizedTarget = QDir::cleanPath(item.targetPath);
            if (normalizedSource.compare(normalizedTarget, Qt::CaseInsensitive) == 0) {
                result.success = true;
                resultList.push_back(result);
                QMetaObject::invokeMethod(safeThis, [safeThis, i, total]() {
                    if (!safeThis) return;
                    emit safeThis->ProgressValueChangedSignal(i + 1, total);
                });
                continue;
            }

            if (!QFile::rename(item.sourcePath, item.targetPath)) {
                result.errorMessage = TR("重命名失败: %1 -> %2").arg(item.sourcePath, item.targetPath);
                safeThis->SetError(result.errorMessage);
                TKLogger::Instance().AddLog(ERROR_LOG, safeThis->GetLastError());
                aborted = true;
            } else {
                result.success = true;
                TKLogger::Instance().AddLog(INFO_LOG, TR("重命名成功: %1 -> %2").arg(item.sourcePath, item.targetPath));
            }

            resultList.push_back(result);
            QMetaObject::invokeMethod(safeThis, [safeThis, i, total]() {
                if (!safeThis) return;
                emit safeThis->ProgressValueChangedSignal(i + 1, total);
            });
        }

        safeThis->m_running = false;
        if (aborted) {
            QMetaObject::invokeMethod(safeThis, [safeThis]() {  // 显示切换回ui线程发送错误信号
                if (!safeThis) return;
                emit safeThis->ErrorSignal(safeThis->GetLastError());
            });
        }
        QMetaObject::invokeMethod(safeThis, [safeThis,
                                             resultList = std::move(resultList)]() {  // 显示切换回ui线程发送完成信号
            if (!safeThis) return;
            emit safeThis->RenameFinishedSignal(resultList);
        });
    }));
}

bool RenameToolService::IsRunning() const {
    return m_running;
}

QString RenameToolService::GetLastError() const {
    return m_lastError;
}

QVector<RenameToolService::PreviewItem> RenameToolService::BuildPreviewItems() {
    QVector<PreviewItem> previewItems;
    m_lastError.clear();

    QHash<QString, Bit32> generatedNameCounter;  // 用于统计生成的目标文件名（不区分大小写）出现的次数，检测冲突
    Bit32 sequenceIndex = 0;                     // 全局序号索引，针对所有文件递增，供Sequence规则使用

    for (Bit32 i = 0; i < m_task.files.size(); ++i) {
        const FileItemDTO& file = m_task.files.at(i);
        if (!file.selected) {  // 未选中的文件跳过
            continue;
        }

        PreviewItem item;
        item.fileIndex = i;
        item.sourcePath = file.originalPath;
        item.preview.originalName = file.originalName;
        item.preview.newName = file.originalName;

        QString ruleError;
        const QString newFileName = ApplyRulesToFileName(file, sequenceIndex, ruleError);
        if (!ruleError.isEmpty()) {  // 规则应用错误
            item.preview.valid = false;
            item.preview.errorMessage = ruleError;
            SetError(ruleError);
        } else {  // 规则应用成功，继续验证文件名合法性
            item.preview.newName = newFileName;
            QString validationError;
            if (!ValidateFileName(newFileName, validationError)) {  // 文件名验证失败
                item.preview.valid = false;
                item.preview.errorMessage = validationError;
                SetError(validationError);
            }
        }

        const QFileInfo srcInfo(file.originalPath);
        const QString targetDirPath =
            m_task.targetDirectory.isEmpty() ? srcInfo.absolutePath() : m_task.targetDirectory;
        item.targetPath = QDir(targetDirPath).filePath(item.preview.newName);

        if (item.preview.valid) {  // 统计生成的目标文件名（不区分大小写）出现的次数，用于后续检测冲突
            const QString key = QDir::cleanPath(item.targetPath).toLower();
            generatedNameCounter[key] = generatedNameCounter.value(key, 0) + 1;
        }

        previewItems.push_back(item);
        ++sequenceIndex;
    }

    // 第二轮检测目标文件名冲突（包括预览内冲突和与现有文件系统的冲突）
    for (Bit32 i = 0; i < previewItems.size(); ++i) {
        PreviewItem& item = previewItems[i];
        if (!item.preview.valid) {  // 已经无效的项跳过
            continue;
        }

        const QString key = QDir::cleanPath(item.targetPath).toLower();
        if (generatedNameCounter.value(key) > 1) {  // 预览内冲突
            const QString message = TR("目标文件名冲突: %1").arg(item.preview.newName);
            item.preview.valid = false;
            item.preview.errorMessage = message;
            SetError(message);
            continue;
        }

        QFileInfo targetInfo(item.targetPath);
        QFileInfo sourceInfo(item.sourcePath);

        // 目标路径与原路径指向同一位置（可能是大小写不同或者符号链接等情况）
        const bool samePath = QDir::cleanPath(targetInfo.absoluteFilePath())
                                  .compare(QDir::cleanPath(sourceInfo.absoluteFilePath()),
                                           Qt::CaseInsensitive) == 0;

        if (targetInfo.exists() && !samePath) {  // 文件系统中已存在同名文件
            const QString message = TR("目标路径已存在: %1").arg(item.targetPath);
            item.preview.valid = false;
            item.preview.errorMessage = message;
            SetError(message);
        }
    }

    return previewItems;
}

QString RenameToolService::ApplyRulesToFileName(const FileItemDTO& file, Bit32 sequenceIndex,
                                                QString& errorMessage) const {
    errorMessage.clear();

    QString currentName = m_task.applyRulesToFullName ? file.originalName : file.baseName;
    const QString extension = file.extension;
    Bit32 currentSequenceValue = 0;

    for (const auto &rule : m_task.rules) {
        if (!rule.enabled) {
            return currentName;
        }

        switch (rule.type) {
            case RenameRuleType::Replace: {
                const QString resolvedFindText =
                    ResolveReservedFields(rule.findText, file, rule.useRegex);
                QString tmp = "";
                if (m_task.applyRulesToFullName) { // 去除多余的扩展名
                    tmp = ResolveReservedFields(rule.replaceText, file, false);
                    Bit32 dotIndex = tmp.lastIndexOf(".");
                    if (dotIndex != -1) {
                        tmp = tmp.left(dotIndex);
                    }
                } else {
                    tmp = ResolveReservedFields(rule.replaceText, file, false);
                }
                const QString resolvedReplaceText = tmp;
                if (rule.useRegex) {
                    QRegularExpression::PatternOptions options = QRegularExpression::NoPatternOption;
                    if (!rule.caseSensitive) {
                        options |= QRegularExpression::CaseInsensitiveOption;
                    }

                    QRegularExpression reg(resolvedFindText, options);
                    if (!reg.isValid()) {
                        errorMessage = TR("正则表达式无效: %1").arg(reg.errorString());
                        return currentName;
                    }
                    currentName.replace(reg, resolvedReplaceText);
                } else {
                    const Qt::CaseSensitivity sensitivity = rule.caseSensitive ? Qt::CaseSensitive : Qt::CaseInsensitive;
                    currentName.replace(resolvedFindText, resolvedReplaceText, sensitivity);
                }
                break;
            }
            case RenameRuleType::Sequence:
                currentSequenceValue = rule.startIndex + sequenceIndex * rule.step;
                currentName += BuildSequenceText(rule, currentSequenceValue);
                break;
            case RenameRuleType::Custom:
                errorMessage = TR("当前版本暂不支持自定义规则");
                return currentName;
            default:
                break;
        }
    };

    return m_task.applyRulesToFullName ? currentName : currentName + "." + extension;
}

bool RenameToolService::ValidateFileName(const QString& fileName, QString& errorMessage) const {
    if (fileName.trimmed().isEmpty()) {
        errorMessage = TR("目标文件名不能为空");
        return false;
    }

    static const QRegularExpression kInvalidChars(R"([<>:"/\\|?*])");
    if (kInvalidChars.match(fileName).hasMatch()) {
        errorMessage = TR("目标文件名包含非法字符: %1").arg(fileName);
        return false;
    }

    if (fileName.endsWith(' ') || fileName.endsWith('.')) {
        errorMessage = TR("目标文件名不能以空格或点结尾: %1").arg(fileName);
        return false;
    }

    if (fileName.length() > 255) {
        errorMessage = TR("目标文件名长度超出限制: %1").arg(fileName);
        return false;
    }

    return true;
}

QString RenameToolService::BuildSequenceText(const RenameRuleDTO& rule, Bit32 sequenceValue) const {
    const QString defaultSequence = QString("%1").arg(sequenceValue, qMax(0, rule.padding), 10, QChar('0'));
    if (rule.format.isEmpty()) {
        return defaultSequence;
    }

    if (rule.format.contains("%1")) {
        return rule.format.arg(defaultSequence);
    }

    QRegularExpression cFormat(R"(%0?(\d*)d)");
    QRegularExpressionMatch match = cFormat.match(rule.format);
    if (match.hasMatch()) {
        const Bit32 width = match.captured(1).isEmpty() ? 0 : match.captured(1).toInt();
        const QString cFormatted = QString("%1").arg(sequenceValue, width, 10, QChar('0'));
        QString out = rule.format;
        out.replace(match.captured(0), cFormatted);
        return out;
    }

    return defaultSequence;
}

QString RenameToolService::ResolveReservedFields(const QString& text, const FileItemDTO& file, bool forRegexPattern) const                                                  
{
    QString resolved = text;
    const QString nameToken = QStringLiteral("{name}");
    QFileInfo fileInfo(file.originalPath);
    QHash<QString, QString> tokenMap = {
        {"name", file.baseName},
        {"ext", file.extension},
        {"date", fileInfo.birthTime().toString("yyyy-MM-dd")},
        {"time", fileInfo.birthTime().toString("hh-mm-ss")},
    };

    QRegularExpression re(R"(\{(\w+)\})");
    QRegularExpressionMatchIterator it = re.globalMatch(resolved);

    while (it.hasNext())
    {
        auto match = it.next();
        QString key = match.captured(1);

        if (tokenMap.contains(key))
        {
            QString value = tokenMap[key];

            if (forRegexPattern)
                value = QRegularExpression::escape(value);

            resolved.replace(match.captured(0), value);
        }
    }

    return m_task.applyRulesToFullName ? resolved + "." + file.extension : resolved;
}

void RenameToolService::SetError(const QString& errorMessage) {
    if (errorMessage.isEmpty()) {
        return;
    }

    m_lastError = errorMessage;
    TKLogger::Instance().AddLog(ERROR_LOG, m_lastError);
}
