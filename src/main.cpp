#include <iostream>
#include <mpi.h>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>

#include "transcoder.hpp"
#include "mpi_conf.hpp"
#include "worker_node.hpp"

using namespace std;
namespace po = boost::program_options;
void run_controller(string filepath, int cluster_size, EncodeType encode_type);

int main(int argc, char**argv) {
  int process_rank, cluster_size;
  int provided;
  string input_path;
  EncodeType encode_type = EncodeType::H265;
  MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &provided);
  if (provided < MPI_THREAD_MULTIPLE) {
    printf("ERROR: The MPI library does not have full thread support\n");
    MPI_Abort(MPI_COMM_WORLD, 1);
  }

  
  po::options_description desc("Allowed options");
  desc.add_options()
      ("help,h", "produce help message")
      ("input,i", po::value<string>(), "path to input file")
      ("encoding,e", po::value<string>(), "encoding scheme to use")
  ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  if(vm.count("encoding")){
    if(vm["encoding"].as<string>() == "h264"){
      encode_type = EncodeType::H264;
    }

    else if(vm["encoding"].as<string>() == "h265"){
      encode_type = EncodeType::H265;
      
    }

    else if(vm["encoding"].as<string>() == "vp8"){
      encode_type = EncodeType::VP8;
      
    }

    else if(vm["encoding"].as<string>() == "vp9"){
      encode_type = EncodeType::VP9;
      
    }

    else{
      cout << "Incorrect encoding scheme" << endl;
      MPI_Abort(MPI_COMM_WORLD, 1);
    }

    
  }

  if(vm.count("input")) {
    input_path = vm["input"].as<string>();
    
  }

  else {
    cout << "ERROR: Specify the file name" << endl;
    MPI_Abort(MPI_COMM_WORLD, 1);
  }

  MPI_Comm_size(MPI_COMM_WORLD, &cluster_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);

  //Controller code
  if(process_rank == CONTROLLER_RANK)
    run_controller(input_path, cluster_size, encode_type);


  //Worker code
  else {
    run_worker(encode_type);
  }
  
  MPI_Finalize();
  return 0;
}
