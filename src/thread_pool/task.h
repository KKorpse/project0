#include <functional>

class Task
{
  private:
	std::function<void()> m_fnTask;

  public:
	Task(std::function<void()> fnTask);
	~Task();

	void Run();
};