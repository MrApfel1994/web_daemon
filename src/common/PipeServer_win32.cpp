#include "PipeServer.h"

namespace WD {
namespace PipeServerInternal {
    const char PIPE_PREFIX[] = "\\\\.\\pipe\\";
}
}

WD::PipeServer::PipeServer(const char *name) : name_(name), handle_(NULL) {
    std::string _name = PipeServerInternal::PIPE_PREFIX;
    _name += name_;

    handle_ = CreateNamedPipe(_name.c_str(), PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED,
                                PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT, 
                                1, 1024, 1024, 50000, NULL);
    if (handle_ == INVALID_HANDLE_VALUE) {
        throw std::runtime_error("Cannot create pipe!");
    }

    event_ = CreateEvent(NULL, TRUE, TRUE, NULL);
    overlap_.hEvent = event_;
    overlap_.Offset = 0;
    overlap_.OffsetHigh = 0;
    pending_io_ = 0;
}

WD::PipeServer::~PipeServer() {
    CloseHandle(handle_);
    CloseHandle(event_);
}

WD::PipeServer::PipeServer(WD::PipeServer &&rhs) noexcept : name_(std::move(rhs.name_)) {
    cur_state_ = rhs.cur_state_;
    handle_ = rhs.handle_;
    rhs.handle_ = NULL;
    event_ = rhs.event_;
    rhs.event_ = INVALID_HANDLE_VALUE;
    overlap_ = rhs.overlap_;
    pending_io_ = rhs.pending_io_;
    rhs.pending_io_ = 0;
}

WD::PipeServer &WD::PipeServer::operator=(WD::PipeServer &&rhs) noexcept {
    CloseHandle(handle_);
    CloseHandle(event_);

    name_ = std::move(rhs.name_);
    cur_state_ = rhs.cur_state_;
    handle_ = rhs.handle_;
    rhs.handle_ = NULL;
    event_ = rhs.event_;
    rhs.event_ = INVALID_HANDLE_VALUE;
    overlap_ = rhs.overlap_;
    pending_io_ = rhs.pending_io_;
    rhs.pending_io_ = 0;

    return *this;
}

bool WD::PipeServer::Connect() {
    BOOL result = ConnectNamedPipe(handle_, &overlap_);
    if (result) {
        return false;
    } else {
        DWORD err = GetLastError();
        if (err == ERROR_IO_PENDING) {
            pending_io_ = 1;
            return true;
        } else if (err == ERROR_PIPE_CONNECTED) {
            pending_io_ = 0;
            return true;
        }
    }
    return false;
}

bool WD::PipeServer::IsAsyncIOComplete() {
    if (pending_io_) {
        DWORD bytes_transfered = 0;
        BOOL result = GetOverlappedResult(handle_, &overlap_, &bytes_transfered, FALSE);
        if (result != 0) {
            bytes_transfered_ = bytes_transfered;
            return true;
        } else {
            return false;
        }
    } else {
        return true;
    }
}

bool WD::PipeServer::WaitForEvent(int timeout) {
    DWORD res = WaitForSingleObject(event_, DWORD(timeout));
    return res == WAIT_OBJECT_0 || res == WAIT_TIMEOUT;
}

bool WD::PipeServer::Read(void *buf, uint32_t buf_size) {
    if (!IsAsyncIOComplete()) return false;

    cur_state_ = Reading;

    DWORD bytes_read = 0;
    BOOL result = ReadFile(handle_, buf, buf_size, &bytes_read, &overlap_);
    DWORD err = GetLastError();
    if (result != 0 && bytes_read != 0) {
        pending_io_ = 0;
        bytes_transfered_ = bytes_read;
        return true;
    } else if (result == 0 && GetLastError() == ERROR_IO_PENDING) {
        pending_io_ = 1;
        return true;
    }
    return false;
}

bool WD::PipeServer::Write(const void *buf, uint32_t size) {
    if (!IsAsyncIOComplete()) return false;

    cur_state_ = Writing;

    DWORD bytes_written = 0;
    BOOL result = WriteFile(handle_, buf, size, &bytes_written, &overlap_);
    if (result != 0 && bytes_written == size) {
        pending_io_ = 0;
        bytes_transfered_ = bytes_written;
        return true;
    } else if (result == 0 && GetLastError() == ERROR_IO_PENDING) {
        pending_io_ = 1;
        return true;
    }
    return false;
}
