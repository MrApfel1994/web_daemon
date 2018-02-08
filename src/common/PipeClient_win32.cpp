#include "PipeClient.h"

namespace WD {
namespace PipeClientInternal {
const char PIPE_PREFIX[] = "\\\\.\\pipe\\";
}
}

WD::PipeClient::PipeClient(const char *name) : name_(name), handle_(NULL) {
    std::string _name = PipeClientInternal::PIPE_PREFIX;
    _name += name_;

    handle_ = CreateFile(_name.c_str(), GENERIC_READ | GENERIC_WRITE,
                         FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING,
                         FILE_ATTRIBUTE_NORMAL, NULL);
    if (handle_ == INVALID_HANDLE_VALUE) {
        throw std::runtime_error("Cannot connect to pipe.");
    }

    DWORD open_mode = PIPE_READMODE_MESSAGE;
    BOOL result = SetNamedPipeHandleState(handle_, &open_mode, NULL, NULL);
    if (!result) {
        throw std::runtime_error("Cannot connect to pipe.");
    }
}

WD::PipeClient::~PipeClient() {
    CloseHandle(handle_);
}

WD::PipeClient::PipeClient(WD::PipeClient &&rhs) noexcept : name_(std::move(rhs.name_)) {
    handle_ = rhs.handle_;
    rhs.handle_ = NULL;
}

WD::PipeClient &WD::PipeClient::operator=(WD::PipeClient &&rhs) noexcept {
    CloseHandle(handle_);

    name_ = std::move(rhs.name_);
    handle_ = rhs.handle_;
    rhs.handle_ = NULL;

    return *this;
}

bool WD::PipeClient::SetBlocking(bool b) {
    DWORD open_mode = PIPE_READMODE_MESSAGE | (b ? PIPE_WAIT : PIPE_NOWAIT);
    BOOL result = SetNamedPipeHandleState(handle_, &open_mode, NULL, NULL);
    return result != 0;
}

bool WD::PipeClient::Read(void *buf, uint32_t buf_size, uint32_t *_bytes_read) {
    DWORD bytes_read = 0;
    BOOL result = ReadFile(handle_, buf, buf_size, &bytes_read, NULL);
    *_bytes_read = (uint32_t)bytes_read;
    return result != 0;
}

bool WD::PipeClient::Write(const void *buf, uint32_t size) {
    DWORD bytes_written = 0;
    BOOL result = WriteFile(handle_, buf, size, &bytes_written, NULL);
    if (!result || bytes_written != size) {
        auto err = GetLastError();
        volatile int ii = 0;
    }
    return result != 0 && bytes_written == size;
}