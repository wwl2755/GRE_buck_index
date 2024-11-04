#!/bin/sh

# dataset
datasets="books fb osm covid"

# Read/Insert ratios
ratios="1:0"

# # sbuck_sizes

sbuck_sizes="4"

# dbuck_sizes
dbuck_sizes="131072"

# # Iterate the string array using for loop
for ratio in $ratios; do
    for dataset in $datasets; do
        # bli
        for sbuck_size in $sbuck_sizes; do
            for dbuck_size in $dbuck_sizes; do
                    command="numactl --cpunodebind=0 --membind=0 ./microbench --output_path=overall.csv --keys_file=../datasets/${dataset} --keys_file_type=binary --read=${ratio%:*} --insert=${ratio#*:} --operations_num=100000000 --table_size=200000000 --init_table_ratio=0.5 --thread_num=1 --index=buckindex --memory --bli_sbuck_size=${sbuck_size} --bli_dbuck_size=${dbuck_size} --bli_initial_filled_ratio=0.6  --error_bound=4"
                    echo "command is $command"
                    $command
            done
        done
    done
done
