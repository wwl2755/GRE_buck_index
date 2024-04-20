#!/bin/sh

# dataset
# datasets="books"
datasets="books"

# Read/Insert ratios
# ratios="1:0"
ratios="0.5:0.5"

operations_nums="20000000 40000000 60000000 80000000 100000000 120000000 140000000 160000000 180000000 200000000 220000000 240000000 260000000 280000000 300000000 320000000 340000000 360000000 380000000"

# # sbuck_sizes
sbuck_sizes="8"

# dbuck_sizes
dbuck_sizes="16384"
# dbuck_sizes="10485760 12582912 14680064"


# # Iterate the string array using for loop
for ratio in $ratios; do
    for dataset in $datasets; do
        for operations_num in $operations_nums; do
            # bli
            for sbuck_size in $sbuck_sizes; do
                for dbuck_size in $dbuck_sizes; do
                    command="numactl --cpunodebind=0 --membind=0  ./microbench_read_after_write --keys_file=../datasets/${dataset} --keys_file_type=binary --read=${ratio%:*} --insert=${ratio#*:} --operations_num=${operations_num} --table_size=200000000 --init_table_ratio=0.05 --thread_num=1 --index=buckindex --memory --bli_sbuck_size=${sbuck_size} --bli_dbuck_size=${dbuck_size} --bli_initial_filled_ratio=0.6  --error_bound=4.8"
                    echo "command is $command"  >> terminal_output.txt
                    $command >> terminal_output.txt
                done
            done
        done
    done
done
