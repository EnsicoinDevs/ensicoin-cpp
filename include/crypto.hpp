#ifndef HASH_HPP
#define HASH_HPP

#include <string>
#include <memory>
#include <cryptopp/integer.h>
#include <cryptopp/eccrypto.h>

using namespace CryptoPP;

using eCurve = ECDSA<ECP, SHA256> ; 

std::string ripemd160(std::string input,bool unpack=false);
std::string sha256(std::string input,bool unpack=false);

class ECDSASignature{
	private:
		std::string signature; //Binary string
	public:
		ECDSASignature(std::string encodedSignature);
		ECDSASignature(std::string message,eCurve::PrivateKey privateKey);
		std::string hex();
		bool verify(std::string message, std::string publicKeyString);
};

std::string hexPublicKey( const ECP::Point q);

class Hashable : public std::enable_shared_from_this<Hashable> {
	public:
		using pointer = std::shared_ptr<Hashable>;
		virtual const std::string rawStr() const = 0;
		const std::string hash() const;
};

#endif /* HASH_HPP */
