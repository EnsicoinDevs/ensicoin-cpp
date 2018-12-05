#ifndef UTXO_HPP
#define UTXO_HPP

#include "transaction.hpp"
#include "networkable.hpp"
#include "networkbuffer.hpp"

#include <leveldb/db.h>
#include <memory>
#include <spdlog/spdlog.h>
#include <string>

/// \brief Classes handling all ressources
namespace manager{

	/// \brief Data needed to use a ressource::UTXO
	struct UTXOdata : public networkable::Networkable{
		/// \brief The value of the output
		uint64_t value;
		/// \brief Denotes if the output is a coinbase
		bool coinbase;
		/// \brief Tells the Height of the Block including
		/// the Transaction
		int height;
		/// \brief The hash of the Transaction to check the signature
		std::string signingHash;
		/// \brief Is the Transaction referenced taken from
		/// the Mempool
		bool fromMempool;
		/// \brief The code to create the Script
		ressources::Script script;

		/// \brief Construct a UTXOData
		UTXOdata(int val, bool isCoinbase, int ht, std::string shash,bool isFromMempool, ressources::Script sc);
		/// \brief Parse UTXOdata from raw bytes
		/// \param networkBuffer buffer containing raw data
		explicit UTXOdata(NetworkBuffer* networkBuffer);

		std::string byteRepr() const override;
	};

	/// \brief Managing UTXOs (Unspent OutputTransaction)
	class UTXOManager{
		public:
			/// \brief Initialize the database
			UTXOManager(std::shared_ptr<spdlog::logger> logger_);

			/// \brief Retrive an element from the database
			UTXOdata getData(ressources::UTXO id) const;

			/// \brief Check the existence of an UTXO
			bool exists(ressources::UTXO id) const;

			/// \brief Tells if an UTXO can be spend at a 
			/// given height
			bool isSpendable(ressources::UTXO id, int currentHeight) const;

			/// \brief Add an UTXO to the database
			void add(ressources::UTXO id, UTXOdata data);
			/// \brief Delete an UTXO because it was spent
			void spend(ressources::UTXO txid);
		private:
			std::shared_ptr<spdlog::logger> logger;
			/// \brief Database storing the UTXO
			leveldb::DB* db;
	};
} // namespace manager

namespace std {
		template <> struct hash<ressources::UTXO>
		{
			/// \brief Get the hash of the UTXO as the hash of the rawStr()
			size_t operator()(const ressources::UTXO & x) const
			{
				return hash<std::string>()(x.byteRepr());
			}
		};
} // namespace std;
#endif /* UTXO_HPP */
