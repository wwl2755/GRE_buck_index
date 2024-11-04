
# 0. Preparation
Download dataset:
```
cd datasets
sh download.sh
```

Config conda env
```
wget https://repo.anaconda.com/miniconda/Miniconda3-latest-Linux-x86_64.sh
chmod +x Miniconda3-latest-Linux-x86_64.sh
./Miniconda3-latest-Linux-x86_64.sh
source ~/.bashrc

conda create --name bli python=3.11
conda activate bli
```

Install python packages
```
pip install numpy
pip install matplotlib
pip install pandas
```


# Figure 1
Figure 1: The average prediction error with different group sizes on the first 10000 keys of books, fb, and osm.

To get Figure 1, run the following command:
```
python Figure1_plot_err_vs_group_size.py
```

# Figure 4
Figure 4: Throughput under different hint choices with the read-only workload.

First, we need to make four build directories, each with a different hint build option.
```
mkdir ../build_nohash
mkdir ../build_mod_hash
mkdir ../build_cl_hash
mkdir ../build_linear_model
```

Then, we should actually build them with different hing options.
```
cd ..

cd build_nohash
cmake .. -DHINT_MODE=no_hash -DCMAKE_BUILD_TYPE=Release
make

cd ../build_linear_model
cmake .. -DHINT_MODE=model_predict -DCMAKE_BUILD_TYPE=Release
make

cd ../build_mod_hash
cmake .. -DHINT_MODE=mod_hash -DCMAKE_BUILD_TYPE=Release
make

cd ../build_cl_hash
cmake .. -DHINT_MODE=cl_hash -DCMAKE_BUILD_TYPE=Release
make
```

After that, we can run the read-only experiment in each directory.
In each build directory, run the following command:
```
sh ../scripts/run_hint_throughput.sh
```

Open the `scan.csv` result in each directory, and copy the corresponding throughput numbers to `Figure4_hint_throughput.csv` in the current directory.

Run the following command to get Figure 4:
```
python Figure4_hint_search_distance.py
```

# Figure 6
Figure 6: Throughput under different read-write ratios.

First, we need to run the experiment:
```
cd build
sh ../script/run_overall.sh
```

After the experiments are done, we can draw the figure:
```
python Figure6_overall_throughput.py
```

# Figure 7
Figure 7: Throughput with different S-Bucket and D-Bucket size on books

First, we need to run the experiment:
```
cd build
sh ../script/run_s_dbuck_size.sh
```


After the experiments are done, we can draw the figure:
```
python Figure7_s_dbuck_size.py
```

# Figure 8
Figure 8: Throughput with different initial fill ratios on books

First, we need to run the experiment:
```
cd build
sh ../script/run_fill_ratio.sh
```

After the experiments are done, we can draw the figure:
```
python Figure8_fill_ratio.py
```

# Figure 9
Figure 9: Throughput with different number of cores

First, we need to run the experiment:
```
cd build
sh ../script/run_mrsw.sh
```

After the experiments are done, we can draw the figure:
```
python Figure9_mrsw.py
```

# Table 1
Table 1: BLI time breakdown on fb

We use vtune to analyse the time breakdown.

Install related driver to get more analysis:
```
cd /opt/intel/oneapi/vtune/latest/sepdk/src
sudo make
sudo insmod sep.ko # can be other similar names, such as sep5.ko

source /opt/intel/oneapi/vtune/latest/vtune-vars.sh

# configure driverless without installing drivers
echo 0 | sudo tee /proc/sys/kernel/perf_event_paranoid
```

Run experiment:
```
cd build
vtune -collect hotspots -r vtune_results/time_breakdown -- numactl --cpunodebind=0 --membind=0  ./build/microbench --keys_file=./datasets/fb --keys_file_type=binary --read=0.5 --insert=0.5 --operations_num=100000000 --table_size=200000000 --init_table_ratio=0.5 --thread_num=1 --index=buckindex --bli_initial_filled_ratio=0.6 --bli_use_linear_regression=1  --bli_sbuck_size=4
```

After that, you can download the results in `build/vtune_results/time_breakdown`, and open it in a Windows/Linux Desktop, then clike "Flame Graph" to see the ratios.

# Table 2
First, run the experiment and log the terminal output to a file.
```
cd build
sh ../script/bulk_load.sh > bulk_load_out.txt
```

Then, open `build/bulk_load_out.txt`, and search for "bulk loading throughput" for each learned index.