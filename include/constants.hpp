#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <cstdlib>
#include <string>  

constexpr int MAX_BLOCK_SIZE = 2*1024;
constexpr int MESSAGE_LIMIT = 2*MAX_BLOCK_SIZE;
constexpr int VERSION = 0;
constexpr int MAGIC = 422021;
constexpr int PORT = 4224;

extern const std::string DATA_PATH;

extern const std::string UTXO_DB;
extern const std::string BLOCKCHAIN_DB;

#endif /* CONSTANTS_HPP */
