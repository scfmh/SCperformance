This is the repo demo for testing different lossy comprssors under the different hpc systems
Before testing, we have to download the dependency libs and related versions we used
ADIOS2 (v2.9.0) https://github.com/ornladios/ADIOS2.git
HDF5 (1.14.1) https://github.com/HDFGroup/hdf5.git
SZ (2.1.12.5) https://github.com/szcompressor/SZ.git
Cusz (0.3.1) https://github.com/szcompressor/cuSZ.git
ZFP (1.0.0) https://github.com/LLNL/zfp.git
MGARD (1.5.0) https://github.com/CODARcode/MGARD.git

A. For perfomace on summit, you should go to the folder summit_performance and selected the ADIOS or HDF5
1. load the related libs as follows.
module load cmake/3.18.4
module load spectrum-mpi/10.4.0.3-20210112
module load python/3.8-anaconda3
module load cuda/11.4
module load gcc/9
module load zstd
2. for each testing, go to the relatted folder and open the build, edit the build.sh based on your dependency path
3. run bash build.sh
4. testing: for example. gc=GCLDLWP_1_26_1800_3600.f32 command line as followsing.
   MGARD GPU: ./build/cpu-application-simulator_GCLDLWP -z 1 -i ${gc} -c output.bp -n 3 3600 1800 26 -t s -s 0 -l 0 -m abs -e 0.1 -d cuda
   SZ GPU: ./build/ZFP_cpu_simulation_iter_single -z -i ${gc} -c output.bp -n 3 3600 1800 26 -t s -e 0.1 -r 4
   ZFP GPU: ./build/SZ_cpu_simulation_cuda_iter_ICLDIWP -i ${gc} -c output.bp -n 3 3600 1800 26 -e 16.73116

B. For the performnce on Frontier, the dependency and modules are different. SZ and ZFP only supports the CPU version
1. load the related libs as follows.
   module load rocm/5.1.0
   module load cmake
   module load cray-mpich/8.1.17
   module load cray-python/3.9.12.1
   module load zstd
2. for each testing, go to the relatted folder and open the build, edit the build.sh based on your dependency path
3. run bash build.sh
4. For example:
   ZFP CPU: ./build/ZFP_cpu_simulation_iter_exaalt -z 1 -i ${gc} -c output.bp -n 3 3600 1800 26 -m abs -e 0.1 -t s
   MGARD CPU: ./build/cpu-application-simulator_exaalt -z 1 -i ${gc} -c  output.bp -t s -n 3 3600 1800 26 -m abs -e 0.1 -l 0 -s 0 -d serial 
   SZ CPU: ./build/SZ_cpu_simulation_ICLDIWP -z 1 -i ${gc} -c output.bp -n 3 3600 1800 26 -m abs -e 0.1 -f sz_16.73116.config

C. if we want to test the parallel performance or not
   we should consider the following sentence:
   For Adios:
   #define MYADIOS_USE_MPI 1
   #include <adios2.h>
   #if MYADIOS_USE_MPI
   #include <mpi.h>
   #endif
   For HDF5: 
   #define HDF5_USE_MPI 1
   #include <hdf5.h>
   #if HDF5_USE_MPI
   #include <mpi.h>
   #include "H5FDmpio.h" 
   #endif

D. For power, if we want to test the GPU power, we can use  nvprof --system-profiling on --print-gpu-trace --log-file
e.g., in MGARD bin file: nvprof --system-profiling on --print-gpu-trace --log-file mgard.log $mgard -z -i {gc} -c temp.mgard -t s -n 3 3600 1800 26
-m abs -e 16.7 -s inf -l 0 -h 0 -d cuda -v 3
If we test the CPU power, we have to use the root permission
