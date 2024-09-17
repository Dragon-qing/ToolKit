#include "mainwindow.h"

#include <QApplication>
#include <QMutex>
#include <QDateTime>
#include <QFile>
#include <QTextStream>

void OutputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    Q_UNUSED(context);

    static QMutex mutex;
    mutex.lock();

    QString text;
    switch(type)
    {
    case QtDebugMsg:
        text = QString("Debug:");
        break;
    case QtInfoMsg:
        text = QString("Info:");
        break;
    case QtWarningMsg:
        text = QString("Warning:");
        break;

    case QtCriticalMsg:
        text = QString("Critical:");
        break;

    case QtFatalMsg:
        text = QString("Fatal:");
    default:
        break;
    }

//    QString context_info = QString("[%1 %2]:").arg(QString(context.function), context.line);//文件名,所在行数
    QString current_date = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString message = QString("%1 %2 %3").arg(current_date, text, msg);//.arg(context_info).arg(msg);

    QFile file("./log.txt");
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream text_stream(&file);
    text_stream << message << "\r\n";
    file.flush();
    file.close();

    mutex.unlock();
}

int main(int argc, char *argv[])
{
    // qInstallMessageHandler(OutputMessage);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
