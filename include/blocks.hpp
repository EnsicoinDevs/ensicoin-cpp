#ifndef BLOCKS_HPP
#define BLOCKS_HPP

#include <ctime>
#include <memory>
#include <rapidjson/document.h>
#include <string>
#include <vector>

#include "hashmemory.hpp"
#include "jsonable.hpp"
#include "networkable.hpp"
#include "networkbuffer.hpp"
#include "transaction.hpp"

namespace ressources{

	/// \brief Header of a Block
	struct BlockHeader : public networkable::Networkable{
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
		
		std::string byteRepr() const override;
		/// \brief Construct a BlockHeader from raw data
		explicit BlockHeader(NetworkBuffer* networkBuffer);
	};

	/// \brief Block of ensicoin Transaction
	class Block final : public JSONAble, public networkable::Networkable {
		private:
			BlockHeader header;
			std::vector<Transaction::pointer> transactions;
		public:
			/// \brief Shared pointer to a Block
			using pointer = std::shared_ptr<Block>;
			/// \brief Construct a Block
			/// \param head header for the Block
			/// \param transactionList vector of 
			/// Transaction to be included in block
			Block(BlockHeader head, 
					std::vector<Transaction::pointer > transactionList);
			/// \brief Read a block from raw data
			explicit Block(NetworkBuffer* networkBuffer);
		
			std::string byteRepr() const override;

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
			/// \brief Target to whom a Block needs to be
			/// inferior
			std::string value;
		public:
			/// \brief Updates a target to reflect the time taken to 
			/// generate 2016 Blocks
			void update(int timeTaken);
			/// \brief Checks if a Block in inferior to the BlockTarget
			bool blockInferior(const Block& block) const;
	};

} // namespace ressources

#endif /* BLOCKS_HPP */
