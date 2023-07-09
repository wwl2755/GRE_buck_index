#!/bin/sh

# dataset
datasets="books"

# Read/Insert ratios
ratios="1:0 0:1 0.5:0.5"

# sbuck_sizes
sbuck_sizes="8"

# dbuck_sizes
dbuck_sizes="4096"

# # Iterate the string array using for loop
# for ratio in $ratios; do
#     for dataset in $datasets; do
#         for sbuck_size in $sbuck_sizes; do
#             for dbuck_size in $dbuck_sizes; do
#                 ./release/microbench --keys_file=./datasets/${dataset} --keys_file_type=binary --read=${ratio%:*} --insert=${ratio#*:} --operations_num=10000000 --table_size=20000000 --init_table_ratio=0.5 --thread_num=1 --index=buckindex --memory --bli_sbuck_size=${sbuck_size} --bli_dbuck_size=${dbuck_size}
#             done
#         done
#     done
# done

# for ratio in $ratios; do
#     for dataset in $datasets; do
#         ./release/microbench --keys_file=./datasets/${dataset} --keys_file_type=binary --read=${ratio%:*} --insert=${ratio#*:} --operations_num=100000000 --table_size=200000000 --init_table_ratio=0.25 --thread_num=1 --index=alex,alexol --memory
#     done
# done


./microbench --keys_file=../datasets/osm --keys_file_type=binary --read=1 --insert=0 --operations_num=100000000 --table_size=200000000 --init_table_ratio=0.5 --thread_num=1 --index=buckindex --memory --bli_sbuck_size=8 --bli_dbuck_size=4096 --error_bound=128
# ./microbench --keys_file=../datasets/fb --keys_file_type=binary --read=1 --insert=0 --operations_num=100000000 --table_size=200000000 --init_table_ratio=0.5 --thread_num=1 --index=buckindex --memory --bli_sbuck_size=8 --bli_dbuck_size=256
# ./release/microbench --keys_file=./datasets/books --keys_file_type=binary --read=0 --insert=1 --operations_num=20000000 --table_size=20000000 --init_table_ratio=0.001 --thread_num=1 --index=buckindex --memory --bli_sbuck_size=8 --bli_dbuck_size=256
# ./release/microbench --keys_file=./datasets/books --keys_file_type=binary --read=0 --insert=1 --operations_num=10000000 --table_size=20000000 --init_table_ratio=0.001 --thread_num=1 --index=alex --memory
# ./release/microbench --keys_file=./datasets/books --keys_file_type=binary --read=0 --insert=1 --operations_num=10000000 --table_size=20000000 --init_table_ratio=0.5 --thread_num=1 --index=alex --memory
