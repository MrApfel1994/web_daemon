#include "PipeClient.h"

#include <stdexcept>

#include <zconf.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

WD::PipeClient::PipeClient(const char *name) {
    name_ = ".";
    name_ += name;

    fd_ = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (fd_ == -1) {
        throw std::runtime_error("Cannot create pipe!");
    }

    addr_ = { 0 };
    addr_.sun_family = AF_UNIX;

    name_ += "-client";
    strcpy(addr_.sun_path, name_.c_str());

    std::remove(name_.c_str());

    int one = 1;
    setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));

    if (bind(fd_, (struct sockaddr *)&addr_, sizeof(addr_)) == -1) {
        close(fd_);
        unlink(name_.c_str());
        throw std::runtime_error("Cannot create pipe!");
    }

    srv_addr_ = { 0 };
    srv_addr_.sun_family = AF_UNIX;

    std::string srv_name = ".";
    srv_name += name;
    srv_name += "-server";
    strcpy(srv_addr_.sun_path, srv_name.c_str());

    SetBlocking(false);

    /*if (connect(fd_, (const sockaddr *)&srv_addr_, sizeof(srv_addr_)) == -1) {
        close(fd_);
        throw std::runtime_error("Cannot create pipe!");
    }*/

    uint8_t v = 255;
    if (!Write(&v, sizeof(v))) {
        close(fd_);
        unlink(name_.c_str());
        throw std::runtime_error("Cannot create pipe!");
    }

    SetBlocking(true);
}

WD::PipeClient::~PipeClient() {
    if (fd_ != -1) {
        close(fd_);
        unlink(name_.c_str());
    }
}

WD::PipeClient::PipeClient(WD::PipeClient &&rhs) noexcept : name_(std::move(rhs.name_)) {
	fd_ = rhs.fd_;
    rhs.fd_ = -1;
    addr_ = rhs.addr_;
    rhs.addr_ = { 0 };
    srv_addr_ = rhs.srv_addr_;
    rhs.srv_addr_ = { 0 };
}

WD::PipeClient &WD::PipeClient::operator=(WD::PipeClient &&rhs) noexcept {
    if (fd_ != -1) {
        close(fd_);
    }

    name_ = std::move(rhs.name_);
    fd_ = rhs.fd_;
    rhs.fd_ = -1;
    addr_ = rhs.addr_;
    rhs.addr_ = { 0 };
    srv_addr_ = rhs.srv_addr_;
    rhs.srv_addr_ = { 0 };

    return *this;
}

bool WD::PipeClient::SetBlocking(bool b) {
    int val = b ? 0 : 1;
    return ioctl(fd_, FIONBIO, (char *)&val) != -1;
}

bool WD::PipeClient::Read(void *buf, uint32_t buf_size, uint32_t *_bytes_read) {
    auto bytes_read = read(fd_, buf, (size_t)buf_size);
    if (bytes_read < 0) {
        return false;
    } else {
        *_bytes_read = (uint32_t)bytes_read;
        return true;
    }
}

bool WD::PipeClient::Write(const void *buf, uint32_t size) {
    auto bytes_written = sendto(fd_, buf, (size_t)size, 0, (const sockaddr *)&srv_addr_, sizeof(srv_addr_));
    return bytes_written == size;
}