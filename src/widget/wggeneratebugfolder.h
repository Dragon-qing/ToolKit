#ifndef WGGENERATEBUGFOLDER_H
#define WGGENERATEBUGFOLDER_H

#include <QWidget>

namespace Ui {
class WgGenerateBugFolder;
}

class WgGenerateBugFolder : public QWidget
{
    Q_OBJECT

public:
    explicit WgGenerateBugFolder(QWidget *parent = nullptr);
    ~WgGenerateBugFolder();



private:
    Ui::WgGenerateBugFolder *ui;
    QString m_sClassName;
    QString m_sSrcPath;
    QString m_sStatementPath;
    QString m_sDirPath;
    QVector<QString> *m_pClassNameVector;
    bool m_bIsSaveUI;

    bool CheckFolder(QString path);
    void ErrorMessage(const char *message);
    void DataCheck();

private slots:
    void BtnSrcPathClicked();
    void BtnStatementClicked();
    void BtnDirPathClicked();
    void BtnStartClicked();

    void CkbIsSaveUI(int arg1);

    // QWidget interface
protected:

};

#endif // WGGENERATEBUGFOLDER_H
