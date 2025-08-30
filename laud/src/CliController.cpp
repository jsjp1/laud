#include "CliController.h"

#include <cstdlib>
#include <iostream>

namespace laud {

const int SAMPLE_RATE = 44100;
const int FRAMES_PER_BUFFER = 512;

void CliController::printHelp() const {
  std::cout << "Usage: laud [options]\n";
  std::cout << "  --help             Show this message\n";
  std::cout << "  --record N         Record for N seconds (loop)\n";
  std::cout << "  --connect ip port  Connect to other peer\n";
  std::cout << "  --play             Play recorded audio\n";
}

int CliController::run(int argc, char *argv[]) {
  if (argc <= 1) {
    std::cout << "No arguments provided.\n";
    std::cout << "Try '--help' for usage.\n";
    return EXIT_FAILURE;
  }

  std::string arg1 = argv[1];

  if (arg1 == "--help") {
    printHelp();

    return EXIT_SUCCESS;
  } else if (arg1 == "--record") {
    if (argc <= 2) {
      std::cout << "record option needs N argument.\n";

      return EXIT_FAILURE;
    }
    int record_time = std::atoi(argv[2]);
    runRecord(record_time);
  } else if (arg1 == "--connect") {
    if (argc <= 3) {
      std::cout << "connect option needs 'ip' and 'port'.\n";

      return EXIT_FAILURE;
    }

    const char *ip = argv[2];
    const char *port = argv[3];

    runConnect(ip, port);
  } else if (arg1 == "--play") {
    // TODO
  } else {
    std::cout << "Unknown option: " << arg1 << "\n";
    printHelp();

    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

void CliController::runRecord(int record_time) {
  ChannelType input_channel_type = ChannelType::MONO;
  ChannelType output_channel_type = ChannelType::MONO;

  AudioManager audio_manager(input_channel_type, output_channel_type,
                             SAMPLE_RATE, FRAMES_PER_BUFFER);
  audio_manager.init();

  while (true) {
    auto recorded_data = audio_manager.recordAudio(record_time);
    audio_manager.playRecordedAudio(recorded_data, record_time);

    std::cout << "Press q to quit, Enter to continue...\n";
    std::string input;
    std::getline(std::cin, input);
    if (input == "q")
      break;
  }

  audio_manager.terminate();
}

void CliController::runConnect(const char *ip, const char *port) {
  NetworkManager network_manager;
  network_manager.init();
  network_manager.setupForP2P(ip, port);

  LOG_INFO("Connect to ip and port: ", ip, ":", port);
}

} // namespace laud