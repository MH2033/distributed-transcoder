#include <iostream>
#include <mpi.h>
#include "transcoder.hpp"
#include "mpi_conf.hpp"
#include "worker_node.hpp"

using namespace std;
void run_controller(string filepath, int cluster_size);

int main(int argc, char**argv) {
  int process_rank, cluster_size;
  int provided;
  MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &provided);
  if (provided < MPI_THREAD_MULTIPLE) {
    printf("ERROR: The MPI library does not have full thread support\n");
    MPI_Abort(MPI_COMM_WORLD, 1);
  }

  if(argc < 2) {
    printf("ERROR: Specify the file name\n");
    MPI_Abort(MPI_COMM_WORLD, 1);
  }
  MPI_Comm_size(MPI_COMM_WORLD, &cluster_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);

  //Controller code
  if(process_rank == CONTROLLER_RANK)
    run_controller(argv[1], cluster_size);


  //Worker code
  else {
    run_worker();
  }
  
  MPI_Finalize();
  return 0;
}
