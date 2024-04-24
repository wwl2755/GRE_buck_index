#!/bin/sh

# dataset
# datasets="books"
datasets="books"

# Read/Insert ratios
# ratios="1:0"
ratios="0:1"

operations_nums="10000000 20000000 30000000 40000000 50000000 60000000 70000000 80000000 90000000 100000000 110000000 120000000 130000000 140000000 150000000 160000000 170000000 180000000 190000000"

# # sbuck_sizes
sbuck_sizes="8"

# dbuck_sizes
dbuck_sizes="16384"
# dbuck_sizes="10485760 12582912 14680064"

#indexes
indexes=""

# # Iterate the string array using for loop
for ratio in $ratios; do
    for dataset in $datasets; do
        for operations_num in $operations_nums; do
            # bli
            for sbuck_size in $sbuck_sizes; do
                for dbuck_size in $dbuck_sizes; do
                    command="numactl --cpunodebind=0 --membind=0  ./microbench --keys_file=../datasets/${dataset} --keys_file_type=binary --read=${ratio%:*} --insert=${ratio#*:} --operations_num=${operations_num} --table_size=200000000 --init_table_ratio=0.1 --thread_num=1 --index=buckindex --memory --bli_sbuck_size=${sbuck_size} --bli_dbuck_size=${dbuck_size} --bli_initial_filled_ratio=0.6  --error_bound=4.8"
                    echo "command is $command"
                    $command
                done
            done

            # others
            for index in $indexes; do
                command="numactl --cpunodebind=0 --membind=0 ./microbench --keys_file=../datasets/${dataset} --keys_file_type=binary --read=${ratio%:*} --insert=${ratio#*:} --operations_num=${operations_num} --table_size=200000000 --init_table_ratio=0.5 --thread_num=1 --index=${index} --memory"
                echo "command is $command"
                $command
            done
        done
    done
done
