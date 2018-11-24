#include "networkbuffer.hpp"
#include "networkable.hpp"

#include <iostream>
#include <string>

void NetworkBuffer::appendBytes(const std::string& binaryString){
	buffer << binaryString; 
}

networkable::Uint16 NetworkBuffer::readUint16(){
	char* binaryRepr = new char[2];
	buffer.read(binaryRepr, 2);
	if(buffer.gcount() != 2){
		std::cerr << "Not enough bytes in buffer\
			to read Uint16" << std::endl;
		return networkable::Uint16(0);
	}
	int value = (unsigned char)(binaryRepr[0]) << 8 |
		(unsigned char)(binaryRepr[1]);
	return networkable::Uint16(value);
}

networkable::Uint32 NetworkBuffer::readUint32(){
	char* binaryRepr = new char[4];
	buffer.read(binaryRepr, 4);
	if(buffer.gcount() != 4){
		std::cerr << "Not enough bytes in buffer\
			to read Uint32" << std::endl;
		return networkable::Uint32(0);
	}
	int value = (unsigned char)(binaryRepr[0]) << 24 |
		(unsigned char)(binaryRepr[1]) << 16 |
		(unsigned char)(binaryRepr[2]) << 8  |
		(unsigned char)(binaryRepr[3]);
	return networkable::Uint32(value);
}

networkable::Uint64 NetworkBuffer::readUint64(){
	char* binaryRepr = new char[8];
	buffer.read(binaryRepr, 8);
	if(buffer.gcount() != 8){
		std::cerr << "Not enough bytes in buffer\
			to read Uint64" << std::endl;
		return networkable::Uint64(0);
	}
	int value = 0;
	for(int i = 0; i < 8; ++i){
		auto uchar=(unsigned char)(binaryRepr[i]);
		value |= (unsigned long)(uchar) << 8*(7-i);
	}
	return networkable::Uint64(value);
}

networkable::Var_uint NetworkBuffer::readVar_uint(){
	char* binaryStart = new char[1];
	buffer.read(binaryStart, 1);
	if(buffer.gcount() != 1){
		std::cerr << "Attempted to read empty Var_uint" 
			<< std::endl;
		return networkable::Var_uint(0);
	}
	if ( binaryStart[0] < char(0xfd) ){
		return networkable::Var_uint(binaryStart[0]);
	}
	else if ( binaryStart[0] == char(0xfd)){
		char* binaryValue = new char[2];
		buffer.read(binaryValue, 2);
		if(buffer.gcount() != 2){
			std::cerr << "Invalid length in Var_uint"
				<< std::endl;
			return networkable::Var_uint(0);
		}
		int value = ((unsigned char)(binaryValue[0]) << 8) |
			((unsigned char)(binaryValue[1]));

		return networkable::Var_uint(value);
	}
	else if ( binaryStart[0] == char(0xfe)){
		char* binaryValue = new char[4];
		buffer.read(binaryValue, 4);
		if(buffer.gcount() != 4){
			std::cerr << "Invalid length in Var_uint"
				<< std::endl;
			return networkable::Var_uint(0);
		}
		int value = ((unsigned char)(binaryValue[0]) << 24) |
			((unsigned char)(binaryValue[1]) << 16) |
			((unsigned char)(binaryValue[2]) << 8)  |
			((unsigned char)(binaryValue[3]));
		return networkable::Var_uint(value);
	}
	else{
		char* binaryValue = new char[8];
		buffer.read(binaryValue, 8);
		if(buffer.gcount() != 8){
			std::cerr << "Invalid length in Var_uint"
				<< std::endl;
			return networkable::Var_uint(0);
		}
		int value = 0;
		for(int i = 0; i < 8; ++i){
			auto byte = (unsigned char)(binaryValue[i]);
			value |= (uint64_t)(byte) << 8*(7-i);
		}
		return networkable::Var_uint(value);
	}
}

networkable::Var_str NetworkBuffer::readVar_str(){
	auto length = readVar_uint().getValue();
	unsigned int readLength = 0;
	unsigned int buffer_size = 4096;
	char * stringBuffer = new char[buffer_size];
	std::string readString = "";
	while( length - readLength > buffer_size){
		readLength += buffer_size;
		buffer.read(stringBuffer, buffer_size);
		readString += stringBuffer;
		if(buffer.gcount() != buffer_size){
			std::cerr << "Inavlid length in Var_str" 
				<< std::endl;
			return networkable::Var_str("");
		}
	}
	buffer.read(stringBuffer, length - readLength);
	if((unsigned int) buffer.gcount() != (length - readLength)){
			std::cerr << "Inavlid length in Var_str" 
				<< std::endl;
			return networkable::Var_str("");
	}
	readString += stringBuffer;
	return networkable::Var_str(readString);
}

networkable::Inv_vect NetworkBuffer::readInv_vect(){
	int typeInt = readUint32().getValue();
	networkable::Inv_vect::ressource_type type;
	if( typeInt == 0){
		type = networkable::Inv_vect::txRes;
	}
	else if(typeInt == 1){
		type = networkable::Inv_vect::blockRes;
	}
	else{
		std::cerr << "Invalid type in Inv_vect" << std::endl;
		type = networkable::Inv_vect::invalidRes;
	}
	auto hash = readHash();
	return networkable::Inv_vect(type,hash);
}

networkable::Address NetworkBuffer::readAddress(){
	auto timestamp = readUint64().getValue();
	char * addressBuffer = new char [16];
	buffer.read(addressBuffer, 16);
	if(buffer.gcount() != 16){
		std::cerr << "Invalid address lenght in Address"
			<< std::endl;
		return networkable::Address(timestamp,
				std::string(16,'0'),
				0);
	}
	auto port = readUint16().getValue();
	return networkable::Address(timestamp, addressBuffer, port);
};

std::string NetworkBuffer::readHash(){
	char * hashBuffer = new char[32];
	buffer.read(hashBuffer, 32);
	if(buffer.gcount() != 32){
		std::cerr << "Invalid hash" << std::endl;
		return std::string(32,char(0x00));
	}
	return hashBuffer;
}

networkable::MessageHeader NetworkBuffer::readMessageHeader(){
	auto magic = readUint32().getValue();
	char * typeBuffer = new char[12];
	buffer.read(typeBuffer, 12);
	if(buffer.gcount() != 12){
		std::cerr << "Invalid type in MessageHeader : " << typeBuffer << std::endl;
		return networkable::MessageHeader(magic,"invalid",0);
	}
	auto payloadLength = readVar_uint().getValue();
	return networkable::MessageHeader(magic,typeBuffer,payloadLength);
}
