#ifndef LINKEDTRANSACTION_HPP
#define LINKEDTRANSACTION_HPP

#include "transaction.hpp"
#include "utxo.hpp"

#include <memory>
#include <unordered_set>

class Mempool;

/// \brief Links a Transaction to the UTXO it depends on
/// \todo Add Transaction as parent class
class LinkedTransaction : public Transaction {
	public:
		using pointer = std::shared_ptr<LinkedTransaction>;
		/// \brief Retrieves necessary UTXOs to create 
		/// the LinkedTransaction from a Transaction
		explicit LinkedTransaction(
			std::shared_ptr<Transaction> tx, 
			Mempool* mempool, 
			UTXOManager* utxos);
		
		bool isOrphan() const;
		/// \brief Gives all inputs who are orphaned
		std::vector<UTXO> getOrphanDeps() const;
		/// \brief Get the value of inputs
		unsigned int inputValue() const;
		

		/// \brief Validate the Transaction
		bool validate(int currentHeight);
		/// \brief Execute the Script to
		/// validate the Transaction
		bool validateScript();
	private:
		Mempool* mempool;
		UTXOManager* utxos;

		std::unordered_set< UTXO > dependencies;
		std::unordered_set< UTXO > mempoolDeps;
};

#endif /* LINKEDTRANSACTION_HPP */
