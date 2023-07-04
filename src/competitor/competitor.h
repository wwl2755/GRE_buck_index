#include"./indexInterface.h"
#include "./alex/alex.h"
#include "./alexol/alex.h"
#include "./artsync/artrowex.h"
#include "./artsync/artolc.h"
#include "./artsync/artunsync.h"
#include "./buckindex/buckindex.h"
#include "./xindex/xindex.h"
#include "./btreeolc/btreeolc.h"
#include "./hot/hot.h"
#include "./hot/hotrowex.h"
#include "./lipp/lipp.h"
#include "./lippol/lippol.h"
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

    assert(sbuck_size_%8 == 0);
    assert(dbuck_size_%32 == 0);
    //index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, sbuck_size_, dbuck_size_>;

    if (sbuck_size_ == 8 && dbuck_size_ == 32) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 8, 32>;
    else if (sbuck_size_ == 8 && dbuck_size_ == 64) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 8, 64>;
    else if (sbuck_size_ == 8 && dbuck_size_ == 128) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 8, 128>;
    else if (sbuck_size_ == 8 && dbuck_size_ == 256) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 8, 256>;
    else if (sbuck_size_ == 8 && dbuck_size_ == 512) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 8, 512>;
    else if (sbuck_size_ == 8 && dbuck_size_ == 1024) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 8, 1024>;
    else if (sbuck_size_ == 8 && dbuck_size_ == 2048) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 8, 2048>;
    else if (sbuck_size_ == 8 && dbuck_size_ == 4096) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 8, 4096>;
    else if (sbuck_size_ == 16 && dbuck_size_ == 32) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 16, 32>;
    else if (sbuck_size_ == 16 && dbuck_size_ == 64) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 16, 64>;
    else if (sbuck_size_ == 16 && dbuck_size_ == 128) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 16, 128>;
    else if (sbuck_size_ == 16 && dbuck_size_ == 256) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 16, 256>;
    else if (sbuck_size_ == 16 && dbuck_size_ == 512) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 16, 512>;
    else if (sbuck_size_ == 16 && dbuck_size_ == 1024) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 16, 1024>;
    else if (sbuck_size_ == 16 && dbuck_size_ == 2048) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 16, 2048>;
    else if (sbuck_size_ == 16 && dbuck_size_ == 4096) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 16, 4096>;
    else if (sbuck_size_ == 32 && dbuck_size_ == 32) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 32, 32>;
    else if (sbuck_size_ == 32 && dbuck_size_ == 64) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 32, 64>;
    else if (sbuck_size_ == 32 && dbuck_size_ == 128) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 32, 128>;
    else if (sbuck_size_ == 32 && dbuck_size_ == 256) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 32, 256>;
    else if (sbuck_size_ == 32 && dbuck_size_ == 512) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 32, 512>;
    else if (sbuck_size_ == 32 && dbuck_size_ == 1024) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 32, 1024>;
    else if (sbuck_size_ == 32 && dbuck_size_ == 2048) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 32, 2048>;
    else if (sbuck_size_ == 32 && dbuck_size_ == 4096) index = new BuckIndexInterface<KEY_TYPE, PAYLOAD_TYPE, 32, 4096>;
    else {
      std::cout << "Invalid bucket size.\n";
      exit(0);
    }


    std::cout << "sbuck_size: " << sbuck_size_ << " dbuck_size: " << dbuck_size_ << std::endl;
    
  }
  // else if (index_type == "wormhole") {
  //   index = new WormholeInterface<KEY_TYPE, PAYLOAD_TYPE>;
  // }
  // else if (index_type == "wormhole_u64") {
  //   index = new WormholeU64Interface<KEY_TYPE, PAYLOAD_TYPE>;
  // }
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