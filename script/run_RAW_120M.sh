#!/bin/sh

# dataset
# datasets="books"
datasets="books"

# Read/Insert ratios
# ratios="1:0"
ratios="0.5:0.5"

operations_nums="240000000"

# # sbuck_sizes
sbuck_sizes="8"

# dbuck_sizes
dbuck_sizes="131072"
# dbuck_sizes="10485760 12582912 14680064"

# bli_merge_n_smo_thresholds="2 4 6 8 10 12 14 16 18 20 22 24 26 28 30 32"
bli_merge_n_smo_thresholds="1"

# # Iterate the string array using for loop
for ratio in $ratios; do
    for dataset in $datasets; do
        for operations_num in $operations_nums; do
            for bli_merge_n_smo_threshold in $bli_merge_n_smo_thresholds; do
                # bli
                for sbuck_size in $sbuck_sizes; do
                    for dbuck_size in $dbuck_sizes; do
                        command="numactl --cpunodebind=0 --membind=0  ./microbench_read_after_write --output_path=out_RAW_120M.csv --keys_file=../datasets/${dataset} --keys_file_type=binary --read=${ratio%:*} --insert=${ratio#*:} --operations_num=${operations_num} --table_size=200000000 --init_table_ratio=0.05 --thread_num=1 --index=buckindex --memory --bli_sbuck_size=${sbuck_size} --bli_dbuck_size=${dbuck_size} --bli_initial_filled_ratio=0.6  --error_bound=4.8 --bli_merge_n_smo_threshold=${bli_merge_n_smo_threshold} --bli_merge_window_size=2"
                        echo "command is $command"  >> RAW_120M.txt
                        $command >> RAW_120M.txt
                    done
                done
            done
        done
    done
done
