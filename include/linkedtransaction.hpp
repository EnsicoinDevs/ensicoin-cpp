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
		/// \brief Shared pointer to a LinkedTransaction
		using pointer = std::shared_ptr<LinkedTransaction>;
		/// \brief Retrieves necessary UTXOs to create 
		/// the LinkedTransaction from a Transaction
		explicit LinkedTransaction(
			std::shared_ptr<Transaction> tx, 
			Mempool* mempool, 
			UTXOManager* utxos);
		
		/// \brief Checks if all inputs are linked to
		/// an UTXO
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
		/// \brief Reference to a Mempool to fetch UTXO
		Mempool* mempool;
		/// \brief Reference to a UTXOManager to fetch
		/// UTXO
		UTXOManager* utxos;

		/// \brief List of Linked UTXO in blocks
		std::unordered_set< UTXO > dependencies;
		/// \brief List of Linked UTXO in a Mempool
		std::unordered_set< UTXO > mempoolDeps;
};

#endif /* LINKEDTRANSACTION_HPP */
