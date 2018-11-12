#ifndef BLOCKCHAIN_HPP
#define BLOCKCHAIN_HPP

#include "blocks.hpp"

#include <leveldb/db.h>
#include <string>

class Blockchain{
	private:
		leveldb::DB* db;
	public:
		Blockchain();
};

#endif /* BLOCKCHAIN_HPP */
