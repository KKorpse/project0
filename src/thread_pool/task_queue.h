#include "task.h"
#include <memory>
#include <queue>
#include <mutex>
#include <condition_variable>

class WorkQueue
{
  private:
    std::queue<std::unique_ptr<Task>> m_queWorks;
    std::mutex m_mtxQueue;

  public:
    WorkQueue();
    ~WorkQueue();

    void AddWork(std::unique_ptr<Task> pWork);

    std::unique_ptr<Task> GetWork();

    void IsEmpty();
    void WorkNum();
};