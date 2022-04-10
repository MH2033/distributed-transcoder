#ifndef TRANSCODER_H
#define TRANSCODER_H

#include <gst/gst.h>
#include <gst/app/gstappsrc.h>
#include <gst/app/app.h>
#include <iostream>

enum class EncodeType{
  H264,
  H265,
  VP8,
  VP9
};

void transcode(std::string input_file_name, std::string output_file_name, EncodeType encode_type);
#endif 