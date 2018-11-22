#ifndef UTIL_HPP
#define UTIL_HPP

#include <iomanip>
#include <iostream>
#include <ostream>
#include <string>

namespace util{
	struct HexCharStruct
	{
		unsigned char c;
		explicit HexCharStruct(unsigned char _c);
	};

	inline std::ostream& operator<<(std::ostream& o, const HexCharStruct& hs)
	{
		return (o << std::hex << std::setfill('0') << std::setw(2) << (int)hs.c);
	}

	inline HexCharStruct hex(unsigned char _c)
	{
		return HexCharStruct(_c);
	}

	inline void printBinaryString(const std::string& bs){
		for(auto& c : bs){
			std::cerr << hex(c) << std::endl;
		}
	}
} // namespace util

#endif /* UTIL_HPP */
