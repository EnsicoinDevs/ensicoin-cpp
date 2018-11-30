#ifndef NETWORKBUFFER_HPP
#define NETWORKBUFFER_HPP

#include "networkable.hpp"
#include <sstream>
#include <string>

/// \brief Class to hold Networkable data
class NetworkBuffer{

	friend class networkable::Uint16;
	friend class networkable::Uint32;
	friend class networkable::Uint64;
	friend class networkable::Str;
	friend class networkable::Var_uint;
	public:
		/// \brief Adds a networkable::Networkable to 
		/// the NetworkBuffer
		void appendBytes(const networkable::Networkable&
				object);
		
		/// \brief Add bytes to the NetworkBuffer
		void appendRawData(const std::string& rawData);

		/// \brief Construct a NetworkBuffer from a binaryString
		explicit NetworkBuffer(const std::string& binaryString);
		/// \brief Construct an empty NetworkBuffer
		NetworkBuffer();

	protected:
		/// \brief Read a networkable::Var_uint from the 
		/// buffer
		/// \details Consume the bytes from the buffer
		uint64_t readVar_uint();
		/// \brief Read a networkable::Uint16 from the buffer
		/// \details Consume the bytes from the buffer
		uint16_t readUint16();
		/// \brief Read a networkable::Uint32 from the buffer
		/// \details Consume the bytes from the buffer
		uint32_t readUint32();
		///\brief Read a networkable::Uint64 from the buffer
		/// \details Consume the bytes from the buffer
		uint64_t readUint64();
		/// \brief Read a networkable::Str from the buffer
		/// \param length of the string
		/// \details Consume the bytes from the buffer
		std::string readStr(size_t length);
	private:
		/// \brief Stringstream used as a buffer 
		std::stringstream buffer;
};

#endif /* NETWORKBUFFER_HPP */
