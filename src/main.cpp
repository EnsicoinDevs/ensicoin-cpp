#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#include <asio.hpp>

#include "constants.hpp"
#include "util.hpp"
#include "logger.hpp"
#include "node.hpp"
#include "networkable.hpp"

int main(){

	auto logger = std::make_shared<Logger>(Logger::TRACE);

	int status;
	status = mkdir(constants::DATA_PATH.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	if( status != 0 && errno != EEXIST){
		char buffer[ 256 ];
		logger->fatal("Error when creating main directory : ", strerror_r( errno, buffer, 256 ));
		return 1;
	}
	status = mkdir(constants::BLOCKCHAIN_DB.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	if( status != 0 && errno != EEXIST){
		char buffer[ 256 ];
		logger->fatal("Error when creating blockchain directory : ", strerror_r( errno, buffer, 256 ));
		return 1;
	}

	asio::io_context io_context;
	Node node(io_context, logger);
	io_context.run();
	
	return 0;
}
