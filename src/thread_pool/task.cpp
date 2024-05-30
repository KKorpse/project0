#include "task.h"

Task::Task(std::function<void()> fnTask) { m_fnTask = fnTask; }

Task::~Task() {}

void Task::Run() { m_fnTask(); }