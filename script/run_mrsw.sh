#!/bin/sh

# dataset
datasets="books fb osm"

# Read/Insert ratios
ratios="0.7:0.3"

# sbuck_sizes
sbuck_sizes="4"

# dbuck_sizes
dbuck_sizes="131072"

# other index types
indexes="alexol lippol xindex finedex"

# threads
threads="2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24"

for thread in $threads; do
    for ratio in $ratios; do
        for dataset in $datasets; do
            for dbuck_size in $dbuck_sizes; do
                for sbuck_size in $sbuck_sizes; do
                    command="numactl --cpunodebind=0 --membind=0 ./bench_mrsw --output_path=mrsw.csv --keys_file=../datasets/${dataset} --keys_file_type=binary --read=${ratio%:*} --insert=${ratio#*:} --operations_num=100000000 --table_size=200000000 --init_table_ratio=0.5 --thread_num=${thread} --index=buckindex --memory --bli_initial_filled_ratio=0.6 --bli_sbuck_size=${sbuck_size} --bli_dbuck_size=${dbuck_size} --error_bound=4.8"
                    echo "command is $command"
                    $command
                done
            done

            # other indexes
            for index in $indexes; do
                command="numactl --cpunodebind=0 --membind=0 ./bench_mrsw --output_path=mrsw.csv --keys_file=../datasets/${dataset} --keys_file_type=binary --read=${ratio%:*} --insert=${ratio#*:} --operations_num=100000000 --table_size=200000000 --init_table_ratio=0.5 --thread_num=${thread} --index=${index} --memory"
                echo "command is $command"
                $command
            done
        done
    done
done