#!/bin/sh

source /opt/intel/oneapi/vtune/latest/vtune-vars.sh
echo 0 | sudo tee /proc/sys/kernel/perf_event_paranoid


command="numactl --cpunodebind=0 --membind=0 ./microbench --output_path=overall.csv --keys_file=../datasets/fb --keys_file_type=binary --read=0.5 --insert=0.5 --operations_num=100000000 --table_size=200000000 --init_table_ratio=0.5 --thread_num=1 --index=buckindex --memory --bli_sbuck_size=4 --bli_dbuck_size=131072 --bli_initial_filled_ratio=0.6  --error_bound=4"
echo "command is $command"

vtune -collect hotspots -r vtune_results/time_breakdown -- $command
