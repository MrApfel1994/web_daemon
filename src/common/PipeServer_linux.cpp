#include "PipeServer.h"

#include <stdexcept>

#include <zconf.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

WD::PipeServer::PipeServer(const char *name) {
    name_ = ".";
    name_ += name;

	fd_ = socket(AF_UNIX, SOCK_DGRAM, 0);
	if (fd_ == -1) {
		throw std::runtime_error("Cannot create pipe!");
	}

    addr_ = { 0 };
    addr_.sun_family = AF_UNIX;

    name_ += "-server";
    strcpy(addr_.sun_path, name_.c_str());

    std::remove(name_.c_str());

    int one = 1;
    setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));

    if (bind(fd_, (struct sockaddr *)&addr_, sizeof(addr_)) == -1) {
        close(fd_);
        throw std::runtime_error("Cannot create pipe!");
    }

    if (ioctl(fd_, FIONBIO, (char *)&one) == -1) {
        close(fd_);
        throw std::runtime_error("Cannot create pipe!");
    }

    cl_addr_ = { 0 };
    cl_addr_.sun_family = AF_UNIX;

    std::string cl_name = ".";
    cl_name += name;
    cl_name += "-client";
    strcpy(cl_addr_.sun_path, cl_name.c_str());
}

WD::PipeServer::~PipeServer() {
	if (fd_ != -1) {
        close(fd_);
        unlink(name_.c_str());
    }
}

WD::PipeServer::PipeServer(WD::PipeServer &&rhs) noexcept : name_(std::move(rhs.name_)) {
    cur_state_ = rhs.cur_state_;
	fd_ = rhs.fd_;
    rhs.fd_ = -1;
    addr_ = rhs.addr_;
    rhs.addr_ = { 0 };
    cl_addr_ = rhs.cl_addr_;
    rhs.cl_addr_ = { 0 };
    pending_io_ = rhs.pending_io_;
    rhs.pending_io_ = false;
}

WD::PipeServer &WD::PipeServer::operator=(WD::PipeServer &&rhs) noexcept {
    if (fd_ != -1) {
        close(fd_);
        unlink(name_.c_str());
    }

    name_ = std::move(rhs.name_);
    cur_state_ = rhs.cur_state_;
    fd_ = rhs.fd_;
    rhs.fd_ = -1;
    addr_ = rhs.addr_;
    rhs.addr_ = { 0 };
    cl_addr_ = rhs.cl_addr_;
    rhs.cl_addr_ = { 0 };
    pending_io_ = rhs.pending_io_;
    rhs.pending_io_ = false;

    return *this;
}

bool WD::PipeServer::Connect() {
    if (Read(&conn_buf[0], sizeof(conn_buf))) {
    }
	return true;
}

bool WD::PipeServer::IsAsyncIOComplete() {
	return !pending_io_;
}

bool WD::PipeServer::WaitForEvent(int time_ms) {
    if (!pending_io_) return true;

    if (cur_state_ == Reading) {
        fd_set set;
        FD_ZERO(&set);
        FD_SET(fd_, &set);

        struct timeval timeout = { 0 };
        timeout.tv_usec = time_ms * 1000;

        int rc = select(fd_ + 1, &set, nullptr, nullptr, &timeout);
        if (rc != -1) {
            auto bytes_read = read(fd_, in_buf_, (size_t)in_buf_size_);
            if (bytes_read >= 0) {
                pending_io_ = false;
                bytes_transfered_ = (uint32_t) (bytes_read < 0 ? 0 : bytes_read);
            }
        }
    }

	return false;
}

bool WD::PipeServer::Read(void *buf, uint32_t buf_size) {
    pending_io_ = true;
    cur_state_ = Reading;

    in_buf_ = buf;
    in_buf_size_ = buf_size;

    auto bytes_read = read(fd_, buf, (size_t)buf_size);
    if (bytes_read < 0) {
        return errno == EAGAIN;
    } else {
        pending_io_ = false;
        bytes_transfered_ = (uint32_t)bytes_read;
        return true;
    }
}

bool WD::PipeServer::Write(const void *buf, uint32_t size) {
	cur_state_ = Writing;

	auto bytes_written = sendto(fd_, buf, (size_t)size, 0, (const sockaddr *)&cl_addr_, sizeof(cl_addr_));
    if (bytes_written < 0) {
        return false;
    } else {
        pending_io_ = false;
        bytes_transfered_ = (uint32_t)bytes_written;
        return true;
    }
}
