#include <string>
#include <vector>
#include <sstream>

#include "transaction.hpp"
#include "crypto.hpp"

const std::string TransactionIdentifier::str(bool pretty){
	std::ostringstream os;
	if(pretty)
		os << "\t\t\t\t\"transationHash\" : \"";
	os << transactionHash;
	if(pretty)
		os <<"\"," << std::endl << "\t\t\t\t\"index\" : ";
	os << index;
	return os.str();
}

const std::string InputTransaction::str(bool pretty){
	std::ostringstream os;
	if(pretty)
		os << "\t\t\t\"previousOutput\" : {" << std::endl;
	os << previousOutput.str(pretty);
	if(pretty)
		os << std::endl << "\t\t\t}," << std::endl << "\t\t\t\"script\" : [" << std::endl;
	while(!inputStack.empty()){
		const std::string stackElem(inputStack.top());
		inputStack.pop();
		if(pretty)
			os << "\t\t\t\t\"";
		os << stackElem;
		if(pretty)
			os << "\"," << std::endl;
	}
	if(pretty){
		os.seekp(-2, os.cur);
		os << std::endl << "\t\t\t]" << std::endl;
	}
	return os.str();
}

const std::string OutputTransaction::str(bool pretty){
	std::ostringstream os;
	if(pretty)
		os << "\t\t\t\"value\" : ";
	os << value;
	if(pretty)
		os << "," << std::endl << "\t\t\t\"script\" : [" << std::endl;
	for(const std::string& stackElem : scriptInstructions){
		if(pretty)
			os << "\t\t\t\t\"";
		os << stackElem;
		if(pretty)
			os << "\"," << std::endl;
	}
	if(pretty){
		os.seekp(-2, os.cur);
		os << std::endl << "\t\t\t]" << std::endl;
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

std::string Transaction::str(bool includeInputs, bool pretty){
	std::ostringstream os;
	if(pretty)
		os << "{" <<std::endl << "\t\"version\" : ";
	os << version;
	if(pretty)
		os << "," << std::endl << "\t\"flags\" : [" << std::endl;
	for(const auto& flag: transactionFlags){
		if(pretty)
			os << "\t\t\"";
		os << flag;
		if(pretty)
			os << "\"," << std::endl;
	}
	if(pretty){
		os.seekp(-2, os.cur);
		os << std::endl << "\t]," << std::endl;
	}
	if(includeInputs){
		if(pretty)
			os << "\t\"inputs\" : [" << std::endl;
		for(auto& input : inputs){
			if(pretty)
				os << "\t\t{" << std::endl; 

			os << input.str(pretty);
			
			if(pretty)
				os << "\t\t}," << std::endl;
		}
		if(pretty){
			os.seekp(-2, os.cur);
			os << std::endl << "\t]," << std::endl;
		}
	}
	if(pretty)
		os << "\t\"outputs\" : [" << std::endl;
	for(auto& output: outputs){
		if(pretty)
			os << "\t\t{" << std::endl;
		os << output.str(pretty);
		if(pretty)
			os << "\t\t}," << std::endl;
	}
	if(pretty){
		os.seekp(-2, os.cur);
		os << std::endl << "\t]" << std::endl << std::endl << "}";
	}

	return os.str();
}

std::string Transaction::calculateHash(bool includeInputs){
	return sha256(sha256(this->str(includeInputs)), true);
}

bool Transaction::validate(){
	if(inputs.empty() || outputs.empty()){
		return false;
	}
}
