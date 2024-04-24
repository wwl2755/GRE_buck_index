# download the key to system keyring
wget -O- https://apt.repos.intel.com/intel-gpg-keys/GPG-PUB-KEY-INTEL-SW-PRODUCTS.PUB \
| gpg --dearmor | sudo tee /usr/share/keyrings/oneapi-archive-keyring.gpg > /dev/null

# add signed entry to apt sources and configure the APT client to use Intel repository:
echo "deb [signed-by=/usr/share/keyrings/oneapi-archive-keyring.gpg] https://apt.repos.intel.com/oneapi all main" | sudo tee /etc/apt/sources.list.d/oneAPI.list

# install oneapi
sudo apt update
sudo apt install intel-basekit

# install tbb-2020.3
wget https://github.com/oneapi-src/oneTBB/releases/download/v2020.3/tbb-2020.3-lin.tgz
tar -xvzf tbb-2020.3-lin.tgz
mv tbb 2020.3
sudo cp -r 2020.3 /opt/intel/oneapi/tbb/

# install jemalloc
sudo apt install libjemalloc-dev

# install cmake
sudo apt  install cmake

# intsall numactl
sudo apt install numactl

# turn off hyper-thereading
echo off | sudo tee /sys/devices/system/cpu/smt/control
# turn hyper-thereading back on
# echo on | sudo tee /sys/devices/system/cpu/smt/control


#install g++-9
sudo apt install g++-9