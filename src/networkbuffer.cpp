#include "networkbuffer.hpp"
#include "networkable.hpp"

#include <iostream>
#include <string>

NetworkBuffer::NetworkBuffer(std::shared_ptr<spdlog::logger> logger_) : logger(logger_) {}

NetworkBuffer::NetworkBuffer(const std::string& binaryString,std::shared_ptr<spdlog::logger> logger_) : logger(logger_) {
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
		logger->error("error in reading Uint16 from buffer : expected 2 bytes got {}", buffer.gcount());
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
		logger->error("error in reading Uint32 from buffer : expected 4 bytes got {}", buffer.gcount());
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
		logger->error("error in reading Uint64 from buffer : expected 8 bytes got {}", buffer.gcount());
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
		logger->error("error in reading Var_uint from buffer : read empty buffer");
		return 0;
	}
	if ( binaryStart[0] < char(0xfd) ){
		return binaryStart[0];
	}
	else if ( binaryStart[0] == char(0xfd)){
		char* binaryValue = new char[2];
		buffer.read(binaryValue, 2);
		if(buffer.gcount() != 2){
			logger->error("error in reading Var_uint from buffer : expected 2 bytes got {}", buffer.gcount());
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
			logger->error("error in reading Var_uint from buffer : expected 4 bytes got {}", buffer.gcount());
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
			logger->error("error in reading Var_uint from buffer : expected 8 bytes got {}", buffer.gcount());
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
			logger->error("error in reading Str from buffer : expected {} bytes got {}",buffer_size, buffer.gcount());
			return "";
		}
	}
	buffer.read(stringBuffer, length - readLength);
	if((unsigned int) buffer.gcount() != (length - readLength)){
			logger->error("error in reading Str from buffer : expected {} bytes got {}",(length-readLength), buffer.gcount());
			return "";
	}
	readString += stringBuffer;
	return readString;
}
