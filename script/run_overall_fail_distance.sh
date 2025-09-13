#!/bin/sh

# BUILD
# cmake .. -DCMAKE_BUILD_TYPE=Release -DBUCKINDEX_DEBUG=ON 

# run
# sh ../script/run_overall_fail_distance.sh >> faile_distance.log.txt

# check stats:
# search "avg fail distance:" in the log

datasets="books fb osm"

# Read/Insert ratios
ratios="1:0"

sbuck_sizes="4"
dbuck_sizes="32768 65536 131072"

for ratio in $ratios; do
    for dataset in $datasets; do
        for sbuck_size in $sbuck_sizes; do
            for dbuck_size in $dbuck_sizes; do
                    command="numactl --cpunodebind=0 --membind=0 ./microbench --output_path=../scripts/Figure6_overall_throughput_fail_distance.csv --keys_file=../datasets/${dataset} --keys_file_type=binary --read=${ratio%:*} --insert=${ratio#*:} --operations_num=100000000 --table_size=200000000 --init_table_ratio=0.5 --thread_num=1 --index=buckindex --memory --bli_sbuck_size=${sbuck_size} --bli_dbuck_size=${dbuck_size} --bli_initial_filled_ratio=0.6  --error_bound=4"
                    echo "command is $command"
                    $command
            done
        done
    done
done
