#include "networkbuffer.hpp"
#include "networkable.hpp"

#include <iostream>
#include <string>

NetworkBuffer::NetworkBuffer() {}

NetworkBuffer::NetworkBuffer(const std::string& binaryString){
	buffer << binaryString;
}

void NetworkBuffer::appendBytes(const networkable::Networkable& 
		object){
	buffer << object.byteRepr(); 
}

void NetworkBuffer::appendRawData(const std::string& rawData){
	buffer << rawData;
}

uint16_t NetworkBuffer::readUint16(){
	char* binaryRepr = new char[2];
	buffer.read(binaryRepr, 2);
	if(buffer.gcount() != 2){
		std::cerr << "Not enough bytes in buffer\
			to read Uint16" << std::endl;
		return 0;
	}
	int value = (unsigned char)(binaryRepr[0]) << 8 |
		(unsigned char)(binaryRepr[1]);
	return value;
}

uint32_t NetworkBuffer::readUint32(){
	char* binaryRepr = new char[4];
	buffer.read(binaryRepr, 4);
	if(buffer.gcount() != 4){
		std::cerr << "Not enough bytes in buffer\
			to read Uint32" << std::endl;
		return 0;
	}
	int value = (unsigned char)(binaryRepr[0]) << 24 |
		(unsigned char)(binaryRepr[1]) << 16 |
		(unsigned char)(binaryRepr[2]) << 8  |
		(unsigned char)(binaryRepr[3]);
	return value;
}

uint64_t NetworkBuffer::readUint64(){
	char* binaryRepr = new char[8];
	buffer.read(binaryRepr, 8);
	if(buffer.gcount() != 8){
		std::cerr << "Not enough bytes in buffer\
			to read Uint64" << std::endl;
		return 0;
	}
	int value = 0;
	for(int i = 0; i < 8; ++i){
		auto uchar=(unsigned char)(binaryRepr[i]);
		value |= (unsigned long)(uchar) << 8*(7-i);
	}
	return value;
}

uint64_t NetworkBuffer::readVar_uint(){
	char* binaryStart = new char[1];
	buffer.read(binaryStart, 1);
	if(buffer.gcount() != 1){
		std::cerr << "Attempted to read empty Var_uint" 
			<< std::endl;
		return 0;
	}
	if ( binaryStart[0] < char(0xfd) ){
		return binaryStart[0];
	}
	else if ( binaryStart[0] == char(0xfd)){
		char* binaryValue = new char[2];
		buffer.read(binaryValue, 2);
		if(buffer.gcount() != 2){
			std::cerr << "Invalid length in Var_uint"
				<< std::endl;
			return 0;
		}
		int value = ((unsigned char)(binaryValue[0]) << 8) |
			((unsigned char)(binaryValue[1]));

		return value;
	}
	else if ( binaryStart[0] == char(0xfe)){
		char* binaryValue = new char[4];
		buffer.read(binaryValue, 4);
		if(buffer.gcount() != 4){
			std::cerr << "Invalid length in Var_uint"
				<< std::endl;
			return 0;
		}
		int value = ((unsigned char)(binaryValue[0]) << 24) |
			((unsigned char)(binaryValue[1]) << 16) |
			((unsigned char)(binaryValue[2]) << 8)  |
			((unsigned char)(binaryValue[3]));
		return value;
	}
	else{
		char* binaryValue = new char[8];
		buffer.read(binaryValue, 8);
		if(buffer.gcount() != 8){
			std::cerr << "Invalid length in Var_uint"
				<< std::endl;
			return 0;
		}
		int value = 0;
		for(int i = 0; i < 8; ++i){
			auto byte = (unsigned char)(binaryValue[i]);
			value |= (uint64_t)(byte) << 8*(7-i);
		}
		return value;
	}
}

std::string NetworkBuffer::readStr(size_t length){
	unsigned int readLength = 0;
	unsigned int buffer_size = 4096;
	char * stringBuffer = new char[buffer_size];
	std::string readString = "";
	while( length - readLength > buffer_size){
		readLength += buffer_size;
		buffer.read(stringBuffer, buffer_size);
		readString += stringBuffer;
		if(buffer.gcount() != buffer_size){
			std::cerr << "Inavlid length in Str" 
				<< std::endl;
			return "";
		}
	}
	buffer.read(stringBuffer, length - readLength);
	if((unsigned int) buffer.gcount() != (length - readLength)){
			std::cerr << "Inavlid length in Str" 
				<< std::endl;
			return "";
	}
	readString += stringBuffer;
	return readString;
}
