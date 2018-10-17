#ifndef TRANSACTION_HPP
#define TRANSACTION_HPP

#include <string>
#include <vector>
#include <iterator>
#include <stack>
#include <sstream>


#include "script.hpp"

struct TransactionIdentifier{
	std::string transactionHash;
	int index;
	
	const std::string str(bool pretty=false);
};
struct InputTransaction{
	TransactionIdentifier previousOutput;
	std::stack<std::string> inputStack;
	
	const std::string str(bool pretty=false);
};
struct OutputTransaction{
	int value;
	std::vector<std::string> scriptInstructions;

	const std::string str(bool pretty=false);
};

class Transaction{
	private:
		int version;
		std::vector<std::string> transactionFlags;
		std::vector<InputTransaction> inputs;
		std::vector<OutputTransaction> outputs;
	public:
		Transaction(	int ver, 	
				std::vector<std::string> initialFlags, 
				std::vector<InputTransaction> initialInputs, 
				std::vector<OutputTransaction> initialOutputs);
		int getVersion();
		std::vector<std::string> getFlags();
		bool validate();
		std::string calculateHash(bool includeInputs);
};

#endif /* TRANSACTION_HPP */
