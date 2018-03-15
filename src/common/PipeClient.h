#pragma once

#include <string>

#ifdef _WIN32
#include <Windows.h>
#else
#include <sys/un.h>
#endif

namespace WD {
class PipeClient {
public:
    PipeClient() = default;
    explicit PipeClient(const char *name);
    ~PipeClient();

    PipeClient(const PipeClient &rhs) = delete;
    PipeClient(PipeClient &&rhs) noexcept;
    PipeClient &operator=(const PipeClient &rhs) = delete;
    PipeClient &operator=(PipeClient &&rhs) noexcept;

    explicit operator bool() const {
#ifdef _WIN32
        return handle_ != INVALID_HANDLE_VALUE;
#else
        return fd_ != -1;
#endif
    }

    bool SetBlocking(bool b);

    bool Read(void *buf, uint32_t buf_size, uint32_t *bytes_read);
    bool Write(const void *buf, uint32_t size);

private:
    std::string name_;
#ifdef _WIN32
    HANDLE handle_ = INVALID_HANDLE_VALUE;
#else
    int fd_ = -1;
    struct sockaddr_un addr_ = {}, srv_addr_ = {};
#endif
};
}