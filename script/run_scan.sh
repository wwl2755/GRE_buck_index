#!/bin/sh

# dataset
datasets="books"

# scan nums
scan_nums="100 1000 10000 100000 1000000"

# # sbuck_sizes
sbuck_sizes="4"

# dbuck_sizes
dbuck_sizes="32 256 2048 16384 131072"

#indexes
indexes="alex lipp pgm finedex"

# # Iterate the string array using for loop
for scan_num in $scan_nums; do
    for dataset in $datasets; do
        # set operations_num to be 100000000 divided by scan_num
        operations_num=$((100000000 / scan_num))
        # bli
        for sbuck_size in $sbuck_sizes; do
            for dbuck_size in $dbuck_sizes; do
                    command="numactl --cpunodebind=0 --membind=0 ./microbench --output_path=scan.csv --keys_file=../datasets/${dataset} --keys_file_type=binary --read=0.0 --insert=0.0 --scan=1.0 --scan_num=${scan_num} --operations_num=${operations_num} --table_size=200000000 --init_table_ratio=1.0 --thread_num=1 --index=buckindex --memory --bli_sbuck_size=${sbuck_size} --bli_dbuck_size=${dbuck_size} --bli_initial_filled_ratio=0.6  --error_bound=4"
                    echo "command is $command"
                    $command
            done
        done

        # others
        for index in $indexes; do
            command="numactl --cpunodebind=0 --membind=0 ./microbench --output_path=scan.csv --keys_file=../datasets/${dataset} --keys_file_type=binary --read=0.0 --insert=0.0 --scan=1.0 --scan_num=${scan_num} --operations_num=${operations_num} --table_size=200000000 --init_table_ratio=1.0 --thread_num=1 --index=${index} --memory"
            echo "command is $command"
            $command
        done
    done
done
