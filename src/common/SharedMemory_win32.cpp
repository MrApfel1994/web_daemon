#include "SharedMemory.h"

#include <iostream>

WD::SharedMemory::SharedMemory(const char *name, size_t size, int flags) : name_(name) {
    if (flags & Create) {
        DWORD access = (flags & ReadWrite) ? PAGE_READWRITE : PAGE_READONLY;
        handle_ = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, access, 0, (DWORD)size, name_.c_str());
        if (handle_ == NULL) {
            throw std::runtime_error("Cannot create shared memory!");
        }
    } else {
        DWORD access = (flags & ReadOnly) ? FILE_MAP_READ : FILE_MAP_ALL_ACCESS;
        handle_ = OpenFileMapping(access, FALSE, name);
        if (handle_ == NULL) {
            throw std::runtime_error("Cannot open shared memory!");
        }
    }

    {
        DWORD access = (flags & ReadOnly) ? FILE_MAP_READ : FILE_MAP_ALL_ACCESS;
        p_buf_ = MapViewOfFile(handle_, access, 0, 0, size);
        if (p_buf_ == NULL) {
            CloseHandle(handle_);
            throw std::runtime_error("Cannot map shared memory!");
        }
    }
}

WD::SharedMemory::~SharedMemory() {
	UnmapViewOfFile(p_buf_);
	CloseHandle(handle_);
}

WD::SharedMemory::SharedMemory(WD::SharedMemory &&rhs) noexcept : name_(std::move(rhs.name_)) {
    size_ = rhs.size_;
    rhs.size_ = 0;
	handle_ = rhs.handle_;
	rhs.handle_ = NULL;
	p_buf_ = rhs.p_buf_;
	rhs.p_buf_ = NULL;
}

WD::SharedMemory &WD::SharedMemory::operator=(WD::SharedMemory &&rhs) noexcept {
	if (handle_) {
		UnmapViewOfFile(p_buf_);
		CloseHandle(handle_);
	}

	name_ = std::move(name_);
    size_ = rhs.size_;
    rhs.size_ = 0;
	handle_ = rhs.handle_;
	rhs.handle_ = NULL;
	p_buf_ = rhs.p_buf_;
	rhs.p_buf_ = NULL;

    return *this;
}
