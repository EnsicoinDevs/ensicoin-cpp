#include <string>
#include <vector>
#include <sstream>

#include "transaction.hpp"
#include "crypto.hpp"

const std::string TransactionIdentifier::str(bool pretty){
	std::ostringstream os;
	if(pretty)
		os << "Hash : ";
	os << transactionHash;
	if(pretty)
		os << std::endl << "Index : ";
	os << index;
	return os.str();
}

const std::string InputTransaction::str(bool pretty){
	std::ostringstream os;
	if(pretty)
		os << "Previous Transaction : (";
	os << previousOutput.str(pretty);
	if(pretty)
		os << ")" << std::endl << "Input stack (script) : " << std::endl;
	while(!inputStack.empty()){
		const std::string stackElem(inputStack.top());
		inputStack.pop();
		if(pretty)
			os << "-";
		os << stackElem;
		if(pretty)
			os << std::endl;
	}
	return os.str();
}

const std::string OutputTransaction::str(bool pretty){
	std::ostringstream os;
	if(pretty)
		os << "Value :";
	os << value;
	if(pretty)
		os << std::endl << "Output script : " << std::endl;
	for(const std::string& stackElem : scriptInstructions){
		if(pretty)
			os << "-";
		os << stackElem;
		if(pretty)
			os << std::endl;
	}
	return os.str();
}
Transaction::Transaction(	int ver,
				std::vector<std::string> intialFlags,
				std::vector<InputTransaction> initialInputs,
				std::vector<OutputTransaction> initialOutputs) :
				version(ver),
				transactionFlags(intialFlags),
				inputs(initialInputs),
				outputs(initialOutputs){}

int Transaction::getVersion(){
	return version;
}

std::vector<std::string> Transaction::getFlags(){
	return transactionFlags;
}


std::string Transaction::calculateHash(bool includeInputs){
	std::ostringstream os;
	os << version;
	for(const auto& flag: transactionFlags){
		os << flag;
	}
	if(includeInputs){
		for(auto& input : inputs){
			os << input.str();
		}
	}
	for(auto& output: outputs){
		os << output.str();
	}
	return sha256(sha256(os.str()), true);
}
