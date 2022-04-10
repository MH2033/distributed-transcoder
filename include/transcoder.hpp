#ifndef TRANSCODER_H
#define TRANSCODER_H

#include <gst/gst.h>
#include <gst/app/gstappsrc.h>
#include <gst/app/app.h>
#include <iostream>

void transcode(std::string input_file_name, std::string output_file_name);
#endif 