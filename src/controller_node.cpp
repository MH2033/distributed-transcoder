#include <iostream>
#include <gst/gst.h>
#include <gst/app/gstappsrc.h>
#include <gst/app/app.h>
#include <mpi.h>
#include <thread>
#include <vector>
#include <fstream>
#include "transcoder.hpp"

using namespace std;
void scatter_file(string path, int num_chunks);
void assemble_files();

void run_controller(string filepath, int cluster_size, EncodeType encode_type){
  scatter_file(filepath, cluster_size);
  string chunk_list_file_name = "./chunks.txt";
  ofstream chunk_list;
  chunk_list.open(chunk_list_file_name, std::ofstream::out | std::ofstream::trunc);
  for(int i = 1; i < cluster_size; i++){
    char chunk_name[1000];
    sprintf(chunk_name, "%s_%05d", filepath.c_str(), i);
    chunk_list << "file '" << string(chunk_name) + ".out" << "'" << endl;
    MPI_Send(chunk_name , strlen(chunk_name), MPI_BYTE, i, 0, MPI_COMM_WORLD);
  }
  chunk_list.close();

  char chunk_name[1000];
  sprintf(chunk_name, "%s_%05d", filepath.c_str(), 0);

  transcode(chunk_name, string(chunk_name) + ".out", encode_type);
  MPI_Barrier(MPI_COMM_WORLD);
  cout << "Barrier reached" << endl;
  //Merge the scattered files
  string file_extention = "mkv";
  string ffmpeg_command = "ffmpeg -f concat -safe 0 -i " + chunk_list_file_name + " -c copy " + filepath + ".out." + file_extention;
  if(system(ffmpeg_command.c_str()) != 0)
    cout << "Failed to assemble files" << endl;
}