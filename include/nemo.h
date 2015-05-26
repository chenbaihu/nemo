#ifndef NEMO_INCLUDE_NEMO_H_
#define NEMO_INCLUDE_NEMO_H_

//#include <stdio.h>
//#include <string>

//#include "rocksdb/status.h"
//#include "rocksdb/options.h"
#include <pthread.h>
#include "rocksdb/db.h"
#include "nemo_iterator.h"

namespace nemo {
//using namespace rocksdb;
//typedef rocksdb::Options Options;
//typedef rocksdb::ReadOptions WriteOptions;
//typedef rocksdb::Status Status;
//typedef rocksdb::Slice Slice;
//typedef rocksdb::DB DB;

typedef struct Kv {
    std::string key;
    std::string val;
}Kv;
typedef struct Kvs {
    std::string key;
    std::string val;
    rocksdb::Status status;
}Kvs;
typedef struct MutexWrite {
    pthread_mutex_t writer_mutex;
    rocksdb::WriteBatch writebatch;
}MutexWriter;

class Nemo
{
public:
    Nemo(const std::string &db_path, rocksdb::Options options);
    ~Nemo() {
        pthread_mutex_destroy(&(writer_kv_.writer_mutex));
    };
    void LockKv();
    void UnlockKv();


    rocksdb::Status Set(const std::string &key, const std::string &val);
    rocksdb::Status Get(const std::string &key, std::string *val);
    rocksdb::Status Delete(const std::string &key);
    rocksdb::Status MultiSet(const std::vector<Kv> kvs);
    rocksdb::Status MultiDel(const std::vector<std::string> keys);
    rocksdb::Status MultiGet(const std::vector<std::string> keys, std::vector<Kvs> &kvss);
    rocksdb::Status Incr(const std::string key, int64_t by, std::string &new_val);
    KIterator* scan(const std::string &start, const std::string &end, uint64_t limit);

private:

    std::string db_path_;
    std::unique_ptr<rocksdb::DB> db_;
    MutexWriter writer_kv_;
//    pthread_mutex_t nemo_kv_mutex_;
//    rocksdb::WriteBatch writebatch_kv;

    Nemo(const Nemo &rval);
    void operator =(const Nemo &rval);

};

}

#endif