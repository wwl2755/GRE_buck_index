
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

