#ifndef JSONABLE_HPP
#define JSONABLE_HPP

#include <string>
#include <rapidjson/document.h>

/// \brief Class that can be reprsented in JSON
class JSONAble{
	protected:
		virtual rapidjson::Document json() const = 0;
	public:
		/// \brief represents a string as JSON
		std::string str() const;
		virtual ~JSONAble();
};

#endif /* JSONABLE_HPP */
