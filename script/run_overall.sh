#!/bin/sh

# dataset
datasets="books fb osm"
# datasets="fb"

# Read/Insert ratios
ratios="1:0 0.9:0.1 0.8:0.2 0.7:0.3 0.6:0.4 0.5:0.5 0.4:0.6 0.3:0.7 0.2:0.8 0.1:0.9 0:1"

# # sbuck_sizes
# sbuck_sizes="2 4"
# sbuck_sizes="1 2 4 8 16 32"
sbuck_sizes="4"

# dbuck_sizes
dbuck_sizes="131072"

#indexes
indexes="alex lipp pgm finedex"

# # Iterate the string array using for loop
for ratio in $ratios; do
    for dataset in $datasets; do
        # bli
        for sbuck_size in $sbuck_sizes; do
            for dbuck_size in $dbuck_sizes; do
                    command="numactl --cpunodebind=0 --membind=0 ./microbench --output_path=../scripts/Figure6_overall_throughput.csv --keys_file=../datasets/${dataset} --keys_file_type=binary --read=${ratio%:*} --insert=${ratio#*:} --operations_num=100000000 --table_size=200000000 --init_table_ratio=0.5 --thread_num=1 --index=buckindex --memory --bli_sbuck_size=${sbuck_size} --bli_dbuck_size=${dbuck_size} --bli_initial_filled_ratio=0.6  --error_bound=4"
                    echo "command is $command"
                    $command
            done
        done

        # others
        for index in $indexes; do
            command="numactl --cpunodebind=0 --membind=0 ./microbench --output_path=../scripts/Figure6_overall_throughput.csv --keys_file=../datasets/${dataset} --keys_file_type=binary --read=${ratio%:*} --insert=${ratio#*:} --operations_num=100000000 --table_size=200000000 --init_table_ratio=0.5 --thread_num=1 --index=${index} --memory"
            echo "command is $command"
            $command
        done
    done
done
