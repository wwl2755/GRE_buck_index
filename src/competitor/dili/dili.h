#pragma once

#include"../indexInterface.h"
#include "DILI.h"
#include "file_utils.h"


template<class KEY_TYPE, class PAYLOAD_TYPE>
class DILIIndexInterface : public indexInterface<KEY_TYPE, PAYLOAD_TYPE> {
public:
  void init(Param *param = nullptr){
        string mirror_dir = "./data/buTree";
        int status = file_utils::path_status(mirror_dir);
        assert(status != 2);
        if (status == 0) {
            file_utils::detect_and_create_dir(mirror_dir);
        }

        idx.set_mirror_dir(mirror_dir);
  }

  void bulk_load(std::pair <KEY_TYPE, PAYLOAD_TYPE> *key_value, size_t num, Param *param = nullptr);

  bool get(KEY_TYPE key, PAYLOAD_TYPE &val, Param *param = nullptr);

  bool put(KEY_TYPE key, PAYLOAD_TYPE value, Param *param = nullptr);

  bool update(KEY_TYPE key, PAYLOAD_TYPE value, Param *param = nullptr);

  bool remove(KEY_TYPE key, Param *param = nullptr);

  size_t scan(KEY_TYPE key_low_bound, size_t key_num, std::pair<KEY_TYPE, PAYLOAD_TYPE> *result,
              Param *param = nullptr);
 
  long long memory_consumption() {
    return idx.total_size();
  }

private:
  DILI idx;

};

template<class KEY_TYPE, class PAYLOAD_TYPE>
void DILIIndexInterface<KEY_TYPE, PAYLOAD_TYPE>::bulk_load(std::pair <KEY_TYPE, PAYLOAD_TYPE> *key_value, size_t num,
                                                          Param *param) {
    vector<pair<long, long>> kvs;


    for (int i = 0; i < num; i++) {
        kvs.push_back(key_value[i]);
    }


    idx.bulk_load(kvs); 
}

template<class KEY_TYPE, class PAYLOAD_TYPE>
bool DILIIndexInterface<KEY_TYPE, PAYLOAD_TYPE>::get(KEY_TYPE key, PAYLOAD_TYPE &val, Param *param) {
  long long ret;

  val = idx.search(key);

  if (val == -1) {
    printf("fail to read key:  %lu\n",key);
    return false;
  }

  return true;
}

template<class KEY_TYPE, class PAYLOAD_TYPE>
bool DILIIndexInterface<KEY_TYPE, PAYLOAD_TYPE>::put(KEY_TYPE key, PAYLOAD_TYPE value, Param *param) {
  bool ret;
  
  ret = idx.insert(key, value);

  if (ret == false) {
    printf("fail to insert key:  %lu\n",key);
    return false;
  }

  return true;
}

template<class KEY_TYPE, class PAYLOAD_TYPE>
bool DILIIndexInterface<KEY_TYPE, PAYLOAD_TYPE>::update(KEY_TYPE key, PAYLOAD_TYPE value, Param *param) {
    return false;
}

template<class KEY_TYPE, class PAYLOAD_TYPE>
bool DILIIndexInterface<KEY_TYPE, PAYLOAD_TYPE>::remove(KEY_TYPE key, Param *param) {
  return false;
}

template<class KEY_TYPE, class PAYLOAD_TYPE>
size_t DILIIndexInterface<KEY_TYPE, PAYLOAD_TYPE>::scan(KEY_TYPE key_low_bound, size_t key_num, std::pair<KEY_TYPE, PAYLOAD_TYPE> *result,
              Param *param) {
  return false;
}