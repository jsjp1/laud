#include "AudioManager.h"
#include "CliController.h"
#include "Logger.h"
#include "NetworkManager.h"

using namespace laud;
int main(int argc, char *argv[]) {
  laud::CliController cli;
  return cli.run(argc, argv);
}