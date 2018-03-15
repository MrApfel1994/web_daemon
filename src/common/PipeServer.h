#pragma once

#include <string>

#ifdef _WIN32
#include <Windows.h>
#else
#include <sys/un.h>
#endif

namespace WD {
class PipeServer {
public:
    PipeServer() = default;
    explicit PipeServer(const char *name);
    ~PipeServer();

    PipeServer(const PipeServer &rhs) = delete;
    PipeServer(PipeServer &&rhs) noexcept;
    PipeServer &operator=(const PipeServer &rhs) = delete;
    PipeServer &operator=(PipeServer &&rhs) noexcept;

    bool IsAsyncIOComplete();
    bool WaitForEvent(int timeout);

    enum eState { Reading, Writing, Invalid };
    eState state() const {
        return cur_state_;
    }

    uint32_t bytes_transfered() const {
        return bytes_transfered_;
    }
    void reset() {
        bytes_transfered_ = 0;
    }

    bool Connect();

    bool Read(void *buf, uint32_t buf_size);
    bool Write(const void *buf, uint32_t size);

private:
    std::string name_;
    eState cur_state_ = Invalid;
#ifdef _WIN32
    HANDLE handle_ = INVALID_HANDLE_VALUE;
    OVERLAPPED overlap_;
    DWORD pending_io_ = 0;
    DWORD bytes_transfered_ = 0;
    HANDLE event_ = INVALID_HANDLE_VALUE;
#else
    int fd_ = -1;
    struct sockaddr_un addr_ = {}, cl_addr_ = {};
    uint32_t bytes_transfered_ = 0;
    char conn_buf[1] = {};
    void *in_buf_ = nullptr;
    uint32_t in_buf_size_ = 0;
    bool pending_io_ = false;
#endif
};
}