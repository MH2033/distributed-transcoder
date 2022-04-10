#include <iostream>
#include <gst/gst.h>
#include <gst/app/gstappsrc.h>
#include <gst/app/app.h>
#include <mpi.h>
#include <thread>
#include <vector>
#include <fstream>
#include "mpi_conf.hpp"
#include "transcoder.hpp"

using namespace std;

void run_worker(EncodeType encode_type){

  //Wait for the controller node to send the file chunk name
  const int msg_buf_size = 10e6;
  char *msg_buf = new char[msg_buf_size];
  int buf_len = 0;
  MPI_Status status;
  MPI_Recv(msg_buf, msg_buf_size, MPI_BYTE, CONTROLLER_RANK, 0, MPI_COMM_WORLD, &status);
  MPI_Get_count(&status, MPI_BYTE, &buf_len);
  string chunk_name(msg_buf, buf_len);
  transcode(chunk_name, chunk_name + ".out", encode_type);
  MPI_Barrier(MPI_COMM_WORLD);
}