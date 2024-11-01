#pragma once

#include"../indexInterface.h"
#include"buck_index.h"

// using namespace buckindex;

template<class KEY_TYPE, class PAYLOAD_TYPE, size_t SBUCKET_SIZE, size_t DBUCKET_SIZE>
class BuckIndexInterface : public indexInterface<KEY_TYPE, PAYLOAD_TYPE> {
public:
  using KeyValueType = buckindex::KeyValue<KEY_TYPE, PAYLOAD_TYPE>;
  void init(Param *param = nullptr){
    idx.init(param->initial_filled_ratio, param->error_bound);
  }

  void bulk_load(std::pair <KEY_TYPE, PAYLOAD_TYPE> *key_value, size_t num, Param *param = nullptr);

  bool get(KEY_TYPE key, PAYLOAD_TYPE &val, Param *param = nullptr);

  bool put(KEY_TYPE key, PAYLOAD_TYPE value, Param *param = nullptr);

  bool update(KEY_TYPE key, PAYLOAD_TYPE value, Param *param = nullptr);

  bool remove(KEY_TYPE key, Param *param = nullptr);

  size_t scan(KEY_TYPE key_low_bound, size_t key_num, std::pair<KEY_TYPE, PAYLOAD_TYPE> *result,
              Param *param = nullptr);
 
  long long memory_consumption() {
    // Use this function to print for now; TODO: change to memeory_consumption
    
    idx.print_lookup_stat();
    idx.dump();
  
    // TODO: change to memory_consumption; currently, it means the number of levels
    return idx.get_num_levels();
  }

private:
  buckindex::BuckIndex<KEY_TYPE, PAYLOAD_TYPE, SBUCKET_SIZE, DBUCKET_SIZE> idx;

};

template<class KEY_TYPE, class PAYLOAD_TYPE, size_t SBUCKET_SIZE, size_t DBUCKET_SIZE>
void BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, SBUCKET_SIZE, DBUCKET_SIZE>::bulk_load(std::pair <KEY_TYPE, PAYLOAD_TYPE> *key_value, size_t num,
                                                          Param *param) {
    vector<KeyValueType> kvs;

    if (key_value[0].first != 0){
        KeyValueType kv1(std::numeric_limits<KEY_TYPE>::min(), 0);
        kvs.push_back(kv1);
    }

    for (int i = 0; i < num; i++) {
        kvs.push_back(KeyValueType(key_value[i].first, key_value[i].second));
    }

    // // debug usgae
    // if(key_value[0].first == 0){
    //   std::cout << "key_value[0].first == 0" << std::endl;
    // }

    idx.bulk_load(kvs); 
}

template<class KEY_TYPE, class PAYLOAD_TYPE, size_t SBUCKET_SIZE, size_t DBUCKET_SIZE>
bool BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, SBUCKET_SIZE, DBUCKET_SIZE>::get(KEY_TYPE key, PAYLOAD_TYPE &val, Param *param) {
  bool ret;

  // // debug usage
  // if(key == 0){
  //   std::cout << "try to get key == 0" << std::endl;
  // }
  
  ret = idx.lookup(key, val);

  if (ret == false) {
    printf("fail to read key:  %lu\n",key);
    return false;
  }

  return true;
}

template<class KEY_TYPE, class PAYLOAD_TYPE, size_t SBUCKET_SIZE, size_t DBUCKET_SIZE>
bool BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, SBUCKET_SIZE, DBUCKET_SIZE>::put(KEY_TYPE key, PAYLOAD_TYPE value, Param *param) {
  KeyValueType kv(key, value);
  bool ret;

  // // debug usage
  // if(key == 0){
  //   std::cout << "try to put key == 0" << std::endl;
  // }

  ret = idx.insert(kv);

  if (ret == false) {
    printf("fail to insert key:  %lu\n",key);
    return false;
  }
  return true;
}

template<class KEY_TYPE, class PAYLOAD_TYPE, size_t SBUCKET_SIZE, size_t DBUCKET_SIZE>
bool BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, SBUCKET_SIZE, DBUCKET_SIZE>::update(KEY_TYPE key, PAYLOAD_TYPE value, Param *param) {
    return false;
}

template<class KEY_TYPE, class PAYLOAD_TYPE, size_t SBUCKET_SIZE, size_t DBUCKET_SIZE>
bool BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, SBUCKET_SIZE, DBUCKET_SIZE>::remove(KEY_TYPE key, Param *param) {
  return false;
}

template<class KEY_TYPE, class PAYLOAD_TYPE, size_t SBUCKET_SIZE, size_t DBUCKET_SIZE>
size_t BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, SBUCKET_SIZE, DBUCKET_SIZE>::scan(KEY_TYPE key_low_bound, size_t key_num, std::pair<KEY_TYPE, PAYLOAD_TYPE> *result,
              Param *param) {
  size_t ret;

  if (key_num <= 1000 || DBUCKET_SIZE <= 1024){
    ret = idx.scan(key_low_bound, key_num, result);
  } else {
    ret = idx.scan_parallel(key_low_bound, key_num, result);
  }
  
  return ret;
}