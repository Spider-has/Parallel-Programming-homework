#include "utils.hpp"
#include <cerrno>
#include <stdexcept>
#include <system_error>
#include <unistd.h>

namespace khasnulin
{
  void checkError(ssize_t err)
  {
    if (err < 0)
    {
      throw std::system_error(errno, std::generic_category());
    }
  }

  FileDescriptor::FileDescriptor(int fd):
      fd_(fd)
  {
    if (fd < 0)
    {
      throw std::invalid_argument("fd can't be negative");
    }
  }

  FileDescriptor::~FileDescriptor() noexcept
  {
    if (fd_ >= 0)
    {
      close(fd_);
    }
  }

  void FileDescriptor::closeFd()
  {
    if (fd_ >= 0)
    {
      int err = close(fd_);
      checkError(err);
      fd_ = -1;
    }
  }

  int FileDescriptor::getFd() const
  {
    return fd_;
  }

  size_t sendBytes(int wr, const char *str, size_t k)
  {
    size_t r = 0;
    ssize_t err = 0;
    while (r < k)
    {
      err = write(wr, str + r, k - r);
      checkError(err);
      r += err;
    }
    return r;
  }

  size_t captureBytes(int rd, char *str, size_t k)
  {
    size_t r = 0;
    ssize_t err = 0;
    while (r < k)
    {
      err = read(rd, str + r, k - r);
      checkError(err);
      if (err == 0)
      {
        break;
      }
      r += err;
    }
    return r;
  }
} // namespace khasnulin
