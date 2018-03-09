#pragma once

#include <cstdint>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

namespace WD {
class Process {
public:
    Process();
    explicit Process(const char *name, bool detached_console = false);
    ~Process();

    Process(const Process &rhs) = delete;
    Process(Process &&rhs) noexcept;
    Process &operator=(const Process &rhs) = delete;
    Process &operator=(Process &&rhs) noexcept;

    bool Run(const char *name, bool detached_console = false);
    void Stop();

    int WaitForCompletion();

    static void Sleep(int time_ms);

    static uint32_t GetCurrentProcessID();
    static bool IsStillRunning(uint32_t id);
private:
#ifdef _WIN32
    STARTUPINFO si_;
    PROCESS_INFORMATION pi_;
#else
    pid_t pid_;
#endif
};
}