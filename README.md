# BLI artifact

This repo is to reproduce the results in our paper "BLI: A High-performance Bucket-based Learned Index with Concurrency Support". 

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

**After this project is successfully built, to reproduce the results in the paper, please refer to scripts/README.md**

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


## 5. Acknowledgement
We thank the authors of [GRE](https://github.com/gre4index/GRE) for their work. This repository is a fork of their work.
