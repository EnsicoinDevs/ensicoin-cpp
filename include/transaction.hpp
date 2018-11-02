#ifndef TRANSACTION_HPP
#define TRANSACTION_HPP

#include <string>
#include <vector>
#include <iterator>
#include <stack>
#include <sstream>
#include <rapidjson/document.h>

#include "script.hpp"
#include "crypto.hpp"

using namespace rapidjson;

struct TransactionIdentifier{
	std::string transactionHash;
	int index;
	
	const std::string str() const;
	Value json(Document* document) const;
	void load(Value* val);
};
struct InputTransaction{
	TransactionIdentifier previousOutput;
	std::stack<std::string> inputStack;
	
	const std::string str() const;
	Value json(Document* document) const;
	void load(Value* val);
};
struct OutputTransaction{
	int value;
	std::vector<std::string> scriptInstructions;

	const std::string str() const;
	Value json(Document* document) const;
	void load(Value* val);
};

class Transaction : public Hashable{
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
		Transaction( rapidjson::Document* doc);
		const int getVersion() const;
		std::vector<std::string> getFlags() const;
		const bool validate();
		const std::string rawStr() const;
		const std::string hashWithoutInputs() const;
		Value json(bool includeInputs, Document* document) const;
};

#endif /* TRANSACTION_HPP */
