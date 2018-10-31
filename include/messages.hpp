#ifndef MESSAGES_HPP
#define MESSAGES_HPP

#include <ctime>
#include <memory>
#include <string>
#include <vector>
#include <rapidjson/document.h>

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
};

class WhoAmI : public Message {
	private:
		int version;
	public:
		WhoAmI();
		explicit WhoAmI(rapidjson::Document* doc);
		rapidjson::Value json(rapidjson::Document* document) const override;
};

class Inv : public Message {
	private:
		std::string ressourceType;
		std::vector<std::string> hashes;
	public:
		Inv(std::string ressource, std::vector<std::string> hashList);
		explicit Inv(rapidjson::Document* doc);
		rapidjson::Value json(rapidjson::Document* document) const override;
};


#endif /* MESSAGES_HPP */
