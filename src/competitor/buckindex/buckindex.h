#pragma once

#include"../indexInterface.h"
#include"buck_index.h"

// using namespace buckindex;

template<class KEY_TYPE, class PAYLOAD_TYPE>
class BuckIndexInterface : public indexInterface<KEY_TYPE, PAYLOAD_TYPE> {
public:
  using KeyValueType = buckindex::KeyValue<KEY_TYPE, PAYLOAD_TYPE>;
  void init(Param *param = nullptr){
    sbuck_size_ = param->sbuck_size;
    dbuck_size_ = param->dbuck_size;
    assert(sbuck_size_ == 4 || sbuck_size_ == 8 || sbuck_size_ == 16 || sbuck_size_ == 32);
    assert(dbuck_size_ == 256);

    switch (sbuck_size_) {
      case 4:
        idx4_.init(param->initial_filled_ratio, param->use_liner_regression, param->use_simd);
        break;
      case 8:
        idx8_.init(param->initial_filled_ratio, param->use_liner_regression, param->use_simd);
        break;
      case 16:
        idx16_.init(param->initial_filled_ratio, param->use_liner_regression, param->use_simd);
        break;
      case 32:
        idx32_.init(param->initial_filled_ratio, param->use_liner_regression, param->use_simd);
        break;
      default:
        assert(false);
    }
  }

  void bulk_load(std::pair <KEY_TYPE, PAYLOAD_TYPE> *key_value, size_t num, Param *param = nullptr);

  bool get(KEY_TYPE key, PAYLOAD_TYPE &val, Param *param = nullptr);

  bool put(KEY_TYPE key, PAYLOAD_TYPE value, Param *param = nullptr);

  bool update(KEY_TYPE key, PAYLOAD_TYPE value, Param *param = nullptr);

  bool remove(KEY_TYPE key, Param *param = nullptr);

  size_t scan(KEY_TYPE key_low_bound, size_t key_num, std::pair<KEY_TYPE, PAYLOAD_TYPE> *result,
              Param *param = nullptr);

  long long memory_consumption() {
    // Use this function to print for now
    // idx->print_lookup_stat();
    //idx->print_insert_stats();
    return 0;
  }

private:
  buckindex::BuckIndex<KEY_TYPE, PAYLOAD_TYPE, 8, 256> idx4_;
  buckindex::BuckIndex<KEY_TYPE, PAYLOAD_TYPE, 8, 256> idx8_;
  buckindex::BuckIndex<KEY_TYPE, PAYLOAD_TYPE, 16, 256> idx16_;
  buckindex::BuckIndex<KEY_TYPE, PAYLOAD_TYPE, 32, 256> idx32_;
  size_t sbuck_size_;
  size_t dbuck_size_;
  // buckindex::BuckIndex<KEY_TYPE, PAYLOAD_TYPE, 64, 256> *idx;
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

    switch (sbuck_size_) {
      case 4:
        idx4_.bulk_load(kvs);
        break;
      case 8:
        idx8_.bulk_load(kvs);
        break;
      case 16:
        idx16_.bulk_load(kvs);
        break;
      case 32:
        idx32_.bulk_load(kvs);
        break;
      default:
        assert(false);
    }
}

template<class KEY_TYPE, class PAYLOAD_TYPE>
bool BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE>::get(KEY_TYPE key, PAYLOAD_TYPE &val, Param *param) {
  bool ret;

  switch (sbuck_size_) {
    case 4:
      ret = idx4_.lookup(key, val);
      break;
    case 8:
      ret = idx8_.lookup(key, val);
      break;
    case 16:
      ret = idx16_.lookup(key, val);
      break;
    case 32:
      ret = idx32_.lookup(key, val);
      break;
    default:
      assert(false);
  }

  if (ret == false) {
    printf("fail to read key:  %lu\n",key);
    return false;
  }

  return true;
}

template<class KEY_TYPE, class PAYLOAD_TYPE>
bool BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE>::put(KEY_TYPE key, PAYLOAD_TYPE value, Param *param) {
  KeyValueType kv(key, value);
  bool ret;

  switch (sbuck_size_) {
    case 4:
      ret = idx4_.insert(kv);
      break;
    case 8:
      ret = idx8_.insert(kv);
      break;
    case 16:
      ret = idx16_.insert(kv);
      break;
    case 32:
      ret = idx32_.insert(kv);
      break;
    default:
      assert(false);
  }

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


template<class KEY_TYPE, class PAYLOAD_TYPE>
size_t BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE>::scan(KEY_TYPE key_low_bound, size_t key_num, std::pair<KEY_TYPE, PAYLOAD_TYPE> *result,
              Param *param) {
    switch (sbuck_size_) {
      case 4:
        return idx4_.scan(key_low_bound, key_num, result);
        break;
      case 8:
        return idx8_.scan(key_low_bound, key_num, result);
        break;
      case 16:
        return idx16_.scan(key_low_bound, key_num, result);
        break;
      case 32:
        return idx32_.scan(key_low_bound, key_num, result);
        break;
    
    default:
      break;
    }
    return 0;
}