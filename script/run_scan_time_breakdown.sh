#!/bin/sh

source /opt/intel/oneapi/vtune/latest/vtune-vars.sh
echo 0 | sudo tee /proc/sys/kernel/perf_event_paranoid


command="numactl --cpunodebind=0 --membind=0 ./microbench --output_path=scan_std.csv --keys_file=../datasets/books --keys_file_type=binary --read=0.0 --insert=0.0 --scan=1.0 --scan_num=10000 --operations_num=1000 --table_size=200000000 --init_table_ratio=1.0 --thread_num=1 --index=buckindex --memory --bli_sbuck_size=4 --bli_dbuck_size=4096 --bli_initial_filled_ratio=0.6  --error_bound=4"
echo "command is $command"

vtune -collect hotspots -r vtune_results/scan_time_breakdown -- $command
