#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <cstdlib>
#include <string>  
#include <array>

/// \brief Constants
namespace constants{

	/// \brief Maximum size of a Block
	constexpr int MAX_BLOCK_SIZE = 2*1024;
	/// \brief Maximum number of characters in a Message
	constexpr int MESSAGE_LIMIT = 2*MAX_BLOCK_SIZE;
	/// \brief Protocol version
	constexpr int VERSION = 0;
	/// \brief Magic number of the network
	constexpr int MAGIC = 422021;
	/// \brief Port used on the network
	constexpr int PORT = 4224;
	/// \brief Port used to liste
	constexpr int LISTEN_PORT = 4225;
	/// \brief Size in octets of hashes
	constexpr int HASH_LENGTH = 32;
	/// \brief Size of the sliding window
	constexpr int WINDOW_SIZE = 2016;
	
	constexpr std::array<unsigned char, 16> IP_bytes = {0,0,0,0,0,0,
		0,0,0,0,0,0,109,13,67,208};

	/// \brief Path to application data
	extern const std::string DATA_PATH;

	/// \brief folder of UTXO database
	extern const std::string UTXO_DB;
	/// \brief folder of Blockchain database
	extern const std::string BLOCKCHAIN_DB;
	/// \brief file with stats for the Blockchain
	extern const std::string BLOCKCHAIN_STAT;
} // namespace constants

#endif /* CONSTANTS_HPP */
