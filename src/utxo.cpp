#include "utxo.hpp"
#include "constants.hpp"
#include "transaction.hpp"

#include <iostream>
#include <leveldb/db.h>
#include <memory>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

std::string UTXOdata::str() const{
	rapidjson::Document doc;
	doc.SetObject();
	rapidjson::Value coinbaseVal(coinbase);
	rapidjson::Value valueVal(value);

	doc.AddMember("coinbase", coinbaseVal, doc.GetAllocator());
	doc.AddMember("value", valueVal, doc.GetAllocator());
	doc.AddMember("height", height, doc.GetAllocator());

	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	doc.Accept(writer);

	return buffer.GetString();
}

UTXOdata::UTXOdata(int val,bool cb) : value(val), coinbase(cb) {
}

UTXOdata::UTXOdata(std::string jsonStr){
	rapidjson::Document doc;
	doc.Parse(jsonStr.c_str());
	if(doc.HasParseError()){
		std::cerr << "Parse error when reading DB : " << jsonStr << std::endl;
		return;
	}
	value = doc["value"].GetInt();
	coinbase = doc["coinbase"].GetBool();
	height = doc["height"].GetInt();
}

UTXOManager::UTXOManager(){
	leveldb::Options options;
	options.create_if_missing = true;
	leveldb::Status status = leveldb::DB::Open(options, UTXO_DB, &db);
	if (!status.ok()) std::cerr << "Error db open : " << status.ToString() << std::endl;
}
UTXOdata UTXOManager::getData(UTXO id) const{
	std::string strData;
	leveldb::Status s = db->Get(leveldb::ReadOptions(), id.str(), &strData);
	if (!s.ok()){
		throw std::runtime_error("UTXO does not exists");
	}	
	UTXOdata data(strData);
	return data;
}

int UTXOManager::getValue(UTXO id) const{
	return getData(id).value;
}
bool UTXOManager::isCoinbase(UTXO id) const{
	return getData(id).coinbase;
}
int UTXOManager::getHeight(UTXO id) const{
	return getData(id).height;
}

bool UTXOManager::exists(UTXO id) const{
	std::string strData;
	leveldb::Status s = db->Get(leveldb::ReadOptions(), id.str(), &strData);
	return s.ok();
}

void UTXOManager::add(UTXO id, int value,bool coinbase){
	auto s = db->Put(leveldb::WriteOptions(), id.str(), UTXOdata(value, coinbase).str());
	if (!s.ok())
		std::cerr << "Error while reading " << id.str() << std::endl;
}

void UTXOManager::spend(UTXO id){
	auto s = db->Delete(leveldb::WriteOptions(), id.str());
	if (!s.ok())
		std::cerr << "Error while deleting " << id.str() << std::endl;
}

