#include <functional>

struct Task
{
	Task(std::function<void(void *vArgs, size_t ulSize)> functionPointer, void *vArgs, size_t ulSize)
	{
		this->functionPointer = functionPointer;
		this->vArgs = vArgs;
		this->ulSize = ulSize;
	}
	std::function<void(void *vArgs, size_t ulSize)> functionPointer;
	void *vArgs;
	size_t ulSize;
};