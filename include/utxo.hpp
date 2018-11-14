#ifndef UTXOMANAGER_HPP
#define UTXOMANAGER_HPP

#include "transaction.hpp"

#include <leveldb/db.h>
#include <memory>
#include <string>

using UTXO = TransactionIdentifier;

namespace std {
	template <> struct hash<UTXO>
	{
		size_t operator()(const UTXO & x) const
		{
			return hash<std::string>()(x.str());
		}
	};
} // namespace std;

inline bool operator==(const UTXO& lhs, const UTXO& rhs){
	return lhs.transactionHash == rhs.transactionHash 
		&& lhs.index == rhs.index;
}
inline bool operator!=(const UTXO& lhs, const UTXO& rhs){return !operator==(lhs,rhs);}

/// \brief Data needed to use a UTXO
struct UTXOdata{
	/// \brief The value of the output
	int value;
	/// \brief Denotes if the output is a coinbase
	bool coinbase;
	/// \brief Tells the Height of the Block including
	/// the Transaction
	int height;
	/// \brief The hash of the Transaction to check the signature
	std::string hashWithoutInputs;
	/// \brief Is the Transaction referenced taken from
	/// the Mempool
	bool fromMempool;
	/// \brief The code to create the Script
	std::vector<std::string> script;

	/// \brief TODO Add fields and fluent API
	UTXOdata(int, bool);
	/// \brief Parse a JSON string into an UTXOdata object
	/// \param jsonStr the JSON string to be parsed
	explicit UTXOdata(std::string jsonStr);

	/// \brief Get the JSON string representing the UTXOdata 
	/// object
	std::string str() const;
};

/// \brief Managing UTXOs (Unspent OutputTransaction)
class UTXOManager{
	public:
		/// \brief Initialize the database
		UTXOManager();

		/// \brief Retrive an element from the database
		UTXOdata getData(UTXO id) const;

		/// \brief Check the existence of an UTXO
		bool exists(UTXO id) const;

		/// \brief Tells if an UTXO can be spend at a 
		/// given height
		bool isSpendable(UTXO id, int currentHeight) const;

		/// \brief Add an UTXO to the database
		void add(UTXO id, UTXOdata data);
		/// \brief Delete an UTXO because it was spent
		void spend(UTXO txid);
	private:
		/// \brief Database storing the UTXO
		leveldb::DB* db;
};

#endif /* UTXOMANAGER_HPP */
