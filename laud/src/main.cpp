#include "CliController.h"

using namespace laud;
int main(int argc, char *argv[]) {
  laud::CliController cli;
  return cli.run(argc, argv);
}