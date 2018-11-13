#ifndef UTXOMANAGER_HPP
#define UTXOMANAGER_HPP

#include "transaction.hpp"

#include <leveldb/db.h>
#include <memory>
#include <string>

using UTXO = TransactionIdentifier;

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
		
		/// \brief Get the value of an UTXO
		int getValue(UTXO id) const;
		/// \brief Check if an UTXO is a coinbase
		bool isCoinbase(UTXO id) const;
		/// \brief Get the height of the Block including
		/// the UTXO
		int getHeight(UTXO id) const;
		/// \brief Get the script of the UTXO
		std::vector<std::string> getOutputScript(UTXO id) const;
		/// \brief Get the hash to be used for calculating
		/// the signature
		std::string getHashSignature(UTXO id) const;
		
		/// \brief Check the existence of an UTXO
		bool exists(UTXO id) const;
		
		/// \brief Add an UTXO to the database
		void add(UTXO id, UTXOdata data);
		/// \brief Delete an UTXO because it was spent
		void spend(UTXO txid);
	private:
		/// \brief Retrive an element from the database
		UTXOdata getData(UTXO id) const;
		/// \brief Database storing the UTXO
		leveldb::DB* db;
};

#endif /* UTXOMANAGER_HPP */
