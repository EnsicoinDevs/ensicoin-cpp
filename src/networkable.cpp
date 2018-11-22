#include "networkable.hpp"
#include "debug.hpp"

#include <iostream>
#include <cryptopp/hex.h>
#include <sstream>

Networkable::~Networkable() {}

std::string Networkable::asBytes() const{
	std::string hexRepr = this->byteRepr();
	std::string decodedBytes;
	
	CryptoPP::StringSource ss(hexRepr, true,
		new CryptoPP::HexDecoder(
			new CryptoPP::StringSink(decodedBytes)
		) // HexDecoder
	); // String Source
	return decodedBytes;
}

Var_uint::Var_uint(uint64_t val) : value(val) {}

Var_uint::Var_uint(const std::string& binaryString){
	if ( binaryString[0] < char(0xfd) ){
		value = binaryString[0];
	}
	else if ( binaryString[0] == char(0xfd)){
		value = ((unsigned char)(binaryString[1]) << 8) |
			((unsigned char)(binaryString[2]));
	}
	else if ( binaryString[0] == char(0xfe)){
		value = ((unsigned char)(binaryString[1]) << 24) |
			((unsigned char)(binaryString[2]) << 16) |
			((unsigned char)(binaryString[3]) << 8)  |
			((unsigned char)(binaryString[4]));
	}
	else{
		for(int i = 2; i < 9; ++i){
			auto byte = (unsigned char)(binaryString[i]);
			value |= (uint64_t)(byte) << 8*(8-i);
		}
		binaryString[9];
	}
}

uint64_t Var_uint::getValue() const{
	return value;
}

std::string Var_uint::byteRepr() const{
	std::ostringstream sstream;
	sstream << std::hex << value;
	auto rawStr = sstream.str();
	if(value <= 252){
		return rawStr;
	}
	if(value <= 0xffff){
		return "fd" + 
		       std::string(4-rawStr.length(),'0') + 
		       rawStr;
		// 3 : 0xFD.value
	}
	if(value <= 0xffffffff){
		return "fe" +
			std::string(8-rawStr.length(),'0') +
			rawStr;
		// 5 : 0xFE.value
	}
	return "ff" +
	       std::string(16-rawStr.length(),'0') +
	       rawStr;
	// 9 0xFF.value
}

Var_str::Var_str(std::string val) : value(val) {}

std::string Var_str::byteRepr() const{
	return Var_uint(value.size()).byteRepr() + value;
}
