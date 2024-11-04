#!/bin/sh

# dataset
# datasets="books"
datasets="fb osm books"

# Read/Insert ratios
# ratios="1:0"
ratios="1:0 0:1 0.5:0.5"

# # sbuck_sizes
# sbuck_sizes="2 4"
sbuck_sizes="1 2 4 8 16 32"

# dbuck_sizes
dbuck_sizes="16384 32768 65536 131072 262144 524288 1048576 2097152 4194304 8388608 10485760 12582912 14680064 16777216 33554432"
# dbuck_sizes="10485760 12582912 14680064"

#indexes
indexes=""

# # Iterate the string array using for loop
for ratio in $ratios; do
    for dataset in $datasets; do
        # bli
        for sbuck_size in $sbuck_sizes; do
            for dbuck_size in $dbuck_sizes; do
                    command="numactl --cpunodebind=0 --membind=0  ./microbench --output_path=../scripts/Figure7_s_dbuck_size.csv --keys_file=../datasets/${dataset} --keys_file_type=binary --read=${ratio%:*} --insert=${ratio#*:} --operations_num=100000000 --table_size=200000000 --init_table_ratio=0.5 --thread_num=1 --index=buckindex --memory --bli_sbuck_size=${sbuck_size} --bli_dbuck_size=${dbuck_size} --bli_initial_filled_ratio=0.6  --error_bound=4.8"
                    echo "command is $command"
                    $command
            done
        done

        # others
        for index in $indexes; do
            command="numactl --cpunodebind=0 --membind=0 ./microbench  --output_path=../scripts/Figure7_s_dbuck_size.csv --keys_file=../datasets/${dataset} --keys_file_type=binary --read=${ratio%:*} --insert=${ratio#*:} --operations_num=100000000 --table_size=200000000 --init_table_ratio=0.5 --thread_num=1 --index=${index} --memory"
            echo "command is $command"
            $command
        done
    done
done
