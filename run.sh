#!/bin/sh

# dataset
# datasets="books fb osm"
datasets="fb"

# Read/Insert ratios
ratios="1:0"
# ratios="1:0 0:1 0.5:0.5"

# # sbuck_sizes
sbuck_sizes="8"

# dbuck_sizes
dbuck_sizes="16384"

# cores
# cores="0 1 2 3 4 5 6 7 8 9 10 11"
cores="1"

#indexes
# indexes="alex lipp finedex pgm"
indexes="dili"


taskset -c 0 ./microbench --keys_file=../datasets/fb --keys_file_type=binary --read=1.0 --insert=0.0 --operations_num=10000000 --table_size=20000000 --init_table_ratio=0.5 --thread_num=1 --index=dili --memory

# # Iterate the string array using for loop
# for ratio in $ratios; do
#     for dataset in $datasets; do
#         # bli
#         for sbuck_size in $sbuck_sizes; do
#             for dbuck_size in $dbuck_sizes; do
#                 for c in $cores; do
#                     command="taskset -c $c ./microbench --keys_file=../datasets/${dataset} --keys_file_type=binary --read=${ratio%:*} --insert=${ratio#*:} --operations_num=10000000 --table_size=20000000 --init_table_ratio=0.5 --thread_num=1 --index=buckindex --memory --bli_sbuck_size=${sbuck_size} --bli_dbuck_size=${dbuck_size} --bli_initial_filled_ratio=0.6 --error_bound=4.8"
#                     echo "command is $command"
#                     taskset -c $c $command
#                     sleep 1
#                 done
#             done
#         done

#         # others
#         for index in $indexes; do
#             for c in $cores; do
#                 command="taskset -c $c ./microbench --keys_file=../datasets/${dataset} --keys_file_type=binary --read=${ratio%:*} --insert=${ratio#*:} --operations_num=100000000 --table_size=200000000 --init_table_ratio=0.5 --thread_num=1 --index=${index} --memory"
#                 echo "command is $command"
#                 taskset -c $c $command
#                 sleep 1
#             done
#         done
#     done
# done