#ifndef NETWORKABLE_HPP
#define NETWORKABLE_HPP

#include "constants.hpp"

#include <algorithm>
#include <sstream>
#include <string>
#include <vector>

class NetworkBuffer;

/// \brief Basic types sendables as bytes by TCP
namespace networkable{

	/// \brief Object to be send to another Node in a byte list
	class Networkable{
		protected:	
			/// \brief Get the binary string 
			/// represesentation of a packed string
			std::string encode(const std::string& 
					packedString) const;
		public:
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
			explicit Var_uint(NetworkBuffer* networkBuffer);
			std::string byteRepr() const override;
			/// \brief Create a Var_uint
			explicit Var_uint(uint64_t val);
			Var_uint();
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
			explicit Uint16(NetworkBuffer* networkBuffer);
			/// \brief Create a Uint16
			explicit Uint16(uint16_t val);
			Uint16();
	};
	/// \brief Networkable uint32
	class Uint32 : public Networkable{
		protected:
			/// \brief Value of the uint32 
			uint32_t value;
		public:
			/// \brief Return the value
			uint32_t getValue() const;
			explicit Uint32(NetworkBuffer* networkBuffer);
			std::string byteRepr() const override;
			/// \brief Create a Uint32
			explicit Uint32(uint32_t val);
			Uint32();
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
			explicit Uint64(NetworkBuffer* networkBuffer);
			/// \brief Create a Uint32
			explicit Uint64(uint64_t val);
			Uint64();
	};

	/// \brief Represent string as packed hexadecimal
	class Str : public Networkable{
		protected:
			/// \brief Value of the string
			std::string value;
		public:
			/// \brief Return the underlying value
			std::string getValue() const;
			std::string byteRepr() const override;
			explicit Str(NetworkBuffer* networkBuffer, size_t strLength);
			/// \brief Construct a Str
			explicit Str(const std::string& val);
			Str();
	};

	template<int size>
	class FixedStr : public Networkable{
		private:
			Str stringValue;
		public:
			std::string getValue() const{
				return stringValue.getValue();
			}
			std::string byteRepr() const override{
				return stringValue.byteRepr();
			}
			explicit FixedStr(NetworkBuffer* networkBuffer) :
				stringValue(networkBuffer, size) {}
			explicit FixedStr(const std::string& val) : stringValue(val) {}
			FixedStr() {};
	};

	using Hash = FixedStr<constants::HASH_LENGTH>;
	using IP = FixedStr<16>;

	/// \brief Networkable string of variable length
	class Var_str : public Networkable{
		protected:
			/// \brief Value of the string
			Str value;
		public:
			/// \brief Returns the underlying value
			std::string getValue() const;
			std::string byteRepr() const override;
			explicit Var_str(NetworkBuffer* networkBuffer);
			/// \brief Create a Var_str
			explicit Var_str(const std::string& val);
			Var_str();
	};

	/// \brief Class to store Networkable Array
	template<class T>
	class Var_Array : public Networkable{
		private:
			/// \brief Networkable data
			std::vector<T> data;
		public:
			/// \brief The type being represented by T
			using valueType = decltype(T().getValue());
			/// \brief Get all the values of the Networkable
			std::vector<valueType> getValue() const{
				std::vector<valueType> out;
				std::transform(data.begin(),
								data.end(),
								std::back_inserter(out),
								[](const T& t){
									return t.getValue();
								});
				return out;
			}
			std::string byteRepr() const override{
				std::ostringstream repr;
				repr << Var_uint(data.size()).byteRepr();
				for(auto& element : data){
					repr << element.byteRepr();
				}
				return repr.str();
			}
			/// \brief Extract a Var_Array from a NetworkBuffer
			explicit Var_Array(NetworkBuffer* networkBuffer){
				auto size = Var_uint(networkBuffer).getValue();
				for(uint64_t i = 0; i < size; ++i){
					data.push_back(T(networkBuffer));
				}
			}
			/// \brief Construct a Var_Array from a vector
			/// of Networkables
			explicit Var_Array(const std::vector<T>&
					networkableArray) : data(networkableArray) {}
			/// \brief Create an empty Array
			Var_Array() : data() {}
			/// \brief Construct a Var_Array from the
			/// represented value
			/// \details Expects to be able to call T(val)
			template<typename T1 = T, typename T2 = valueType>
			explicit Var_Array(const std::vector<valueType>&
					valueArray,
					typename std::enable_if<!std::is_same<T1,T2>::value>::type * = nullptr) {
				std::transform(valueArray.begin(),
								valueArray.end(),
								std::back_inserter(data),
								[](const valueType& val){
									return T(val);
								});
			}
	};
	
	using HashArray = Var_Array<Hash>;
	using Var_strArray = Var_Array<Var_str>;

	/// \brief Ressource reference (hash and type)
	struct Inv_vect : public Networkable{
		/// \brief Possible ressources
		enum ressource_type { blockRes, txRes, invalidRes };

		/// \brief ressource_type of the Inv_vect
		ressource_type type;
		/// \brief hash of the ressource_type
		std::string hash;

		Inv_vect getValue() const;

		/// \brief Construct an Inv_vect from fields
		Inv_vect(ressource_type res, 
				std::string resHash);
		explicit Inv_vect(NetworkBuffer* networkBuffer);
		Inv_vect();

		std::string byteRepr() const override;
	};

	using Inv_Array = Var_Array<Inv_vect>;
	
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
			/// \brief Returns a human readable value
			Address getValue() const;
			/// \brief Returns the address
			std::string getAddress() const;
			/// \brief Returns the port
			uint16_t getPort() const;
			/// \brief Returns the time since the last known activity
			uint64_t getTimestamp() const;
			/// \brief Construct an Address
			Address(uint64_t lastActive, std::string ipAddress,
					uint16_t prt);
			explicit Address(NetworkBuffer* buffer);
			Address();
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
		explicit MessageHeader(NetworkBuffer* buffer);
		MessageHeader();
		std::string byteRepr() const override;
	};

} //namespace networkable

#endif /* NETWORKABLE_HPP */
