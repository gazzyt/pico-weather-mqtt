#include "logger.h"
#include "memory_status.h"
#include <malloc.h>
#include <stdio.h>

void MemoryStatus::PrintMemoryStatus()
{
    LogInfo("Free heap = {}", GetFreeHeap());
}

unsigned int MemoryStatus::GetFreeHeap()
{
    struct mallinfo m = mallinfo();

    return GetTotalHeap() - m.uordblks;
}

unsigned int MemoryStatus::GetTotalHeap()
{
    extern char __StackLimit, __bss_end__;

    return &__StackLimit - &__bss_end__;
}