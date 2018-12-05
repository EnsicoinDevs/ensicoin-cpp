#ifndef NETWORKBUFFER_HPP
#define NETWORKBUFFER_HPP

#include "networkable.hpp"

#include <spdlog/spdlog.h>
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
		NetworkBuffer(const std::string& binaryString,std::shared_ptr<spdlog::logger> logger_);
		/// \brief Construct an empty NetworkBuffer
		explicit NetworkBuffer(std::shared_ptr<spdlog::logger> logger_);
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
		std::shared_ptr<spdlog::logger> logger;
};

namespace asio{
	inline const NetworkBuffer* buffer_sequence_begin(const NetworkBuffer& buffer){
		return &buffer;
	}
	inline const NetworkBuffer* buffer_sequence_end(const NetworkBuffer& buffer){
		return &buffer;
	}
}

#endif /* NETWORKBUFFER_HPP */
