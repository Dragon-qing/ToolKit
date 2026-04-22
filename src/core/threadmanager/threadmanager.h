/*
 * @Author: Dragon-qing
 * @Date: 2026-04-20
 * @LastEditors: Dragon-qing
 * @FilePath: \ToolKit\src\core\threadmanager\threadmanager.h
 * @Description: 线程池管理类
 */
#ifndef THREADMANAGER_H
#define THREADMANAGER_H
#include <QThreadPool>

class ThreadManager
{
public:
    static ThreadManager &Instance();
    ~ThreadManager();
    QThreadPool& cpuPool() { return m_cpuPool; } // 返回CPU密集型任务线程池
    QThreadPool& ioPool() { return m_ioPool; } // 返回IO密集型任务线程池

private:
    QThreadPool m_cpuPool; // CPU密集型任务线程池
    QThreadPool m_ioPool; // IO密集型任务线程池

    ThreadManager();

};

class ThreadPoolTask : public QRunnable
{
public:
    ThreadPoolTask(std::function<void()> func) : m_func(func) {}
    ~ThreadPoolTask() override = default;

    void run() override;
private:
    std::function<void()> m_func; // 任务函数
};
#endif