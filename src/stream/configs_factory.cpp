/*  Copyright (C) 2014-2018 FastoGT. All right reserved.
    This file is part of iptv_cloud.
    iptv_cloud is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    iptv_cloud is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with iptv_cloud.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "stream/configs_factory.h"

#include <map>
#include <string>

#include "config_fields.h"
#include "gst_constants.h"

#include "stream/streams/configs/encoding_config.h"
#include "stream/streams/configs/relay_config.h"

#include "utils/arg_converter.h"

namespace iptv_cloud {
namespace stream {

namespace {

const static size_t kDefaultRestartAttempts = 10;

template <typename T>
void CheckAndSetValue(const utils::ArgsMap& args, const std::string& name, std::map<std::string, T>* map) {
  if (!map) {
    return;
  }

  T result = T();
  if (utils::ArgsGetValue(args, name, &result)) {
    map->insert(std::make_pair(name, result));
  }
}

bool InitVideoEncodersWithArgs(const utils::ArgsMap& config,
                               video_encoders_args_t* video_encoder_args,
                               video_encoders_str_args_t* video_encoder_str_args) {
  if (!video_encoder_args || !video_encoder_str_args) {
    return false;
  }

  video_encoders_args_t video_encoder_args_;
  video_encoders_str_args_t video_encoder_str_args_;

  // nvh264enc
  CheckAndSetValue(config, NV_H264_ENC_PRESET, &video_encoder_args_);

  // mfxh264enc
  CheckAndSetValue(config, MFX_H264_ENC_PRESET, &video_encoder_args_);
  CheckAndSetValue(config, MFX_H264_GOP_SIZE, &video_encoder_args_);

  // x264enc
  CheckAndSetValue(config, X264_ENC_SPEED_PRESET, &video_encoder_args_);
  CheckAndSetValue(config, X264_ENC_THREADS, &video_encoder_args_);
  CheckAndSetValue(config, X264_ENC_TUNE, &video_encoder_args_);
  CheckAndSetValue(config, X264_ENC_KEY_INT_MAX, &video_encoder_args_);
  CheckAndSetValue(config, X264_ENC_VBV_BUF_CAPACITY, &video_encoder_args_);
  CheckAndSetValue(config, X264_ENC_RC_LOOKAHED, &video_encoder_args_);
  CheckAndSetValue(config, X264_ENC_QP_MAX, &video_encoder_args_);
  CheckAndSetValue(config, X264_ENC_PASS, &video_encoder_args_);
  CheckAndSetValue(config, X264_ENC_ME, &video_encoder_args_);
  CheckAndSetValue(config, X264_ENC_SLICED_THREADS, &video_encoder_args_);
  CheckAndSetValue(config, X264_ENC_B_ADAPT, &video_encoder_args_);
  CheckAndSetValue(config, X264_ENC_BYTE_STREAM, &video_encoder_args_);
  CheckAndSetValue(config, X264_ENC_QUANTIZER, &video_encoder_args_);

  CheckAndSetValue(config, X264_ENC_PROFILE, &video_encoder_str_args_);
  CheckAndSetValue(config, X264_ENC_STREAM_FORMAT, &video_encoder_str_args_);
  CheckAndSetValue(config, X264_ENC_OPTION_STRING, &video_encoder_str_args_);

  bool x264_enc_inter = false;
  if (utils::ArgsGetValue(config, X264_ENC_INTERLACED, &x264_enc_inter) && x264_enc_inter) {
    video_encoder_args_[X264_ENC_INTERLACED] = 1;
  }

  bool x264_enc_dct = false;
  if (utils::ArgsGetValue(config, X264_ENC_DCT8X8, &x264_enc_dct) && x264_enc_dct) {
    video_encoder_args_[X264_ENC_DCT8X8] = 1;
  }

  // vaapih264enc
  CheckAndSetValue(config, VAAPI_H264_ENC_KEYFRAME_PERIOD, &video_encoder_args_);
  CheckAndSetValue(config, VAAPI_H264_ENC_TUNE, &video_encoder_args_);
  CheckAndSetValue(config, VAAPI_H264_ENC_MAX_BFRAMES, &video_encoder_args_);
  CheckAndSetValue(config, VAAPI_H264_ENC_NUM_SLICES, &video_encoder_args_);
  CheckAndSetValue(config, VAAPI_H264_ENC_INIT_QP, &video_encoder_args_);
  CheckAndSetValue(config, VAAPI_H264_ENC_MIN_QP, &video_encoder_args_);
  CheckAndSetValue(config, VAAPI_H264_ENC_RATE_CONTROL, &video_encoder_args_);
  CheckAndSetValue(config, VAAPI_H264_ENC_CABAC, &video_encoder_args_);
  CheckAndSetValue(config, VAAPI_H264_ENC_DCT8X8, &video_encoder_args_);
  CheckAndSetValue(config, VAAPI_H264_ENC_CPB_LENGTH, &video_encoder_args_);

  // openh264enc
  CheckAndSetValue(config, OPEN_H264_ENC_MUTLITHREAD, &video_encoder_args_);
  CheckAndSetValue(config, OPEN_H264_ENC_COMPLEXITY, &video_encoder_args_);
  CheckAndSetValue(config, OPEN_H264_ENC_RATE_CONTROL, &video_encoder_args_);
  CheckAndSetValue(config, OPEN_H264_ENC_GOP_SIZE, &video_encoder_args_);

  // eavcenc
  CheckAndSetValue(config, EAVC_ENC_PRESET, &video_encoder_args_);
  CheckAndSetValue(config, EAVC_ENC_PROFILE, &video_encoder_args_);
  CheckAndSetValue(config, EAVC_ENC_PERFORMANCE, &video_encoder_args_);
  CheckAndSetValue(config, EAVC_ENC_BITRATE_MODE, &video_encoder_args_);
  CheckAndSetValue(config, EAVC_ENC_BITRATE_PASS, &video_encoder_args_);
  CheckAndSetValue(config, EAVC_ENC_BITRATE_MAX, &video_encoder_args_);
  CheckAndSetValue(config, EAVC_ENC_VBV_SIZE, &video_encoder_args_);
  CheckAndSetValue(config, EAVC_ENC_PICTURE_MODE, &video_encoder_args_);
  CheckAndSetValue(config, EAVC_ENC_ENTROPY_MODE, &video_encoder_args_);
  CheckAndSetValue(config, EAVC_ENC_GOP_MAX_BCOUNT, &video_encoder_args_);
  CheckAndSetValue(config, EAVC_ENC_GOP_MAX_LENGTH, &video_encoder_args_);
  CheckAndSetValue(config, EAVC_ENC_GOP_MIN_LENGTH, &video_encoder_args_);
  CheckAndSetValue(config, EAVC_ENC_LEVEL, &video_encoder_args_);
  CheckAndSetValue(config, EAVC_ENC_DEBLOCK_MODE, &video_encoder_args_);
  CheckAndSetValue(config, EAVC_ENC_DEBLOCK_ALPHA, &video_encoder_args_);
  CheckAndSetValue(config, EAVC_ENC_DEBLOCK_BETA, &video_encoder_args_);
  CheckAndSetValue(config, EAVC_ENC_INITIAL_DELAY, &video_encoder_args_);
  CheckAndSetValue(config, EAVC_ENC_FIELD_ORDER, &video_encoder_args_);
  bool eabc_enc_gop = false;
  if (utils::ArgsGetValue(config, EAVC_ENC_GOP_ADAPTIVE, &eabc_enc_gop) && eabc_enc_gop) {
    video_encoder_args_[EAVC_ENC_GOP_ADAPTIVE] = 1;
  }

  *video_encoder_args = video_encoder_args_;
  *video_encoder_str_args = video_encoder_str_args_;
  return true;
}

}  // namespace

Config* make_config(const utils::ArgsMap& config) {
  uint8_t stream_type;
  if (!utils::ArgsGetValue(config, TYPE_FIELD, &stream_type)) {
    WARNING_LOG() << "Define " TYPE_FIELD " variable and make it valid.";
    return nullptr;
  }

  input_t input_urls;
  if (!read_input(config, &input_urls)) {
    WARNING_LOG() << "Define " INPUT_FIELD " variable and make it valid.";
    return nullptr;
  }

  bool is_multi_input = input_urls.size() > 1;
  bool is_timeshift_and_rec =
      (stream_type == TIMESHIFT_RECORDER && !is_multi_input) || (stream_type == CATCHUP && !is_multi_input);

  output_t output_urls;
  if (!is_timeshift_and_rec) {
    if (!read_output(config, &output_urls)) {
      WARNING_LOG() << "Define " OUTPUT_FIELD " variable and make it valid.";
      return nullptr;
    }
  }

  size_t max_restart_attempts;
  if (!utils::ArgsGetValue(config, RESTART_ATTEMPTS_FIELD, &max_restart_attempts)) {
    max_restart_attempts = kDefaultRestartAttempts;
  }
  CHECK(max_restart_attempts > 0) << "restart attempts must be grether than 0!";

  Config conf(static_cast<StreamType>(stream_type), max_restart_attempts, input_urls, output_urls);

  time_t ttl_sec;
  if (utils::ArgsGetValue(config, AUTO_EXIT_TIME_FIELD, &ttl_sec)) {
    conf.SetTimeToLigeStream(ttl_sec);
  }

  streams::AudioVideoConfig aconf(conf);
  bool have_video;
  if (utils::ArgsGetValue(config, HAVE_VIDEO_FIELD, &have_video)) {
    aconf.SetHaveVideo(have_video);
  }

  bool have_audio;
  if (utils::ArgsGetValue(config, HAVE_AUDIO_FIELD, &have_audio)) {
    aconf.SetHaveAudio(have_audio);
  }

  int audio_select;
  if (utils::ArgsGetValue(config, AUDIO_SELECT_FIELD, &audio_select)) {
    aconf.SetAudioSelect(audio_select);
  }

  bool loop;
  if (utils::ArgsGetValue(config, LOOP_FIELD, &loop)) {
    aconf.SetLoop(loop);
  }

  if (stream_type == SCREEN) {
    return new streams::AudioVideoConfig(aconf);
  } else if (stream_type == RELAY || stream_type == TIMESHIFT_PLAYER) {
    streams::RelayConfig* rconfig = new streams::RelayConfig(aconf);

    std::string video_parser;
    if (utils::ArgsGetValue(config, VIDEO_PARSER_FIELD, &video_parser)) {
      rconfig->SetVideoParser(video_parser);
    }
    std::string audio_parser;
    if (utils::ArgsGetValue(config, AUDIO_PARSER_FIELD, &audio_parser)) {
      rconfig->SetAudioParser(audio_parser);
    }

    return rconfig;
  } else if (stream_type == ENCODE) {
    streams::EncodingConfig* econfig = new streams::EncodingConfig(aconf);
    bool deinterlace;
    if (utils::ArgsGetValue(config, DEINTERLACE_FIELD, &deinterlace)) {
      econfig->SetDeinterlace(deinterlace);
    }
    int frame_rate;
    if (utils::ArgsGetValue(config, FRAME_RATE_FIELD, &frame_rate)) {
      econfig->SetFrameRate(frame_rate);
    }

    double volume;
    if (utils::ArgsGetValue(config, VOLUME_FIELD, &volume)) {
      econfig->SetVolume(volume);
    }

    std::string video_codec;
    if (utils::ArgsGetValue(config, VIDEO_CODEC_FIELD, &video_codec)) {
      econfig->SetVideoEncoder(video_codec);
    }
    std::string audio_codec;
    if (utils::ArgsGetValue(config, AUDIO_CODEC_FIELD, &audio_codec)) {
      econfig->SetAudioEncoder(audio_codec);
    }

    int audio_channels;
    if (utils::ArgsGetValue(config, AUDIO_CHANNELS_FIELD, &audio_channels)) {
      econfig->SetAudioChannelsCount(audio_channels);
    }

    common::draw::Size size;
    if (utils::ArgsGetValue(config, SIZE_FIELD, &size)) {
      econfig->SetSize(size);
    }

    int v_bitrate;
    if (utils::ArgsGetValue(config, VIDEO_BIT_RATE_FIELD, &v_bitrate)) {
      econfig->SetVideoBitrate(v_bitrate);
    }

    int a_bitrate;
    if (utils::ArgsGetValue(config, AUDIO_BIT_RATE_FIELD, &a_bitrate)) {
      econfig->SetAudioBitrate(a_bitrate);
    }

    Logo logo;
    if (utils::ArgsGetValue(config, LOGO_FIELD, &logo)) {
      econfig->SetLogo(logo);
    }

    common::media::Rational rat;
    if (utils::ArgsGetValue(config, ASPECT_RATIO_FIELD, &rat)) {
      econfig->SetAspectRatio(rat);
    }
    decklink_video_mode_t decl_vm;
    if (utils::ArgsGetValue(config, DECKLINK_VIDEO_MODE_FILELD, &decl_vm)) {
      econfig->SetDecklinkMode(decl_vm);
    }

    video_encoders_args_t video_encoder_args;
    video_encoders_str_args_t video_encoder_str_args;
    if (InitVideoEncodersWithArgs(config, &video_encoder_args, &video_encoder_str_args)) {
      econfig->SetVideoEncoderArgs(video_encoder_args);
      econfig->SetVideoEncoderStrArgs(video_encoder_str_args);
    }

    return econfig;
  } else if (stream_type == TIMESHIFT_RECORDER || stream_type == CATCHUP) {
    streams::RelayConfig rel(aconf);

    std::string video_parser;
    if (utils::ArgsGetValue(config, VIDEO_PARSER_FIELD, &video_parser)) {
      rel.SetVideoParser(video_parser);
    }
    std::string audio_parser;
    if (utils::ArgsGetValue(config, AUDIO_PARSER_FIELD, &audio_parser)) {
      rel.SetAudioParser(audio_parser);
    }

    streams::TimeshiftConfig* tconf = new streams::TimeshiftConfig(rel);
    if (stream_type == TIMESHIFT_RECORDER || stream_type == CATCHUP) {
      time_t timeshift_chunk_duration;
      if (utils::ArgsGetValue(config, TIMESHIFT_CHUNK_DURATION_FIELD, &timeshift_chunk_duration)) {
        tconf->SetTimeShiftChunkDuration(timeshift_chunk_duration);
      }
      CHECK(tconf->GetTimeShiftChunkDuration()) << "Avoid division by zero";
    }

    return tconf;
  }

  NOTREACHED() << "Unhandled stream type: " << stream_type;
  return nullptr;
}

}  // namespace stream
}  // namespace iptv_cloud
