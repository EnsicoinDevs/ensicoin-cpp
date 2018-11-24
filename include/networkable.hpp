#ifndef NETWORKABLE_HPP
#define NETWORKABLE_HPP

#include <string>
#include <vector>

/// \brief Basic types sendables as bytes by TCP
namespace networkable{

	/// \brief Object to be send to another Node in a byte list
	class Networkable{
		public:
			/// \brief Get the binary string represesentation
			/// of the Networkable object
			std::string asBytes() const;
			/// \brief Get the represention of the object
			/// as a packed hexadecimal string
			virtual std::string byteRepr() const = 0;
			virtual ~Networkable() = 0;
	};

	/// \brief Networkable unsigned int of variable length
	/// \details Size is either 2,3,5 or 9 bytes depending on
	/// the value of the integer. Prefixes to indicate length are
	/// 2:No Prefix, 3:fd, 5:fe, 9:ff
	class Var_uint : public Networkable{
		protected:
			/// \brief Value of the at most 64 bit integer
			uint64_t value;
		public:
			/// \brief Return the uint64 value
			uint64_t getValue() const;
			std::string byteRepr() const override;
			/// \brief Create a Var_uint
			explicit Var_uint(uint64_t val);
	};

	/// \brief Networkable uint16
	class Uint16 : public Networkable{
		protected:
			/// \brief value of the 16 bit uint
			uint16_t value;
		public:
			/// \brief Return the value
			uint16_t getValue() const;
			std::string byteRepr() const override;
			/// \brief Create a Uint16
			explicit Uint16(uint16_t val);
	};
	/// \brief Networkable uint32
	class Uint32 : public Networkable{
		protected:
			/// \brief Value of the uint32 
			uint32_t value;
		public:
			/// \brief Return the value
			uint32_t getValue() const;
			std::string byteRepr() const override;
			/// \brief Create a Uint32
			explicit Uint32(uint32_t val);
	};
	/// \brief Networkable uint64
	class Uint64 : public Networkable{
		protected:
			/// \brief Value of the uint64
			uint64_t value;
		public:
			/// \brief Return the value
			uint64_t getValue() const;
			std::string byteRepr() const override;
			/// \brief Create a Uint32
			explicit Uint64(uint64_t val);
	};

	/// \brief Networkable string of variable length
	class Var_str : public Networkable{
		protected:
			/// \brief Value of the string
			std::string value;
		public:
			/// \brief Returns the underlying value
			std::string getValue() const;
			std::string byteRepr() const override;
			/// \brief Create a Var_str
			explicit Var_str(const std::string& val);
	};
	
	/// \brief Ressource reference (hash and type)
	struct Inv_vect : public Networkable{
		/// \brief Possible ressources
		enum ressource_type { blockRes, txRes, invalidRes };

		/// \brief ressource_type of the Inv_vect
		ressource_type type;
		/// \brief hash of the ressource_type
		std::string hash;

		/// \brief Construct an Inv_vect from fields
		Inv_vect(ressource_type res, 
				std::string resHash);

		std::string byteRepr() const override;
	};
	
	/// \brief Network address of a known Node
	class Address : public Networkable{
		private:
			/// \brief Last time the Node was seen
			uint64_t timestamp;
			/// \brief IPv6 address of the Node
			std::string address;
			/// \brief Port used by the Node
			uint16_t port;
		public:
			/// \brief Returns the address
			std::string getAddress() const;
			/// \brief Returns the port
			uint16_t getPort() const;
			/// \brief Returns the time since the last known activity
			uint64_t getTimestamp() const;
			/// \brief Construct an Address
			Address(uint64_t lastActive, std::string ipAddress, uint16_t prt);
			std::string byteRepr() const override;
	};

	/// \brief A header for a message::Message
	struct MessageHeader : public Networkable{
		/// \brief The message::Message magic number
		uint32_t magic;
		/// \brief The message::Message type
		std::string type;
		/// \brief The Length of the message::Message payload
		uint64_t payloadLength;
		
		/// \brief Construct a MessageHeader
		MessageHeader(uint32_t _magic,std::string _type,
				uint64_t pldSize);
		std::string byteRepr() const override;
	};

} //namespace networkable

#endif /* NETWORKABLE_HPP */
