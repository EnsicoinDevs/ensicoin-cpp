#ifndef BLOCKCHAIN_HPP
#define BLOCKCHAIN_HPP

#include "blocks.hpp"

#include <leveldb/db.h>
#include <string>
#include <map>

/// \brief Stores Block on disk and links them
class Blockchain{
	private:
		/// \brief DB storing Block
		leveldb::DB* blocksDB;
		/// \brief DB storing the following Block
		leveldb::DB* chainDB;
		/// \brief DB storing stats on the Blockchain
		leveldb::DB* statDB;
	public:
		
		Blockchain();
};

class BlockIndex{
	private:
		Blockchain blockchain;
		std::map<std::string, ressources::Block> cachedBlocks;
	public:
		std::vector<ressources::BlockTarget> getWindowFrom(const std::string& latestBlock);
};

#endif /* BLOCKCHAIN_HPP */
