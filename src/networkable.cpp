#include "networkable.hpp"
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

	Uint32::Uint32(uint32_t val) : value(val) {}

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

	Var_uint::Var_uint(uint64_t val) : value(val) {}

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

	std::string Str::getValue() const{
		return value;
	}
	std::string Str::byteRepr() const{
		return value;
	}
	Str::Str(const std::string& val) : value(val) {}

	Var_str::Var_str(const std::string& val) : value(val) {}

	std::string Var_str::getValue() const{
		return value.getValue();
	}
	std::string Var_str::byteRepr() const{
		auto sizeBytes =
			Var_uint(value.getValue().size()).byteRepr();
		return sizeBytes + value.byteRepr();
	}

	Inv_vect::Inv_vect(Inv_vect::ressource_type res, 
			std::string resHash) : 
		type(res),
		hash(resHash) {}

	std::string Inv_vect::byteRepr() const{
		if( type == invalidRes ){
			std::cerr << "Cannot create bytes of\
				invalidRes" << std::endl;
			return "INVALID RESSOURCE";
		}
		return Uint32(type).byteRepr() + 
			Str(hash).byteRepr();
	}

	MessageHeader::MessageHeader(uint32_t _magic, 
			std::string _type,
			uint64_t pldSize) : 
		magic(_magic),
		type(_type),
		payloadLength(pldSize) {}
	std::string MessageHeader::byteRepr() const{
		std::string out = Uint32(magic).byteRepr();
		out += Str(type).byteRepr();
		out += Var_uint(payloadLength).byteRepr();
		return out;
	}

} // namespace networkable
