#include "linkedtransaction.hpp"

#include "mempool.hpp"
#include "transaction.hpp"
#include "utxo.hpp"

#include <algorithm>
#include <iostream>
#include <numeric>

LinkedTransaction::LinkedTransaction(std::shared_ptr<Transaction> tx,
				     Mempool* mempoolPtr,
				     UTXOManager* utxosPtr) : 
				     mempool(mempoolPtr),
				     utxos(utxosPtr) {
	for(auto& inputReference : tx->getInputsId()){
		auto inputHash = inputReference.transactionHash;
		if(mempool->exists(inputHash)){
			mempoolDeps.insert(inputReference);
		}
		else if(utxos->exists(inputReference)){
			dependencies.insert(inputReference);
		}
	}
}

bool LinkedTransaction::isOrphan() const{
	return (dependencies.size() + mempoolDeps.size()) != 
	        inputs.size();
}

std::vector<UTXO> LinkedTransaction::getOrphanDeps() const{
	std::vector<UTXO> orphaned;
	std::copy_if( getInputsId().begin(),
		      getInputsId().end(),
		      std::back_inserter(orphaned),
		      [&dependencies = dependencies,
		      &mempoolDeps = mempoolDeps](UTXO id){
				return (dependencies.count(id) == 0)
				&& (mempoolDeps.count(id) == 0);
		      });
	return orphaned;
}

int LinkedTransaction::inputValue() const{
	if (isOrphan()){
		std::cerr << "Cannot calculate value of orphan" << std::endl;
		return 0;
	}
	int sum = 0;
	std::accumulate( dependencies.begin(),
			 dependencies.end(),
			 sum,
			 [&utxos = utxos](int s, UTXO utxo){
				return s + 
				       utxos->getData(utxo).value;
			 });
	std::accumulate( mempoolDeps.begin(),
			 mempoolDeps.end(),
			 sum,
			 [&mempool = mempool](int s, UTXO utxo){
		         	return s + 
				       mempool->valueOfOutput(utxo);
			 });
	return sum;
}

bool LinkedTransaction::validate(int currentHeight){
	if(!check()) return false;
	if(isOrphan()) return false;
	if(mempool->exists(hash())) return false;
	if(inputValue() <= 
	   outputValue()) return false;
	if(std::any_of(getInputsId().begin(), 
		       getInputsId().end(),
		       [&utxos = utxos, currentHeight](UTXO id){
				return !utxos->isSpendable(id, currentHeight);
			})) return false;
	if(!validateScript()) return false;
	return true;
}

bool LinkedTransaction::validateScript(){
	for(auto& input : inputs){
		auto id = input.previousOutput;
		std::vector<std::string> scriptInstr;
		std::string hashToSign;
		if(dependencies.count(id) == 0){
			// Then fetch from disk
			auto data = utxos->getData(id);
			scriptInstr = data.script;
			hashToSign = data.hashWithoutInputs;
		}
		else{ 	// Be sure that the transaction is not
		      	// Orphan
			scriptInstr = mempool->scriptOfOutput(id);
			hashToSign = mempool->signingHash(id.transactionHash);
		}
		Script script(scriptInstr.begin(),
			      scriptInstr.end(),
			      input.inputStack,
			      hashToSign);
	}
	return false;
}
