#ifndef MEMPOOL_HPP
#define MEMPOOL_HPP

#include "hashmemory.hpp"
#include "transaction.hpp"
#include "utxo.hpp"

#include <map>
#include <memory>
#include <string>
#include <vector>

class Mempool{
	using outpoint = std::pair<std::string, int>;
	public:
		Mempool();
		
		bool addTransaction(std::shared_ptr<Transaction> tr);
		
		bool checkInputReference(std::shared_ptr<Transaction> tx) const ;

		int getInputValue(UTXO id) const;
		bool exists(std::string txHash) const;
		bool isOrphan(std::shared_ptr<Transaction> tx) const;
		std::string getHashSignature(UTXO id) const;

		bool isSpendable(UTXO id, int currentHeight) const;
		std::vector<std::string> getOutputScript(UTXO id) const;

		TXType type(std::string txHash) const;
	private:
		UTXOManager utxos;
		
		HashMemory<Transaction> mainPool;
		HashMemory<Transaction> orphans;
		std::map< outpoint , std::string > orphanUsingHash;
};	

#endif /* MEMPOOL_HPP */
