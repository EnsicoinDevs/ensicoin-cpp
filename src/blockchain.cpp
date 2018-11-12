#include "blockchain.hpp"
#include "constants.hpp"

#include <iostream>
#include <leveldb/db.h>

Blockchain::Blockchain(){
	leveldb::Options options;
	options.create_if_missing = true;
	leveldb::Status status = leveldb::DB::Open(options, BLOCKCHAIN_DB, &db);
	if (!status.ok()) std::cerr << "Error db open : " << status.ToString() << std::endl;
}
