#ifndef LINKEDTRANSACTION_HPP
#define LINKEDTRANSACTION_HPP

#include "transaction.hpp"
#include "utxo.hpp"

#include <memory>
#include <unordered_set>

class Mempool;

/// \brief Links a Transaction to the UTXO it depends on
class LinkedTransaction : public std::enable_shared_from_this<LinkedTransaction> {
	public:
		using pointer = std::shared_ptr<LinkedTransaction>;
		/// \brief Retrieves necessary UTXOs to create 
		/// the LinkedTransaction from a Transaction
		explicit LinkedTransaction(
			std::shared_ptr<Transaction> tx, 
			Mempool* mempool, 
			UTXOManager* utxos);
		
		/// \brief Returns the hash of the underlying 
		/// Transaction
		std::string hash() const;
		/// \brief Get the hash used to sign the
		/// Transaction
		std::string signingHash() const;
		/// \brief Checks if the transaction is Orphan
		bool isOrphan() const;
		/// \brief Gives all inputs who are orphaned
		std::vector<UTXO> getOrphanDeps() const;
		/// \brief Get the value of inputs
		int inputValue() const;
		/// \brief Gets the value of the index-th output
		int valueOfOutput(int index) const;
		/// \brief Get the script of an output
		std::vector<std::string> scriptOfOutput(int index) const;
		/// \brief Get the value of outputs
		int outputValue() const;
		
		/// \brief Gets the number of outputs
		int outputCount() const;

		/// \brief Validate the Transaction
		bool validate(int currentHeight);
		/// \brief Execute the Script to
		/// validate the Transaction
		bool validateScript();
	private:
		std::shared_ptr<Transaction> transactionPointer;

		Mempool* mempool;
		UTXOManager* utxos;

		std::unordered_set< UTXO > dependencies;
		std::unordered_set< UTXO > mempoolDeps;
};

#endif /* LINKEDTRANSACTION_HPP */
