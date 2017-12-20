#include "test_common.h"

#include <cstring>

#include "../Process.h"
#include "../SharedMemory.h"

void test_shared_memory() {
    require_nothrow(
        auto sm = WD::SharedMemory{ "shared_chunk", (size_t)128, WD::Create | WD::ReadWrite };
        auto pr = WD::Process{ "test_common --child 1" };
        require(pr.WaitForCompletion() == 0);
        require(strcmp((const char *)sm.buf(), "Test String") == 0);
    );
}

void test_shared_memory_child() {
    require_nothrow(
        auto sm = WD::SharedMemory{ "shared_chunk", (size_t)128, WD::ReadWrite };
        strcpy((char *)sm.buf(), "Test String");
    );
}