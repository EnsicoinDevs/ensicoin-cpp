#include "blockchain.hpp"
#include "constants.hpp"

#include <cstdio>
#include <iostream>
#include <leveldb/db.h>
#include <memory>
#include <rapidjson/document.h>
#include <sys/stat.h>

namespace manager{

	Blockchain::Blockchain(std::shared_ptr<Logger> logger_) : logger(logger_) {
		leveldb::Options options;
		options.create_if_missing = true;
		leveldb::Status statusBlocks = 
			leveldb::DB::Open(options, 
					constants::BLOCKCHAIN_DB+"/blocks",
					&blocksDB);
		if (!statusBlocks.ok()) 
			logger->fatal("can't open block DB : ",statusBlocks.ToString());


		leveldb::Status statusChain = 
			leveldb::DB::Open(options, 
					constants::BLOCKCHAIN_DB+"/chain", 
					&chainDB);
		if (!statusChain.ok())
			logger->fatal("can't open chain DB : ",statusChain.ToString());

		leveldb::Status blockchainInfo =
			leveldb::DB::Open(options,
					constants::BLOCKCHAIN_DB+"/stat",
					&statDB);
		if(!blockchainInfo.ok()) 
			logger->fatal("can't open info DB : ",blockchainInfo.ToString());
	}

	bool Blockchain::exists(const std::string& hash) const{
		std::string strData;
		leveldb::Status s = blocksDB->Get(leveldb::ReadOptions(), 
				hash, &strData);
		return s.ok();
	}

	ressources::Block Blockchain::getBlock(const std::string& hash){
		std::string strData;
		leveldb::Status s = blocksDB->Get(leveldb::ReadOptions(), 
						hash, &strData);
		if (!s.ok()){
			logger->error("block ", hash, " does not exist");
		}
		NetworkBuffer buffer(strData, logger);
		return ressources::Block(&buffer);
	}

	BlockIndex::BlockIndex(std::shared_ptr<Logger> logger_) : 
		blockchain(logger_),
		logger(logger_) {}

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
