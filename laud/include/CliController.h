#ifndef __CLI_CONTROLLER_H__
#define __CLI_CONTROLLER_H__

#include "AudioManager.h"
#include "Logger.h"
#include "NetworkManager.h"

namespace laud {

class CliController {
public:
  int run(int argc, char *argv[]);

private:
  void printHelp() const;
  void runRecord(int record_time);
  void runConnect(const char *ip, const char *port);
};

} // namespace laud

#endif