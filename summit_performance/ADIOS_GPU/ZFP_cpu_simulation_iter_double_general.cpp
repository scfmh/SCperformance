#include <ios>       //std::ios_base::failure
#include <iostream>  //std::cout
#include <numeric>   //std::iota
#include <stdexcept> //std::invalid_argument std::exception
#include <vector>
#include <chrono>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <cuda_runtime_api.h>
#include <cuda.h>

#include "zfp.h"

//#define MYADIOS_USE_MPI 1
#include <adios2.h>
#if MYADIOS_USE_MPI
#include <mpi.h>
#endif

using namespace std::chrono;

class Timer {
public:
  void start() { err = clock_gettime(CLOCK_REALTIME, &start_time); }
  void end() {
    err = clock_gettime(CLOCK_REALTIME, &end_time);
    total_time +=
        (double)(end_time.tv_sec - start_time.tv_sec) +
        (double)(end_time.tv_nsec - start_time.tv_nsec) / (double)1000000000;
  }
  double get() {
    double time =
        (double)(end_time.tv_sec - start_time.tv_sec) +
        (double)(end_time.tv_nsec - start_time.tv_nsec) / (double)1000000000;
    clear();
    return time;
  }
  void clear() { total_time = 0; }
  void print(std::string s) {
    std::cout << s << " time: " << total_time << "s" << std::endl;
    clear();
  }

private:
  int err = 0;
  double total_time = 0;
  struct timespec start_time, end_time;
};


void print_usage_message(std::string error) {
  if (error.compare("") != 0) {
    std::cout  << error << std::endl;
  }
  printf("Options\n\
\t -z: compress data\n\
\t\t -i <path to data file to be compressed>\n\
\t\t -c <path to compressed file>\n\
\t\t -t <s|d>: data type (s: single; d:double)\n\
\t\t -n <ndim>: total number of dimensions\n\
\t\t\t [dim1]: slowest dimention\n\
\t\t\t [dim2]: 2nd slowest dimention\n\
\t\t\t  ...\n\
\t\t\t [dimN]: fastest dimention\n\
\t\t -m <abs|rel>: error bound mode (abs: abolute; rel: relative)\n\
\t\t -e <error>: error tolerance\n\
\n\
\t -x: decompress data\n\
\t\t -c <path to compressed file>\n\
\t\t -o <path to decompressed file>\n");
  exit(0);
}

bool has_arg(int argc, char *argv[], std::string option) {
  for (int i = 0; i < argc; i++) {
    if (option.compare(std::string(argv[i])) == 0) {
      return true;
    }
  }
  return false;
}

bool require_arg(int argc, char *argv[], std::string option) {
  for (int i = 0; i < argc; i++) {
    if (option.compare(std::string(argv[i])) == 0) {
      return true;
    }
  }
  print_usage_message("missing option: " + option + ".");
  return false;
}

std::string get_arg(int argc, char *argv[], std::string option) {
  if (require_arg(argc, argv, option)) {
    for (int i = 0; i < argc; i++) {
      if (option.compare(std::string(argv[i])) == 0) {
        return std::string(argv[i + 1]);
      }
    }
  }
  return std::string("");
}

int get_arg_int(int argc, char *argv[], std::string option) {
  if (require_arg(argc, argv, option)) {
    std::string arg;
    int i;
    for (i = 0; i < argc; i++) {
      if (option.compare(std::string(argv[i])) == 0) {
        arg = std::string(argv[i + 1]);
      }
    }
    try {
      int d = std::stoi(arg);
      return d;
    } catch (std::invalid_argument const &e) {
      print_usage_message("illegal argument for option " + option + ".");
      return 0;
    }
  }
  return 0;
}

double get_arg_double(int argc, char *argv[], std::string option) {
  if (require_arg(argc, argv, option)) {
    std::string arg;
    int i;
    for (i = 0; i < argc; i++) {
      if (option.compare(std::string(argv[i])) == 0) {
        arg = std::string(argv[i + 1]);
      }
    }
    try {
      double d = std::stod(arg);
      return d;
    } catch (std::invalid_argument const &e) {
      print_usage_message("illegal argument for option " + option + ".");
    }
  }
  return 0;
}

std::vector<size_t> get_arg_dims(int argc, char *argv[], std::string option) {
  std::vector<size_t> shape;
  if (require_arg(argc, argv, option)) {
    std::string arg;
    int arg_idx = 0, i;
    for (i = 0; i < argc; i++) {
      if (option.compare(std::string(argv[i])) == 0) {
        arg = std::string(argv[i + 1]);
        arg_idx = i + 1;
      }
    }
    try {
      size_t d = std::stoi(arg);
      for (size_t i = 0; i < d; i++) {
        shape.push_back(std::stoi(argv[arg_idx + 1 + i]));
      }
      return shape;
    } catch (std::invalid_argument const &e) {
      print_usage_message("illegal argument for option " + option + ".");
      return shape;
    }
  }
  return shape;
}

void readfile(const char *input_file, double *in_buff, size_t read_size) {
  //std::cout <<  "Loading file: " << input_file << "\n";
  FILE *pFile;
  pFile = fopen(input_file, "rb");
  if (pFile == NULL) {
    std::cout << "file open error!\n";
    exit(1);
  }
  // fseek(pFile, 0, SEEK_END);
  // size_t lSize = ftell(pFile);
  // rewind(pFile);
  fread(in_buff, 1, read_size, pFile);
  fclose(pFile);
  // min_max(lSize/sizeof(T), in_buff);
}

void writefile(const char *output_file, size_t num_bytes, double *out_buff) {
  FILE *file = fopen(output_file, "w");
  fwrite(out_buff, 1, num_bytes, file);
  fclose(file);
}



struct result {
	double write;
  double read;
  double comp;
  double decomp;
};



result  launch_compress(int D, const char *input_file, const char *output_file, std::vector<size_t> shape, 
 double accuracy,int num_sim_iterations,bool use_compression, int compute_delay, zfp_type type, int fixed_rate){
 int rank, size;

#if MYADIOS_USE_MPI
 MPI_Comm_rank(MPI_COMM_WORLD, &rank);
 MPI_Comm_size(MPI_COMM_WORLD, &size);
#else
 rank = 0;
 size = 1;
#endif
 
/** ADIOS class factory of IO class objects */
#if MYADIOS_USE_MPI
 adios2::ADIOS adios(MPI_COMM_WORLD);
#else
 adios2::ADIOS adios;
#endif

 result res;
 adios2::IO io = adios.DeclareIO("SimulationData");
 io.SetEngine("BP4");

 adios2::Engine writer = io.Open(output_file, adios2::Mode::Write);

 size_t original_size = 1;
 for (int i = 0; i < D; i++)
   original_size *= shape[i];
 double* original_data = new double[original_size];
 /**********************read information *************/
 readfile(input_file, original_data, original_size * sizeof(double));
 double *d_original_data;
 unsigned char *d_compressed_data;

/*this will list the general zfp information*/
 int status = 0;    /* return value: 0 = success */
 type = zfp_type_double;
  //zfp_type type;     /* array scalar type */
 zfp_field* field;  /* array meta data */
 zfp_stream* zfp;   /* compressed stream */
 //void* buffer;      /* storage for compressed stream */
 size_t bufsize;    /* byte size of compressed buffer */
 bitstream* stream; /* bit stream to write to or read from */
 size_t compressed_size;    /* byte size of compressed stream */

 //void *compressed_data = (void *)new T[original_size];
 unsigned char * compressed_data = (unsigned char *) new double[original_size];
 compressed_size = original_size * sizeof(double);
 double * decompressed_data = (double *) new double[original_size];
 /* allocate meta data for the 3D array a[nz][ny][nx] */
 //zfp->exec.policy = zfp_exec_cuda; 
 
 if (rank == 0) 
 {
   std::cout << "Data per rank: " << (double)original_size*sizeof(double)/1e9 << " GB\n";
 }

 Timer timer, timer_total; 
 //#if MYADIOS_USE_MPI
 //MPI_Barrier(MPI_COMM_WORLD);
 //#endif
 //timer_total.start();
 //for (int sim_iter = 0; sim_iter < num_sim_iterations; sim_iter++) 
 //{
  //writer.BeginStep();
  sleep(compute_delay);
  #if MYADIOS_USE_MPI
 MPI_Barrier(MPI_COMM_WORLD);
 #endif
 //timer_total.start();
  cudaMalloc((void**)&d_original_data, sizeof(double) * original_size);
  cudaMemcpy(d_original_data, original_data, sizeof(double) * original_size,
           cudaMemcpyHostToDevice);
  timer_total.start();
  if (use_compression) /***use compression or not ***********/
  {
   
  // ********* Compression ********** //
   #if MYADIOS_USE_MPI
   MPI_Barrier(MPI_COMM_WORLD);
   #endif
   timer.start();
   //cudaMalloc((void**)&d_original_data, sizeof(double) * original_size);
   //cudaMemcpy(d_original_data, original_data, sizeof(double) * original_size,
           //cudaMemcpyHostToDevice);
   cudaMalloc((void**)&d_compressed_data, sizeof(double) * original_size);
   cudaDeviceSynchronize();
   if (D == 1) {
    field = zfp_field_1d(d_original_data, type, shape[0]);
  } else if (D == 2) {
    field = zfp_field_2d(d_original_data, type, shape[1], shape[0]);
  } else if (D == 3) {
    field = zfp_field_3d(d_original_data, type, shape[2], shape[1], shape[0]);
  } else if (D == 4) {
    field = zfp_field_4d(d_original_data, type, shape[3], shape[2], shape[1],
                         shape[0]);
  } else {
    std::cout << "wrong D\n";
    exit(-1);
  }
   zfp = zfp_stream_open(NULL);
   zfp_stream_set_rate(zfp, fixed_rate, type, zfp_field_dimensionality(field), zfp_false);
   bufsize = zfp_stream_maximum_size(zfp, field);
   stream = stream_open(d_compressed_data, sizeof(double) * original_size);
   zfp_stream_set_bit_stream(zfp, stream);
   zfp_stream_rewind(zfp);
   
   if (zfp_stream_set_execution(zfp, zfp_exec_cuda)) {
   compressed_size = zfp_compress(zfp, field);
}  
   cudaMemcpy(compressed_data, d_compressed_data, compressed_size, cudaMemcpyDeviceToHost);
  /* cudaFree(d_original_data);
   cudaFree(d_compressed_data);

   zfp_field_free(field);
   zfp_stream_close(zfp);
   stream_close(stream);*/

   cudaDeviceSynchronize();
   #if MYADIOS_USE_MPI
   MPI_Barrier(MPI_COMM_WORLD);
   #endif
   timer.end();
   double compression_time = timer.get();
   #if MYADIOS_USE_MPI
   double max_compression_time;
   MPI_Reduce(&compression_time, &max_compression_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
   if (rank == 0)
   {
    res.comp = max_compression_time;
    std::cout  << "Compression time: " << max_compression_time << "\n";
    std::cout  << "Compression throughput: " << (double)original_size * sizeof(double) * size/max_compression_time/1e9 << " GB/s.\n";
    std::cout  << "Compression ratio: "<< (double)original_size * sizeof(double) / (compressed_size) << "\n";
   }
   #else
   if (rank == 0)
   {
    res.comp = compression_time;
    std::cout  << "Compression time: " << compression_time << "\n";
    std::cout  << "Compression throughput: " << (double)original_size * sizeof(double) * size/compression_time/1e9 << " GB/s.\n";
    std::cout  << "Compression ratio: "<< (double)original_size * sizeof(double) / (compressed_size) << "\n";
   }
   #endif
    timer.clear();
  } 
 // ********* Write********** //
 std::string var_name = "var_step" + std::to_string(0);
 size_t var_size = use_compression ? compressed_size : original_size * sizeof(double);
 //unsigned char *var_data = use_compression ? (unsigned char *)compressed_data : (unsigned char *) original_data;
 unsigned char *var_data = use_compression ? (unsigned char *)d_compressed_data : (unsigned char *) d_original_data;
 adios2::Variable<unsigned char> simulation_var;
 simulation_var = io.DefineVariable<unsigned char>(var_name,{var_size*size}, {var_size*rank}, {var_size});
 #if MYADIOS_USE_MPI
 MPI_Barrier(MPI_COMM_WORLD);
 #endif
 timer.start();
 writer.BeginStep();
 writer.Put<unsigned char>(simulation_var, var_data, adios2::Mode::Sync);
 writer.EndStep();
 writer.Close();
 cudaDeviceSynchronize();
 #if MYADIOS_USE_MPI
 MPI_Barrier(MPI_COMM_WORLD);
 #endif
 timer.end();
 double write_time = timer.get();
 #if MYADIOS_USE_MPI
 double max_write_time;
 MPI_Reduce(&write_time, &max_write_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
 if (rank == 0) 
 {
  res.write = max_write_time;
  std::cout << "Write time: "<< max_write_time << "\n";
  std::cout << "Write throughput: "<< (double)var_size* size/max_write_time/1e9 << " GB/s.\n";
 }
 #else
 if (rank == 0)
 {
  res.write = write_time;
  std::cout << "Write time: "<< write_time << "\n";
  std::cout << "Write throughput: "<< (double)var_size* size/write_time/1e9 << " GB/s.\n";
 }
 #endif
 timer.clear();
 //writer.EndStep();

 //}
 //writer.Close();
 #if MYADIOS_USE_MPI
 MPI_Barrier(MPI_COMM_WORLD);
 #endif
 timer_total.end();
 double total_time = timer_total.get();
 timer_total.clear();
 #if MYADIOS_USE_MPI
 double max_total_time;
 MPI_Reduce(&total_time, &max_total_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
 if (rank == 0) 
 {
  std::cout << "Total write time: " << max_total_time << "\n";
 }
 #else
if (rank == 0)
 {
  std::cout << "Total write time: " << total_time << "\n";
 }
#endif
  cudaDeviceSynchronize();
 #if MYADIOS_USE_MPI
 MPI_Barrier(MPI_COMM_WORLD);
 #endif
 timer_total.start();
 adios2::Engine reader = io.Open(output_file, adios2::Mode::Read);
 //for (int sim_iter = 0; sim_iter < num_sim_iterations; sim_iter++) 
 //{
  sleep(compute_delay);

  //reader.BeginStep(); 
  //std::string var_name = "var_step" + std::to_string(0);
  // ********* Read ********** //
  std::vector<unsigned char> var_data_vec;
  //adios2::Variable<unsigned char> simulation_var;
  simulation_var = io.InquireVariable<unsigned char>(var_name);
  adios2::Dims var_shape = simulation_var.Shape();
  //size_t var_size = var_shape[0] / size;
  adios2::Box<adios2::Dims> sel({var_size*rank}, {var_size});
  simulation_var.SetSelection(sel);
  #if MYADIOS_USE_MPI
  MPI_Barrier(MPI_COMM_WORLD);
  #endif
  timer.start();
  reader.BeginStep();
  reader.Get<unsigned char>(simulation_var, var_data_vec, adios2::Mode::Sync);
  reader.EndStep();
  reader.Close();
  #if MYADIOS_USE_MPI
  MPI_Barrier(MPI_COMM_WORLD);
  #endif
  timer.end();
  double read_time = timer.get();
  #if MYADIOS_USE_MPI
  double max_read_time;
  
  MPI_Reduce(&read_time, &max_read_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
  if (rank == 0) {
   res.read = max_read_time;
   std::cout <<  "Read time: " << max_read_time << "\n";
   std::cout <<  "Read throughput: "<< (double)var_size* size/max_read_time/1e9 << " GB/s.\n";
  }
  #else
  if (rank == 0) {
   res.read = read_time;
   std::cout <<  "Read time: " << read_time << "\n";
   std::cout <<  "Read throughput: "<< (double)var_size* size/read_time/1e9 << " GB/s.\n";
  }
  #endif
  timer.clear();
  if (use_compression) {
    // ********* Decompression ********** //
    #if MYADIOS_USE_MPI
    MPI_Barrier(MPI_COMM_WORLD);
    #endif
    timer.start();
    double *d_decompressed_data;
    unsigned char *d_compressed_data;
    cudaMalloc((void**)&d_decompressed_data, sizeof(double) * original_size);
   cudaMemcpy(d_decompressed_data, compressed_data, compressed_size,
            cudaMemcpyHostToDevice);
   cudaDeviceSynchronize();
    
    if (zfp_exec_cuda) 
    size_t result = zfp_decompress(zfp, field);
    cudaDeviceSynchronize();
    cudaMemcpy(decompressed_data, d_decompressed_data, sizeof(double) * original_size,
             cudaMemcpyDeviceToHost);
    cudaDeviceSynchronize();

   /*cudaFree(d_decompressed_data);
   cudaFree(d_compressed_data);
    zfp_field_free(field);
   zfp_stream_close(zfp);
  stream_close(stream); */
    #if MYADIOS_USE_MPI
    MPI_Barrier(MPI_COMM_WORLD);
    #endif
    timer.end();
    double decompress_time = timer.get();
    #if MYADIOS_USE_MPI
    double max_decompress_time;
    MPI_Reduce(&decompress_time, &max_decompress_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
 
    if (rank == 0) 
    {
      res.decomp = max_decompress_time;
      std::cout <<  "Decompression time: "<< max_decompress_time << "\n";
      std::cout <<  "Decompression throughput: "<< (double)original_size * sizeof(double) * size/max_decompress_time/1e9 << " GB/s.\n";
    }
    #else
    if (rank == 0)
    {
      res.decomp = decompress_time;
      std::cout <<  "Decompression time: "<< decompress_time << "\n";
      std::cout <<  "Decompression throughput: "<< (double)original_size * sizeof(double) * size/decompress_time/1e9 << " GB/s.\n";
    }
    #endif
    timer.clear();
  }
  //reader.EndStep(); 
 //}
 //reader.Close();
 #if MYADIOS_USE_MPI
 MPI_Barrier(MPI_COMM_WORLD);
 #endif
 timer_total.end();
 total_time = timer_total.get();
 timer_total.clear();
 #if MYADIOS_USE_MPI
 max_total_time;
 MPI_Reduce(&total_time, &max_total_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
 if (rank == 0) 
 {
  std::cout << "Total read time: " << max_total_time << "\n"; 
 }
 #else
 if (rank == 0)
 {
  std::cout << "Total read time: " << total_time << "\n";
 }
 #endif
  if (use_compression) {
  cudaDeviceSynchronize();
  cudaFree(d_original_data);
  //cudaFree(d_decompressed_data);
  cudaFree(d_compressed_data);

  zfp_field_free(field);
  zfp_stream_close(zfp);
  stream_close(stream);}
 delete[](double *) original_data;
 delete[](unsigned char *) compressed_data;
 delete[] (double *)decompressed_data;
 //return 0;
 return res;
}

bool run(int argc, char *argv[]) {
  int rank, size;
 int num_sim_iterations = 1;

#if MYADIOS_USE_MPI
 MPI_Comm_rank(MPI_COMM_WORLD, &rank);
 MPI_Comm_size(MPI_COMM_WORLD, &size);
#else
 rank = 0;
 size = 1;
#endif
 
 std::string input_file = get_arg(argc, argv, "-i");
 std::string output_file = get_arg(argc, argv, "-c");
 if (rank == 0) 
  std::cout <<  "original data: " << input_file << "\n";
 if (rank == 0) 
  std::cout <<  "compressed data: " << output_file << "\n";
 
 int DIM = get_arg_int(argc, argv, "-n");
 std::vector<size_t> shape = get_arg_dims(argc, argv, "-n");

 double accuracy = get_arg_double(argc, argv, "-e");
 int fixed_rate = get_arg_int(argc, argv, "-r");
zfp_type type; 
std::string dt = get_arg(argc, argv, "-t");
 if (dt.compare("s") == 0) {
    type = zfp_type_float;
    if (rank == 0) std::cout <<  "data type: Single precision\n";
  } else if (dt.compare("d") == 0) {
    type = zfp_type_double;
    if (rank == 0) std::cout << "data type: Double precision\n";
  } else
    if (rank == 0) print_usage_message("wrong data type.");

  int compute_delay = 0;
  if (has_arg(argc, argv, "-k")) {
    compute_delay = get_arg_int(argc, argv, "-k");
    if (rank == 0) std::cout << "compute delay: " << compute_delay << "\n";
  }

  
	std::vector<double> write, read, comp, decomp, write_comp, read_comp;
  bool use_compression;
  std::vector<int> ebs = {1, 2, 11, 21};
  result res_no_comp;
  use_compression = false;
    res_no_comp = launch_compress(DIM, input_file.c_str(), output_file.c_str(), shape, accuracy, 1, use_compression, compute_delay, type, fixed_rate);

  /*for (int i = 0; i < 4; i++) {
    write.push_back(res_no_comp.write);
    read.push_back(res_no_comp.read);     
  }*/
  for (int fixed_rate : ebs) {
    //std::cout << "this is the error "<<accuracy<<std::endl;
    result res_comp;
    use_compression = true; 
    res_comp = launch_compress(
        DIM, input_file.c_str(), output_file.c_str(), shape, accuracy, 1, use_compression, compute_delay, type, fixed_rate); 
    comp.push_back(res_comp.comp);
    decomp.push_back(res_comp.decomp);
    write_comp.push_back(res_comp.write);
    read_comp.push_back(res_comp.read);
  }

  if (rank == 0) {
  for (double time : write) std::cout << time << ", ";
  std::cout << "\n";
  for (double time : read) std::cout << time << ", ";
  std::cout << "\n";
  for (double time : comp) std::cout << time << ", ";
  std::cout << "\n";
  for (double time : decomp) std::cout << time << ", ";
  std::cout << "\n";
  for (double time : write_comp) std::cout << time << ", ";
  std::cout << "\n";
  for (double time : read_comp) std::cout << time << ", ";
  std::cout << "\n";
	}
  return true;
}


int main(int argc, char *argv[]) {
int size;
int rank;
#if MYADIOS_USE_MPI
 MPI_Init(&argc, &argv);
 MPI_Comm_rank(MPI_COMM_WORLD, &rank);
 MPI_Comm_size(MPI_COMM_WORLD, &size);
#else
 rank = 0;
 size = 1;
#endif

  run(argc, argv);
  #if MYADIOS_USE_MPI
  MPI_Finalize();
  #endif
  return 0;
}
