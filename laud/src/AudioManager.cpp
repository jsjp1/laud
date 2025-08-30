#include "AudioManager.h"
#include "ChannelType.h"
#include "Logger.h"

#include <cstdlib>
#include <iostream>
#include <portaudio.h>
#include <vector>

namespace laud {

AudioManager::AudioManager(ChannelType input_channel_type,
                           ChannelType output_channel_type, int sample_rate,
                           int frames_per_buffer)
    : input_channel_type(input_channel_type),
      output_channel_type(output_channel_type), sample_rate(sample_rate),
      frames_per_buffer(frames_per_buffer) {};

AudioManager::~AudioManager() {};

void AudioManager::init() const {
  PaError err = Pa_Initialize();

  if (err != paNoError) {
    LOG_ERROR("PortAudio init error: ", Pa_GetErrorText(err));
    exit(EXIT_FAILURE);
  }

  LOG_DEBUG(
      "\nInput Channel Type: ", channelTypeToString(this->input_channel_type),
      "\nOutput Channel Type: ", channelTypeToString(this->output_channel_type),
      "\nSample rate: ", this->sample_rate,
      "\nFrames per buffer: ", this->frames_per_buffer, "\n");

  LOG_INFO("PortAudio init success.");
}

const std::vector<int16_t> &AudioManager::recordAudio(const int record_time) {
  int total_frames = this->sample_rate * record_time;
  std::vector<int16_t> recorded_data(total_frames);

  PaStream *input_stream;
  PaError err = Pa_OpenDefaultStream(
      &input_stream, static_cast<int>(this->input_channel_type), 0, paInt16,
      this->sample_rate, this->frames_per_buffer, nullptr, nullptr);

  if (err != paNoError) {
    LOG_ERROR("Open input stream error: ", Pa_GetErrorText(err));
    return recorded_data;
  }

  err = Pa_StartStream(input_stream);
  if (err != paNoError) {
    LOG_ERROR("Start input stream error: ", Pa_GetErrorText(err));
    return recorded_data;
  }

  LOG_INFO("Recording ", record_time, " seconds...");

  int frames_recorded = 0;
  while (frames_recorded < total_frames) {
    int frames_to_read =
        std::min(this->frames_per_buffer, total_frames - frames_recorded);
    err = Pa_ReadStream(input_stream, recorded_data.data() + frames_recorded,
                        frames_to_read);
    if (err && err != paInputOverflowed) {
      LOG_ERROR("Read error: ", Pa_GetErrorText(err));
      break;
    }
    frames_recorded += frames_to_read;
  }

  Pa_StopStream(input_stream);
  Pa_CloseStream(input_stream);

  LOG_INFO("Recording finished!");

  return recorded_data;
}

void AudioManager::playRecordedAudio(const std::vector<int16_t> &recorded_data,
                                     const int record_time) {
  PaStream *output_stream;
  PaError err = Pa_OpenDefaultStream(
      &output_stream, 0, static_cast<int>(this->output_channel_type), paInt16,
      this->sample_rate, this->frames_per_buffer, nullptr, nullptr);
  if (err != paNoError) {
    LOG_ERROR("Open output stream error: ", Pa_GetErrorText(err));
    return;
  }

  err = Pa_StartStream(output_stream);
  if (err != paNoError) {
    LOG_ERROR("Start output stream error: ", Pa_GetErrorText(err));
    return;
  }

  LOG_INFO("Playing back...");

  const int total_frames = this->sample_rate * record_time;
  int frames_played = 0;
  while (frames_played < total_frames) {
    int frames_to_write =
        std::min(this->frames_per_buffer, total_frames - frames_played);
    err = Pa_WriteStream(output_stream, recorded_data.data() + frames_played,
                         frames_to_write);

    if (err && err != paOutputUnderflowed) {
      LOG_ERROR("Write Error: ", Pa_GetErrorText(err));
      break;
    }
    frames_played += frames_to_write;
  }

  Pa_StopStream(output_stream);
  Pa_CloseStream(output_stream);

  return;
}

void AudioManager::terminate() const {
  PaError err = Pa_Terminate();
  if (err != paNoError) {
    LOG_ERROR("PortAudio terminate error: ", Pa_GetErrorText(err));
    exit(EXIT_FAILURE);
  }
}

} // namespace laud