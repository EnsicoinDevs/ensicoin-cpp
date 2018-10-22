#include "blocks.hpp"
#include "transaction.hpp"
#include "crypto.hpp"

#include <vector>
#include <sstream>

Block::Block(BlockHeader head, std::vector<Transaction> transactionList) :
	header(head),
	transactions(transactionList) {}

Block::Block() : header({ 0, {}, "", "", 0, 0}), transactions({}) {}

const std::string Block::jsonString() const {
	std::ostringstream os;
	os << header.blockFlags[0] << std::endl;
	return os.str();
}

const std::string Block::str() const {
	std::ostringstream os;
	os << header.version;
	for(const auto& flag: header.blockFlags){
		os << flag;
	}
	os << header.hashPrevBlock << header.hashTransactions << header.timestamp << header.nonce;
	for(const auto& transaction : transactions){
		os << transaction.str(true); 
	}
	return os.str();
}

const std::string Block::hash() const{
	return (const std::string) sha256(sha256(str()),true);
}

const bool Block::validate(){
	
}
