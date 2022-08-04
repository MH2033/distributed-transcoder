#/bin/bash


#encoding params
bitrate=2048000
preset=4
profile=1
bitrateType=0
peakBitrate=$( expr 2 '*' $bitrate)

gst-launch-1.0 -e filesrc location=bbb_sunflower_1080p_30fps_normal.mp4 ! queue ! qtdemux name=demux demux.video_0 ! queue ! h264parse ! queue ! mux.video demux.audio_0 ! queue ! mpegaudioparse !  queue ! mux.audio_0 splitmuxsink location=out_%05d.mp4 max-size-bytes=130000000 muxer=mp4mux name=mux

gst-launch-1.0 filesrc location=out_00000.mp4 ! qtdemux name=demux qtmux name=mux ! filesink location=out_00000_h265.mp4 \
demux. ! queue ! h264parse ! nvv4l2decoder enable-max-performance=true ! nvv4l2h265enc bitrate=$bitrate preset-level=$preset profile=$profile control-rate=$bitrateType peak-bitrate=$peakBitrate insert-sps-pps=true insert-aud=true insert-vui=true maxperf-enable=true ! h265parse ! mux. \
demux. ! queue ! mpegaudioparse ! mux. & PID1=$!

gst-launch-1.0 filesrc location=out_00001.mp4 ! qtdemux name=demux qtmux name=mux ! filesink location=out_00001_h265.mp4 \
demux. ! queue ! h264parse ! nvv4l2decoder enable-max-performance=true ! nvv4l2h265enc bitrate=$bitrate preset-level=$preset profile=$profile control-rate=$bitrateType peak-bitrate=$peakBitrate insert-sps-pps=true insert-aud=true insert-vui=true maxperf-enable=true ! h265parse ! mux. \
demux. ! queue ! mpegaudioparse ! mux. & PID2=$!

wait $PID1
wait $PID2

gst-launch-1.0 filesrc location=bbb_sunflower_1080p_30fps_short.mp4 ! qtdemux name=demux matroskamux name=mux ! filesink location=out_00001_vp8.webm \
demux. ! queue ! h264parse ! nvv4l2decoder enable-max-performance=true ! nvv4l2vp8enc maxperf-enable=true ! mux. \
demux. ! queue ! mpegaudioparse ! mux.