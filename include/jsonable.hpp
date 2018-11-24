#ifndef JSONABLE_HPP
#define JSONABLE_HPP

#include <string>
#include <rapidjson/document.h>

/// \brief Class that can be reprsented in JSON
class JSONAble{
	protected:
		/// \brief Create a rapidjson::Document from an object
		virtual rapidjson::Document json() const = 0;
	public:
		/// \brief Convert the rapidjson::Document into a string
		std::string str() const;
		virtual ~JSONAble();
};

#endif /* JSONABLE_HPP */
