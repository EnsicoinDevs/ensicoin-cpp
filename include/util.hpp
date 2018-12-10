#ifndef UTIL_HPP
#define UTIL_HPP

#include <iomanip>
#include <iostream>
#include <ostream>
#include <string>

/// \brief Utility functions
namespace util{
	/// \brief To show char a hexadecimal
	struct HexCharStruct
	{
		/// \brief Char to be converted to hexadecimal
		unsigned char c;
		/// \brief Create a HexCharStruct from a char
		explicit HexCharStruct(unsigned char _c);
	};

	/// \brief Insert into string as a padded hexadecimal number of two digits
	inline std::ostream& operator<<(std::ostream& o, const HexCharStruct& hs)
	{
		return (o << std::hex << std::setfill('0') << std::setw(2) << (int)hs.c);
	}
	
	/// \brief Convert char to hexadecimal
	inline HexCharStruct hex(unsigned char _c)
	{
		return HexCharStruct(_c);
	}
	
	/// \brief Print each byte of a binary string
	inline void printBinaryString(const std::string& bs){
		for(auto& c : bs){
			std::cerr << hex(c) << "|";
		}
		std::cout << std::endl;
	}
} // namespace util

#endif /* UTIL_HPP */
