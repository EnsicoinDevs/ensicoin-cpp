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

		bool exists(UTXO id) const;

		void add(UTXO id, int value, bool coinbase);
		void spend(UTXO txid);
	private:
		UTXOdata getData(UTXO id) const;
		leveldb::DB* db;
};

#endif /* UTXOMANAGER_HPP */
