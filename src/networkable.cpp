#include "networkable.hpp"
#include "constants.hpp"
#include "networkbuffer.hpp"
#include "util.hpp"

#include <cryptopp/hex.h>
#include <iostream>
#include <iomanip>
#include <sstream>

namespace networkable{

	Networkable::~Networkable() {}

	std::string Networkable::encode(const std::string& 
			hexRepr) const{
		std::string decodedBytes;

		CryptoPP::StringSource ss(hexRepr, true,
				new CryptoPP::HexDecoder(
					new CryptoPP::\
					StringSink(decodedBytes)
					) // HexDecoder
				); // String Source
		return decodedBytes;
	}

	Uint16::Uint16(uint16_t val) : value(val) {}
	Uint16::Uint16() {}

	std::string Uint16::byteRepr() const{
		std::ostringstream sstream;
		sstream << std::setfill('0')
			<< std::setw(4) 
			<< std::hex 
			<< value;
		return encode(sstream.str());
	}
	
	uint16_t Uint16::getValue() const{
		return value;
	}

	Uint16::Uint16(NetworkBuffer* networkBuffer) :
		value(networkBuffer->readUint16()) {}

	Uint32::Uint32(uint32_t val) : value(val) {}
	Uint32::Uint32() {}

	std::string Uint32::byteRepr() const{
		std::ostringstream sstream;
		sstream << std::setfill('0')
			<< std::setw(8)
			<< std::hex
			<< value;
		return encode(sstream.str());
	}
	uint32_t Uint32::getValue() const{
		return value;
	}
	Uint32::Uint32(NetworkBuffer* networkBuffer) :
		value(networkBuffer->readUint32()) {}

	Uint64::Uint64(uint64_t val) : value(val) {}
	std::string Uint64::byteRepr() const{
		std::ostringstream sstream;
		sstream << std::setfill('0')
			<< std::setw(16)
			<< std::hex
			<< value;
		return encode(sstream.str());
	}
	uint64_t Uint64::getValue() const{
		return value;
	}
	Uint64::Uint64(NetworkBuffer* networkBuffer) :
		value(networkBuffer->readUint64()) {}
	Uint64::Uint64() {}

	Var_uint::Var_uint(uint64_t val) : value(val) {}
	Var_uint::Var_uint() {}

	uint64_t Var_uint::getValue() const{
		return value;
	}

	std::string Var_uint::byteRepr() const{
		std::ostringstream sstream;
		sstream << std::hex << value;
		auto rawStr = sstream.str();
		if(value <= 252){
			rawStr = std::string(2-rawStr.length(), '0')
				+ rawStr;
		}
		else if(value <= 0xffff){
			rawStr = "fd" + 
				std::string(4-rawStr.length(),'0') + 
				rawStr;
			// 3 : 0xFD.value
		}
		else if(value <= 0xffffffff){
			rawStr = "fe" +
				std::string(8-rawStr.length(),'0') +
				rawStr;
			// 5 : 0xFE.value
		}
		else{
			rawStr = "ff" +
				std::string(16-rawStr.length(),'0') +
				rawStr;
			// 9 0xFF.value
		}
		return encode(rawStr);
	}

	Var_uint::Var_uint(NetworkBuffer* networkBuffer) :
		value(networkBuffer->readVar_uint()) {}

	std::string Str::getValue() const{
		return value;
	}
	std::string Str::byteRepr() const{
		return value;
	}
	Str::Str(const std::string& val) : value(val) {}
	Str::Str() {}

	Str::Str(NetworkBuffer* buffer,
			 size_t stringSize) : 
		value(buffer->readStr(stringSize)){}

	Var_str::Var_str(const std::string& val) : value(val) {}

	std::string Var_str::getValue() const{
		return value.getValue();
	}
	std::string Var_str::byteRepr() const{
		auto sizeBytes =
			Var_uint(value.getValue().size()).byteRepr();
		return sizeBytes + value.byteRepr();
	}
	Var_str::Var_str(NetworkBuffer* networkBuffer){
		auto size = Var_uint(networkBuffer).getValue();
		value = Str(networkBuffer, size);
	}
	
	/*template<typename T>
	Var_Array<std::shared_ptr<T> >::Var_Array(
			NetworkBuffer* networkBuffer){
		auto size = Var_uint(networkBuffer).getValue();
		for(uint64_t i = 0; i < size; ++i){
			data.push_back(T(networkBuffer));
		}
	}

	template<typename T>
	Var_Array<std::shared_ptr<T> >::Var_Array(const std::\
			vector<std::shared_ptr<T> >& networkableArray) : 
		data(networkableArray) {}

	template<typename T>
	Var_Array<std::shared_ptr<T> >::Var_Array() : data() {}*/

	Inv_vect::Inv_vect(Inv_vect::ressource_type res, 
			std::string resHash) : 
		type(res),
		hash(resHash) {}

	Inv_vect::Inv_vect() {}
	
	Inv_vect::Inv_vect(NetworkBuffer* buffer){
		int typeInt = Uint32(buffer).getValue();
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
		hash = Str(buffer,constants::HASH_LENGTH).getValue();
	}

	std::string Inv_vect::byteRepr() const{
		if( type == invalidRes ){
			std::cerr << "Cannot create bytes of\
				invalidRes" << std::endl;
			return "INVALID RESSOURCE";
		}
		return Uint32(type).byteRepr() + 
			Str(hash).byteRepr();
	}

	Inv_vect Inv_vect::getValue() const{
		return *this;
	}

	IP Address::getAddress() const{
		return address;
	}
	uint16_t Address::getPort() const{
		return port;
	}
	uint64_t Address::getTimestamp() const{
		return timestamp;
	}
	Address Address::getValue() const{
		return *this;
	}
	Address::Address(uint64_t lastActive,
					 IP ipAddr,
					 uint16_t _port) :
		timestamp(lastActive),
		address(ipAddr),
		port(_port) {}
	Address::Address(NetworkBuffer* buffer) :
		timestamp(Uint64(buffer).getValue()),
		address(IP(buffer).getValue()),
		port(Uint16(buffer).getValue()) {}
	std::string Address::byteRepr() const{
		return Uint64(timestamp).byteRepr() + IP(address).byteRepr() + Uint16(port).byteRepr();
	}
	Address::Address() {}

	MessageHeader::MessageHeader(uint32_t _magic, 
			std::string _type,
			uint64_t pldSize) : 
		magic(_magic),
		type(_type),
		payloadLength(pldSize) {}
	std::string MessageHeader::byteRepr() const{
		std::string out = Uint32(magic).byteRepr();
		out += FixedStr<12>(type).byteRepr();
		out += Uint64(payloadLength).byteRepr();
		return out;
	}
	MessageHeader::MessageHeader(NetworkBuffer* buffer) :
		magic(Uint32(buffer).getValue()),
		type(FixedStr<12>(buffer).getValue()),
		payloadLength(Uint64(buffer).getValue()) {}
	MessageHeader::MessageHeader() {}

} // namespace networkable
