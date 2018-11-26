#ifndef TRANSACTION_HPP
#define TRANSACTION_HPP

#include "hashmemory.hpp"
#include "jsonable.hpp"
#include "networkable.hpp"
#include "networkbuffer.hpp"
#include "script.hpp"

#include <iterator>
#include <memory>
#include <rapidjson/document.h>
#include <sstream>
#include <stack>
#include <string>
#include <vector>

/// \brief Ressources manipulated by the Node
namespace ressources {

	/// \brief Reference to the OutputTransaction of another
	/// Transaction 
	/// \todo change index to uint[x]
	struct TransactionIdentifier : 
		public networkable::Networkable {
		/// \brief Hash of the refrenced Transaction
		std::string transactionHash;
		/// \brief Index of the OutputTransaction
		unsigned int index;

		std::string byteRepr() const override;
		/// \brief Extract the raw data to construct a 
		/// TransactionIdentifier
		explicit TransactionIdentifier(NetworkBuffer*
				networkBuffer);

		/// \brief Construct a TransactionIdentifier from
		/// the data fields
		TransactionIdentifier(std::string transactionHash,
				unsigned int index);

		/// \brief Get the JSON reperesentation
		/// \param document Document used for the 
		/// Allocation of members
		rapidjson::Value json(rapidjson::Document* 
				document) const;
	};
	/// \brief Repersent an input of a Transation
	struct InputTransaction : public networkable::Networkable{
		/// \brief Reference to another Transaction
		TransactionIdentifier previousOutput;
		/// \brief The script attached to the input
		Script script;

		std::string byteRepr() const override;
		/// \brief Extract raw data to create a 
		/// InputTransaction
		explicit InputTransaction(NetworkBuffer*
				networkBuffer);
		/// \brief Construct a InputTransaction
		InputTransaction( TransactionIdentifier id,
							Script sc);

		/// \brief Get the JSON reperesentation
		/// \param document Document used for the
		/// Allocation of members
		rapidjson::Value json(rapidjson::Document* 
				document) const;
	};

	/// \brief Represent the output of a Transaction
	struct OutputTransaction : public networkable::Networkable {
		/// \brief The Value make available
		uint64_t value;
		/// \brief the code attached to the input
		Script script;

		std::string byteRepr() const override;
		/// \brief Extract raw data to construct a 
		/// OutputTransaction
		explicit OutputTransaction(NetworkBuffer*
				networkBuffer);
		OutputTransaction( uint64_t val, Script sc);

		/// \brief Get the JSON reperesentation
		/// \param document Document used for the
		/// Allocation of members
		rapidjson::Value json(rapidjson::Document* 
				document) const;
	};

	class Mempool;


	/// \brief A class representing an exchange of ensicoins
	class Transaction : public JSONAble,
	public std::enable_shared_from_this<Transaction>,
	public networkable::Networkable {
		protected:
			/// \brief Referenced in constants.hpp
			int version;
			/// \brief User defined flags
			std::vector<std::string> transactionFlags;
			/// \brief All InputTransaction
			std::vector<InputTransaction> inputs;
			/// \brief All OutputTransaction
			std::vector<OutputTransaction> outputs;
		public:
			/// \brief Types a Transaction can be
			enum TXType { Orphan, Regular };
			/// \brief Shared ptr to a Transaction
			using pointer = std::shared_ptr<Transaction>;
			/// \brief Create a Transaction from
			/// all data fields
			Transaction(	int ver, 	
					std::vector<std::string>
						initialFlags, 
					std::vector<InputTransaction>
						initialInputs, 
					std::vector<\
						OutputTransaction>
						initialOutputs);
			/// \brief Parse a Transaction from a 
			/// NetworkBuffer
			/// \param networkBuffer Buffer containing
			/// raw data
			explicit Transaction(NetworkBuffer*
					networkBuffer);

			/// \brief returns the version used
			int getVersion() const;
			/// \brief returns the flags
			std::vector<std::string> getFlags() const;
			/// \brief returns all Transaction referenced
			std::vector<TransactionIdentifier>
				getInputsId() const;
			/// \brief Calculate the value of all
			/// OutputTransaction
			unsigned int outputValue() const;

			/// \brief Get all the InputTransaction
			std::vector<InputTransaction> 
				getInputs() const;
			/// \brief Get the script for the designed
			/// OutputTransaction
			Script getScriptOfOutput(int index) const;

			/// \brief Checks if the transaction has an 
			/// OutputTransaction
			/// \param index index to be verified if
			/// exists
			bool hasOutput(unsigned int index) const;
			/// \brief Get the value of the index-th
			/// output
			unsigned int valueOfOutput(unsigned int 
					index) const;
			/// \brief Gets the number of 
			/// OutputTransaction
			unsigned int outputCount() const;

			/// \brief Checks if the Transaction is
			/// valid by itself
			bool check();

			std::string byteRepr() const override;
			/// \brief Taking two times sha256 of the
			/// byteRepr
			std::string hash() const;
			/// \brief Get the hash to be used for
			/// signing the Transaction
			std::string signingHash() const;
			rapidjson::Document json() const final;
	};

} // namespace ressources

#endif /* TRANSACTION_HPP */
