#ifndef BLOCKS_HPP
#define BLOCKS_HPP

#include <string>
#include <vector>
#include <memory>
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
		std::vector<std::shared_ptr<Transaction> > transactions;
	public:
		Block();
		Block(BlockHeader head, std::vector<std::shared_ptr<Transaction> > transactionList);
		explicit Block(rapidjson::Document* document);
		const std::string rawStr() const;
		rapidjson::Value json(rapidjson::Document* document) const;
		const bool validate();
};

#endif /* BLOCKS_HPP */
