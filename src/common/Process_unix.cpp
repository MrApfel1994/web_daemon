#include "Process.h"

#include <csignal>
#include <cstring>

#include <stdexcept>
#include <string>
#include <vector>

#include <spawn.h>
#include <sys/wait.h>

#ifdef __APPLE__
extern char **environ;
#endif

WD::Process::Process() {
    pid_ = { 0 };
}

WD::Process::Process(const char *name, bool detached_console) {
    pid_ = { 0 };

    if (!Run(name, detached_console)) {
        throw std::runtime_error("Cannot run process!");
    }
}

WD::Process::~Process() {
    Stop();
}

WD::Process::Process(WD::Process &&rhs) noexcept {
    pid_ = rhs.pid_;
    rhs.pid_ = { 0 };
}

WD::Process &WD::Process::operator=(WD::Process &&rhs) noexcept {
    Stop();

    pid_ = rhs.pid_;
    rhs.pid_ = { 0 };

    return *this;
}

bool WD::Process::Run(const char *name, bool detached_console) {
    Stop();

    // slow, but i don't care
    std::string _name, executable;
    std::vector<std::string> args;

    _name = name;

    auto tok = (char *)_name.data();
    tok = strtok(tok, " ");

    executable = tok;

    while (tok) {
        args.emplace_back(tok);
        tok = strtok(nullptr, " ");
    }

    if (args.size() > 16) return false;

    char *argv[16] = { nullptr };

    for (int i = 0; i < (int)args.size(); i++) {
        argv[i] = (char *)args[i].data();
    }

    // check if file exists
    if (access(executable.c_str(), F_OK) == -1) {
        return false;
    }

    int res = posix_spawn(&pid_, executable.c_str(), nullptr, nullptr, argv, environ);
    return res == 0;
}

void WD::Process::Stop() {
    if (pid_) {
        kill(pid_, SIGUSR1);

        pid_ = { 0 };
    }
}

int WD::Process::WaitForCompletion() {
    int status = 0;
    waitpid(pid_, &status, 0);
    return status;
}

void WD::Process::Sleep(int time_ms) {
    usleep((useconds_t)time_ms * 1000);
}

uint32_t WD::Process::GetCurrentProcessID() {
    return 0;
}

bool WD::Process::IsStillRunning(uint32_t id) {
    return true;
}