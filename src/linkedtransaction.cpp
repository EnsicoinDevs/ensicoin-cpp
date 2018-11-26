#include "linkedtransaction.hpp"

#include "mempool.hpp"
#include "transaction.hpp"
#include "utxo.hpp"

#include <algorithm>
#include <iostream>
#include <numeric>
#include <rapidjson/document.h>

namespace ressources{
	LinkedTransaction::LinkedTransaction(std::shared_ptr<Transaction> tx,
			manager::Mempool* mempoolPtr,
			manager::UTXOManager* utxosPtr) : 
		Transaction(*tx),
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

	unsigned int LinkedTransaction::inputValue() const{
		if (isOrphan()){
			std::cerr << "Cannot calculate value of orphan" << std::endl;
			return 0;
		}
		unsigned int sum = 0;
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
					}))
			return false;
		if(!validateScript()) return false;
		return true;
	}

	bool LinkedTransaction::validateScript(){
		for(auto& input : inputs){
			auto id = input.previousOutput;
			std::string shash;
			Script outScript;
			if(dependencies.count(id) == 0){
				// Then fetch from disk
				auto data = utxos->getData(id);
				outScript = data.script;
				shash = data.signingHash;
			}
			else{ 	// Be sure that the transaction is not
				// Orphan
				outScript = mempool->scriptOfOutput(id);
				shash = mempool->signingHash(id.transactionHash);
			}
			outScript.setSHash(shash);
			input.script.chain(&outScript);
			while(!input.script.done()){
				if(!input.script.step()){
					return false;
				}
			}
		}
		return true;
	}

} // namespace ressources
