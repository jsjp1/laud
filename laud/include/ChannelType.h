#ifndef __CHANNEL_TYPE_H__
#define __CHANNEL_TYPE_H__

#include <string>

namespace laud {

enum class ChannelType {
  MONO = 1,
  STEREO = 2,
};

inline const std::string channelTypeToString(const ChannelType type) {
  switch (type) {
  case ChannelType::MONO:
    return std::string("MONO");
  case ChannelType::STEREO:
    return std::string("STEREO");
  default:
    return std::string("UNKNOWN");
  }
}

} // namespace laud

#endif