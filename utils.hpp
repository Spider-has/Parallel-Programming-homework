#ifndef UTILS_HPP
#define UTILS_HPP

#include <unistd.h>
namespace khasnulin
{
  constexpr size_t message_size = 256;

  void checkError(ssize_t err);

  class FileDescriptor
  {
  public:
    explicit FileDescriptor(int fd);

    FileDescriptor(const FileDescriptor &) = delete;
    FileDescriptor(FileDescriptor &&) = delete;

    FileDescriptor &operator=(const FileDescriptor &) = delete;
    FileDescriptor &operator=(FileDescriptor &&) = delete;

    ~FileDescriptor() noexcept;

    void closeFd();
    int getFd() const;

  private:
    int fd_;
  };

  size_t sendBytes(int wr, const char *str, size_t k);
  size_t captureBytes(int rd, char *str, size_t k);

}
#endif
