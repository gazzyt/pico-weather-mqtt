#include "logger.h"
#include "memory_status.h"
#include <malloc.h>

void MemoryStatus::PrintMemoryStatus()
{
    LogInfo("Free heap = {}", GetFreeHeap());
}

unsigned int MemoryStatus::GetFreeHeap()
{
    struct mallinfo malloc_info = mallinfo();

    return GetTotalHeap() - malloc_info.uordblks;
}

unsigned int MemoryStatus::GetTotalHeap()
{
    extern char __StackLimit;
    extern char __bss_end__;

    return &__StackLimit - &__bss_end__;
}