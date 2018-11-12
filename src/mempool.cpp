#include "mempool.hpp"
#include "hashmemory.hpp"
#include "transaction.hpp"
#include "utxo.hpp"

#include <deque>
#include <iostream>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>

Mempool::Mempool() {

}

bool Mempool::exists(std::string txHash ) const{
	return mainPool.exists(txHash);
}

//Gets the type of a registered transaction
TXType Mempool::type(std::string txHash) const{
	if(mainPool.exists(txHash))
		return Regular;
	else if (orphans.exists(txHash))
		return Orphan;
	else
		throw std::runtime_error("Cannot give type of non-existant transaction");
}

bool Mempool::isOrphan(std::shared_ptr<Transaction> tx) const{
	for(auto& input : tx->getInputsId()){
		if(exists(input.transactionHash) || utxos.exists(input))
			return false;
	}
	return true;
}

int Mempool::getInputValue(UTXO id) const{
	return utxos.getValue(id);
}

bool Mempool::isSpendable(UTXO id, int currentHeight) const{
	if(!utxos.isCoinbase(id))
		return true;
	else return currentHeight - utxos.getHeight(id) >= 42;
}

/*bool Mempool::checkInputReference(std::shared_ptr<Transaction> tx) const{

  }

  int Mempool::inputValue(std::string txHash) const{

  }

  bool Mempool::addTransaction(std::shared_ptr<Transaction> tr){

  }*/
