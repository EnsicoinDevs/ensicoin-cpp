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

bool Mempool::outputExists(UTXO id) const{
	return exists(id.transactionHash) && mainPool.get(id.transactionHash)->hasOutput(id.index);
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
bool Mempool::checkOrphan(std::string orphan) const{
	auto tx = orphans.get(orphan);
	for(auto& input : tx->getInputsId()){
		if(exists(input.transactionHash) || utxos.exists(input))
			return false;
	}
	return true;
}

std::vector<UTXO> Mempool::orphanDeps(std::shared_ptr<Transaction> tx) const{
	std::vector<UTXO> out;
	for(auto& inputID : tx->getInputsId()){
		if(!exists(inputID.transactionHash) && !utxos.exists(inputID))
			out.push_back(inputID);
	}
	return out;
}

int Mempool::getInputValue(UTXO id) const{
	if(outputExists(id))
		return mainPool.get(id.transactionHash)->getOutputValue(id.index);
	return utxos.getValue(id);
}

bool Mempool::isSpendable(UTXO id) const{
	if(!utxos.isCoinbase(id))
		return true;
	else return currentHeight - utxos.getHeight(id) >= 42;
}

std::vector<std::string> Mempool::getOutputScript(UTXO id) const {
	return utxos.getOutputScript(id);
}

std::string Mempool::getHashSignature(UTXO id) const{
	return utxos.getHashSignature(id);
}


bool Mempool::addTransaction(std::shared_ptr<Transaction> tr){
	if(isOrphan(tr)){
		if(!tr->check())
			return false;
		if(orphanExists(tr->hash()))
				return false;
		orphans.add(tr);
		for(auto& dep : orphanDeps(tr))
			orphanUsingUTXO[dep.str()].push_back(tr->hash());
		return true;
	}
	else{
		if(exists(tr->hash()))
			return false;
		if(!tr->validate(this))
			return false;
		mainPool.add(tr);
		for(int i=0; i < tr->getOutputNumber(); i++){
			UTXO txOutput = {tr->hash(), i};
			auto orphanDeps = orphanUsingUTXO[txOutput.str()];
			orphanUsingUTXO.erase(txOutput.str());
			for(auto& orphan : orphanDeps)
				updateOrphan(orphan);
		}
		return true;
	}
}

void Mempool::updateOrphan(std::string orphan){
	if(!checkOrphan(orphan)){
		auto tx = orphans.get(orphan);
		orphans.erase(orphan);
		addTransaction(tx);
	}
}
