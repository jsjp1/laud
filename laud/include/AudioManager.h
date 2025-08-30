#ifndef __AUDIO_MANAGER_H__
#define __AUDIO_MANAGER_H__

#include "ChannelType.h"
#include <cstdlib>
#include <vector>

namespace laud {

class AudioManager {
public:
  AudioManager(ChannelType input_channel_type, ChannelType output_channel_type,
               int sample_rate, int frames_per_buffer);
  ~AudioManager();

  void init() const;
  const std::vector<int16_t> &recordAudio(const int record_time);
  void playRecordedAudio(const std::vector<int16_t> &recorded_data,
                         const int record_time);
  void terminate() const;

private:
  ChannelType input_channel_type;
  ChannelType output_channel_type;

  int sample_rate;
  int frames_per_buffer;
};

} // namespace laud

#endif