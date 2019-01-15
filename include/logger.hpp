#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>
#include <sstream>
#include <ctime>

class Logger{
	public:
		enum log_level {
			TRACE = 0,
			INFO = 1,
			WARN = 2,
			ERROR = 3,
			FATAL = 4
		};
		inline void set_level(log_level new_level){
			level = new_level;
		}
		Logger(log_level initial_level) : level(initial_level) {}

		template<typename ... Args>
		void trace(Args ... args){
			if(level <= TRACE)
				prefix("\x1B[1m\x1B[37mTRACE\x1B[0m", args ...);
		}
		template<typename ... Args>
		void info(Args ... args){
			if(level <= INFO)
				prefix("\x1B[1m\x1B[92mINFO\x1B[0m", args ...);
		}
		template<typename ... Args>
		void warn(Args ... args){
			if(level <= WARN)
				prefix("\x1B[1m\x1B[33mWARN\x1B[0m", args ...);
		}
		template<typename ... Args>
		void error(Args ... args){
			if(level <= ERROR)
				prefix("\x1B[1m\x1B[31mERROR\x1B[0m", args ...);
		}
		template<typename ... Args>
		void fatal(Args ... args){
			prefix("\x1B[1m\x1B[91mFATAL\x1B[0m", args ...);
		}
	private:
		template<typename ... Args>
		inline void prefix(std::string level_repr,Args ... args) {
			auto tm = std::time(nullptr);
			char mbstr[100];
			std::strftime(mbstr, sizeof(mbstr),
					      "%a %d %b %Y %H:%M:%S",
						  std::localtime(&tm));
			std::ostringstream os;
			os << mbstr << " [" << level_repr << "] ";
			generate(os, args ...);
			std::cout << os.str() << std::endl;
		}

		inline std::ostringstream& generate(std::ostringstream& os){
			return os;
		}
		template<typename T,typename ... Args>
		std::ostringstream& generate(std::ostringstream& os,
									 T first_arg,
									 Args ... args) {
			os << first_arg;
			return generate(os, args ...);
		}
		log_level level;
};

#endif /* LOGGER_HPP */
