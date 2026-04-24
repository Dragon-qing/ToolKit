#include "TKLogger.h"

#include "threadmanager.h"

ThreadManager::ThreadManager()
{
    int ideal = QThread::idealThreadCount();

    m_cpuPool.setMaxThreadCount(qMax(2, ideal - 1));     // CPU任务
    m_ioPool.setMaxThreadCount(ideal * 2);                // IO任务

}

ThreadManager::~ThreadManager()
{
    m_cpuPool.waitForDone();
    m_ioPool.waitForDone();
}

ThreadManager &ThreadManager::Instance()
{
    static ThreadManager m_data;
    return m_data;
}

void ThreadPoolTask::run()
{
    if (!m_func)
        return;
    try {
        m_func();
    } catch (const std::exception &e) {
        TKLogger::Instance().AddLog(ERROR_LOG, QString("ThreadPoolTask exception: %1").arg(e.what()));
    } catch (...) {
        TKLogger::Instance().AddLog(ERROR_LOG, "ThreadPoolTask unknown exception");
    }
}