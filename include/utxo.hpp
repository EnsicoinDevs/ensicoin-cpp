#ifndef UTXOMANAGER_HPP
#define UTXOMANAGER_HPP

#include "transaction.hpp"

#include <leveldb/db.h>
#include <memory>
#include <string>

using UTXO = TransactionIdentifier;

struct UTXOdata{
	int value;
	bool coinbase;
	int height;
	std::string hashWithoutInputs;

	std::vector<std::string> script;
	
	UTXOdata(int, bool);
	explicit UTXOdata(std::string jsonStr);

	std::string str() const;
};

class UTXOManager{
	public:
		UTXOManager();
		
		int getValue(UTXO id) const;
		bool isCoinbase(UTXO id) const;
		int getHeight(UTXO id) const;
		std::vector<std::string> getOutputScript(UTXO id) const;
		std::string getHashSignature(UTXO id) const;

		bool exists(UTXO id) const;

		void add(UTXO id, UTXOdata data);
		void spend(UTXO txid);
	private:
		UTXOdata getData(UTXO id) const;
		leveldb::DB* db;
};

#endif /* UTXOMANAGER_HPP */
