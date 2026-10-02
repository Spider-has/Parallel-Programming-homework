#include "utils.hpp"
#include <csignal>
#include <iostream>
#include <sys/wait.h>

int main()
{
  signal(SIGPIPE, SIG_IGN);

  int pps[2] = {};
  int code = 0;

  try
  {
    ssize_t err = pipe(pps);
    khasnulin::checkError(err);

    khasnulin::FileDescriptor rd(pps[0]);
    khasnulin::FileDescriptor wr(pps[1]);

    pid_t pid = fork();
    khasnulin::checkError(pid);
    if (!pid)
    {
      wr.closeFd();

      char fd_str[100] = {};
      sprintf(fd_str, "%d", rd.getFd());

      execl("./child_bin", "child_bin", fd_str, nullptr);

      khasnulin::checkError(-1);
    }

    rd.closeFd();

    while (std::cin)
    {
      char bytes_buffer[khasnulin::message_size] = {0};
      std::cin.read(bytes_buffer, khasnulin::message_size);
      size_t readed = static_cast< size_t >(std::cin.gcount());

      if (readed > 0)
      {
        khasnulin::sendBytes(wr.getFd(), bytes_buffer, readed);
      }
    }

    wr.closeFd();

    waitpid(pid, 0, 0);
  }
  catch (const std::exception &e)
  {
    code = 1;
    std::cerr << "Parent exception: " << e.what() << "\n";
  }
  catch (...)
  {
    code = 1;
    std::cerr << "Parent unknown exception\n";
  }
  return code;
}
