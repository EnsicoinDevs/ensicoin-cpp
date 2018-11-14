#ifndef MEMPOOL_HPP
#define MEMPOOL_HPP

#include "hashmemory.hpp"
#include "linkedtransaction.hpp"
#include "transaction.hpp"
#include "utxo.hpp"

#include <map>
#include <memory>
#include <string>
#include <vector>

/// \brief Pool of Transaction not in Block
class Mempool{
	public:
		/// \brief Construct Mempool and load LevelDB
		Mempool();
		
		/// \brief Add Transaction to Mempool
		/// \param tr Transaction to add
		/// \details Checks if Transaction is 
		/// Orphan and is it is valid
		bool addTransaction(std::shared_ptr<Transaction> tr);
		
		/// \brief Checks if Transaction exists in Mempool
		/// \param txHash hash of a Transaction
		bool exists(std::string txHash) const;
		/// \brief Check if a Transaction is a registered 
		/// Orphan
		/// \param txHash hash of a Transaction
		bool orphanExists(std::string txHash) const;
		
		/// \brief Fetches the value of a TransationOutput 
		/// identified by a TransactionIdentifier
		int valueOfOutput(TransactionIdentifier id) const;
		/// \brief Fetch the hash used to sign a Transaction
		/// by the TransactionIdentifier
		std::string signingHash(std::string txHash) const;
		/// \brief Fetch the script of a TransationOutput by
		/// the TransactionIdentifier
		std::vector<std::string> scriptOfOutput(TransactionIdentifier id) const;

		/// \brief Add one to the currentHeight
		void incrementHeight();
		/// \brief Remove one from the currentHeight
		void decrementHeight();
	
		
		/// \brief Gets the type of a registered Transaction
		/// \param txHash hash of the Transaction
		TXType type(std::string txHash) const;
	private:
		UTXOManager utxos;
		/// \brief Height of the Blockchain
		int currentHeight;
		
		/// \brief Process either a new Transaction or 
		/// an Orphan who found all the corresponding
		/// UTXO
		void registerTransaction(LinkedTransaction::pointer tx);
		/// \brief Handle an output being registered
		/// \param orphanHash update the orphan depending on
		/// a Transaction
		void updateOrphan(std::string orphanHash);

		HashMemory<LinkedTransaction> mainPool;
		HashMemory<LinkedTransaction> orphans;
		/// \brief Map of orphan using an UTXO
		/// \details There by design can't be more than
		/// one Transaction using a specified output and 
		/// as such you can reject all orphans refereing to
		/// the same input
		std::map< std::string , std::string > orphanUsingUTXO;
};	

#endif /* MEMPOOL_HPP */
