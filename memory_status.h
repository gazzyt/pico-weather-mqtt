#ifndef MEMORY_STATUS_H
#define MEMORY_STATUS_H

#include <stdint.h>

class MemoryStatus
{
public:
    MemoryStatus() = delete;
    static void PrintMemoryStatus();
    static unsigned int GetFreeHeap();

private:
    static unsigned int GetTotalHeap();
};

#endif
