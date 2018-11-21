#ifndef BLOCKCHAIN_HPP
#define BLOCKCHAIN_HPP

#include "blocks.hpp"

#include <leveldb/db.h>
#include <string>

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

#endif /* BLOCKCHAIN_HPP */
