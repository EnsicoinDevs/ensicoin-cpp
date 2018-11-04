#ifndef MESSAGES_HPP
#define MESSAGES_HPP

#include "blocks.hpp"
#include "transaction.hpp"

#include <ctime>
#include <memory>
#include <rapidjson/document.h>
#include <string>
#include <vector>

class Message : public std::enable_shared_from_this<Message> {
	protected:
		int magic;
		std::string type;
		std::time_t timestamp;

		explicit Message(std::string messageType);
		explicit Message(rapidjson::Document* doc);
	public:
		using messagePointer = std::shared_ptr<Message>;
		const std::string str() const;
		const std::string getType() const;
		virtual rapidjson::Value json(rapidjson::Document* document) const;
		virtual ~Message() = 0;
};

class WhoAmI : public Message {
	private:
		int version;
	public:
		WhoAmI();
		explicit WhoAmI(rapidjson::Document* doc);
		rapidjson::Value json(rapidjson::Document* document) const override;
};

struct InvData {
	std::string type;
	std::vector<std::string> hashes;
	
	InvData(std::string t, std::vector<std::string> hL);
	InvData(rapidjson::Value* val);
	rapidjson::Value json(rapidjson::Document* doc) const;
};

class Inv : public Message {
	private:
		InvData data;
	public:
		Inv(InvData dt);
		explicit Inv(rapidjson::Document* doc);
		rapidjson::Value json(rapidjson::Document* document) const override;
};

class GetData : public Message {
	private:
		InvData invData;
	public:
		explicit GetData(InvData inv);
		explicit GetData(rapidjson::Document* doc);
		rapidjson::Value json(rapidjson::Document* document) const override;
};

class NotFound : public Message {
	private:
		std::string type;
		std::string hash;
	public:
		NotFound(std::string resType, std::string hashType);
		explicit NotFound(rapidjson::Document* doc);
		rapidjson::Value json(rapidjson::Document* document) const override;
};

class BlockMessage : public Message {
	private:
		std::shared_ptr<Block> block;
	public:
		explicit BlockMessage(std::shared_ptr<Block> blockPtr);
		explicit BlockMessage(rapidjson::Document* doc);
		rapidjson::Value json(rapidjson::Document* document) const override;
};

class TransactionMessage : public Message {
	private:
		std::shared_ptr<Transaction> transaction;
	public:
		explicit TransactionMessage(std::shared_ptr<Transaction> trPtr);
		explicit TransactionMessage(rapidjson::Document* doc);
		rapidjson::Value json(rapidjson::Document* doc) const override;
};

class GetBlocks : public Message {
	private:
		std::vector< std::string > blockHashes;
		std::string stopHash;
	public:
		GetBlocks(std::vector< std::string > hashList, std::string stopHashString);
		explicit GetBlocks(rapidjson::Document* doc);
		rapidjson::Value json(rapidjson::Document* doc) const override;
};

class GetMempool : public Message{
	public:
		GetMempool();
		explicit GetMempool(rapidjson::Document* doc);
		rapidjson::Value json(rapidjson::Document* doc) const override;
};

#endif /* MESSAGES_HPP */
