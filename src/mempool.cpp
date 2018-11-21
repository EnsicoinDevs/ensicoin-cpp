#include "mempool.hpp"
#include "hashmemory.hpp"
#include "linkedtransaction.hpp"
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

bool Mempool::orphanExists(std::string txHash) const{
	return orphans.exists(txHash);
}

void Mempool::incrementHeight(){
	currentHeight++;
}
void Mempool::decrementHeight(){
	currentHeight--;
}

int Mempool::valueOfOutput(TransactionIdentifier id) const {
	return mainPool.get(id.transactionHash)->valueOfOutput(id.index);
}
std::string Mempool::signingHash(std::string txHash) const{
	return mainPool.get(txHash)->hashWithoutInputs();
}
std::vector<std::string> Mempool::scriptOfOutput(TransactionIdentifier id) const{
	return mainPool.get(id.transactionHash)->getScriptOfOutput(id.index);
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

bool Mempool::addTransaction(std::shared_ptr<Transaction> tr){
	if(!tr->check()) 
		return false;
	auto linkedTX = std::make_shared<LinkedTransaction>(tr, this, &utxos);
	if(linkedTX->isOrphan()){
		if(orphanExists(linkedTX->hash()))
				return false;
		orphans.add(linkedTX);
		for(auto& dep : linkedTX->getOrphanDeps())
			orphanUsingUTXO[dep.str()] = 
				linkedTX->hash();
		return true;
	}
	else{
		if(exists(linkedTX->hash()))
			return false;
		if(!linkedTX->validate(currentHeight))
			return false;
		mainPool.add(linkedTX);
		registerTransaction(linkedTX);
		return true;
	}
}

void Mempool::registerTransaction(LinkedTransaction::pointer tx){
	for(unsigned int i=0; i < tx->outputCount(); i++){
		UTXO txOutput = {tx->hash(), i};
		auto orphan = orphanUsingUTXO[txOutput.str()];
		orphanUsingUTXO.erase(txOutput.str());
		updateOrphan(orphan);
	}
}

void Mempool::updateOrphan(std::string orphan){
	auto lTx = orphans.get(orphan);
	if(!lTx->isOrphan()){
		orphans.erase(orphan);
		mainPool.add(lTx);
		registerTransaction(lTx);
	}
}
