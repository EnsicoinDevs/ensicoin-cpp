#include "script.hpp"
#include "crypto.hpp"

#include <string>
#include <stack>
#include <vector>
#include <iterator>
#include <iostream>

Script::Script( std::vector<std::string>::iterator initialPointer,
		std::vector<std::string>::iterator endPointer, 
		std::stack<std::string> initalData,
		std::string trHash ) : 
			codePointer(initialPointer), 
			endCode(endPointer), 
			data(initalData),
			valid(true),
			transactionHash(trHash) { }

void Script::debug(){
		
	std::cerr << "Initial Stack : ";
	std::stack<std::string> copyData(data);
	while( !copyData.empty() ){
		std::string topData(copyData.top());
		copyData.pop();	
		std::cerr << topData << "|";
	}

	std::cerr << std::endl;
	
	while( codePointer < endCode){
		std::cerr << " Valid : " << step() << std::endl;
		
		std::cerr << " Stack : ";
		std::stack<std::string> copyDataBis(data);
		while( !copyData.empty() ){
			std::string topData(copyDataBis.top());
			copyDataBis.pop();	
			std::cerr << topData << "|";
		}
		std::cerr << std::endl;
	}
}

bool Script::done() const{
	return codePointer == endCode;
}

bool Script::step(){
	if(codePointer < endCode){
		if ( *codePointer == "OP_DUP" ) {
			data.push(data.top());
		}
		else if ( *codePointer == "OP_EQUAL" ){
			std::string firstTop(data.top());
			data.pop();
			std::string secondTop(data.top());
			data.pop();
			data.push( secondTop == firstTop ? "true" : "false" );
		}
		else if ( *codePointer == "OP_HASH160"){
			std::string toHash(data.top());
			data.pop();
			data.push(ripemd160(toHash));
		}
		else if ( *codePointer == "OP_VERIFY"){
			std::string stackTop = data.top();
			data.pop();
			if (stackTop == "false") {
				valid = false;
			}
		}
		else if ( *codePointer == "OP_CHECKSIG" ){
			std::string pubKey = data.top();
			data.pop();
			std::string signatureString = data.top();
			data.pop();
	
			ECDSASignature signature(signatureString);
			if(!signature.verify(transactionHash, pubKey)){
				valid = false;
			}
		}
		else {
			data.push(*codePointer);
		}
		codePointer++;
	}
	return valid;
}
