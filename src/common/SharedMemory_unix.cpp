#include "SharedMemory.h"

#include <iostream>
#include <stdexcept>

#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include <cstring>

WD::SharedMemory::SharedMemory(const char *name, size_t size, int flags) : name_(name), size_(size) {
    int oflags = 0;
    if (flags & Create) oflags |= O_CREAT;
    if (flags & ReadOnly) oflags |= O_RDONLY;
    if (flags & ReadWrite) oflags |= O_RDWR;
    shm_ = shm_open(name_.c_str(), oflags, 0777);
    if (shm_ == -1) {
        printf("%s\n", strerror(errno));
        throw std::runtime_error("Cannot create shared memory!");
    }

    if (flags & Create) {
        if (ftruncate(shm_, size) == -1) {
            close(shm_);
            shm_unlink(name);
            throw std::runtime_error("Cannot create shared memory!");
        }
        is_owner_ = true;
    }

    int access = (flags & ReadWrite) ? (PROT_WRITE | PROT_READ) : PROT_READ;
    p_buf_ = mmap(nullptr, size, access, MAP_SHARED, shm_, 0);
    if (p_buf_ == MAP_FAILED) {
        close(shm_);
        if (is_owner_) {
            shm_unlink(name_.c_str());
        }
        throw std::runtime_error("Cannot create shared memory!");
    }
}

WD::SharedMemory::~SharedMemory() {
	munmap(p_buf_, size_);
    close(shm_);
    if (is_owner_) {
        shm_unlink(name_.c_str());
    }
}

WD::SharedMemory::SharedMemory(WD::SharedMemory &&rhs) noexcept : name_(std::move(rhs.name_)) {
    size_ = rhs.size_;
    rhs.size_ = 0;
	is_owner_ = rhs.is_owner_;
    rhs.is_owner_ = false;
    shm_ = rhs.shm_;
    rhs.shm_ = -1;
    p_buf_ = rhs.p_buf_;
    rhs.p_buf_ = nullptr;
}

WD::SharedMemory &WD::SharedMemory::operator=(WD::SharedMemory &&rhs) noexcept {
    if (shm_ != -1) {
        munmap(p_buf_, size_);
        close(shm_);
        if (is_owner_) {
            shm_unlink(name_.c_str());
        }
    }

	name_ = std::move(rhs.name_);
    size_ = rhs.size_;
    rhs.size_ = 0;
    is_owner_ = rhs.is_owner_;
    rhs.is_owner_ = false;
    shm_ = rhs.shm_;
    rhs.shm_ = -1;
    p_buf_ = rhs.p_buf_;
    rhs.p_buf_ = nullptr;

    return *this;
}
