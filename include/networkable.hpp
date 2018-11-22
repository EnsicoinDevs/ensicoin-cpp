#ifndef NETWORKABLE_HPP
#define NETWORKABLE_HPP

#include <string>
#include <vector>

/// \brief Object to be send to another Node in a byte list
class Networkable{
	public:
		/// \brief Get the binary string represesentation
		/// of the Networkable object
		std::string asBytes() const;
		/// \brief Calculate the hex string representing
		/// the object
		virtual std::string byteRepr() const = 0;
		virtual ~Networkable() = 0;
};

/// \brief Networkable unsigned int of variable length
/// \details Size is either 2,3,5 or 9 bytes depending on
/// the value of the integer. Prefixes to indicate length are
/// 2:No Prefix, 3:fd, 5:fe, 9:ff
class Var_uint : public Networkable{
	protected:
		/// \brief Value of the 64 bit integer
		uint64_t value;
	public:
		/// \brief Return the uint64 value
		uint64_t getValue() const;
		/// \brief Representation as packed hex string
		std::string byteRepr() const override;
		/// \brief Create a Var_uint
		explicit Var_uint(uint64_t val);
		/// \brief Read a Var_uint from a binary reprentation
		explicit Var_uint(const std::string& binaryString);
};

/// \brief Networkable string of variable length
class Var_str : public Networkable{
	protected:
		std::string value;
	public:
		/// \brief Representation as packed hex string
		std::string byteRepr() const override;
		/// \brief Create a Var_str
		explicit Var_str(std::string val);
};

#endif /* NETWORKABLE_HPP */
