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
    assert(dbuck_size_ == 32 || dbuck_size_ == 64 || dbuck_size_ == 128 || dbuck_size_ == 256);

    // TODO: change switch to xxx
    if (dbuck_size_ == 256) {
      switch (sbuck_size_) {
        case 4:
          idx_s4_.init(param->initial_filled_ratio, param->use_liner_regression, param->use_simd);
          break;
        case 8:
          idx_s8_.init(param->initial_filled_ratio, param->use_liner_regression, param->use_simd);
          break;
        case 16:
          idx_s16_.init(param->initial_filled_ratio, param->use_liner_regression, param->use_simd);
          break;
        case 32:
          idx_s32_.init(param->initial_filled_ratio, param->use_liner_regression, param->use_simd);
          break;
        default:
          assert(false);
      }
    } else {
      switch (dbuck_size_) {
        case 32:
          idx_d32_.init(param->initial_filled_ratio, param->use_liner_regression, param->use_simd);
          break;
        case 64:
          idx_d64_.init(param->initial_filled_ratio, param->use_liner_regression, param->use_simd);
          break;
        case 128:
          idx_d128_.init(param->initial_filled_ratio, param->use_liner_regression, param->use_simd);
          break;
        case 256:
          idx_d256_.init(param->initial_filled_ratio, param->use_liner_regression, param->use_simd);
          break;
        default:
          assert(false);
      }
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
  // indexes with different s-bucket sizes
  buckindex::BuckIndex<KEY_TYPE, PAYLOAD_TYPE, 4, 256> idx_s4_;
  buckindex::BuckIndex<KEY_TYPE, PAYLOAD_TYPE, 8, 256> idx_s8_;
  buckindex::BuckIndex<KEY_TYPE, PAYLOAD_TYPE, 16, 256> idx_s16_;
  buckindex::BuckIndex<KEY_TYPE, PAYLOAD_TYPE, 32, 256> idx_s32_;

  // indexes with different d-bucket sizes
  buckindex::BuckIndex<KEY_TYPE, PAYLOAD_TYPE, 8, 32> idx_d32_;
  buckindex::BuckIndex<KEY_TYPE, PAYLOAD_TYPE, 8, 64> idx_d64_;
  buckindex::BuckIndex<KEY_TYPE, PAYLOAD_TYPE, 8, 128> idx_d128_;
  buckindex::BuckIndex<KEY_TYPE, PAYLOAD_TYPE, 8, 256> idx_d256_;

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

    if (dbuck_size_ == 256) {
      switch (sbuck_size_) {
        case 4:
          idx_s4_.bulk_load(kvs);
          break;
        case 8:
          idx_s8_.bulk_load(kvs);
          break;
        case 16:
          idx_s16_.bulk_load(kvs);
          break;
        case 32:
          idx_s32_.bulk_load(kvs);
          break;
        default:
          assert(false);
      }
    } else {
      switch (dbuck_size_) {
        case 32:
          idx_d32_.bulk_load(kvs);
          break;
        case 64:
          idx_d64_.bulk_load(kvs);
          break;
        case 128:
          idx_d128_.bulk_load(kvs);
          break;
        case 256:
          idx_d256_.bulk_load(kvs);
          break;
        default:
          assert(false);
      }
    }
    
}

template<class KEY_TYPE, class PAYLOAD_TYPE>
bool BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE>::get(KEY_TYPE key, PAYLOAD_TYPE &val, Param *param) {
  bool ret;

  if (dbuck_size_ == 256) {
    switch (sbuck_size_) {
      case 4:
        ret = idx_s4_.lookup(key, val);
        break;
      case 8:
        ret = idx_s8_.lookup(key, val);
        break;
      case 16:
        idx_s16_.lookup(key, val);
        break;
      case 32:
        ret = idx_s32_.lookup(key, val);
        break;
      default:
        assert(false);
    }
  } else {
    switch (dbuck_size_) {
      case 32:
        ret = idx_d32_.lookup(key, val);
        break;
      case 64:
        ret = idx_d64_.lookup(key, val);
        break;
      case 128:
        ret = idx_d128_.lookup(key, val);
        break;
      case 256:
        ret = idx_d256_.lookup(key, val);
        break;
      default:
        assert(false);
    }
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

  if (dbuck_size_ == 256) {
    switch (sbuck_size_) {
      case 4:
        ret = idx_s4_.insert(kv);
        break;
      case 8:
        ret = idx_s8_.insert(kv);
        break;
      case 16:
        ret = idx_s16_.insert(kv);
        break;
      case 32:
        ret = idx_s32_.insert(kv);
        break;
      default:
        assert(false);
    }
  } else {
    switch (dbuck_size_) {
      case 32:
        ret = idx_d32_.insert(kv);
        break;
      case 64:
        ret = idx_d64_.insert(kv);
        break;
      case 128:
        ret = idx_d128_.insert(kv);
        break;
      case 256:
        ret = idx_d256_.insert(kv);
        break;
      default:
        assert(false);
    }
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
  size_t ret;

  if (dbuck_size_ == 256) {
    switch (sbuck_size_) {
      case 4:
        ret = idx_s4_.scan(key_low_bound, key_num, result);
        break;
      case 8:
        ret = idx_s8_.scan(key_low_bound, key_num, result);
        break;
      case 16:
        ret = idx_s16_.scan(key_low_bound, key_num, result);
        break;
      case 32:
        ret = idx_s32_.scan(key_low_bound, key_num, result);
        break;
      default:
        assert(false);
    }
  } else {
    switch (dbuck_size_) {
      case 32:
        ret = idx_d32_.scan(key_low_bound, key_num, result);
        break;
      case 64:
        ret = idx_d64_.scan(key_low_bound, key_num, result);
        break;
      case 128:
        ret = idx_d128_.scan(key_low_bound, key_num, result);
        break;
      case 256:
        ret = idx_d256_.scan(key_low_bound, key_num, result);
        break;
      default:
        assert(false);
    }
  }
  
  return ret;
}