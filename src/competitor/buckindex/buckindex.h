#pragma once

#include"../indexInterface.h"
#include"buck_index.h"

// using namespace buckindex;

template<class KEY_TYPE, class PAYLOAD_TYPE>
class BuckIndexInterface : public indexInterface<KEY_TYPE, PAYLOAD_TYPE> {
public:
  using KeyValueType = buckindex::KeyValue<KEY_TYPE, PAYLOAD_TYPE>;
  void init(Param *param = nullptr) {
    idx.init(param->initial_filled_ratio, param->use_liner_regression, param->use_simd);
  }

  void bulk_load(std::pair <KEY_TYPE, PAYLOAD_TYPE> *key_value, size_t num, Param *param = nullptr);

  bool get(KEY_TYPE key, PAYLOAD_TYPE &val, Param *param = nullptr);

  bool put(KEY_TYPE key, PAYLOAD_TYPE value, Param *param = nullptr);

  bool update(KEY_TYPE key, PAYLOAD_TYPE value, Param *param = nullptr);

  bool remove(KEY_TYPE key, Param *param = nullptr);

  size_t scan(KEY_TYPE key_low_bound, size_t key_num, std::pair<KEY_TYPE, PAYLOAD_TYPE> *result,
              Param *param = nullptr) {
    return 0;
  }

  long long memory_consumption() {
    // Use this function to print for now
    idx.print_lookup_stat();
    //idx.print_insert_stats();
    idx.dump();
    return 0;
  }

private:
  buckindex::BuckIndex<KEY_TYPE, PAYLOAD_TYPE, 8, 256> idx; // TODO: add bucket size as command-line args
};

template<class KEY_TYPE, class PAYLOAD_TYPE>
void BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE>::bulk_load(std::pair <KEY_TYPE, PAYLOAD_TYPE> *key_value, size_t num,
                                                          Param *param) {
    vector<KeyValueType> kvs;

    KeyValueType kv1(std::numeric_limits<KEY_TYPE>::min(), 0);
    kvs.push_back(kv1);

    for (int i = 0; i < num; i++) {
        kvs.push_back(KeyValueType(key_value[i].first, key_value[i].second));
    }
    idx.bulk_load(kvs);
}

template<class KEY_TYPE, class PAYLOAD_TYPE>
bool BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE>::get(KEY_TYPE key, PAYLOAD_TYPE &val, Param *param) {
  bool ret = idx.lookup(key,val);
  if (ret == false) {
    printf("fail to read key:  %lu\n",key);
    return false;
  }
  return true;
}

template<class KEY_TYPE, class PAYLOAD_TYPE>
bool BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE>::put(KEY_TYPE key, PAYLOAD_TYPE value, Param *param) {
  KeyValueType kv(key, value);
  bool ret = idx.insert(kv);
  if (ret == false) {
    printf("fail to insert key:  %lu\n",key);
    return false;
  }
  return true;
}

template<class KEY_TYPE, class PAYLOAD_TYPE>
bool BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE>::update(KEY_TYPE key, PAYLOAD_TYPE value, Param *param) {
    return false;
}

template<class KEY_TYPE, class PAYLOAD_TYPE>
bool BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE>::remove(KEY_TYPE key, Param *param) {
  return false;
}

