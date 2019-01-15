#include "logger.hpp"
#include "utxo.hpp"
#include "constants.hpp"
#include "networkable.hpp"
#include "networkbuffer.hpp"
#include "transaction.hpp"

#include <iostream>
#include <leveldb/db.h>
#include <memory>

namespace manager{

	std::string UTXOdata::byteRepr() const{
		return 	networkable::Uint64(value).byteRepr() +
				networkable::Uint16(coinbase).byteRepr() +
				networkable::Uint32(height).byteRepr() +
				networkable::Str(signingHash).byteRepr() +
				networkable::Uint16(fromMempool).byteRepr() +
				script.byteRepr();
	}

	UTXOdata::UTXOdata(	int val,
						bool cb,
						int ht,
						std::string shash,
						bool fm,
						ressources::Script sc) : 
			value(val), 
			coinbase(cb),
			height(ht),
			signingHash(shash),
			fromMempool(fm),
			script(sc) {}

	UTXOdata::UTXOdata(NetworkBuffer* networkBuffer) : 
		value(networkable::Uint64(networkBuffer).getValue()),
		coinbase(networkable::Uint16(networkBuffer).getValue()),
		height(networkable::Uint32(networkBuffer).getValue()),
		signingHash(networkable::Hash(networkBuffer).getValue()),
		fromMempool(networkable::Uint16(networkBuffer).getValue()),
		script(networkBuffer)
				{}

	UTXOManager::UTXOManager(std::shared_ptr<Logger> logger_)
		: logger(logger_) {
		leveldb::Options options;
		options.create_if_missing = true;
		leveldb::Status status = leveldb::DB::Open(options, 
													constants::UTXO_DB, &db);
		if (!status.ok()){
			logger->fatal("can't open utxo db : ", status.ToString());
		}
	}

	UTXOdata UTXOManager::getData(ressources::UTXO id) const{
		std::string strData;
		leveldb::Status s = db->Get(leveldb::ReadOptions(), 
						id.byteRepr(), &strData);
		if (!s.ok()){
			logger->error("error in reading UTXO ", id.transactionHash,": ", id.index ," : ", id.index, s.ToString());
		}
		NetworkBuffer buffer(strData,logger);
		UTXOdata data(&buffer);
		return data;
	}

	bool UTXOManager::exists(ressources::UTXO id) const{
		std::string strData;
		leveldb::Status s = db->Get(leveldb::ReadOptions(), 
				id.byteRepr(), &strData);
		return s.ok();
	}

	void UTXOManager::add(ressources::UTXO id, UTXOdata data){
		auto s = db->Put(leveldb::WriteOptions(), 
				id.byteRepr(), data.byteRepr());
		if (!s.ok()){
			logger->error("error in adding UTXO ", id.transactionHash,": ", id.index ," : ", id.index, s.ToString());
		}
	}

	void UTXOManager::spend(ressources::UTXO id){
		auto s = db->Delete(leveldb::WriteOptions(), id.byteRepr());
		if (!s.ok()){
			logger->error("error in removing UTXO ", id.transactionHash,": ", id.index ," : ", id.index, s.ToString());
		}
	}

	bool UTXOManager::isSpendable(ressources::UTXO id, int currentHeight) const{
		auto data = getData(id);
		if (!data.coinbase) return true;
		else return currentHeight - data.height >= 42;
	}

} // namespace manager
