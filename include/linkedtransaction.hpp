#ifndef LINKEDTRANSACTION_HPP
#define LINKEDTRANSACTION_HPP

#include "transaction.hpp"
#include "utxo.hpp"

#include <memory>
#include <unordered_set>

namespace manager{
	class Mempool;
}

namespace ressources{

	/// \brief Links a Transaction to the UTXO it depends on
	/// \todo Add Transaction as parent class
	class LinkedTransaction : public Transaction {
		public:
			/// \brief Shared pointer to a LinkedTransaction
			using pointer = std::shared_ptr<LinkedTransaction>;
			/// \brief Retrieves necessary UTXOs to create 
			/// the LinkedTransaction from a Transaction
			explicit LinkedTransaction(
					Transaction::pointer tx, 
					manager::Mempool* mempool, 
					manager::UTXOManager* utxos);

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
			manager::Mempool* mempool;
			/// \brief Reference to a UTXOManager to fetch
			/// UTXO
			manager::UTXOManager* utxos;

			/// \brief List of Linked UTXO in blocks
			std::unordered_set< UTXO > dependencies;
			/// \brief List of Linked UTXO in a Mempool
			std::unordered_set< UTXO > mempoolDeps;
	};

} // namespace ressources

#endif /* LINKEDTRANSACTION_HPP */
