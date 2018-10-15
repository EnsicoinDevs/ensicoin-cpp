#include <string>
#include <stack>
#include <vector>
#include <iterator>
#include <iostream>

#include "transaction.hpp"
#include "crypto.hpp"

Script::Script( std::vector<std::string>::iterator initialPointer,
		std::vector<std::string>::iterator endPointer, 
		std::stack<std::string> initalData) : 
			codePointer(initialPointer), 
			endCode(endPointer), 
			data(initalData),
			valid(true) { }

void Script::debug(){
	while( codePointer < endCode){
		std::cout << *codePointer << std::endl;
		codePointer++;
	}
}

void Script::step(){
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
	}
	else {
		data.push(*codePointer);
	}
}
