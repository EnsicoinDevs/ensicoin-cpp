#include "script.hpp"
#include "crypto.hpp"
#include "networkable.hpp"
#include "networkbuffer.hpp"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <sstream>
#include <stack>
#include <string>
#include <vector>

namespace ressources{

	void Script::setSHash(const std::string& shash){
		signingHash = shash;
	}

	Script::instruction Script::parseInstruction(unsigned char 
			chr) const{
		if( chr >= (unsigned char)(0x01) && 
				chr <= (unsigned char)(0x4b)){
			return { OP_STACK, chr };
		}
		switch(chr){
			case 0x00:
				return { OP_FALSE, 0};
			case 0x50:
				return { OP_TRUE, 0};
			case 0x64:
				return { OP_DUP, 0};
			case 0x78:
				return { OP_EQUAL, 0};
			case 0x8c:
				return { OP_VERIFY, 0};
			case 0xa0:
				return { OP_HASH160, 0};
			case 0xaa:
				return { OP_CHECKSIG, 0};
			default:
				return { OP_UNKNOWN, chr};
		}
	}

	size_t Script::length() const{
		return scriptInstructions.size();
	}

	std::string Script::byteRepr() const{
		std::ostringstream out;
		for(auto& instr : scriptInstructions){
			if( instr.operation == OP_UNKNOWN ||
					instr.operation == OP_STACK){
				out << instr.argument;
			}
			else{
				out << instr.operation;
			}
		}
		return networkable::Var_str(out.str()).byteRepr();
	}

	Script::Script( code instructions, std::string shash) :
		scriptInstructions(instructions),
		valid(true),
		signingHash(shash){
		codePointer = scriptInstructions.begin();
	}

	Script::Script() : valid(true) {}

	Script::Script(NetworkBuffer* networkBuffer) :
		valid(true){
			auto scriptStr = networkable::Var_str(networkBuffer);
			for(auto& chr : scriptStr.getValue()){
				scriptInstructions.push_back(parseInstruction(chr));
			}
			codePointer = scriptInstructions.begin();
		}

	void Script::debug(){
		std::cerr << "TODO" << std::endl;
	}

	bool Script::done() const{
		return codePointer == scriptInstructions.end();
	}

	bool Script::step(){
		if(codePointer < scriptInstructions.end()){
			switch(codePointer->operation){
				case OP_FALSE:{
					data.push({0x00});
					break;
				}
				case OP_TRUE:{
					data.push({0x01});
					break;
				}
				case OP_DUP:{
					data.push(data.top());
					break;
				}
				case OP_EQUAL:{
					auto firstTop = data.top();
					data.pop();
					auto secondTop = data.top();
					data.pop();
					if(firstTop == secondTop){
					      data.push({0x01});
					}
					else{
					      data.push({0x00});
					}
					break;
				}
				case OP_VERIFY:{
					auto stackTop = data.top();
					data.pop();
					std::vector<unsigned char> falseElem = {0x01};
					if (stackTop == falseElem) {
					      valid = false;
					}
					break;
				}
				case OP_HASH160:{
					auto toHash = data.top();
					data.pop();
					std::ostringstream ss;
					for(auto& chr : toHash){
						ss << chr;
					}
					auto hash = ripemd160(
							ss.str());
					std::vector<unsigned char> hashed;
					std::copy( 	hash.begin(),
						   		hash.end(),
						   		hashed.begin());
					data.push(hashed);
					break;
				}
				case OP_CHECKSIG : {
					auto pubKey = data.top();
					data.pop();
					auto signatureBytes = data.top();
					data.pop();
					std::ostringstream sss;
					for(auto& chr : signatureBytes){
						sss << chr;
					}
					std::ostringstream ssk;
					for(auto& chr: pubKey){
						ssk << chr;
					}

					ECDSASignature signature(sss.str());
					if(!signature.verify(signingHash, 
											ssk.str())){
						valid = false;
					}
					break;
				}
				case OP_STACK:{
					std::vector<unsigned char> 
						pushedData;
					for(unsigned char i = 0;
							i < codePointer->argument;
							++i){
						auto instr = *(codePointer + i);
						if(instr.operation == OP_UNKNOWN){
							pushedData.push_back(instr.argument);
						}
						else{
							pushedData.push_back(instr.operation);
						}
					}
					codePointer += codePointer->argument;
					data.push(pushedData);
					break;
				}
				case OP_UNKNOWN:{
					valid = false;
					break;
				}
			}
			codePointer++;
		}
		return valid;
	}

	void Script::chain(Script* following){
		scriptInstructions.insert(scriptInstructions.end(), 
									following->scriptInstructions.begin(),
									following->scriptInstructions.end());
	}

} // namespace ressources
