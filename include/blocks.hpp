#ifndef BLOCKS_HPP
#define BLOCKS_HPP

#include <string>
#include <vector>
#include <ctime>

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
		std::vector<Transaction> transactions;
	public:
		Block();
		Block(BlockHeader head, std::vector<Transaction> transactionList);
		const std::string str() const;
		const std::string jsonString() const;
		const bool validate();
};

Block* GenesisBlock = new Block({0,{"ici cest limag"},"","",1566862920,42},
			 {});

#endif /* BLOCKS_HPP */
