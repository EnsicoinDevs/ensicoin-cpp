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
	public:
		Mempool();
		
		bool addTransaction(std::shared_ptr<Transaction> tr);
		
		bool checkInputReference(std::shared_ptr<Transaction> tx) const ;

		int getInputValue(UTXO id) const;
		bool exists(std::string txHash) const;
		bool orphanExists(std::string txHash) const;
		bool outputExists(UTXO id) const;
		
		bool isOrphan(std::shared_ptr<Transaction> tx) const;
		std::string getHashSignature(UTXO id) const;

		bool isSpendable(UTXO id) const;
		std::vector<std::string> getOutputScript(UTXO id) const;
		void incrementHeight();
		void decrementHeight();

		std::vector<UTXO> orphanDeps(std::shared_ptr<Transaction> tx) const;

		TXType type(std::string txHash) const;
	private:
		UTXOManager utxos;
		int currentHeight;
		
		void updateOrphan(UTXO id);

		HashMemory<Transaction> mainPool;
		HashMemory<Transaction> orphans;
		// UTXO str-> Orphan using said UTXO
		std::map< std::string , std::string > orphanUsingUTXO;
};	

#endif /* MEMPOOL_HPP */
