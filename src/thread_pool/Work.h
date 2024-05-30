#include <functional>

class Work
{
  private:
	std::function<void()> m_fnTask;

  public:
	Work(std::function<void()> fnTask);
	~Work();

	void Run();
};