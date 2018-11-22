#ifndef DEBUG_HPP
#define DEBUG_HPP

#include <ostream>

namespace debug{
	struct HexCharStruct
	{
		unsigned char c;
		explicit HexCharStruct(unsigned char _c);
	};

	inline std::ostream& operator<<(std::ostream& o, const HexCharStruct& hs)
	{
		return (o << std::hex << (int)hs.c);
	}

	inline HexCharStruct hex(unsigned char _c)
	{
		return HexCharStruct(_c);
	}
} // namespace debug

#endif /* DEBUG_HPP */
