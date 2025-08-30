#include "AudioManager.h"
#include "Logger.h"
#include "NetworkManager.h"

#include <iostream>

const int SAMPLE_RATE = 44100; // 44100Hz, 44100 per seconds
const int FRAMES_PER_BUFFER = 512;

using namespace laud;
int main(int argc, char *argv[]) {
  if (argc <= 1) {
    std::cout << "No arguments provided.\n";
    std::cout << "Try '--help' for usage.\n";
    return EXIT_FAILURE;
  }

  int record_time = 5; // default record time 5 secs

  std::vector<int16_t> recorded_data;

  // Audio manager setting
  ChannelType input_channel_type = ChannelType::MONO;
  ChannelType output_channel_type = ChannelType::MONO;

  AudioManager audio_manager = AudioManager(
      input_channel_type, output_channel_type, SAMPLE_RATE, FRAMES_PER_BUFFER);
  audio_manager.init();

  std::string arg1 = argv[1];
  if (arg1 == "--help") {
    std::cout << "Usage: laud [options]\n";
    std::cout << "  --help             Show this message\n";
    std::cout << "  --record N         Record for N seconds\n";
    std::cout << "  --connect ip port  Connect to other\n";
    std::cout << "  --play             Play recorded audio\n";

    return EXIT_SUCCESS;
  } else if (arg1 == "--record") {
    if (argc <= 2) {
      std::cout << "record option needs N argument for record time.\n";
      std::cout << "Try '--help' for usage.\n";

      return EXIT_FAILURE;
    }

    const std::string arg2 = argv[2];
    record_time = std::atoi(arg2.c_str());

    recorded_data = audio_manager.recordAudio(record_time);
    audio_manager.playRecordedAudio(recorded_data, record_time);
  } else if (arg1 == "--connect") {
    if (argc <= 3) {
      std::cout << "connect option needs 'ip' and 'port' information.\n";
      std::cout << "Try '--help' for usage.\n";

      return EXIT_FAILURE;
    }

    const std::string ip = argv[2];
    const std::string port = argv[3];
    LOG_DEBUG("Enetr ip and port information: ", ip, ":", port);

    NetworkManager network_manager = NetworkManager();
    network_manager.init();

  } else if (arg1 == "--play") {
    // TODO
  }
}