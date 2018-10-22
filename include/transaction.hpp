#ifndef TRANSACTION_HPP
#define TRANSACTION_HPP

#include <string>
#include <vector>
#include <iterator>
#include <stack>
#include <sstream>

#include <rapidjson/document.h>
#include "script.hpp"

using namespace rapidjson;

struct TransactionIdentifier{
	std::string transactionHash;
	int index;
	
	const std::string str() const;
	Value json(Document& document) const;
};
struct InputTransaction{
	TransactionIdentifier previousOutput;
	std::stack<std::string> inputStack;
	
	const std::string str() const;
	Value json(Document& document) const;
};
struct OutputTransaction{
	int value;
	std::vector<std::string> scriptInstructions;

	const std::string str() const;
	Value json(Document& document) const;
};

class Transaction{
	private:
		int version;
		std::vector<std::string> transactionFlags;
		std::vector<InputTransaction> inputs;
		std::vector<OutputTransaction> outputs;
	public:
		Transaction();
		Transaction(	int ver, 	
				std::vector<std::string> initialFlags, 
				std::vector<InputTransaction> initialInputs, 
				std::vector<OutputTransaction> initialOutputs);
		const int getVersion() const;
		std::vector<std::string> getFlags() const;
		const bool validate();
		const std::string str(bool includeInputs) const;
		Value json(bool includeInputs, Document& document) const;
		const std::string calculateHash(bool includeInputs) const;
};

#endif /* TRANSACTION_HPP */
