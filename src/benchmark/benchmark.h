#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <algorithm>
#include <atomic>
#include <memory>
#include <random>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <thread>
#include <ctime>

#include "../tscns.h"
#include "omp.h"
#include "tbb/parallel_sort.h"
#include "flags.h"
#include "utils.h"
#include "../competitor/competitor.h"
#include "../competitor/indexInterface.h"
#include "pgm_metric.h"
#include <jemalloc/jemalloc.h>
#include <sys/time.h>
#include <ittnotify.h> // Intel Vtune Profiling API


template<typename KEY_TYPE, typename PAYLOAD_TYPE>
class Benchmark {
    typedef indexInterface <KEY_TYPE, PAYLOAD_TYPE> index_t;

    enum Operation {
        READ = 0, INSERT, DELETE, SCAN, UPDATE
    };

    // parameters
    double read_ratio = 1;
    double insert_ratio = 0;
    double delete_ratio = 0;
    double update_ratio = 0;
    double scan_ratio = 0;
    size_t scan_num = 100;
    size_t operations_num;
    long long table_size = -1;
    size_t init_table_size;
    double init_table_ratio;
    double del_table_ratio;
    size_t thread_num = 1;
    std::vector <std::string> all_index_type;
    std::vector <std::string> all_thread_num;
    std::string index_type;
    std::string keys_file_path;
    std::string keys_file_type;
    std::string sample_distribution;
    bool latency_sample = false;
    double latency_sample_ratio = 0.01;
    int error_bound;
    std::string output_path;
    size_t random_seed;
    bool memory_record;
    bool dataset_statistic;
    bool data_shift = false;
    // buckindex parameters
    double bli_initial_filled_ratio = 0.5;
    size_t bli_sbuck_size = 8;
    size_t bli_dbuck_size = 256;
    int bli_merge_n_smo_threshold = 2;
    int bli_merge_window_size = 2;

    std::vector <KEY_TYPE> init_keys;
    KEY_TYPE *keys;
    std::pair <KEY_TYPE, PAYLOAD_TYPE> *init_key_values;
    size_t init_key_values_size;
    std::vector <std::pair<Operation, KEY_TYPE>> operations;
    std::mt19937 gen;

    struct Stat {
        std::vector<double> latency;
        std::vector<double> latency_read;
        std::vector<double> latency_write;
        uint64_t throughput = 0;
        uint64_t throughput_read = 0;
        uint64_t throughput_write = 0;
        size_t fitness_of_dataset = 0;
        long long memory_consumption = 0;
        uint64_t success_insert = 0;
        uint64_t success_read = 0;
        uint64_t success_update = 0;
        uint64_t success_remove = 0;
        uint64_t scan_not_enough = 0;

        void clear() {
            latency.clear();
            throughput = 0;
            fitness_of_dataset = 0;
            memory_consumption = 0;
            success_insert = 0;
            success_read = 0;
            success_update = 0;
            success_remove = 0;
            scan_not_enough = 0;
        }
    } stat;

    struct alignas(CACHELINE_SIZE)
    ThreadParam {
        std::vector<std::pair<uint64_t, uint64_t>> latency_read;
        std::vector<std::pair<uint64_t, uint64_t>> latency_write;
        std::vector<std::pair<uint64_t, uint64_t>> latency;
        uint64_t success_insert = 0;
        uint64_t success_read = 0;
        uint64_t success_update = 0;
        uint64_t success_remove = 0;
        uint64_t scan_not_enough = 0;
    };
    typedef ThreadParam param_t;
public:
    Benchmark() {
    }

    KEY_TYPE *load_keys() {
        // Read keys from file
        COUT_THIS("Reading data from file.");

        if (table_size > 0) keys = new KEY_TYPE[table_size];


        if (keys_file_type == "binary") {
            table_size = load_binary_data(keys, table_size, keys_file_path);
            if (table_size <= 0) {
                COUT_THIS("Could not open key file, please check the path of key file.");
                exit(0);
            }
        } else if (keys_file_type == "text") {
            table_size = load_text_data(keys, table_size, keys_file_path);
            if (table_size <= 0) {
                COUT_THIS("Could not open key file, please check the path of key file.");
                exit(0);
            }
        } else {
            COUT_THIS("Could not open key file, please check the path of key file.");
            exit(0);
        }

        if (!data_shift) {
            std::sort(keys, keys + table_size);
            // tbb::parallel_sort(keys, keys + table_size);
            auto last = std::unique(keys, keys + table_size);
            table_size = last - keys;
            std::shuffle(keys, keys + table_size, gen);
        }

        init_table_size = init_table_ratio * table_size;
        std::cout << "Table size is " << table_size << ", Init table size is " << init_table_size << std::endl;

        for (auto j = 0; j < 10; j++) {
            std::cout << keys[j] << " ";
        }
        std::cout << std::endl;

        // prepare data
        COUT_THIS("prepare init keys.");
        init_keys.resize(init_table_size);
// #pragma omp parallel for num_threads(thread_num)
        for (size_t i = 0; i < init_table_size; ++i) {
            init_keys[i] = (keys[i]);
        }
        std::sort(init_keys.begin(), init_keys.end());
        // tbb::parallel_sort(init_keys.begin(), init_keys.end());

        std::cout << "Init keys: min = " << init_keys[0] << ", max = " << init_keys[init_keys.size() - 1] << std::endl;

        init_key_values = new std::pair<KEY_TYPE, PAYLOAD_TYPE>[init_keys.size()];
// #pragma omp parallel for num_threads(thread_num)
        for (int i = 0; i < init_keys.size(); i++) {
            init_key_values[i].first = init_keys[i];
            init_key_values[i].second = 123456789; // TODO: change to key
        }
        COUT_VAR(table_size);
        COUT_VAR(init_keys.size());

        init_key_values_size = init_keys.size();
        init_keys.clear();

        return keys;
    }

    inline void prepare(index_t *&index, const KEY_TYPE *keys) {
        // initilize Index (sort keys first)
        Param param = Param(thread_num, 0,
            bli_initial_filled_ratio,
            bli_sbuck_size, bli_dbuck_size, error_bound,
            bli_merge_n_smo_threshold, bli_merge_window_size);
        
        index = get_index<KEY_TYPE, PAYLOAD_TYPE>(index_type, &param);
        index->init(&param);

        // deal with the background thread case
        thread_num = param.worker_num;

        COUT_THIS("bulk loading");

        struct timeval startTime, endTime;
        long time = 0;
        gettimeofday(&startTime, 0);

        index->bulk_load(init_key_values, init_key_values_size, &param);

        gettimeofday(&endTime, 0);
        time += (endTime.tv_sec - startTime.tv_sec) * 1000000 + (endTime.tv_usec - startTime.tv_usec);
        COUT_THIS("[bulk loading time]: " << time / 1000000.0 << "s");
        COUT_THIS("[bulk loading throughput]: " << static_cast<uint64_t>(init_key_values_size / (time / 1000000.0)) << " keys/sec");
    }

    /*
   * keys_file_path:      the path where keys file at
   * keys_file_type:      binary or text
   * read_ratio:          the ratio of read operation
   * insert_ratio         the ratio of insert operation
   * delete_ratio         the ratio of delete operation
   * update_ratio         the ratio of update operation
   * scan_ratio           the ratio of scan operation
   * scan_num             the number of keys that every scan operation need to scan
   * operations_num      the number of operations(read, insert, delete, update, scan)
   * table_size           the total number of keys in key file
   * init_table_size      the number of keys that will be used in bulk loading
   * thread_num           the number of worker thread
   * index_type           the type of index(xindex, hot, alex...). Detail could be refered to src/competitor
   * sample_distribution  the distribution of
   * latency_sample_ratio the ratio of latency sampling
   * error_bound          the error bound of PGM metric
   * output_path          the path to store result
  */
    inline void parse_args(int argc, char **argv) {
        auto flags = parse_flags(argc, argv);
        keys_file_path = get_required(flags, "keys_file"); // required
        keys_file_type = get_with_default(flags, "keys_file_type", "binary");
        read_ratio = stod(get_required(flags, "read")); // required
        insert_ratio = stod(get_with_default(flags, "insert", "0")); // required
        delete_ratio = stod(get_with_default(flags, "delete", "0"));
        update_ratio = stod(get_with_default(flags, "update", "0"));
        scan_ratio = stod(get_with_default(flags, "scan", "0"));
        scan_num = stoi(get_with_default(flags, "scan_num", "100"));
        operations_num = stoi(get_with_default(flags, "operations_num", "1000000000")); // required
        table_size = stoi(get_with_default(flags, "table_size", "-1"));
        init_table_ratio = stod(get_with_default(flags, "init_table_ratio", "0.5"));
        del_table_ratio = stod(get_with_default(flags, "del_table_ratio", "0.5"));
        init_table_size = 0;
        all_thread_num = get_comma_separated(flags, "thread_num"); // required
        all_index_type = get_comma_separated(flags, "index"); // required
        sample_distribution = get_with_default(flags, "sample_distribution", "uniform");
        latency_sample = get_boolean_flag(flags, "latency_sample");
        latency_sample_ratio = stod(get_with_default(flags, "latency_sample_ratio", "0.01"));
        error_bound = stoi(get_with_default(flags, "error_bound", "64"));
        output_path = get_with_default(flags, "output_path", "./out.csv");
        random_seed = stoul(get_with_default(flags, "seed", "1866"));
        gen.seed(random_seed);
        memory_record = get_boolean_flag(flags, "memory");
        dataset_statistic = get_boolean_flag(flags, "dataset_statistic");
        data_shift = get_boolean_flag(flags, "data_shift");
        bli_initial_filled_ratio = stod(get_with_default(flags, "bli_initial_filled_ratio", "0.5"));
        bli_sbuck_size = stoi(get_with_default(flags, "bli_sbuck_size", "8"));
        bli_dbuck_size = stoi(get_with_default(flags, "bli_dbuck_size", "256"));
        bli_merge_n_smo_threshold = stoi(get_with_default(flags, "bli_merge_n_smo_threshold", "2"));
        bli_merge_window_size = stoi(get_with_default(flags, "bli_merge_window_size", "2"));

        COUT_THIS("[micro] Read:Insert:Update:Scan:Delete= " << read_ratio << ":" << insert_ratio << ":" << update_ratio << ":"
                                                      << scan_ratio << ":" << delete_ratio);
        double ratio_sum = read_ratio + insert_ratio + delete_ratio + update_ratio + scan_ratio;
        double insert_delete = insert_ratio + delete_ratio;
        INVARIANT(insert_delete == insert_ratio || insert_delete == delete_ratio);
        INVARIANT(ratio_sum > 0.9999 && ratio_sum < 1.0001);  // avoid precision lost
        INVARIANT(sample_distribution == "zipf" || sample_distribution == "uniform");
        INVARIANT(all_thread_num.size() > 0);
    }


    void generate_operations(KEY_TYPE *keys) {
        // prepare operations
        operations.reserve(operations_num);
        COUT_THIS("sample keys.");
        KEY_TYPE *sample_ptr = nullptr;
        if (sample_distribution == "uniform") {
            sample_ptr = get_search_keys(&init_keys[0], init_table_size, operations_num, &random_seed);
        } else if (sample_distribution == "zipf") {
            sample_ptr = get_search_keys_zipf(&init_keys[0], init_table_size, operations_num, &random_seed);
        }

        // generate operations(read, insert, update, scan)
        COUT_THIS("generate operations.");
        std::uniform_real_distribution<> ratio_dis(0, 1);
        size_t sample_counter = 0, insert_counter = init_table_size;
        size_t delete_counter = table_size * (1 - del_table_ratio);

        if (data_shift) {
            size_t rest_key_num = table_size - init_table_size;
            if(rest_key_num > 0) {
                std::sort(keys + init_table_size, keys + table_size);
                std::random_shuffle(keys + init_table_size, keys + table_size);
            }
        }
        
        size_t temp_counter = 0;
        for (size_t i = 0; i < operations_num; ++i) {
            auto prob = ratio_dis(gen);
            if (prob < read_ratio) {
                // if (temp_counter >= table_size) {
                //     operations_num = i;
                //     break;
                // }
                // operations.push_back(std::pair<Operation, KEY_TYPE>(READ, keys[temp_counter++]));
                operations.push_back(std::pair<Operation, KEY_TYPE>(READ, sample_ptr[sample_counter++]));
            } else if (prob < read_ratio + insert_ratio) {
                if (insert_counter >= table_size) {
                    operations_num = i;
                    break;
                }
                operations.push_back(std::pair<Operation, KEY_TYPE>(INSERT, keys[insert_counter++]));
            } else if (prob < read_ratio + insert_ratio + update_ratio) {
                operations.push_back(std::pair<Operation, KEY_TYPE>(UPDATE, sample_ptr[sample_counter++]));
            } else if (prob < read_ratio + insert_ratio + update_ratio + scan_ratio) {
                operations.push_back(std::pair<Operation, KEY_TYPE>(SCAN, sample_ptr[sample_counter++]));
            } else {
                if (delete_counter >= table_size) {
                    operations_num = i;
                    break;
                }
                operations.push_back(std::pair<Operation, KEY_TYPE>(DELETE, keys[delete_counter++]));
                // operations.push_back(std::pair<Operation, KEY_TYPE>(DELETE, sample_ptr[sample_counter++]));
            }
        }

        COUT_VAR(operations.size());

        delete[] sample_ptr;
        delete[] keys;
    }

    void generate_operations_read_after_write(KEY_TYPE *keys) {
        // prepare operations
        operations.reserve(operations_num);

        // generate operations(read, insert, update, scan)
        COUT_THIS("generate operations.");
        std::uniform_real_distribution<> ratio_dis(0, 1);
        size_t sample_counter = 0, insert_counter = init_table_size;
        size_t delete_counter = table_size * (1 - del_table_ratio);

        if (data_shift) {
            size_t rest_key_num = table_size - init_table_size;
            if(rest_key_num > 0) {
                std::sort(keys + init_table_size, keys + table_size);
                std::random_shuffle(keys + init_table_size, keys + table_size);
            }
        }
        
        size_t temp_counter = 0;
        // generate write operations
        int n_write_ops = operations_num * insert_ratio;
        int n_read_ops = operations_num * read_ratio;
        for (int i = 0; i < n_write_ops; ++i) {
            if (insert_counter >= table_size) {
                operations_num = i;
                break;
            }
            operations.push_back(std::pair<Operation, KEY_TYPE>(INSERT, keys[insert_counter++]));
        }

        COUT_THIS("sample keys."); 
        KEY_TYPE *sample_ptr = nullptr;
        if (sample_distribution == "uniform") {
            sample_ptr = get_search_keys(keys + init_table_size, n_write_ops, n_read_ops, &random_seed);
        } else if (sample_distribution == "zipf") {
            sample_ptr = get_search_keys_zipf(keys + init_table_size, n_write_ops, n_read_ops, &random_seed);
        }

        for (size_t i = 0; i < n_read_ops; ++i) {
            operations.push_back(std::pair<Operation, KEY_TYPE>(READ, sample_ptr[sample_counter++]));
        }

        COUT_VAR(operations.size());

        delete[] sample_ptr;
        delete[] keys;
    }

    void run(index_t *index) {
        std::thread *thread_array = new std::thread[thread_num];
        param_t params[thread_num];
        TSCNS tn;
        tn.init();
        printf("Begin running\n");
        auto start_time = tn.rdtsc();
        auto end_time = tn.rdtsc();
    //    System::profile("perf.data", [&]() {
#pragma omp parallel num_threads(thread_num)
        {
            // thread specifier
            auto thread_id = omp_get_thread_num();
            // auto thread_id = 0;
            auto paramI = Param(thread_num, thread_id,
                bli_initial_filled_ratio,
                0, 0, 0,
                0, 0);
            // Latency Sample Variable
            int latency_sample_interval = operations_num / (operations_num * latency_sample_ratio);
            auto latency_sample_start_time = tn.rdtsc();
            auto latency_sample_end_time = tn.rdtsc();
            param_t &thread_param = params[thread_id];
            thread_param.latency.reserve(operations_num / latency_sample_interval);
            // Operation Parameter
            PAYLOAD_TYPE val;
            std::pair <KEY_TYPE, PAYLOAD_TYPE> *scan_result = new std::pair<KEY_TYPE, PAYLOAD_TYPE>[scan_num];
            // waiting all thread ready
#pragma omp barrier
#pragma omp master
            start_time = tn.rdtsc();
// running benchmark
#pragma omp for schedule(dynamic, 10000)
            for (auto i = 0; i < operations_num; i++) {
                auto op = operations[i].first;
                auto key = operations[i].second;

                if (latency_sample && i % latency_sample_interval == 0)
                    latency_sample_start_time = tn.rdtsc();

                if (op == READ) {  // get
                    auto ret = index->get(key, val, &paramI);
                    // if(!ret) {
                    //     printf("read not found, Key %lu\n",key);
                    //     continue;
                    // }
                    if(val != 123456789) {
                        printf("read failed, Key %lu, val %llu\n",key, val);
                        // exit(1);
                    }
                    thread_param.success_read += ret;
                } else if (op == INSERT) {  // insert
                    auto ret = index->put(key, 123456789, &paramI);
                    thread_param.success_insert += ret;
                } else if (op == UPDATE) {  // update
                    auto ret = index->update(key, 234567891, &paramI);
                    thread_param.success_update += ret;
                } else if (op == SCAN) { // scan
                    auto scan_len = index->scan(key, scan_num, scan_result, &paramI);
                    if (scan_len != scan_num) {
                        thread_param.scan_not_enough++;
                    }
                } else if (op == DELETE) { // delete
                    auto ret = index->remove(key, &paramI);
                    thread_param.success_remove += ret;
                }

                if (latency_sample && i % latency_sample_interval == 0) {
                    latency_sample_end_time = tn.rdtsc();
                    thread_param.latency.push_back(std::make_pair(latency_sample_start_time, latency_sample_end_time));
                }
            } // omp for loop
#pragma omp master
            end_time = tn.rdtsc();
        } // all thread join here

    //    });
        auto diff = tn.tsc2ns(end_time) - tn.tsc2ns(start_time);
        printf("Finish running\n");


        // gather thread local variable
        for (auto &p: params) {
            if (latency_sample) {
                for (auto e : p.latency) {
                    auto temp = (tn.tsc2ns(e.first) - tn.tsc2ns(e.second)) / (double) 1000000000;
                    stat.latency.push_back(tn.tsc2ns(e.second) - tn.tsc2ns(e.first));
                }
            }
            stat.success_read += p.success_read;
            stat.success_insert += p.success_insert;
            stat.success_update += p.success_update;
            stat.success_remove += p.success_remove;
            stat.scan_not_enough += p.scan_not_enough;
        }
        // calculate throughput
        stat.throughput = static_cast<uint64_t>(operations_num / (diff/(double) 1000000000));

        // calculate dataset metric
        if (dataset_statistic) {
            std::sort(keys, keys + table_size);
            stat.fitness_of_dataset = pgmMetric::PGM_metric(keys, table_size, error_bound);
        }

        // record memory consumption
        if (memory_record)
            stat.memory_consumption = index->memory_consumption();

        print_stat();

        delete[] thread_array;
    }

    void run_read_after_write(index_t *index) {
        std::thread *thread_array = new std::thread[thread_num];
        param_t params[thread_num];
        TSCNS tn;
        tn.init();
        printf("Begin running\n");
        auto start_time_read = tn.rdtsc();
        auto end_time_read = tn.rdtsc();
        auto start_time_write = tn.rdtsc();
        auto end_time_write = tn.rdtsc();
    //    System::profile("perf.data", [&]() {
#pragma omp parallel num_threads(thread_num)
        {
            // thread specifier
            auto thread_id = omp_get_thread_num();
            // auto thread_id = 0;
            auto paramI = Param(thread_num, thread_id,
                bli_initial_filled_ratio,
                0, 0, 0,
                0, 0);
            // Latency Sample Variable
            int latency_sample_interval = operations_num / (operations_num * latency_sample_ratio);
            auto latency_sample_start_time = tn.rdtsc();
            auto latency_sample_end_time = tn.rdtsc();
            param_t &thread_param = params[thread_id];
            thread_param.latency.reserve(operations_num / latency_sample_interval);
            // Operation Parameter
            PAYLOAD_TYPE val;
            std::pair <KEY_TYPE, PAYLOAD_TYPE> *scan_result = new std::pair<KEY_TYPE, PAYLOAD_TYPE>[scan_num];
            // waiting all thread ready
            start_time_write = tn.rdtsc();
// running read benchmark
            for (int i = 0; i < operations_num * insert_ratio; i++) {
                auto op = operations[i].first;
                auto key = operations[i].second;

                if (latency_sample && i % latency_sample_interval == 0)
                    latency_sample_start_time = tn.rdtsc();

                if (op == INSERT) {  // insert
                    auto ret = index->put(key, 123456789, &paramI);
                    thread_param.success_insert += ret;
                } else {
                    cerr << "Error: read after write benchmark, the first " << operations_num * insert_ratio << " operations should be insert operation." << endl;
                    cerr << "i = " << i << ", op = " << op << ", key = " << key << endl;
                    assert(false);
                }

                if (latency_sample && i % latency_sample_interval == 0) {
                    latency_sample_end_time = tn.rdtsc();
                    thread_param.latency.push_back(std::make_pair(latency_sample_start_time, latency_sample_end_time));
                    thread_param.latency_write.push_back(std::make_pair(latency_sample_start_time, latency_sample_end_time));
                }
            } // omp for loop
            end_time_write = tn.rdtsc();
            start_time_read = tn.rdtsc();
// running write benchmark
            for (int i = operations_num * insert_ratio; i < operations_num; i++) {
                auto op = operations[i].first;
                auto key = operations[i].second;

                if (latency_sample && i % latency_sample_interval == 0)
                    latency_sample_start_time = tn.rdtsc();

                if (op == READ) {  // get
                    auto ret = index->get(key, val, &paramI);
                    // if(!ret) {
                    //     printf("read not found, Key %lu\n",key);
                    //     continue;
                    // }
                    if(val != 123456789) {
                        printf("read failed, Key %lu, val %llu\n",key, val);
                        // exit(1);
                    }
                    thread_param.success_read += ret;
                } else {
                    cerr << "Error: read after write benchmark, the next " << operations_num * read_ratio << " operations should be read operation." << endl;
                    cerr << "i = " << i << ", op = " << op << ", key = " << key << endl;
                    assert(false);
                }

                if (latency_sample && i % latency_sample_interval == 0) {
                    latency_sample_end_time = tn.rdtsc();
                    thread_param.latency.push_back(std::make_pair(latency_sample_start_time, latency_sample_end_time));
                    thread_param.latency_read.push_back(std::make_pair(latency_sample_start_time, latency_sample_end_time));
                }
            } // omp for loop
            end_time_read = tn.rdtsc();
        } // all thread join here

    //    });
        auto diff_read = tn.tsc2ns(end_time_read) - tn.tsc2ns(start_time_read);
        auto diff_write = tn.tsc2ns(end_time_write) - tn.tsc2ns(start_time_write);
        auto diff = diff_read + diff_write;
        printf("Finish running\n");


        // gather thread local variable
        for (auto &p: params) {
            if (latency_sample) {
                for (auto e : p.latency) {
                    auto temp = (tn.tsc2ns(e.first) - tn.tsc2ns(e.second)) / (double) 1000000000;
                    stat.latency.push_back(tn.tsc2ns(e.second) - tn.tsc2ns(e.first));
                }
                for (auto e : p.latency_read) {
                    auto temp = (tn.tsc2ns(e.first) - tn.tsc2ns(e.second)) / (double) 1000000000;
                    stat.latency_read.push_back(tn.tsc2ns(e.second) - tn.tsc2ns(e.first));
                }
                for (auto e : p.latency_write) {
                    auto temp = (tn.tsc2ns(e.first) - tn.tsc2ns(e.second)) / (double) 1000000000;
                    stat.latency_write.push_back(tn.tsc2ns(e.second) - tn.tsc2ns(e.first));
                }
                
            }
            stat.success_read += p.success_read;
            stat.success_insert += p.success_insert;
            stat.success_update += p.success_update;
            stat.success_remove += p.success_remove;
            stat.scan_not_enough += p.scan_not_enough;
        }
        // calculate throughput
        stat.throughput = static_cast<uint64_t>(operations_num / (diff/(double) 1000000000));
        stat.throughput_read = static_cast<uint64_t>(operations_num * read_ratio / (diff_read/(double) 1000000000));
        stat.throughput_write = static_cast<uint64_t>(operations_num * insert_ratio / (diff_write/(double) 1000000000));

        // calculate dataset metric
        if (dataset_statistic) {
            std::sort(keys, keys + table_size);
            stat.fitness_of_dataset = pgmMetric::PGM_metric(keys, table_size, error_bound);
        }

        // record memory consumption
        if (memory_record)
            stat.memory_consumption = index->memory_consumption();

        print_stat();

        delete[] thread_array;
    }


    void print_stat(bool header = false, bool clear_flag = true) {
        double avg_latency = 0;
        // average latency
        for (auto t : stat.latency) {
            avg_latency += t;
        }
        avg_latency /= stat.latency.size();

        // average read latency
        double avg_latency_read = 0;
        for (auto t : stat.latency_read) {
            avg_latency_read += t;
        }
        avg_latency_read /= stat.latency_read.size();

        // average write latency
        double avg_latency_write = 0;
        for (auto t : stat.latency_write) {
            avg_latency_write += t;
        }
        avg_latency_write /= stat.latency_write.size();

        // latency variance
        double latency_variance = 0;
        if (latency_sample) {
            for (auto t : stat.latency) {
                latency_variance += (t - avg_latency) * (t - avg_latency);
            }
            latency_variance /= stat.latency.size();
            std::sort(stat.latency.begin(), stat.latency.end());
        }

        // read latency variance
        double latency_variance_read = 0;
        if (latency_sample) {
            for (auto t : stat.latency_read) {
                latency_variance_read += (t - avg_latency_read) * (t - avg_latency_read);
            }
            latency_variance_read /= stat.latency_read.size();
            std::sort(stat.latency_read.begin(), stat.latency_read.end());
        }

        // write latency variance
        double latency_variance_write = 0;
        if (latency_sample) {
            for (auto t : stat.latency_write) {
                latency_variance_write += (t - avg_latency_write) * (t - avg_latency_write);
            }
            latency_variance_write /= stat.latency_write.size();
            std::sort(stat.latency_write.begin(), stat.latency_write.end());
        }

        printf("[Throughput = %llu]\n", stat.throughput);
        printf("[Throughput Read = %llu]\n", stat.throughput_read);
        printf("[Throughput Write = %llu]\n", stat.throughput_write);
        printf("Memory: %lld\n", stat.memory_consumption);
        printf("success_read: %llu\n", stat.success_read);
        printf("success_insert: %llu\n", stat.success_insert);
        printf("success_update: %llu\n", stat.success_update);
        printf("success_remove: %llu\n", stat.success_remove);
        printf("scan_not_enough: %llu\n", stat.scan_not_enough);

        // time id
        std::time_t t = std::time(nullptr);
        char time_str[100];

        if (!file_exists(output_path)) {
            std::ofstream ofile;
            ofile.open(output_path, std::ios::app);
            ofile << "id" << ",";
            ofile << "operations_num" << ",";
            ofile << "read_ratio" << "," << "insert_ratio" << "," << "update_ratio" << "," << "scan_ratio" << "," << "delete_ratio" << ",";
            ofile << "key_path" << ",";
            ofile << "index_type" << ",";
            ofile << "throughput" << ",";
            ofile << "throughput_read" << ",";
            ofile << "throughput_write" << ",";
            ofile << "init_table_size" << ",";
            ofile << "memory_consumption" << ",";
            ofile << "thread_num" << ",";
            ofile << "min" << ",";
            ofile << "50 percentile" << ",";
            ofile << "90 percentile" << ",";
            ofile << "99 percentile" << ",";
            ofile << "99.9 percentile" << ",";
            ofile << "99.99 percentile" << ",";
            ofile << "max" << ",";
            ofile << "avg" << ",";
            ofile << "seed" << ",";
            ofile << "scan_num" << ",";
            ofile << "latency_variance" << ",";
            ofile << "latency_sample" << ",";
            ofile << "data_shift" << ",";
            ofile << "pgm" << ",";
            ofile << "error_bound" ",";
            ofile << "table_size" << ",";

            // if index_type == buckindex, output its parameters
            if (index_type == "buckindex") {
                ofile << "bli_initial_filled_ratio" << ",";
                ofile << "bli_sbuck_size" << ",";
                ofile << "bli_dbuck_size" << ",";
            }
            ofile << std::endl;
            
            std::cout << std::endl;
        }

        std::ofstream ofile;
        ofile.open(output_path, std::ios::app);
        if (std::strftime(time_str, sizeof(time_str), "%Y%m%d%H%M%S", std::localtime(&t))) {
            ofile << time_str << ',';
        }
        ofile << operations_num << ",";
        ofile << read_ratio << "," << insert_ratio << "," << update_ratio << "," << scan_ratio << "," << delete_ratio << ",";

        ofile << keys_file_path << ",";
        ofile << index_type << ",";
        ofile << stat.throughput << ",";
        ofile << stat.throughput_read << ",";
        ofile << stat.throughput_write << ",";
        ofile << init_table_size << ",";
        ofile << stat.memory_consumption << ",";
        ofile << thread_num << ",";
        if (latency_sample) {
            ofile << stat.latency[0] << ",";
            ofile << stat.latency[0.5 * stat.latency.size()] << ",";
            ofile << stat.latency[0.9 * stat.latency.size()] << ",";
            ofile << stat.latency[0.99 * stat.latency.size()] << ",";
            ofile << stat.latency[0.999 * stat.latency.size()] << ",";
            ofile << stat.latency[0.9999 * stat.latency.size()] << ",";
            ofile << stat.latency[stat.latency.size() - 1] << ",";
            ofile << avg_latency << ",";
        } else {
            ofile << 0 << ",";
            ofile << 0 << ",";
            ofile << 0 << ",";
            ofile << 0 << ",";
            ofile << 0 << ",";
            ofile << 0 << ",";
            ofile << 0 << ",";
            ofile << 0 << ",";
        }
        ofile << random_seed << ",";
        ofile << scan_num << ",";
        ofile << latency_variance << ",";
        ofile << latency_sample << ",";
        ofile << data_shift << ",";
        ofile << stat.fitness_of_dataset << ",";
        ofile << error_bound << ",";
        ofile << table_size << ",";
        if (index_type == "buckindex") {
            ofile << bli_initial_filled_ratio << ",";
            ofile << bli_sbuck_size << ",";
            ofile << bli_dbuck_size << ",";
        }
        ofile << std::endl;
        ofile.close();

        if (clear_flag) stat.clear();
    }

    void run_benchmark() {
        // __itt_domain* domain = __itt_domain_create("MyDomain");
        load_keys();
        generate_operations(keys);
        // __itt_resume();
        for (auto s: all_index_type) {
            COUT_THIS("index type: " << s);
            for (auto t: all_thread_num) {
                thread_num = stoi(t);
                index_type = s;
                index_t *index;
                prepare(index, keys);
                // __itt_string_handle* task = __itt_string_handle_create("MyTask");
                // __itt_task_begin(domain, __itt_null, __itt_null, task);
                run(index);
                // __itt_task_end(domain);
                if (index != nullptr) delete index;
            }
            COUT_THIS("--------------------");
        }

        delete[] init_key_values;
        // __itt_pause();
    }

    void run_benchmark_read_after_write() {
        // __itt_domain* domain = __itt_domain_create("MyDomain");
        load_keys();
        generate_operations_read_after_write(keys);
        // __itt_resume();
        for (auto s: all_index_type) {
            COUT_THIS("index type: " << s);
            for (auto t: all_thread_num) {
                thread_num = stoi(t);
                index_type = s;
                index_t *index;
                prepare(index, keys);
                // __itt_string_handle* task = __itt_string_handle_create("MyTask");
                // __itt_task_begin(domain, __itt_null, __itt_null, task);
                run_read_after_write(index);
                // __itt_task_end(domain);
                if (index != nullptr) delete index;
            }
            COUT_THIS("--------------------");
        }

        delete[] init_key_values;
        // __itt_pause();
    }


};
