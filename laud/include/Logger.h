#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <fstream>
#include <iostream>
#include <sstream>

#define LOG_DEBUG(...) Logger::getInstance().log(LogLevel::DEBUG, __VA_ARGS__)
#define LOG_INFO(...) Logger::getInstance().log(LogLevel::INFO, __VA_ARGS__)
#define LOG_WARNING(...)                                                       \
  Logger::getInstance().log(LogLevel::WARNING, __VA_ARGS__)
#define LOG_ERROR(...) Logger::getInstance().log(LogLevel::ERROR, __VA_ARGS__)

namespace laud {

enum class LogLevel { DEBUG, INFO, WARNING, ERROR };

class Logger {
private:
  Logger() = default;
  ~Logger() {
    if (this->file_.is_open())
      this->file_.close();
  }
  Logger(const Logger &) = delete;
  Logger &operator=(const Logger &) = delete;

  std::ofstream file_;
  std::mutex mutex_;

public:
  static Logger &getInstance() {
    static Logger instance;
    return instance;
  }

  void setLogFile(const std::string &filename);
  template <typename... Args> void log(LogLevel level, Args &&...args) {
    std::lock_guard<std::mutex> lock(mutex_);

    std::string levelStr;
    switch (level) {
    case LogLevel::DEBUG:
      levelStr = "[DEBUG   ]";
      break;
    case LogLevel::INFO:
      levelStr = "[INFO    ]";
      break;
    case LogLevel::WARNING:
      levelStr = "[WARNING ]";
      break;
    case LogLevel::ERROR:
      levelStr = "[ERROR   ]";
      break;
    }

    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);

    std::string timestr = std::string(std::ctime(&time));
    timestr.pop_back();

    std::ostringstream oss;
    (oss << ... << args);

    std::string logMsg = timestr + " " + levelStr + " " + oss.str();

    std::cout << logMsg << std::endl;

    if (file_.is_open()) {
      file_ << logMsg << std::endl;
    }
  }
};

} // namespace laud

#endif