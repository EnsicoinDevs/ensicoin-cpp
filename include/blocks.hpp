#ifndef BLOCKS_HPP
#define BLOCKS_HPP

#include <string>
#include <vector>
#include <ctime>
#include <rapidjson/document.h>

#include "transaction.hpp"
#include "crypto.hpp"

struct BlockHeader{
	int version;
	std::vector<std::string> blockFlags;
	std::string hashPrevBlock;
	std::string hashTransactions;
	std::time_t timestamp;
	int nonce;
};

class Block : public Hashable{
	private:
		BlockHeader header;
		std::vector<Transaction* > transactions;
	public:
		Block();
		Block(BlockHeader head, std::vector<Transaction* > transactionList);
		const std::string str() const;
		rapidjson::Value json(rapidjson::Document& document) const;
		const bool validate();
};

#endif /* BLOCKS_HPP */
