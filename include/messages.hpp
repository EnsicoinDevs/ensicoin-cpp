#ifndef MESSAGES_HPP
#define MESSAGES_HPP

#include "blocks.hpp"
#include "transaction.hpp"

#include <ctime>
#include <memory>
#include <rapidjson/document.h>
#include <string>
#include <vector>


/// \brief Base class for all network communcation
class Message : public std::enable_shared_from_this<Message> {
	protected:
		/// \brief Defined in constants.hpp
		int magic;
		/// \brief Type of message
		std::string type;
		/// \brief Creation timestamp
		std::time_t timestamp;

		/// \brief Construc a Message with empty content
		/// \param type set the type of the content
		explicit Message(std::string type);
		/// \brief Parses a rapidjson::Document in a Message
		/// \param doc JSON representation
		explicit Message(rapidjson::Document* doc);
	public:
		using messagePointer = std::shared_ptr<Message>;
		/// \brief gets the JSON string representation
		std::string str() const;
		/// \brief get the type of the Message
		std::string getType() const;
		/// \brief Get the JSON representation of the Message
		/// \param document Allocator for creating members
		virtual rapidjson::Value json(rapidjson::Document* document) const;
		virtual ~Message() = 0;
};

/// \brief Message used for intializing the Connection
class WhoAmI : public Message {
	private:
		/// \brief Defined in constants.hpp
		int version;
	public:
		/// \brief Construct a WhoAmI Message
		WhoAmI();
		/// \brief Parses a WhoAmI JSON Message
		/// \param doc JSON representation
		explicit WhoAmI(rapidjson::Document* doc);
		/// \brief Gets the JSON representation
		/// \param document Allocator for members
		rapidjson::Value json(rapidjson::Document* document) const override;
};

/// \brief Data transmission over the network by mean of hashes
struct InvData {
	/// \brief Either "t" or "b" for Transaction or Block
	std::string type;
	/// \brief List of hashes of ressoucres communicated
	std::vector<std::string> hashes;
	
	/// \brief Creates InvData from fields
	InvData(std::string t, std::vector<std::string> hL);
	/// \brief Parse a JSON Value into an InvData
	/// \param val the JSON representation to be parsed
	explicit InvData(rapidjson::Value* val);
	/// \brief Create a JSON representation
	/// \param doc Document for Allocation of memebers
	rapidjson::Value json(rapidjson::Document* doc) const;
};

/// \brief Message to get the JSON String representing ressources
class GetData : public Message {
	private:
		/// \brief Ressources asked
		InvData invData;
	public:
		/// \brief Creates a GetData from an InvData
		explicit GetData(InvData inv);
		/// \brief Parses a JSON representation in a GetData
		/// \param doc JSON to be parsed
		explicit GetData(rapidjson::Document* doc);
		/// \brief Create the JSON representation
		/// \param document Document used for Allocation of 
		/// members
		rapidjson::Value json(rapidjson::Document* document) const override;
};

/// \brief Transmit Block or Transaction to a peer
class Inv : public Message {
	private:
		/// \brief data to be transmited
		InvData data;
	public:
		/// \brief Retrives the type from InvData
		std::string getRessourceType() const;
		/// \brief Retriveses the vector of hashes 
		/// from InvData
		std::vector<std::string> getRessources() const;
		
		/// \brief Create a response to an Inv to request
		/// ressources by GetData
		std::shared_ptr<GetData> respondRequest() const;

		/// \brief Create a Inv from an InvData
		explicit Inv(InvData dt);
		/// \brief Parse a JSON Document into an Inv
		/// \param doc JSON to be parsed
		explicit Inv(rapidjson::Document* doc);
		/// \brief Create the JSON representation
		/// \param document Document used for Allocation of
		/// members
		rapidjson::Value json(rapidjson::Document* document) const override;
};


/// \brief Message signaling an asked ressource is unknown
class NotFound : public Message {
	private:
		/// \brief Either "b" or "t" for Block or Transaction
		std::string resType;
		/// \brief The unknown hash
		std::string hash;
	public:
		/// \brief Create a NotFound from the ressource
		NotFound(std::string resType, std::string hashType);
		/// \brief Parses a JSON representation in a GetData
		/// \param doc JSON to be parsed
		explicit NotFound(rapidjson::Document* doc);
		/// \brief Create the JSON representation
		/// \param document Document used for Allocation of 
		/// members
		rapidjson::Value json(rapidjson::Document* document) const override;
};

/// \brief Message sending a Block over
class BlockMessage : public Message {
	private:
		/// \brief Pointer to the sent Block to avoid RAM
		/// usage
		std::shared_ptr<Block> block;
	public:
		/// \brief Create a BlockMessage from a Block
		explicit BlockMessage(std::shared_ptr<Block> blockPtr);
		/// \brief Parses a JSON representation in a GetData
		/// \param doc JSON to be parsed
		explicit BlockMessage(rapidjson::Document* doc);
		/// \brief Create the JSON representation
		/// \param document Document used for Allocation of 
		/// members
		rapidjson::Value json(rapidjson::Document* document) const override;
};

/// \brief Message sending a Transaction over
class TransactionMessage : public Message {
	private:
		/// \brief Pointer to the sent Transaction to 
		/// avoid RAM usgae
		std::shared_ptr<Transaction> transaction;
	public:
		/// \brief Create a TransactionMessage from a
		/// Transaction
		explicit TransactionMessage(std::shared_ptr<Transaction> trPtr);
		/// \brief Parses a JSON representation in a GetData
		/// \param doc JSON to be parsed
		explicit TransactionMessage(rapidjson::Document* doc);
		/// \brief Create the JSON representation
		/// \param doc Document used for Allocation of 
		/// members
		rapidjson::Value json(rapidjson::Document* doc) const override;
};

/// \brief Message asking a node to sync Block until a certain point
class GetBlocks : public Message {
	private:
		/// \brief Known Block for reference
		std::vector< std::string > blockHashes;
		/// \brief Where to stop sending Block
		std::string stopHash;
	public:
		/// \brief Create a GetBlocks from the known hashes
		GetBlocks(std::vector< std::string > hashList, std::string stopHashString);
		/// \brief Parses a JSON representation in a GetData
		/// \param doc JSON to be parsed
		explicit GetBlocks(rapidjson::Document* doc);
		/// \brief Create the JSON representation
		/// \param doc Document used for Allocation of 
		/// members
		rapidjson::Value json(rapidjson::Document* doc) const override;
};

/// \brief Message to sync the Mempool with another node
class GetMempool : public Message{
	public:
		/// \brief Create a GetMempool
		GetMempool();
		/// \brief Parses a JSON representation in a GetData
		/// \param doc JSON to be parsed
		explicit GetMempool(rapidjson::Document* doc);
		/// \brief Create the JSON representation
		/// \param document Document used for Allocation of 
		/// members
		rapidjson::Value json(rapidjson::Document* doc) const override;
};

#endif /* MESSAGES_HPP */
