#include "Logger.h"

#include <iostream>

namespace laud {

void Logger::setLogFile(const std::string &filename) {
  file_.open(filename, std::ios::app);
}

} // namespace laud