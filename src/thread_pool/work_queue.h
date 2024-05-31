#include "work.h"
#include <memory>
#include <queue>
#include <mutex>
#include <condition_variable>

class WorkQueue
{
  private:
    std::queue<std::unique_ptr<Work>> m_queWorks;
    std::mutex m_mtxQueue;

  public:
    WorkQueue();
    ~WorkQueue();

    void AddWork(std::unique_ptr<Work> pWork);

    std::unique_ptr<Work> GetWork();

    void IsEmpty();
    void WorkNum();
};