#include "utils.hpp"
#include <iostream>

int main(int argc, char **argv)
{
  if (argc < 2)
  {
    std::cerr << "Usage: child_bin <read_fd>\n";
    return 1;
  }

  int code = 0;
  try
  {
    int read_fd = std::atoi(argv[1]);
    khasnulin::FileDescriptor rd(read_fd);

    char bytes_buffer[khasnulin::message_size] = {};
    size_t result = 1;
    while (result)
    {
      result = khasnulin::captureBytes(rd.getFd(), bytes_buffer, khasnulin::message_size);
      if (result > 0)
      {
        std::cout.write(bytes_buffer, static_cast< std::streamsize >(result));
      }
    }
    std::cout << "\n";
  }
  catch (const std::exception &e)
  {
    code = 1;
    std::cerr << "Child exception: " << e.what() << "\n";
  }
  catch (...)
  {
    code = 1;
    std::cerr << "Child unknown exception\n";
  }
  return code;
}
