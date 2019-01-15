#ifndef MESSAGES_HPP
#define MESSAGES_HPP

#include "blocks.hpp"
#include "networkable.hpp"
#include "networkbuffer.hpp"
#include "transaction.hpp"

#include <ostream>
#include <ctime>
#include <memory>
#include <string>
#include <vector>

/// \brief Networkable content to be sent
namespace message{

	enum message_type{	
				whoami, 
				whoamiack, 
				getaddr, 
				addr, 
				inv, 
				getdata, 
				notfound, 
				block, 
				tx, 
				getblocks, 
				getmempool,
				unknown};
	

	/// \brief Get the message_type as a string
	inline std::string getTypeAsString(message_type type) {
		std::string tp;
		switch(type){
			case whoami:
				tp = "whoami";
				break;
			case whoamiack:
				tp = "whoamiack";
				break;
			case getaddr:
				tp = "getaddr";
				break;
			case addr:
				tp =  "addr";
				break;
			case inv:
				tp = "inv";
				break;
			case getdata:
				tp = "getdata";
				break;
			case notfound:
				tp = "notfound";
				break;
			case block:
				tp = "block";
				break;
			case tx:
				tp = "tx";
				break;
			case getblocks:
				tp = "getblocks";
				break;
			case getmempool:
				tp = "getmempool";
				break;
			case unknown:
				tp = "unknown";
				break;
		}
		return tp;
	}
	inline std::ostream& operator<<(std::ostream& os, message_type type){
		os << getTypeAsString(type);
		return os;
	}
	inline message_type getTypeFromString(const std::string& typeStr){
		if(typeStr.find("whoamiack") != std::string::npos)
			return whoamiack;
		else if(typeStr.find("whoami") != std::string::npos)
			return whoami;
		else if(typeStr.find("getaddr") != std::string::npos)
			return getaddr;
		else if(typeStr.find("addr") != std::string::npos)
			return addr;
		else if(typeStr.find("inv") != std::string::npos)
			return inv;
		else if(typeStr.find("getblocks") != std::string::npos)
			return getblocks;
		else if(typeStr.find("getmempool") != std::string::npos)
			return getmempool;
		else if(typeStr.find("getdata") != std::string::npos)
			return getdata;
		else if(typeStr.find("notfound") != std::string::npos)
			return notfound;
		else if(typeStr.find("block") != std::string::npos)
			return block;
		else if(typeStr.find("tx") != std::string::npos)
			return tx;
		else
			return unknown;
	}

	/// \brief Base class for all network communcation
	class Message : public std::enable_shared_from_this<Message>,
	public networkable::Networkable {
		public:
			/// \brief Shared_pointer to a Message
			using pointer = std::shared_ptr<Message>;
			/// \brief get the type of the Message
			message_type getType() const;
			/// \brief Parse a string into message_type
			std::string byteRepr() const override;
		protected:
			/// \brief Defined in constants.hpp
			int magic;
			/// \brief Type of message
			message_type type;

			/// \brief Get the payload as a byteRepr
			virtual std::string payload() const = 0;

			/// \brief Construct a Message with empty 
			/// Payload
			/// \param type set the type of the content
			explicit Message(message_type type);
	};

	/// \brief Message used for intializing the Connection
	class WhoAmI : public Message {
		private:
			std::string payload() const override;
			/// \brief Defined in constants.hpp
			uint32_t version;
			/// \brief Address of the node 
			networkable::Address address;
			/// \brief Services provided by a node
			std::vector<std::string> services;
		public:
			/// \brief Construct a WhoAmI Message
			WhoAmI(networkable::Address address);
			inline uint32_t getVersion() const{
				return version;
			}
			inline networkable::Address getAddress() const{
				return address;
			}
			inline std::vector<std::string> getServices() const{
				return services;
			};
			/// \brief Parses a WhoAmI Message
			/// \param networkBuffer buffer containing
			/// the raw Representation
			explicit WhoAmI(NetworkBuffer* 
					networkBuffer);
	};

	/// \brief Acknowledge a Connection after a WhoAmI
	class WhoAmIAck : public Message {
		private:
			std::string payload() const override;
		public:
			/// \brief Construct a WhoAmIAck
			WhoAmIAck();
	};

	/// \brief Send all known Node
	class Addr : public Message {
		private:
			std::string payload() const override;
			/// \brief List of known Node address
			std::vector<networkable::Address> addresses;
		public:
			/// \brief Construt a Addr from addresses
			explicit Addr(std::vector<networkable::\
					Address> addressList);
			/// \brief Parse a Addr Message
			/// \param networkBuffer buffer containing
			/// the raw representation
			explicit Addr(NetworkBuffer* networkBuffer);
	};

	/// \brief Ask for all the Node known
	class GetAddr : public Message{
		private:
			std::string payload() const override;
		public:
			/// Construct a GetAddr
			GetAddr();
	};

	/// \brief Message to get the JSON String representing 
	/// ressources
	class GetData : public Message {
		private:
			std::string payload() const override;
			/// \brief Ressources asked
			std::vector<networkable::Inv_vect> data;
		public:
			/// \brief Gives the type of the data asked
			networkable::Inv_vect::ressource_type 
				dataType() const;
			/// \brief Gives all the hashes of the 
			/// asked data
			std::vector<std::string> dataAsked() const;

			/// \brief Creates a GetData 
			/// from a vector of networkable::Inv_vect
			explicit GetData(std::vector<networkable::\
					Inv_vect> invData);
			/// \brief Parses a GetData
			/// \param networkBuffer buffer containing 
			/// the raw data
			explicit GetData(NetworkBuffer* 
					networkBuffer);
	};

	/// \brief Transmit Block or Transaction to a peer
	class Inv : public Message {
		private:
			std::string payload() const override;

			/// \brief data to be transmited
			std::vector<networkable::Inv_vect> data;
		public:
			inline std::vector<networkable::Inv_vect> getData() const{
				return data;
			}

			/// \brief Create a Inv from a vector of
			/// Inv_vect
			explicit Inv(std::vector<networkable::\
					Inv_vect> invData);
			/// \brief Parse an Inv
			/// \param networkBuffer buffer containing 
			/// the raw data
			explicit Inv(NetworkBuffer* networkBuffer);
	};


	/// \brief Message signaling an asked ressource is unknown
	class NotFound : public Message {
		private:
			std::string payload() const override;
			/// \brief Unknown ressources
			std::vector<networkable::Inv_vect> data;
		public:
			/// \brief Create a NotFound from the 
			/// ressources
			explicit NotFound(std::vector<networkable::\
					Inv_vect> invData);
			/// \brief Parses a NotFound
			/// \param networkBuffer buffer containing
			/// the raw data
			explicit NotFound(NetworkBuffer* 
					networkBuffer);
	};

	/// \brief Message sending a ressources::Block over
	class BlockMessage : public Message {
		private:
			std::string payload() const override;
			/// \brief Pointer to the sent ressources::Block to avoid
			/// RAM usage
			ressources::Block::pointer blockRef;
		public:
			/// \brief Returns a pointer to the ressources::Block
			ressources::Block::pointer getBlock() const;
			/// \brief Create a BlockMessage from a ressources::Block
			explicit BlockMessage(ressources::Block::pointer
					blockPtr);
			/// \brief Parses a binary string in a 
			/// BlockMessage
			/// \param networkBuffer buffer containing
			/// the raw data
			explicit BlockMessage(NetworkBuffer* 
					networkBuffer);
	};

	/// \brief Message sending a ressources::Transaction over
	class TransactionMessage : public Message {
		private:
			std::string payload() const override;
			/// \brief Pointer to the sent ressources::Transaction to
			/// avoid RAM usgae
			ressources::Transaction::pointer transaction;
		public:
			/// \brief Returns a shared pointer to the ressources::Transaction
			ressources::Transaction::pointer getTx() const;
			/// \brief Create a TransactionMessage from a
			/// ressources::Transaction
			explicit TransactionMessage(ressources::Transaction::pointer);
			/// \brief Parses a binary string in a 
			/// TransactionMessage
			/// \param networkBuffer buffer containing 
			/// the raw data
			explicit TransactionMessage(NetworkBuffer* 
					networkBuffer);
	};

	/// \brief Message asking a node to sync ressources::Block 
	/// until a certain point
	class GetBlocks : public Message {
		private:
			std::string payload() const override;
			/// \brief Known ressources::Block for reference
			std::vector< std::string > blockHashes;
			/// \brief Where to stop sending ressources::Block
			std::string stopHash;
		public:
			/// \brief Create a GetBlocks from the 
			/// known hashes
			GetBlocks(std::vector< std::string > 
					hashList, 
					std::string stopHashString);
			/// \brief Parses a binary string in a 
			/// GetBlocks
			/// \param networkBuffer buffer containing 
			/// the raw data
			explicit GetBlocks(NetworkBuffer* 
					networkBuffer);
	};

	/// \brief Message to sync the Mempool with another node
	class GetMempool : public Message{
		private:
			std::string payload() const override;
		public:
			/// \brief Create a GetMempool
			GetMempool();
	};

}; // namespace message

#endif /* MESSAGES_HPP */
