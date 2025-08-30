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
  int totalFrames = this->sample_rate * record_time;
  std::vector<int16_t> recordedData(totalFrames);

  PaStream *inputStream;
  PaError err = Pa_OpenDefaultStream(
      &inputStream, static_cast<int>(this->input_channel_type), 0, paInt16,
      this->sample_rate, this->frames_per_buffer, nullptr, nullptr);

  if (err != paNoError) {
    LOG_ERROR("Open input stream error: ", Pa_GetErrorText(err));
    return recordedData;
  }

  err = Pa_StartStream(inputStream);
  if (err != paNoError) {
    LOG_ERROR("Start input stream error: ", Pa_GetErrorText(err));
    return recordedData;
  }

  LOG_INFO("Recording ", record_time, " seconds...");

  int framesRecorded = 0;
  while (framesRecorded < totalFrames) {
    int framesToRead =
        std::min(this->frames_per_buffer, totalFrames - framesRecorded);
    err = Pa_ReadStream(inputStream, recordedData.data() + framesRecorded,
                        framesToRead);
    if (err && err != paInputOverflowed) {
      LOG_ERROR("Read error: ", Pa_GetErrorText(err));
      break;
    }
    framesRecorded += framesToRead;
  }

  Pa_StopStream(inputStream);
  Pa_CloseStream(inputStream);

  LOG_INFO("Recording finished!");

  return recordedData;
}

void AudioManager::playRecordedAudio(const std::vector<int16_t> &recorded_data,
                                     const int record_time) {
  PaStream *outputStream;
  PaError err = Pa_OpenDefaultStream(
      &outputStream, 0, static_cast<int>(this->output_channel_type), paInt16,
      this->sample_rate, this->frames_per_buffer, nullptr, nullptr);
  if (err != paNoError) {
    LOG_ERROR("Open output stream error: ", Pa_GetErrorText(err));
    return;
  }

  err = Pa_StartStream(outputStream);
  if (err != paNoError) {
    LOG_ERROR("Start output stream error: ", Pa_GetErrorText(err));
    return;
  }

  LOG_INFO("Playing back...");

  const int total_frames = this->sample_rate * record_time;
  int framesPlayed = 0;
  while (framesPlayed < total_frames) {
    int framesToWrite =
        std::min(this->frames_per_buffer, total_frames - framesPlayed);
    err = Pa_WriteStream(outputStream, recorded_data.data() + framesPlayed,
                         framesToWrite);

    if (err && err != paOutputUnderflowed) {
      LOG_ERROR("Write Error: ", Pa_GetErrorText(err));
      break;
    }
    framesPlayed += framesToWrite;
  }

  Pa_StopStream(outputStream);
  Pa_CloseStream(outputStream);

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