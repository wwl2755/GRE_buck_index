# GRE
GRE is a benchmark suite for learned indexes and traditional indexes to measure throughput and latency with custom workload (read / write ratio) and any dataset. GRE quantifies datasets using local and global hardness, and includes a synthetic data generator to generate data with various hardness.

See details in our VLDB 2022 paper below. If you use our work, please cite:
```
Chaichon Wongkham, Baotong Lu, Chris Liu, Zhicong Zhong, Eric Lo, and Tianzheng Wang. Are Updatable Learned Indexes Ready?. PVLDB, 15(11): 3004 - 3017, 2022.
```

## Requirements
- gcc 8.3.0+
- cmake 3.14.0+

## Dependencies
- intel-mkl 2018.4.274
- intel-tbb 2020.3
- jemalloc

## Config
1. Install intel oneapi(which includes **intel mkl and tbb**)
```bash
# download the key to system keyring
wget -O- https://apt.repos.intel.com/intel-gpg-keys/GPG-PUB-KEY-INTEL-SW-PRODUCTS.PUB \
| gpg --dearmor | sudo tee /usr/share/keyrings/oneapi-archive-keyring.gpg > /dev/null

# add signed entry to apt sources and configure the APT client to use Intel repository:
echo "deb [signed-by=/usr/share/keyrings/oneapi-archive-keyring.gpg] https://apt.repos.intel.com/oneapi all main" | sudo tee /etc/apt/sources.list.d/oneAPI.list

# install oneapi
sudo apt update
sudo apt install intel-basekit
```

**Install tbb2020.3**

- goto https://github.com/oneapi-src/oneTBB/releases?page=2
- Find [Threading Building Blocks 2020 Update 3](https://github.com/oneapi-src/oneTBB/releases/tag/v2020.3)
- Download [tbb-2020.3-lin.tgz](https://github.com/oneapi-src/oneTBB/releases/download/v2020.3/tbb-2020.3-lin.tgz) use `wget https://github.com/oneapi-src/oneTBB/releases/download/v2020.3/tbb-2020.3-lin.tgz`
- Unzip the file `tar -xvzf tbb-2020.3-lin.tgz`
- Rename it to `2020.3`: `mv tbb tbb-2020.3`
- Copy it under  `/opt/intel/oneapi/tbb/`: `sudo cp -r tbb-2020.3 /opt/intel/oneapi/tbb/`

2. Change the paths in GRE code
   - replace `/opt/intel` with `/opt/intel/oneapi`

   - replace `/mkl` with `/mkl/latest` in `cmake/FindMKL.cmake`

   - replace `/opt/intel/mkl/` with `/opt/intel/oneapi/mkl/latest/` in `findex`, `xindex`

   - repace `tbb` with `tbb/<version number>` in `cmake/findTBB.cmake` and `lippol/src/cmake/findTBB.cmake`

   - replace `set(TBB_LIB_PATH_SUFFIX "lib/intel64/gcc4.4")` with what's inside `/opt/intel/oneapi/tbb/2020.3/lib/intel64`. In my case, it's `gcc4.8`

3. For OS <= Ubuntu18.04, upgrade gcc and g++

```bash
sudo apt install gcc-8
sudo apt install g++-8
```
4. For OS <= Ubuntu18.04, set the environment variable to specify the gcc and g++ version
   
   ```bash
   export CC=/usr/bin/gcc-8
   export CXX=/usr/bin/g++-8
   ```
5. Install jemalloc: `sudo apt install libjemalloc-dev`


## Build
```
git submodule update --init # only for the first time
mkdir -p build
cd build
```
### Debug mode
```
cmake .. -DBUCKINDEX_DEBUG=ON && make
```
### Release mode
```
cmake .. -DCMAKE_BUILD_TYPE=Release && make
```

### Build with alternative design choices
```
# not using simd
cmake .. -DBUCKINDEX_NOT_USE_SIMD=ON -DCMAKE_BUILD_TYPE=Release && make

# not using linear regression; using endpoint linear model instead
cmake .. -DBUCKINDEX_NOT_USE_LINEAR_REGRESSION=ON -DCMAKE_BUILD_TYPE=Release && make
```

### Build with various hint systems
```
cmake .. -DHINT_MODE=mod_hash
cmake .. -DHINT_MODE=cl_hash
cmake .. -DHINT_MODE=murmur_hash
cmake .. -DHINT_MODE=model_predict
cmake .. -DHINT_MODE=no_hash
```



## Basic usage
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

For additional features, add additional flags:
- Latency
```
--latency_sample --latency_sample_ratio=0.01
```
- Range query (eg. range = 100)
```
--scan_ratio=1 --scan_num=100
```
- To use Zipfian distribution for lookup
```
--sample_distribution=zipf
```
- To perform data-shift experiment. Note that the key file needs to be generated like so (changing from one dataset to another). This flag just simply prevent the keys be shuffled and preserving the order in the key file
```
--data_shift
```
- Calculate data hardness (PLA-metric) with specified model error bound of the input dataset
```
--dataset_statistic --error_bound=32
```
- If the index implement memory consumption interface
```
--memory
```
All the result will be output to the csv file specified in --output_path flag.

## Update with latest commit

1. Navigate to the submodule directory:

```
cd src/competitor/alex/src
```

2. Ensure that the submodule is on the desired branch (usually main or master). To check the current branch, run:

```
git branch
```

If you need to switch to another branch, use:

```
git checkout <branch-name>
```

3. Pull the latest changes from the submodule's remote repository:

```
git pull origin <branch-name>
```

4. After updating the submodule, navigate back to the main repository:

```
cd ../../../../
```

5. Commit the updated submodule reference in the main repository:

```
git add src/competitor/alex/src
git commit -m "Update submodule to the latest version"
```

6. Now, the submodule is updated to the latest version, and the main repository's reference to the submodule is updated as well.

Remember to push your changes to the main repository's remote if needed:

```
git push origin <branch-name>
```


## Performance analysis
Install related driver to get more analysis:
```
cd /opt/intel/oneapi/vtune/latest/sepdk/src
make
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