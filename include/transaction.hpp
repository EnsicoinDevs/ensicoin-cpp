#ifndef TRANSACTION_HPP
#define TRANSACTION_HPP

#include "hashmemory.hpp"
#include "script.hpp"

#include <iterator>
#include <rapidjson/document.h>
#include <sstream>
#include <stack>
#include <string>
#include <vector>

using namespace rapidjson;

struct TransactionIdentifier{
	std::string transactionHash;
	int index;
	
	std::string str() const;
	Value json(Document* document) const;
	void load(Value* val);
};
struct InputTransaction{
	TransactionIdentifier previousOutput;
	std::stack<std::string> inputStack;
	
	std::string str() const;
	Value json(Document* document) const;
	void load(Value* val);
};
struct OutputTransaction{
	int value;
	std::vector<std::string> scriptInstructions;

	std::string str() const;
	Value json(Document* document) const;
	void load(Value* val);
};

class Mempool;

enum TXType { Orphan, Regular };

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
		explicit Transaction( rapidjson::Document* doc);
		
		int getVersion() const;
		std::vector<std::string> getFlags() const;
		std::vector<TransactionIdentifier> getInputsId() const;
		
		int inputValue(Mempool* mempool) const;
		int outputValue() const;

		bool check();
		bool validate(Mempool* mempool,int currentHeight);
		bool validateScript() const;

		std::string rawStr() const;
		std::string hashWithoutInputs() const;
		std::string hash() const;
		Value json(bool includeInputs, Document* document) const;
		std::string str() const;
};

#endif /* TRANSACTION_HPP */
