#ifndef DATACOMRPT_H
#define DATACOMRPT_H

#include <QString>
#include <QMap>
#include <QVariant>
#include <QThread>
#include <QObject>

#include "common.h"
QT_BEGIN_NAMESPACE
class ReadFileThread;
QT_END_NAMESPACE

enum _Context_Type{
    INVALID = 0,    // 无效
    INFO_PART = 1,  // 文件头部分
    RESULT_PART,    // 结果部分
    CONFIG_PART,    // 配置部分
    DATA_PART       // 数据部分
};

typedef struct ComResult
{
    QString resid;
    QString name;
    QString value;
    QPair<fBit64, fBit64> range;
    QString unit;
    ComResult(){
        name = "";
        value = "";
        range = QPair<fBit64, fBit64>();
        unit = "";
    }
}ComResult;

Q_DECLARE_METATYPE(ComResult)  // 注册到 Qt 的类型系统

class DataComRpt : public QObject
{
    Q_OBJECT
public:
    DataComRpt(QObject *parent = nullptr);
    ~DataComRpt();
    static DataComRpt &Instance();
    void SetPath(QString path);
    ReadFileThread *GetThreadClass();
    Bit32 GetTotalPosNum();
    QVariant GetValue(Bit32 type, QVariant key = QVariant(), QVariant subKey = QVariant());
    QString GetFileName();
    QString GetMask(){ return m_sMask; }
    Bit32 GetResultCount();
private:
    QString m_sPath; // 数据文件路径
    QMap<QString, QVariant> m_infoMap; // 文件头数据
    QMap<QString, QVariant> m_configMap; // 配置数据
    QList<QVector<Bit64>*> m_dataList; // 数据部分
    ReadFileThread *m_pThread;
    QString m_sMask; // 当前数据文件的mask
    QList<ComResult> m_resList;

    void Clear();
};

class ReadFileThread : public QThread
{
    Q_OBJECT
public:
    ReadFileThread(QObject *parent = nullptr);
    void SetConfig(QString path, QMap<QString, QVariant>* info, QMap<QString, QVariant>* config,
                   QList<QVector<Bit64>*>* data, QList<ComResult>* res);

signals:
    void ProcessSignal(int eta);

protected:
    void run() override;

private:
    QString m_sPath; // 数据文件路径
    QMap<QString, QVariant>* m_pInfoMap; // 文件头数据
    QMap<QString, QVariant>* m_pConfigMap; // 配置数据
    QList<QVector<Bit64>*>* m_pDataList; // 数据部分
    QList<ComResult>* m_pResList; // 结果数据

    Bit32 ProcessFileBatch();// 处理解析数据文件(一次读取)
    void ParseLine(Bit32 type, QString tmp);
    void ParseLines(Bit32 type, QStringList list);

    QPair<fBit64, fBit64> Str2Range(QString str);
};

#endif // DATACOMRPT_H
