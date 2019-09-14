#include <string>

class Base64
{
public:
	static std::string encode(unsigned char const* , unsigned int len);
	static std::string decode(std::string const& s);
};