#include <cstring>
#include <cstdio>
#include <string>
#include <openssl/ripemd.h>

#include "hash.hpp"

std::string ripemd160(std::string input){
	const char* string = input.c_str();
	unsigned char digest[RIPEMD160_DIGEST_LENGTH];
	RIPEMD160_CTX ctx;
    	RIPEMD160_Init(&ctx);
   	RIPEMD160_Update(&ctx, string, strlen(string));
    	RIPEMD160_Final(digest, &ctx);
	
	char mdString[RIPEMD160_DIGEST_LENGTH*2+1];
    	for (int i = 0; i < RIPEMD160_DIGEST_LENGTH; i++)
        	sprintf(&mdString[i*2], "%02x", (unsigned int)digest[i]);

	return std::string(mdString);
}
