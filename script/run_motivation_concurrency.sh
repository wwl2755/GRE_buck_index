#!/bin/sh

# dataset
# datasets="books fb osm"
datasets="books"

# Read/Insert ratios
ratios="0.5:0.5"

indexes="alex alexol lipp lippol"

# # Iterate the string array using for loop
for ratio in $ratios; do
    for dataset in $datasets; do
        # others
        for index in $indexes; do
            command="numactl --cpunodebind=0 --membind=0 ./microbench --output_path=motivation_concurrency.csv --keys_file=../datasets/${dataset} --keys_file_type=binary --read=${ratio%:*} --insert=${ratio#*:} --operations_num=100000000 --table_size=200000000 --init_table_ratio=0.5 --thread_num=1 --index=${index} --memory"
            echo "command is $command"
            $command
        done
    done
done
