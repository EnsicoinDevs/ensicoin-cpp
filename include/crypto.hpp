#ifndef HASH_HPP
#define HASH_HPP

#include <string>
#include <memory>
#include <cryptopp/integer.h>
#include <cryptopp/eccrypto.h>

using namespace CryptoPP;

using eCurve = ECDSA<ECP, SHA256> ; 

/// \brief Take the RIPEMD160 hash
/// \param input string to be hashed
/// \param unpack handles a binary string
std::string ripemd160(const std::string& binaryString);
/// \brief Take the SHA256 hash
/// \param binaryString binary input to be hashed
std::string sha256(const std::string& binaryString);

/// \brief Signatures using secp256k1
class ECDSASignature{
	private:
		/// \brief A binary string signature
		std::string signature;
	public:
		/// \brief Construct a ECDSASignature from signature
		/// \param encodedSignature signature as hex 
		/// representation
		ECDSASignature(std::string encodedSignature);
		/// \brief Construct a ECDSASignature to sign a 
		/// string
		/// \param message a string to be signed
		/// \param privateKey a secp256k1 private key
		ECDSASignature(std::string message,eCurve::PrivateKey privateKey);
		/// \brief Hex representation of signature
		std::string hex() const ;
		/// \brief verifies if a message is signed
		/// \param message string to be verified
		/// \param publicKeyString Hex reprensation
		/// of public the public key for checking with
		bool verify(std::string message, std::string publicKeyString) const;
};

/// \brief Hex reprensation of a public key
/// \brief q public point on secp256k1
std::string hexPublicKey( const ECP::Point q);

#endif /* HASH_HPP */
