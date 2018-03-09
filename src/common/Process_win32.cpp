#include "Process.h"

#include <stdexcept>

WD::Process::Process() {
    ZeroMemory(&si_, sizeof(si_));
    si_.cb = sizeof(si_);
    ZeroMemory(&pi_, sizeof(pi_));
}

WD::Process::Process(const char *name, bool detached_console) {
    ZeroMemory(&si_, sizeof(si_));
    si_.cb = sizeof(si_);
    ZeroMemory(&pi_, sizeof(pi_));

    if (!Run(name, detached_console)) {
        throw std::runtime_error("Cannot run process!");
    }
}

WD::Process::~Process() {
    TerminateProcess(pi_.hProcess, 0);
}

WD::Process::Process(WD::Process &&rhs) noexcept {
    si_ = rhs.si_;
    pi_ = rhs.pi_;

    ZeroMemory(&rhs.si_, sizeof(rhs.si_));
    rhs.si_.cb = sizeof(rhs.si_);
    ZeroMemory(&rhs.pi_, sizeof(rhs.pi_));
}

WD::Process &WD::Process::operator=(WD::Process &&rhs) noexcept {
    Stop();

    si_ = rhs.si_;
    pi_ = rhs.pi_;

    ZeroMemory(&rhs.si_, sizeof(rhs.si_));
    rhs.si_.cb = sizeof(rhs.si_);
    ZeroMemory(&rhs.pi_, sizeof(rhs.pi_));

    return *this;
}

bool WD::Process::Run(const char *name, bool detached_console) {
    Stop();

    std::string _name = name;
    BOOL result = CreateProcess(NULL, (char *)_name.data(), NULL, NULL, FALSE, detached_console ? DETACHED_PROCESS : 0, NULL, NULL, &si_, &pi_);
    return result != 0;
}

void WD::Process::Stop() {
    if (pi_.hProcess) {
        TerminateProcess(pi_.hProcess, 0);

        ZeroMemory(&si_, sizeof(si_));
        si_.cb = sizeof(si_);
        ZeroMemory(&pi_, sizeof(pi_));
    }
}

int WD::Process::WaitForCompletion() {
    WaitForSingleObject(pi_.hProcess, INFINITE);
    DWORD exit_code = -1;
    GetExitCodeProcess(pi_.hProcess, &exit_code);
    return (int)exit_code;
}

void WD::Process::Sleep(int time_ms) {
    ::Sleep(time_ms);
}

uint32_t WD::Process::GetCurrentProcessID() {
    return (uint32_t)GetProcessId(GetCurrentProcess());
}

bool WD::Process::IsStillRunning(uint32_t id) {
    HANDLE handle = OpenProcess(PROCESS_QUERY_INFORMATION, 0, (DWORD)id);
    DWORD exit_code = 0;
    if (GetExitCodeProcess(handle, &exit_code)) {
        CloseHandle(handle);
        return exit_code == STILL_ACTIVE;
    } else {
        CloseHandle(handle);
        return false;
    }
}