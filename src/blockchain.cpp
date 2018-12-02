#include "blockchain.hpp"
#include "constants.hpp"

#include <cstdio>
#include <iostream>
#include <leveldb/db.h>
#include <rapidjson/document.h>
#include <sys/stat.h>

namespace manager{

	Blockchain::Blockchain(){
		leveldb::Options options;
		options.create_if_missing = true;
		leveldb::Status statusBlocks = 
			leveldb::DB::Open(options, 
					constants::BLOCKCHAIN_DB+"/blocks",
					&blocksDB);
		if (!statusBlocks.ok()) std::cerr << 
			"Error blockDb open : " << 
				statusBlocks.ToString() << 
				std::endl;

		leveldb::Status statusChain = 
			leveldb::DB::Open(options, 
					constants::BLOCKCHAIN_DB+"/chain", 
					&chainDB);
		if (!statusChain.ok()) std::cerr << 
			"Error blockDb open : " << 
				statusChain.ToString() << 
				std::endl;

		leveldb::Status blockchainInfo =
			leveldb::DB::Open(options,
					constants::BLOCKCHAIN_DB+"/stat",
					&statDB);
		if(!blockchainInfo.ok()) std::cerr <<
			"Error opening statDB : " <<
				blockchainInfo.ToString() <<
				std::endl;
	}

	ressources::Block Blockchain::getBlock(const std::string& hash){
		std::string strData;
		leveldb::Status s = blocksDB->Get(leveldb::ReadOptions(), 
						hash, &strData);
		if (!s.ok()){
			throw std::runtime_error("Block does not exists");
		}
		NetworkBuffer buffer(strData);
		return ressources::Block(&buffer);
	}

	std::vector<ressources::BlockTarget> BlockIndex::getWindowFrom(const std::string& latestBlock){
		std::string currentHash = latestBlock;
		int count = 0;
		std::vector<ressources::BlockTarget> out;
		for(int i = 0; i < count; ++i){
			auto header = retrieve(currentHash)->getHeader();
			out.push_back(header.target);
			currentHash = header.hashPrevBlock;
		}
		return out;
	}

	ressources::Block::pointer BlockIndex::retrieve(
			const std::string& hash){
		if(cachedBlocks.count(hash) == 1){
			return std::make_shared<ressources::Block>
				(cachedBlocks.at(hash));
		}
		else{
			auto block = blockchain.getBlock(hash);

		}
	}
} // namespace manager
