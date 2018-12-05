#include "mempool.hpp"
#include "hashmemory.hpp"
#include "linkedtransaction.hpp"
#include "transaction.hpp"
#include "utxo.hpp"

#include <deque>
#include <iostream>
#include <map>
#include <memory>
#include <spdlog/spdlog.h>
#include <stdexcept>
#include <string>

namespace manager{

	Mempool::Mempool(std::shared_ptr<spdlog::logger> logger_) :
		utxos(logger_),
		logger(logger_) {
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

	int Mempool::valueOfOutput(ressources::TransactionIdentifier id) const {
		return mainPool.get(id.transactionHash)->valueOfOutput(id.index);
	}
	std::string Mempool::signingHash(std::string txHash) const{
		return mainPool.get(txHash)->signingHash();
	}
	ressources::Script Mempool::scriptOfOutput(ressources::TransactionIdentifier id) const{
		return mainPool.get(id.transactionHash)->getScriptOfOutput(id.index);
	}

	//Gets the type of a registered transaction
	ressources::Transaction::TXType Mempool::type(std::string txHash) const{
		if(mainPool.exists(txHash))
			return ressources::Transaction::TXType::Regular;
		else if (orphans.exists(txHash))
			return ressources::Transaction::TXType::Orphan;
		else{
			logger->critical("cannot give type of non-existant transaction : {}", txHash);
			return ressources::Transaction::TXType::Unknown;
		}
	}

	bool Mempool::addTransaction(ressources::Transaction::pointer tr){
		if(!tr->check()) 
			return false;
		auto linkedTX = std::make_shared<ressources::LinkedTransaction>(tr, this, &utxos);
		if(linkedTX->isOrphan()){
			if(orphanExists(linkedTX->hash()))
				return false;
			orphans.add(linkedTX);
			for(auto& dep : linkedTX->getOrphanDeps())
				orphanUsingUTXO[dep.byteRepr()] = 
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

	void Mempool::registerTransaction(ressources::LinkedTransaction::pointer tx){
		for(unsigned int i=0; i < tx->outputCount(); i++){
			ressources::UTXO txOutput = {tx->hash(), i};
			auto orphan = orphanUsingUTXO[txOutput.byteRepr()];
			orphanUsingUTXO.erase(txOutput.byteRepr());
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

} // namespace manager
