#ifndef BLOCKCHAIN_HPP
#define BLOCKCHAIN_HPP

#include "utxo.hpp"
#include "blocks.hpp"

#include <leveldb/db.h>
#include <string>
#include <map>
#include <memory>
#include <spdlog/spdlog.h>

namespace manager{

	/// \brief Stores Block on disk and links them
	class Blockchain{
		private:
			/// \brief DB storing Block
			leveldb::DB* blocksDB;
			/// \brief DB storing the following Block
			leveldb::DB* chainDB;
			/// \brief DB storing stats on the Blockchain
			leveldb::DB* statDB;
			/// \brief DB storing data on how to update 
			/// the Blockchain
			leveldb::DB* updateDB;
			std::shared_ptr<spdlog::logger> logger;
		public:
			ressources::Block getBlock(const std::string& hash);
			void registerBlock(const ressources::Block& block);
			void deleteBlock(const std::string& hash);
			Blockchain(std::shared_ptr<spdlog::logger> logger_);
	};

	class BlockIndex{
		private:
			Blockchain blockchain;
			int blockCount;
			std::map<std::string, ressources::Block> cachedBlocks;
			std::shared_ptr<spdlog::logger> logger;
		public:
			std::vector<ressources::BlockTarget> getWindowFrom(
					const std::string& statringBlock);
			ressources::Block::pointer retrieve(
					const std::string& hash);
			void popBlock(UTXOManager* utxos);
			void pushBlock(UTXOManager* utxos, 
					const ressources::Block& block);
			BlockIndex(std::shared_ptr<spdlog::logger> logger_);
	};

} // namespace manager
#endif /* BLOCKCHAIN_HPP */
