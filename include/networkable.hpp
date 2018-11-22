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

class Var_uint : public Networkable{
	protected:
		unsigned long value;
	public:
		std::string byteRepr() const override;
		explicit Var_uint(unsigned long val);
		explicit Var_uint(const std::string& binaryString);
};

class Var_str : public Networkable{
	protected:
		std::string value;
	public:
		std::string byteRepr() const override;
		explicit Var_str(std::string val);
};

#endif /* NETWORKABLE_HPP */
