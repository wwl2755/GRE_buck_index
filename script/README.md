
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
```


# Figure 1
To get Figure 1, run the following command:
```
python Figure1_plot_err_vs_group_size.py
```