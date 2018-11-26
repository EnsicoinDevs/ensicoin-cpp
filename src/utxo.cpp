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
		value(networkBuffer->readUint64().getValue()),
		coinbase(networkBuffer->readUint16().getValue()),
		height(networkBuffer->readUint32().getValue()),
		signingHash(networkBuffer->readHash()),
		fromMempool(networkBuffer->readUint16().getValue()),
		script(networkBuffer)
				{}

	UTXOManager::UTXOManager(){
		leveldb::Options options;
		options.create_if_missing = true;
		leveldb::Status status = leveldb::DB::Open(options, 
													UTXO_DB, &db);
		if (!status.ok()){
			std::cerr << "Error db open : " 
				<< status.ToString() << std::endl;
		}
	}

	UTXOdata UTXOManager::getData(UTXO id) const{
		std::string strData;
		leveldb::Status s = db->Get(leveldb::ReadOptions(), 
						id.byteRepr(), &strData);
		if (!s.ok()){
			throw std::runtime_error("UTXO does not exists");
		}
		NetworkBuffer buffer(strData);
		UTXOdata data(&buffer);
		return data;
	}

	bool UTXOManager::exists(UTXO id) const{
		std::string strData;
		leveldb::Status s = db->Get(leveldb::ReadOptions(), 
				id.byteRepr(), &strData);
		return s.ok();
	}

	void UTXOManager::add(UTXO id, UTXOdata data){
		auto s = db->Put(leveldb::WriteOptions(), 
				id.byteRepr(), data.byteRepr());
		if (!s.ok()){
			std::cerr << "Error while reading " << 
				id.byteRepr() << std::endl;
		}
	}

	void UTXOManager::spend(UTXO id){
		auto s = db->Delete(leveldb::WriteOptions(), id.byteRepr());
		if (!s.ok()){
			std::cerr << "Error while deleting " << 
				id.byteRepr() << std::endl;
		}
	}

	bool UTXOManager::isSpendable(UTXO id, int currentHeight) const{
		auto data = getData(id);
		if (!data.coinbase) return true;
		else return currentHeight - data.height >= 42;
	}

}
