#include "messages.hpp"

#include "transaction.hpp"

#include <memory>
#include <rapidjson/document.h>

TransactionMessage::TransactionMessage(std::shared_ptr<Transaction> trPtr) : Message("transaction"), transaction(trPtr) {}

TransactionMessage::TransactionMessage(rapidjson::Document* doc) : Message(doc) {
	auto& transactionVal = (*doc)["message"];
	rapidjson::Document trDoc;
	trDoc.SetObject();
	trDoc.CopyFrom(transactionVal, trDoc.GetAllocator());
	transaction = std::make_shared<Transaction>(&trDoc);
}

rapidjson::Value TransactionMessage::json(rapidjson::Document* doc) const{
	rapidjson::Value messageValue = Message::json(doc);
	rapidjson::Value content = transaction->json(true, doc);

	messageValue.AddMember("message", content, doc->GetAllocator());
	return messageValue;
}
