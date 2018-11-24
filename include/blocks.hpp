#ifndef BLOCKS_HPP
#define BLOCKS_HPP

#include <string>
#include <vector>
#include <memory>
#include <ctime>
#include <rapidjson/document.h>

#include "jsonable.hpp"
#include "transaction.hpp"
#include "hashmemory.hpp"


/// \brief Header of a Block
struct BlockHeader{
	/// \brief Version of protocol
	int version;
	/// \brief Flags
	std::vector<std::string> blockFlags;
	/// \brief Hash of the previous block
	std::string hashPrevBlock;
	/// \brief Hash of the Transaction vector
	std::string hashTransactions;
	/// \brief Timestamp of the creation
	std::time_t timestamp;
	/// \brief Block nonce
	int nonce;
	
	std::string rawStr() const;
	std::string hash() const;
};

/// \brief Block of ensicoin Transaction
class Block final : public JSONAble {
	private:
		BlockHeader header;
		std::vector<std::shared_ptr<Transaction> > transactions;
	public:
		/// \brief Creates a block with only 0 or
		/// empty structures
		Block();
		/// \brief Construct a Block
		/// \param head header for the Block
		/// \param transactionList vector of 
		/// Transaction to be included in block
		Block(BlockHeader head, 
		      std::vector<std::shared_ptr<Transaction> > transactionList);
		/// \brief Reads a JSON Block
		/// \param document a rapidjson::Document pointer
		/// to be read
		/// \details Reads a block from the JSON
		/// representation, does not check if the 
		/// JSON represents a valid Block.
		explicit Block(rapidjson::Document* document);
		
		/// \brief Raw string represntation
		/// \details The raw string representation is 
		/// obtained by concanating each field of the 
		/// Block without separator
		std::string rawStr() const;

		/// \brief Hash of the Block
		/// \details Creates the hash by applying twice
		/// sha256 to the raw string
		std::string hash() const;
		/// \brief lossy compression of the hash for 
		/// BlockTarget comparaison
		std::string compressedHash() const;

		/// \brief Validates the Block
		/// \details Checks if all Transaction are valid
		/// and that a coinbase exists
		bool validate();

		rapidjson::Document json() const override;
};

/// \brief A target for a Block
class BlockTarget{
	private:
		std::string value;
	public:
		void update(int timeTaken);
		bool blockInferior(const Block& block) const;
};


#endif /* BLOCKS_HPP */
