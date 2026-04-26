#include <QFileInfo>

#include "common.h"
#include "exeexternaltool.h"
#include "tklogger.h"
#include "toolfactory.h"

#include "archiveservice.h"

ArchiveService::ArchiveService()
    : m_p7zTool(ToolFactory::Instance().CreateTool("7zip")),
      m_pMD5Tool(ToolFactory::Instance().CreateTool("md5ForBTF")) {}

ArchiveService::~ArchiveService() {}

void ArchiveService::ClearCache() {
    m_nameList.clear();
    m_pathList.clear();
}

Bit32 ArchiveService::AddFileOrDir(const QString& path) {
    QFileInfo fileInfo(path);
    if (!fileInfo.exists()) {
        return -1;
    }

    m_nameList.append(fileInfo.fileName());
    m_pathList.append(fileInfo.absoluteFilePath());

    return 0;
}

Bit32 ArchiveService::RemoveByIndex(Bit32 index)
{
    if (index < 0 || index >= m_pathList.size()) {
        return -1;
    }
    m_nameList.removeAt(index);
    m_pathList.removeAt(index);
    return 0;
}

bool ArchiveService::ContainsName(const QString& name) const {
    return m_nameList.contains(name);
}

void ArchiveService::SetConfig(const QString& savePath, const QString& format, bool md5Required) {
    m_sSavePath = savePath;
    m_sFormat = format;
    m_bMD5Required = md5Required;
}

Bit32 ArchiveService::Start() {
    SevenZipExternalTool* sevenZipTool = dynamic_cast<SevenZipExternalTool*>(m_p7zTool.get());
    MD5ForBTFTool* md5Tool = dynamic_cast<MD5ForBTFTool*>(m_pMD5Tool.get());

    if (sevenZipTool == nullptr || md5Tool == nullptr) {
        TKLogger::Instance().AddLog(ERROR_LOG, TR("压缩工具初始化失败"));
        return -1;
    }
    sevenZipTool->SetConfiguration(m_pathList, m_sSavePath, m_sFormat);
    if (m_bMD5Required) {
        md5Tool->SetConfiguration(m_sSavePath);
        connect(sevenZipTool->GetProcess(), QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
        md5Tool, &MD5ForBTFTool::OnReadyRunSlot, Qt::UniqueConnection);
    }
    else {
        // 防止执行不必要的链接
        disconnect(sevenZipTool->GetProcess(), QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
        md5Tool, &MD5ForBTFTool::OnReadyRunSlot);
    }
    sevenZipTool->Run();
    return 0;
}
