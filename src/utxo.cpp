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
	
	rapidjson::Value hashVal;
	hashVal.SetString(hashWithoutInputs.c_str(), hashWithoutInputs.length(), doc.GetAllocator());

	doc.AddMember("hashWithoutInputs", hashVal, doc.GetAllocator());

	rapidjson::Value scriptVal(rapidjson::kArrayType);
	for(auto& elem : script){
		rapidjson::Value strVal;
		strVal.SetString(elem.c_str(), elem.length(), doc.GetAllocator());
		scriptVal.PushBack(strVal, doc.GetAllocator());
	}
	
	doc.AddMember("script", scriptVal, doc.GetAllocator());
	doc.AddMember("fromMempool", fromMempool, doc.GetAllocator());

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
	hashWithoutInputs = doc["hashWithoutInputs"].GetString();
	for(auto& elem : doc["script"].GetArray())
		script.push_back(elem.GetString());
	fromMempool = doc["fromMempool"].GetBool();
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

bool UTXOManager::exists(UTXO id) const{
	std::string strData;
	leveldb::Status s = db->Get(leveldb::ReadOptions(), id.str(), &strData);
	return s.ok();
}

void UTXOManager::add(UTXO id, UTXOdata data){
	auto s = db->Put(leveldb::WriteOptions(), id.str(), data.str());
	if (!s.ok())
		std::cerr << "Error while reading " << id.str() << std::endl;
}

void UTXOManager::spend(UTXO id){
	auto s = db->Delete(leveldb::WriteOptions(), id.str());
	if (!s.ok())
		std::cerr << "Error while deleting " << id.str() << std::endl;
}

bool UTXOManager::isSpendable(UTXO id, int currentHeight) const{
	auto data = getData(id);
	if (!data.coinbase) return true;
	else return currentHeight - data.height >= 42;
}
