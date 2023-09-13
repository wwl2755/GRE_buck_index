#!/bin/sh

# dataset
datasets="books fb osm"

# Read/Insert ratios
ratios="1:0"
# ratios="1:0 0:1 0.5:0.5"

# # sbuck_sizes
sbuck_sizes="8"

# dbuck_sizes
dbuck_sizes="16384"

# cores
# cores="0 1 2 3 4 5 6 7 8 9 10 11"
cores="0 1"

#indexes
indexes="alex lipp finedex pgm"

# # Iterate the string array using for loop
# for ratio in $ratios; do
#     for dataset in $datasets; do
#         # bli
#         for sbuck_size in $sbuck_sizes; do
#             for dbuck_size in $dbuck_sizes; do
#                 for c in $cores; do
#                     command="taskset -c $c ./microbench --keys_file=../datasets/${dataset} --keys_file_type=binary --read=${ratio%:*} --insert=${ratio#*:} --operations_num=100000000 --table_size=200000000 --init_table_ratio=0.5 --thread_num=1 --index=buckindex --memory --bli_sbuck_size=${sbuck_size} --bli_dbuck_size=${dbuck_size} --bli_initial_filled_ratio=0.6"
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



# ./release/microbench --keys_file=./datasets/books --keys_file_type=binary --read=1 --insert=0 --operations_num=100000000 --table_size=200000000 --init_table_ratio=0.5 --thread_num=1 --index=buckindex --memory --bli_sbuck_size=8 --bli_dbuck_size=4096 --bli_initial_filled_ratio=0.7
# ./release/microbench --keys_file=./datasets/fb --keys_file_type=binary --read=1 --insert=0 --operations_num=100000000 --table_size=200000000 --init_table_ratio=0.5 --thread_num=1 --index=buckindex --memory --bli_sbuck_size=8 --bli_dbuck_size=4096 --bli_initial_filled_ratio=0.7
# ./release/microbench --keys_file=./datasets/books --keys_file_type=binary --read=0 --insert=1 --operations_num=20000000 --table_size=20000000 --init_table_ratio=0.001 --thread_num=1 --index=buckindex --memory --bli_sbuck_size=8 --bli_dbuck_size=256
# ./release/microbench --keys_file=./datasets/books --keys_file_type=binary --read=0 --insert=1 --operations_num=10000000 --table_size=20000000 --init_table_ratio=0.001 --thread_num=1 --index=alex --memory
# ./release/microbench --keys_file=./datasets/books --keys_file_type=binary --read=0 --insert=1 --operations_num=10000000 --table_size=20000000 --init_table_ratio=0.5 --thread_num=1 --index=alex --memory


# repeatedly run the same experiment 20 times
# command="./microbench --keys_file=../datasets/books --keys_file_type=binary --read=1 --insert=0 --operations_num=100000000 --table_size=200000000 --init_table_ratio=0.5 --thread_num=1 --index=buckindex --memory --bli_sbuck_size=8 --bli_dbuck_size=16384 --bli_initial_filled_ratio=0.6"

# i=21
# while [ $i -le 40 ]
# do
#     echo "Run number $i"
#     command="vtune -collect uarch-exploration -r ../vtune_results/pure_read_kv_ref_uarch_$i -- ./microbench --keys_file=../datasets/books --keys_file_type=binary --read=1 --insert=0 --operations_num=100000000 --table_size=200000000 --init_table_ratio=0.5 --thread_num=1 --index=buckindex --memory --bli_sbuck_size=8 --bli_dbuck_size=16384 --bli_initial_filled_ratio=0.6"
#     # echo "command is $command"
#     $command
#     i=$((i+1))  # increment the counter
#     sleep 1  # Optional: wait for a second before the next run
# done


# command="./microbench --keys_file=../datasets/books --keys_file_type=binary --read=1 --insert=1 --operations_num=100000000 --table_size=200000000 --init_table_ratio=0.5 --thread_num=1 --index=buckindex --memory --bli_sbuck_size=8 --bli_dbuck_size=16384 --bli_initial_filled_ratio=0.6"

# cores="0 1"
# for c in $cores
# do
#     echo "Running on core $c" >> out.csv
#     i=1
#     while [ $i -le 5 ]
#     do
#         echo "Run number $i"
#         taskset -c $c $command
# 	    #$command
#         i=$((i+1))  # increment the counter
#         sleep 1  # Optional: wait for a second before the next run
#     done
# done


taskset -c 0 ./microbench --keys_file=../datasets/books --keys_file_type=binary --read=1 --insert0 --operations_num=100000000 --table_size=200000000 --init_table_ratio=0.5 --thread_num=1 --index=buckindex --memory --bli_sbuck_size=8 --bli_dbuck_size=16384 --bli_initial_filled_ratio=0.6 --error_bound=4.8