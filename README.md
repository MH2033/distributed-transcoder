# Distributed Video Transcoder

A distributed transcoding tool built with gstreamer, ffmpeg and open MPI for Nvidia jetson platfrom

## Building
 The project can be easily built by using docker build command. Note that the jetpack version of the container and the board must match.

 ```
 sudo docker build . -t "dtranscoder:v0.8.0"
 ```

## Executing

After you successfully built the image you can easily run the application by using mpi run. The code below illustrates an example and also highlights the necessary flags.

```
sudo docker run -v ~/vid.mp4:/vid.mp4 \
				--security-opt seccomp=unconfined -it --net=host --runtime nvidia \
				dtranscoder:v0.8.0 mpirun --allow-run-as-root dtrancoder -i vid.mp4 -e h265
```

## Flags
 - **-h**\
 Prints the help message

 - **-i**\
 Sets the input file path

 - **-e**\
 Sets the encoding shceme. Options: h264, h265, vp8, vp9
