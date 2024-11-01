#include"./indexInterface.h"
#include "./alex/alex.h"
#include "./alexol/alex.h"
#include "./artsync/artrowex.h"
#include "./artsync/artolc.h"
#include "./artsync/artunsync.h"
#include "./buckindex/buckindex.h"
// #include "./buckindex/buckindex_concurrent.h"
#include "./xindex/xindex.h"
#include "./btreeolc/btreeolc.h"
#include "./hot/hot.h"
#include "./hot/hotrowex.h"
#include "./lipp/lipp.h"
#include "./lippol/lippol.h"
#include "dili/dili.h"
#include "pgm/pgm.h"
#include "btree/btree.h"
// #include "wormhole/wormhole.h"
// #include "wormhole_u64/wormhole_u64.h"
// #include "masstree/masstree.h"
#include "finedex/finedex.h"
#include "iostream"


template<class KEY_TYPE, class PAYLOAD_TYPE>
indexInterface<KEY_TYPE, PAYLOAD_TYPE> *get_index(std::string index_type, Param *param) {
  indexInterface<KEY_TYPE, PAYLOAD_TYPE> *index;
  if (index_type == "alexol") {
    index = new alexolInterface<KEY_TYPE, PAYLOAD_TYPE>;
  }
  else if(index_type == "alex") {
    index = new alexInterface<KEY_TYPE, PAYLOAD_TYPE>;
  }
  else if (index_type == "btreeolc") {
    index = new BTreeOLCInterface<KEY_TYPE, PAYLOAD_TYPE>;
  }
  else if (index_type == "buckindex") {
    // assert(param->sbuck_size == 8 || param->sbuck_size == 16 || param->sbuck_size == 32);
    // assert(param->dbuck_size == 32 || param->dbuck_size == 64 || param->dbuck_size == 128 || param->dbuck_size == 256 || param->dbuck_size == 512 || param->dbuck_size == 1024);
    int sbuck_size_ = param->sbuck_size;
    int dbuck_size_ = param->dbuck_size;

    // index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 8, 256>;

    //index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, sbuck_size_, dbuck_size_>;
    if (sbuck_size_ == 1 && dbuck_size_ == 32) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 1, 32>;
    else if (sbuck_size_ == 1 && dbuck_size_ == 64) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 1, 64>;
    else if (sbuck_size_ == 1 && dbuck_size_ == 128) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 1, 128>;
    else if (sbuck_size_ == 1 && dbuck_size_ == 256) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 1, 256>;
    else if (sbuck_size_ == 1 && dbuck_size_ == 512) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 1, 512>;
    else if (sbuck_size_ == 1 && dbuck_size_ == 1024) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 1, 1024>;
    else if (sbuck_size_ == 1 && dbuck_size_ == 2048) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 1, 2048>;
    else if (sbuck_size_ == 1 && dbuck_size_ == 4096) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 1, 4096>;
    else if (sbuck_size_ == 1 && dbuck_size_ == 8192) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 1, 8192>;
    else if (sbuck_size_ == 1 && dbuck_size_ == 16384) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 1, 16384>; // 16K
    else if (sbuck_size_ == 1 && dbuck_size_ == 32768) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 1, 32768>; // 32K
    else if (sbuck_size_ == 1 && dbuck_size_ == 65536) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 1, 65536>; // 64K
    else if (sbuck_size_ == 1 && dbuck_size_ == 131072) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 1, 131072>; // 128K
    else if (sbuck_size_ == 1 && dbuck_size_ == 262144) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 1, 262144>; // 256K
    else if (sbuck_size_ == 1 && dbuck_size_ == 524288) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 1, 524288>; // 512K
    else if (sbuck_size_ == 1 && dbuck_size_ == 1048576) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 1, 1048576>; // 1M
    else if (sbuck_size_ == 1 && dbuck_size_ == 2097152) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 1, 2097152>; // 2M
    else if (sbuck_size_ == 1 && dbuck_size_ == 4194304) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 1, 4194304>; // 4M
    else if (sbuck_size_ == 1 && dbuck_size_ == 8388608) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 1, 8388608>; // 8M
    else if (sbuck_size_ == 1 && dbuck_size_ == 10485760) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 1, 10485760>; // 10M
    else if (sbuck_size_ == 1 && dbuck_size_ == 12582912) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 1, 12582912>; // 12M
    else if (sbuck_size_ == 1 && dbuck_size_ == 14680064) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 1, 14680064>; // 14M
    else if (sbuck_size_ == 1 && dbuck_size_ == 16777216) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 1, 16777216>; // 16M
    else if (sbuck_size_ == 1 && dbuck_size_ == 33554432) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 1, 33554432>; // 32M
    else if (sbuck_size_ == 2 && dbuck_size_ == 32) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 2, 32>;
    else if (sbuck_size_ == 2 && dbuck_size_ == 64) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 2, 64>;
    else if (sbuck_size_ == 2 && dbuck_size_ == 128) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 2, 128>;
    else if (sbuck_size_ == 2 && dbuck_size_ == 256) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 2, 256>;
    else if (sbuck_size_ == 2 && dbuck_size_ == 512) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 2, 512>;
    else if (sbuck_size_ == 2 && dbuck_size_ == 1024) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 2, 1024>;
    else if (sbuck_size_ == 2 && dbuck_size_ == 2048) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 2, 2048>;
    else if (sbuck_size_ == 2 && dbuck_size_ == 4096) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 2, 4096>;
    else if (sbuck_size_ == 2 && dbuck_size_ == 8192) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 2, 8192>;
    else if (sbuck_size_ == 2 && dbuck_size_ == 16384) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 2, 16384>; // 16K
    else if (sbuck_size_ == 2 && dbuck_size_ == 32768) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 2, 32768>; // 32K
    else if (sbuck_size_ == 2 && dbuck_size_ == 65536) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 2, 65536>; // 64K
    else if (sbuck_size_ == 2 && dbuck_size_ == 131072) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 2, 131072>; // 128K
    else if (sbuck_size_ == 2 && dbuck_size_ == 262144) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 2, 262144>; // 256K
    else if (sbuck_size_ == 2 && dbuck_size_ == 524288) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 2, 524288>; // 512K
    else if (sbuck_size_ == 2 && dbuck_size_ == 1048576) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 2, 1048576>; // 1M
    else if (sbuck_size_ == 2 && dbuck_size_ == 2097152) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 2, 2097152>; // 2M
    else if (sbuck_size_ == 2 && dbuck_size_ == 4194304) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 2, 4194304>; // 4M
    else if (sbuck_size_ == 2 && dbuck_size_ == 8388608) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 2, 8388608>; // 8M
    else if (sbuck_size_ == 2 && dbuck_size_ == 10485760) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 2, 10485760>; // 10M
    else if (sbuck_size_ == 2 && dbuck_size_ == 12582912) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 2, 12582912>; // 12M
    else if (sbuck_size_ == 2 && dbuck_size_ == 14680064) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 2, 14680064>; // 14M
    else if (sbuck_size_ == 2 && dbuck_size_ == 16777216) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 2, 16777216>; // 16M
    else if (sbuck_size_ == 2 && dbuck_size_ == 33554432) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 2, 33554432>; // 32M
    else if (sbuck_size_ == 4 && dbuck_size_ == 32) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 4, 32>;
    else if (sbuck_size_ == 4 && dbuck_size_ == 64) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 4, 64>;
    else if (sbuck_size_ == 4 && dbuck_size_ == 128) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 4, 128>;
    else if (sbuck_size_ == 4 && dbuck_size_ == 256) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 4, 256>;
    else if (sbuck_size_ == 4 && dbuck_size_ == 512) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 4, 512>;
    else if (sbuck_size_ == 4 && dbuck_size_ == 1024) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 4, 1024>;
    else if (sbuck_size_ == 4 && dbuck_size_ == 2048) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 4, 2048>;
    else if (sbuck_size_ == 4 && dbuck_size_ == 4096) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 4, 4096>;
    else if (sbuck_size_ == 4 && dbuck_size_ == 8192) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 4, 8192>;
    else if (sbuck_size_ == 4 && dbuck_size_ == 16384) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 4, 16384>; // 16K
    else if (sbuck_size_ == 4 && dbuck_size_ == 32768) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 4, 32768>; // 32K
    else if (sbuck_size_ == 4 && dbuck_size_ == 65536) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 4, 65536>; // 64K
    else if (sbuck_size_ == 4 && dbuck_size_ == 131072) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 4, 131072>; // 128K
    else if (sbuck_size_ == 4 && dbuck_size_ == 262144) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 4, 262144>; // 256K
    else if (sbuck_size_ == 4 && dbuck_size_ == 524288) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 4, 524288>; // 512K
    else if (sbuck_size_ == 4 && dbuck_size_ == 1048576) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 4, 1048576>; // 1M
    else if (sbuck_size_ == 4 && dbuck_size_ == 2097152) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 4, 2097152>; // 2M
    else if (sbuck_size_ == 4 && dbuck_size_ == 4194304) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 4, 4194304>; // 4M
    else if (sbuck_size_ == 4 && dbuck_size_ == 8388608) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 4, 8388608>; // 8M
    else if (sbuck_size_ == 4 && dbuck_size_ == 10485760) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 4, 10485760>; // 10M
    else if (sbuck_size_ == 4 && dbuck_size_ == 12582912) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 4, 12582912>; // 12M
    else if (sbuck_size_ == 4 && dbuck_size_ == 14680064) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 4, 14680064>; // 14M
    else if (sbuck_size_ == 4 && dbuck_size_ == 16777216) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 4, 16777216>; // 16M
    else if (sbuck_size_ == 4 && dbuck_size_ == 33554432) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 4, 33554432>; // 32M
    else if (sbuck_size_ == 8 && dbuck_size_ == 32) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 8, 32>;
    else if (sbuck_size_ == 8 && dbuck_size_ == 64) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 8, 64>;
    else if (sbuck_size_ == 8 && dbuck_size_ == 128) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 8, 128>;
    else if (sbuck_size_ == 8 && dbuck_size_ == 256) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 8, 256>;
    else if (sbuck_size_ == 8 && dbuck_size_ == 512) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 8, 512>;
    else if (sbuck_size_ == 8 && dbuck_size_ == 1024) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 8, 1024>;
    else if (sbuck_size_ == 8 && dbuck_size_ == 2048) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 8, 2048>;
    else if (sbuck_size_ == 8 && dbuck_size_ == 4096) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 8, 4096>;
    else if (sbuck_size_ == 8 && dbuck_size_ == 8192) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 8, 8192>;
    else if (sbuck_size_ == 8 && dbuck_size_ == 16384) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 8, 16384>; // 16K
    else if (sbuck_size_ == 8 && dbuck_size_ == 32768) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 8, 32738>; // 32K
    else if (sbuck_size_ == 8 && dbuck_size_ == 65536) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 8, 65536>; // 64K
    else if (sbuck_size_ == 8 && dbuck_size_ == 131072) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 8, 131072>; // 128K
    else if (sbuck_size_ == 8 && dbuck_size_ == 262144) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 8, 262144>; // 256K
    else if (sbuck_size_ == 8 && dbuck_size_ == 524288) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 8, 524288>; // 512K
    else if (sbuck_size_ == 8 && dbuck_size_ == 1048576) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 8, 1048576>; // 1M
    else if (sbuck_size_ == 8 && dbuck_size_ == 2097152) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 8, 2097152>; // 2M
    else if (sbuck_size_ == 8 && dbuck_size_ == 4194304) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 8, 4194304>; // 4M
    else if (sbuck_size_ == 8 && dbuck_size_ == 8388608) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 8, 8388608>; // 8M
    else if (sbuck_size_ == 8 && dbuck_size_ == 10485760) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 8, 10485760>; // 10M
    else if (sbuck_size_ == 8 && dbuck_size_ == 12582912) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 8, 12582912>; // 12M
    else if (sbuck_size_ == 8 && dbuck_size_ == 14680064) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 8, 14680064>; // 14M
    else if (sbuck_size_ == 8 && dbuck_size_ == 16777216) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 8, 16777216>; // 16M
    else if (sbuck_size_ == 8 && dbuck_size_ == 33554432) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 8, 33554432>; // 32M
    else if (sbuck_size_ == 16 && dbuck_size_ == 1024) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 16, 1024>;
    else if (sbuck_size_ == 16 && dbuck_size_ == 2048) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 16, 2048>;
    else if (sbuck_size_ == 16 && dbuck_size_ == 4096) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 16, 4096>;
    else if (sbuck_size_ == 16 && dbuck_size_ == 8192) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 16, 8192>;
    else if (sbuck_size_ == 16 && dbuck_size_ == 16384) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 16, 16384>; //16K
    else if (sbuck_size_ == 16 && dbuck_size_ == 32768) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 16, 32768>; // 32K
    else if (sbuck_size_ == 16 && dbuck_size_ == 65536) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 16, 65536>; // 64K
    else if (sbuck_size_ == 16 && dbuck_size_ == 131072) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 16, 131072>; // 128K
    else if (sbuck_size_ == 16 && dbuck_size_ == 262144) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 16, 262144>; // 256K
    else if (sbuck_size_ == 16 && dbuck_size_ == 524288) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 16, 524288>; // 512K
    else if (sbuck_size_ == 16 && dbuck_size_ == 1048576) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 16, 1048576>; // 1M
    else if (sbuck_size_ == 16 && dbuck_size_ == 2097152) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 16, 2097152>; // 2M
    else if (sbuck_size_ == 16 && dbuck_size_ == 4194304) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 16, 4194304>; // 4M
    else if (sbuck_size_ == 16 && dbuck_size_ == 8388608) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 16, 8388608>; // 8M
    else if (sbuck_size_ == 16 && dbuck_size_ == 10485760) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 16, 10485760>; // 10M
    else if (sbuck_size_ == 16 && dbuck_size_ == 12582912) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 16, 12582912>; // 12M
    else if (sbuck_size_ == 16 && dbuck_size_ == 14680064) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 16, 14680064>; // 14M
    else if (sbuck_size_ == 16 && dbuck_size_ == 16777216) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 16, 16777216>; // 16M
    else if (sbuck_size_ == 16 && dbuck_size_ == 33554432) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 16, 33554432>; // 32M
    else if (sbuck_size_ == 32 && dbuck_size_ == 1024) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 32, 1024>;
    else if (sbuck_size_ == 32 && dbuck_size_ == 2048) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 32, 2048>;
    else if (sbuck_size_ == 32 && dbuck_size_ == 4096) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 32, 4096>;
    else if (sbuck_size_ == 32 && dbuck_size_ == 8192) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 32, 8192>;
    else if (sbuck_size_ == 32 && dbuck_size_ == 16384) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 32, 16384>; //16K
    else if (sbuck_size_ == 32 && dbuck_size_ == 32768) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 32, 32768>; // 32K
    else if (sbuck_size_ == 32 && dbuck_size_ == 65536) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 32, 65536>; // 64K
    else if (sbuck_size_ == 32 && dbuck_size_ == 131072) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 32, 131072>; // 128K
    else if (sbuck_size_ == 32 && dbuck_size_ == 262144) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 32, 262144>; // 256K
    else if (sbuck_size_ == 32 && dbuck_size_ == 524288) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 32, 524288>; // 512K
    else if (sbuck_size_ == 32 && dbuck_size_ == 1048576) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 32, 1048576>; // 1M
    else if (sbuck_size_ == 32 && dbuck_size_ == 2097152) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 32, 2097152>; // 2M
    else if (sbuck_size_ == 32 && dbuck_size_ == 4194304) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 32, 4194304>; // 4M
    else if (sbuck_size_ == 32 && dbuck_size_ == 8388608) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 32, 8388608>; // 8M
    else if (sbuck_size_ == 32 && dbuck_size_ == 10485760) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 32, 10485760>; // 10M
    else if (sbuck_size_ == 32 && dbuck_size_ == 12582912) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 32, 12582912>; // 12M
    else if (sbuck_size_ == 32 && dbuck_size_ == 14680064) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 32, 14680064>; // 14M
    else if (sbuck_size_ == 32 && dbuck_size_ == 16777216) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 32, 16777216>; // 16M
    else if (sbuck_size_ == 32 && dbuck_size_ == 33554432) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 32, 33554432>; // 32M
    else {
      std::cout << "Invalid bucket size.\n";
      std::cout << "sbuck_size: " << sbuck_size_ << " dbuck_size: " << dbuck_size_ << std::endl;
      exit(0);
    }


    std::cout << "sbuck_size: " << sbuck_size_ << " dbuck_size: " << dbuck_size_ << std::endl;
    
  } 
  // else if (index_type == "bli_concurrent") {
  //   assert(param->sbuck_size == 8 || param->sbuck_size == 16 || param->sbuck_size == 32);
  //   assert(param->dbuck_size == 32 || param->dbuck_size == 64 || param->dbuck_size == 128 || param->dbuck_size == 256);
  //   int sbuck_size_ = param->sbuck_size;
  //   int dbuck_size_ = param->dbuck_size;

  //   if (sbuck_size_ == 8 && dbuck_size_ == 32) index = new BLI_ConcurrentInterface<KEY_TYPE, PAYLOAD_TYPE, 8, 32>;
  //   else if (sbuck_size_ == 8 && dbuck_size_ == 64) index = new BLI_ConcurrentInterface<KEY_TYPE, PAYLOAD_TYPE, 8, 64>;
  //   else if (sbuck_size_ == 8 && dbuck_size_ == 128) index = new BLI_ConcurrentInterface<KEY_TYPE, PAYLOAD_TYPE, 8, 128>;
  //   else if (sbuck_size_ == 8 && dbuck_size_ == 256) index = new BLI_ConcurrentInterface<KEY_TYPE, PAYLOAD_TYPE, 8, 256>;
  //   else if (sbuck_size_ == 16 && dbuck_size_ == 32) index = new BLI_ConcurrentInterface<KEY_TYPE, PAYLOAD_TYPE, 16, 32>;
  //   else if (sbuck_size_ == 16 && dbuck_size_ == 64) index = new BLI_ConcurrentInterface<KEY_TYPE, PAYLOAD_TYPE, 16, 64>;
  //   else if (sbuck_size_ == 16 && dbuck_size_ == 128) index = new BLI_ConcurrentInterface<KEY_TYPE, PAYLOAD_TYPE, 16, 128>;
  //   else if (sbuck_size_ == 16 && dbuck_size_ == 256) index = new BLI_ConcurrentInterface<KEY_TYPE, PAYLOAD_TYPE, 16, 256>;
  //   else if (sbuck_size_ == 32 && dbuck_size_ == 32) index = new BLI_ConcurrentInterface<KEY_TYPE, PAYLOAD_TYPE, 32, 32>;
  //   else if (sbuck_size_ == 32 && dbuck_size_ == 64) index = new BLI_ConcurrentInterface<KEY_TYPE, PAYLOAD_TYPE, 32, 64>;
  //   else if (sbuck_size_ == 32 && dbuck_size_ == 128) index = new BLI_ConcurrentInterface<KEY_TYPE, PAYLOAD_TYPE, 32, 128>;
  //   else if (sbuck_size_ == 32 && dbuck_size_ == 256) index = new BLI_ConcurrentInterface<KEY_TYPE, PAYLOAD_TYPE, 32, 256>;
  //   else {
  //     std::cout << "Invalid bucket size.\n";
  //     exit(0);
  //   }

  //   std::cout << "sbuck_size: " << sbuck_size_ << " dbuck_size: " << dbuck_size_ << std::endl;
  // }
  // else if (index_type == "wormhole") {
  //   index = new WormholeInterface<KEY_TYPE, PAYLOAD_TYPE>;
  // }
  // else if (index_type == "wormhole_u64") {
  //   index = new WormholeU64Interface<KEY_TYPE, PAYLOAD_TYPE>;
  // }
  else if ( index_type == "dili") {
    index = new DILIIndexInterface<KEY_TYPE, PAYLOAD_TYPE>;
  }
  else if( index_type == "hot") {
    index = new HotInterface<KEY_TYPE, PAYLOAD_TYPE>;
  }
  else if( index_type == "hotrowex") {
    index = new HotRowexInterface<KEY_TYPE, PAYLOAD_TYPE>;
  }
  // else if (index_type == "masstree") {
  //   index = new MasstreeInterface<KEY_TYPE, PAYLOAD_TYPE>;
  // }
  else if (index_type == "xindex") {
    index = new xindexInterface<KEY_TYPE, PAYLOAD_TYPE>;
  }
  else if (index_type == "pgm") {
    index = new pgmInterface<KEY_TYPE, PAYLOAD_TYPE>;
  }
  else if(index_type == "btree") {
    index = new BTreeInterface<KEY_TYPE, PAYLOAD_TYPE>;
  }
  else if (index_type == "artolc") {
    index = new ARTOLCInterface<KEY_TYPE, PAYLOAD_TYPE>;
  }
  // else if (index_type == "artrowex") {
  //   index = new ARTROWEXInterface<KEY_TYPE, PAYLOAD_TYPE>;
  // }
  else if (index_type == "artunsync") {
    index = new ARTUnsynchronizedInterface<KEY_TYPE, PAYLOAD_TYPE>;
  }
  else if (index_type == "lippol") {
    index = new LIPPOLInterface<KEY_TYPE, PAYLOAD_TYPE>;
  }
  else if (index_type == "lipp") {
    index = new LIPPInterface<KEY_TYPE, PAYLOAD_TYPE>;
  }
  else if (index_type == "finedex") {
    index = new finedexInterface<KEY_TYPE, PAYLOAD_TYPE>;
  }
  else {
    std::cout << "Could not find a matching index called " << index_type << ".\n";
    exit(0);
  }

  return index;
}