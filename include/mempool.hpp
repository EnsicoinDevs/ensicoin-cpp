#ifndef MEMPOOL_HPP
#define MEMPOOL_HPP

#include "hashmemory.hpp"
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
		
		//bool checkInputReference(std::shared_ptr<Transaction> tx) const ;
		/// \brief Get value of UTXO
		/// \param id reference to a Transaction
		/// \return Gets the value of the output pointed by
		/// the UTXO
		int getInputValue(UTXO id) const;
		/// \brief Checks if Transaction exists in Mempool
		/// \param txHash hash of a Transaction
		bool exists(std::string txHash) const;
		/// \brief Check if a Transaction is a registered 
		/// Orphan
		/// \param txHash hash of a Transaction
		bool orphanExists(std::string txHash) const;
		/// \brief Check if a indexed output exists
		/// \param id hash and index of output
		/// \return True if and only in Transaction exists in
		/// mainPool and has an input id.index
		bool outputExists(UTXO id) const;
		
		/// \brief Check if a Transaction is Orphan
		/// \param tx Transaction to identify
		bool isOrphan(std::shared_ptr<Transaction> tx) const;
		/// \brief Gets the hash used to sign a Transaction
		/// \param id reference to the Transaction
		std::string getHashSignature(UTXO id) const;

		/// \brief Check if an output can be spent
		/// \param id reference to an output
		/// \return True if the Transaction is regular
		/// or if it is a coinbase and 42 Block have passed
		bool isSpendable(UTXO id) const;
		/// \brief Get the script of an output
		/// \param id TransationOutput to get the script from
		std::vector<std::string> getOutputScript(UTXO id) const;
		/// \brief Add one to the currentHeight
		void incrementHeight();
		/// \brief Remove one from the currentHeight
		void decrementHeight();
	
		/// \brief Get all inputs which are Orphan
		/// \param tx Transaction to check
		std::vector<UTXO> orphanDeps(std::shared_ptr<Transaction> tx) const;
		
		/// \brief Gets the type of a registered Transaction
		/// \param txHash hash of the Transaction
		TXType type(std::string txHash) const;
	private:
		UTXOManager utxos;
		/// \brief Height of the Blockchain
		int currentHeight;
		
		/// \brief Handle an output being registered
		/// \param orphanHash update the orphan depending on
		/// a Transaction
		void updateOrphan(std::string orphanHash);
		/// \brief Checks if an orphan is still orphan
		/// \param orphanHash ash of an Orphan Transaction
		bool checkOrphan(std::string orphanHash) const;

		HashMemory<Transaction> mainPool;
		HashMemory<Transaction> orphans;
		/// \brief Map of orphan using an UTXO
		/// \details There by design can't be more than
		/// one Transaction using a specified output and 
		/// as such you can reject all orphans refereing to
		/// the same input
		std::map< std::string , std::string > orphanUsingUTXO;
};	

#endif /* MEMPOOL_HPP */
