#include "work.h"

Work::Work(std::function<void()> fnTask) { m_fnTask = fnTask; }

Work::~Work() {}

void Work::Run() { m_fnTask(); }