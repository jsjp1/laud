#include "AudioManager.h"
#include "Logger.h"

#include <iostream>

const int SAMPLE_RATE = 44100; // 44100Hz, 44100 per seconds
const int FRAMES_PER_BUFFER = 512;

using namespace laud;
int main() {
  ChannelType input_channel_type = ChannelType::MONO;
  ChannelType output_channel_type = ChannelType::MONO;

  AudioManager manager = AudioManager(input_channel_type, output_channel_type,
                                      SAMPLE_RATE, FRAMES_PER_BUFFER);

  manager.init();

  const int record_time = 3;
  const std::vector<int16_t> &data = manager.recordAudio(record_time);
  manager.playRecordedAudio(data, record_time);
  manager.terminate();
}