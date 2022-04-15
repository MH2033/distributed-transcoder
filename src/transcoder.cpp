#include <mpi.h>
#include <math.h>
#include <thread>
#include <map>
#include "transcoder.hpp"
#include "mpi_conf.hpp"

using namespace std;
static gboolean bus_call(GstBus *bus, GstMessage *msg, gpointer data){
    // printf("New bus msg\n");
    GError *err = NULL;
    gchar *dbg_info = NULL;
    switch (GST_MESSAGE_TYPE (msg)) {
        case GST_MESSAGE_ERROR:
            gst_message_parse_error (msg, &err, &dbg_info);
            g_printerr ("[ERROR] from element %s: %s\n",
                GST_OBJECT_NAME (msg->src), err->message);
            g_printerr ("Debugging info: %s\n", (dbg_info) ? dbg_info : "none");
            g_error_free (err);
            g_free (dbg_info);
            break;

        case GST_MESSAGE_INFO:
            gst_message_parse_info (msg, &err, &dbg_info);
            g_printerr ("[INFO] from element %s: %s\n",
                GST_OBJECT_NAME (msg->src), err->message);
            g_printerr ("Debugging info: %s\n", (dbg_info) ? dbg_info : "none");
            g_error_free (err);
            g_free (dbg_info);
            break;

        case GST_MESSAGE_WARNING:
            gst_message_parse_warning (msg, &err, &dbg_info);
            g_printerr ("[WARNING] from element %s: %s\n",
                GST_OBJECT_NAME (msg->src), err->message);
            g_printerr ("Debugging info: %s\n", (dbg_info) ? dbg_info : "none");
            g_error_free (err);
            g_free (dbg_info);
            break;

        case GST_MESSAGE_STATE_CHANGED:{
            GstState oldState, newState, pendingState;
            gst_message_parse_state_changed(msg, &oldState, &newState, &pendingState);
            g_printerr ("[STATE CHANGE] from element %s: %s -> %s\n",
                GST_OBJECT_NAME (msg->src), gst_element_state_get_name(oldState), gst_element_state_get_name(newState));

            // g_error_free (oldState);
            // g_free(oldState);
            // g_free(newState);
            // g_free(pendingState);
            break;
        }

        case GST_MESSAGE_QOS:{
            guint64 processed, dropped;
            gst_message_parse_qos_stats(msg, NULL, &processed, &dropped);
            g_printerr ("[QOS STAT] from element %s: Processed buffers = %ld, Dropped buffers = %ld\n",
                GST_OBJECT_NAME (msg->src), processed, dropped);

            break;
        }
        case GST_MESSAGE_EOS:{
            /* end-of-stream */
            break;
        }
        default:
            g_printerr("Unkonw message Type on gst bus acknowledged Type: %.0f\n", log2((int)GST_MESSAGE_TYPE (msg)));
    }


}

void transcode(string input_file_name, string output_file_name, EncodeType encode_type){
  cout << "File to transcode: " << input_file_name << endl;
  cout << "Output file: " << output_file_name << endl;

  map<EncodeType, string> encoder_name_map;
  encoder_name_map.insert({EncodeType::H264, "h264"});
  encoder_name_map.insert({EncodeType::H265, "h265"});
  encoder_name_map.insert({EncodeType::VP8, "vp8"});
  encoder_name_map.insert({EncodeType::VP9, "vp9"});
  string muxer = "matroskamux";
  string parser = " ";
  switch(encode_type){
    case EncodeType::H264:
      parser = "! h264parse";
      break;
    
    case EncodeType::H265:
      parser = "! h265parse";
      break;
  }
  
  gst_init(NULL, NULL);
  // auto pipeline = gst_parse_launch("appsrc do-timestamp=true name=src format=4 is-live=true ! queue ! h264parse ! nvv4l2decoder enable-max-performance=true ! nvv4l2h265enc insert-sps-pps=true insert-aud=true insert-vui=true maxperf-enable=true ! h265parse ! appsink name=sink emit-signals=true sync=false", NULL);
  // bitrate=$bitrate preset-level=$preset profile=$profile control-rate=$bitrateType peak-bitrate=$peakBitrate insert-sps-pps=true insert-aud=true insert-vui=true maxperf-enable=true
  string gst_launch_str = "filesrc name=src location=" + input_file_name + " ! queue ! qtdemux name=demux " + muxer + " name=mux ! filesink location=" + output_file_name + " \
                                    demux. ! queue ! h264parse ! nvv4l2decoder enable-max-performance=true ! nvv4l2" + encoder_name_map.at(encode_type) + "enc maxperf-enable=true " + parser + " ! mux. \
                                    demux. ! queue ! mpegaudioparse ! mux.";

  // cout << gst_launch_str << endl;       
  GstElement *pipeline = gst_parse_launch(gst_launch_str.c_str(), NULL);
  GstBus *bus = gst_pipeline_get_bus (GST_PIPELINE (pipeline));

  gst_element_set_state(pipeline, GST_STATE_PLAYING);
  auto msg = gst_bus_timed_pop_filtered (bus, GST_CLOCK_TIME_NONE,
        (GstMessageType)(GST_MESSAGE_ERROR | GST_MESSAGE_EOS));

  gst_element_set_state(pipeline, GST_STATE_NULL);
  gst_object_unref(bus);
  gst_object_unref(pipeline);

}