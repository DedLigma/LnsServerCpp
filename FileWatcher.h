#include <boost/filesystem.hpp>
#include <boost/filesystem/file_status.hpp>
#include <boost/thread.hpp>
#include <iostream>
#include <string>
#include <chrono>

using namespace std::chrono_literals;

inline int watchFile(const std::string &filePath, int changer, std::time_t &lastWriteTime) {
  boost::filesystem::path path(filePath);
  
    std::time_t currentWriteTime = boost::filesystem::last_write_time(path);
    if (currentWriteTime != lastWriteTime) {
      changer *= -1;
      lastWriteTime = currentWriteTime;
  }
  return changer;
}

