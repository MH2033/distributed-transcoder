#include <iostream>
#include <fstream>
#include <gst/gst.h>
#include <gst/app/gstappsrc.h>
#include <gst/app/app.h>

using namespace std;

void scatter_file(string path, int num_chunks){
  gst_init(NULL, NULL);
  ifstream file(path, ios::binary | ios::ate);
  int chunk_size = file.tellg() / num_chunks;
  string gst_launch_command = "filesrc location=" + path +  " ! queue ! qtdemux name=demux demux.video_0 ! queue ! h264parse ! queue ! mux.video demux.audio_0 ! queue ! mpegaudioparse !  queue ! mux.audio_0 splitmuxsink location=" + path + "_%05d" + " max-size-bytes=" + to_string(chunk_size) + " muxer=qtmux name=mux";
  auto pipe = gst_parse_launch(gst_launch_command.c_str(), NULL);
  gst_element_set_state (pipe, GST_STATE_PLAYING);

  auto bus = gst_element_get_bus(pipe);

  auto msg = gst_bus_poll(bus, (GstMessageType)(GST_MESSAGE_EOS | GST_MESSAGE_ERROR), -1);
  gst_message_unref (msg);

  gst_element_set_state(pipe, GST_STATE_NULL);
  gst_object_unref(pipe);
  gst_object_unref(bus);
}