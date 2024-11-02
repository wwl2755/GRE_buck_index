# GRE including BLI

## 1. Install
Follow the steps in `setup.sh`


## 2. Initialize submodules
```
git submodule update --init # only for the first time
mkdir -p build
cd build
```

## 3. Build 
The default is to use release mode.
Release mode
```
cmake .. -DCMAKE_BUILD_TYPE=Release && make
```
Debug mode
```
cmake .. -DBUCKINDEX_DEBUG=ON && make
```
Build with alternative design choices
```
# using simd
cmake .. -DBUCKINDEX_USE_SIMD=ON -DCMAKE_BUILD_TYPE=Release && make

# not using linear regression; using endpoint linear model instead
cmake .. -DBUCKINDEX_NOT_USE_LINEAR_REGRESSION=ON -DCMAKE_BUILD_TYPE=Release && make
```

Build with various hint systems
```
cmake .. -DHINT_MODE=mod_hash
cmake .. -DHINT_MODE=cl_hash
cmake .. -DHINT_MODE=murmur_hash
cmake .. -DHINT_MODE=model_predict
cmake .. -DHINT_MODE=no_hash
```

## 4. Basic usage
Below is the basic usage of the benchmark. To reproduce the result in the paper, please refer check the README in the `scripts` folder.

To calculate throughput:
```
./build/microbench \
--keys_file=./data/dataset \
--keys_file_type={binary,text} \
--read=0.5 --insert=0.5 \
--operations_num=800000000 \
--table_size=-1 \
--init_table_ratio=0.5 \
--thread_num=24 \
--index=index_name \
```

Example:
```
./build/microbench \
--keys_file=./datasets/covid \
--keys_file_type=binary \
--read=0.0 --insert=1.0 \
--operations_num=800000000 \
--table_size=-1 \
--init_table_ratio=0.1 \
--thread_num=1 \
--index=buckindex \
--bli_initial_filled_ratio=0.6 \
--bli_sbuck_size=8 \
--bli_dbuck_size=256
```

table_size=-1 is to infer from the first line of the file.
init_table_ratio is to specify the proportion of the dataset to bulkload.


## 5. Performance analysis
Install related driver to get more analysis:
```
cd /opt/intel/oneapi/vtune/latest/sepdk/src
sudo make
sudo insmod sep.ko
```
Run vtune:
```
source /opt/intel/oneapi/vtune/latest/vtune-vars.sh

# configure driverless without installing drivers
echo 0 | sudo tee /proc/sys/kernel/perf_event_paranoid

# or install driver to get more detailed analysis
## step1: instll the vtsspp driver
cd /opt/intel/oneapi/vtune/latest/sepdk/src/vtsspp
make
sudo make install
## step2: load all drivers
sudo -s /opt/intel/oneapi/vtune/latest/sepdk/src/insmod-sep
## step3: check if the driver is loaded
lsmod | grep sep

# replace <result_dir> with the directory you want to store the result
vtune -collect hotspots -r vtune_results/<result _dir> -- ./build/microbench --keys_file=./datasets/books --keys_file_type=binary --read=1.0 --insert=0.0 --operations_num=800000000 --table_size=-1 --init_table_ratio=0.1 --thread_num=1 --index=buckindex --bli_initial_filled_ratio=0.6 --bli_use_linear_regression=1 --bli_use_simd=1 --bli_sbuck_size=8
```
Other vtune collection type:
```
-collect hotspots
-collect hрc-performance 
-collect uarch-exploration
```

## 6. Acknowledgement
We thank the authors of [GRE](https://github.com/gre4index/GRE) for their work. This repository is a fork of their work.
