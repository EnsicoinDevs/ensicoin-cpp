#include "blockchain.hpp"
#include "constants.hpp"

#include <cstdio>
#include <iostream>
#include <leveldb/db.h>
#include <rapidjson/document.h>
#include <sys/stat.h>

Blockchain::Blockchain(){
	leveldb::Options options;
	options.create_if_missing = true;
	leveldb::Status statusBlocks = 
		leveldb::DB::Open(options, 
				  BLOCKCHAIN_DB+"/blocks",
				  &blocksDB);
	if (!statusBlocks.ok()) std::cerr << 
		"Error blockDb open : " << 
		statusBlocks.ToString() << 
		std::endl;

	leveldb::Status statusChain = 
		leveldb::DB::Open(options, 
				  BLOCKCHAIN_DB+"/chain", 
				  &chainDB);
	if (!statusChain.ok()) std::cerr << 
			  "Error blockDb open : " << 
			  statusChain.ToString() << 
			  std::endl;
	
	leveldb::Status blockchainInfo =
		leveldb::DB::Open(options,
					BLOCKCHAIN_DB+"/stat",
					&statDB);
	if(!blockchainInfo.ok()) std::cerr <<
			"Error opening statDB : " <<
			blockchainInfo.ToString() <<
			std::endl;

	//auto s = statDB->Put(leveldb::WriteOptions(), "currentBlock", );
	auto s = statDB->Put(leveldb::WriteOptions(), "height","1");
}
