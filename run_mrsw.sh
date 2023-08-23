#!/bin/sh

# dataset
datasets="osm"

# Read/Insert ratios
ratios="1:0 0:1 0.5:0.5"

# sbuck_sizes
sbuck_sizes="8"

# dbuck_sizes
dbuck_sizes="16384"

# other index types
indexes="alexol, lippol"

# cores
# cores="1,2,3,4,5,6,7,8,9,10,11,12"
cores="1 2"

# threads
threads="2,3,4,5,6,7,8,9,10,11,12"

# Iterate the string array using for loop
for thread in $threads; do
    for ratio in $ratios; do
        for dataset in $datasets; do
            for dbuck_size in $dbuck_sizes; do
                for sbuck_size in $sbuck_sizes; do
                        command="./bench_mrsw --keys_file=../datasets/${dataset} --keys_file_type=binary --read=${ratio%:*} --insert=${ratio#*:} --operations_num=100000000 --table_size=200000000 --init_table_ratio=0.5 --thread_num=${thread} --index=buckindex --memory --bli_sbuck_size=${sbuck_size} --bli_dbuck_size=${dbuck_size} --error_bound=4.8"
                    for core in $cores; do
                        echo "Core $core, cmd is $command"
                        # taskset -c $core $command --bli_dbuck_size=$dbuck_size 
                        # sleep 1
                    done
                done
            done

            # other indexes
            for index in $indexes; do
                command="./bench_mrsw --keys_file=../datasets/${dataset} --keys_file_type=binary --read=${ratio%:*} --insert=${ratio#*:} --operations_num=100000000 --table_size=200000000 --init_table_ratio=0.5 --thread_num=${thread} --index=${index} --memory"
                for core in $cores; do
                    echo "Core $core, cmd is $command"
                    # taskset -c $core $command --bli_dbuck_size=$dbuck_size 
                    # sleep 1
                done 
            done
        done
    done
done