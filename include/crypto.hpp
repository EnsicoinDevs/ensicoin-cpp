#ifndef HASH_HPP
#define HASH_HPP

#include <string>
#include <cryptopp/integer.h>
#include <cryptopp/eccrypto.h>

using namespace CryptoPP;

using eCurve = ECDSA<ECP, SHA256> ; 

std::string ripemd160(std::string input);

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

#endif /* HASH_HPP */
