#ifndef NETWORKBUFFER_HPP
#define NETWORKBUFFER_HPP

#include "networkable.hpp"
#include <sstream>
#include <string>

/// \brief Class to hold Networkable data
class NetworkBuffer{
	public:
		/// \brief Read a networkable::Var_uint from the 
		/// buffer
		/// \details Consume the bytes from the buffer
		networkable::Var_uint readVar_uint();
		/// \brief Read a networkable::Uint16 from the buffer
		/// \details Consume the bytes from the buffer
		networkable::Uint16 readUint16();
		/// \brief Read a networkable::Uint32 from the buffer
		/// \details Consume the bytes from the buffer
		networkable::Uint32 readUint32();
		///\brief Read a networkable::Uint64 from the buffer
		/// \details Consume the bytes from the buffer
		networkable::Uint64 readUint64();
		///\brief Read a networkable::Var_str from the buffer
		/// \details Consume the bytes from the buffer
		networkable::Var_str readVar_str();
		/// \brief Read a networkable::Var_str from the 
		/// buffer
		/// \details Consume the bytes from the buffer
		networkable::Inv_vect readInv_vect();
		/// \brief Read a networkable::Address from the 
		/// buffer
		/// \details Consume the bytes from the buffer
		networkable::Address readAddress();
		/// \brief Read a hash from the buffer
		/// \details Consume the bytes from the buffer
		std::string readHash();
		/// \brief Read a networkable::MessageHeader from 
		/// the buffer
		/// \details Consume the bytes from the buffer
		networkable::MessageHeader readMessageHeader();
		/// \brief Read a networkable::Str from the buffer
		/// \param length of the string
		/// \details Consume the bytes from the buffer
		networkable::Str readStr(size_t length);
		/// \brief Adds a networkable::Networkable to 
		/// the NetworkBuffer
		void appendBytes(const networkable::Networkable&
				object);
		
		/// \brief Add bytes to the NetworkBuffer
		void appendRawData(const std::string& rawData);

		/// \brief Construct a NetworkBuffer from a binaryString
		explicit NetworkBuffer(const std::string& binaryString);
	private:
		/// \brief Stringstream used as a buffer 
		std::stringstream buffer;
};

#endif /* NETWORKBUFFER_HPP */
