#pragma once

#include <string>

#ifdef _WIN32
#include <Windows.h>
#endif

namespace WD {
enum eOpenFlags { Create = 1, ReadOnly = 2, ReadWrite = 4 };

class SharedMemory {
public:

    SharedMemory() = default;
    SharedMemory(const char *name, size_t size, int flags);
    ~SharedMemory();

    SharedMemory(const SharedMemory &rhs) = delete;
    SharedMemory(SharedMemory &&rhs) noexcept;
    SharedMemory &operator=(const SharedMemory &rhs) = delete;
    SharedMemory &operator=(SharedMemory &&rhs) noexcept;

    explicit operator bool() const {
#ifdef _WIN32
        return p_buf_ != NULL;
#else
        return p_buf_ != nullptr;
#endif
    }

    const std::string &name() const { return name_; }
    size_t size() const { return size_; }

    void *buf() { return p_buf_; }
private:
    std::string name_;
    size_t size_ = 0;
#ifdef _WIN32
    HANDLE handle_ = NULL;
    LPVOID p_buf_ = NULL;
#else
    bool is_owner_ = false;
    int shm_ = -1;
    void *p_buf_ = nullptr;
#endif
};
}