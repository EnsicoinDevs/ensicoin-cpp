#ifndef MESSAGES_HPP
#define MESSAGES_HPP

#include <ctime>
#include <string>

#include <rapidjson/document.h>

class Message {
	protected:
		int magic;
		std::string type;
		std::time_t timestamp;

		Message(std::string messageType);
		Message(rapidjson::Document* doc);
	public:
		const std::string str() const;
		const std::string getType() const;
		virtual rapidjson::Value json(rapidjson::Document* document) const;
};

class WhoAmI : public Message {
	private:
		int version;
	public:
		WhoAmI();
		WhoAmI(rapidjson::Document* doc);
		rapidjson::Value json(rapidjson::Document* document) const override;
};


#endif /* MESSAGES_HPP */
