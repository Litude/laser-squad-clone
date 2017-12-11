#include <exception>

class JSONLoadException : public std::exception
{
    virtual const char* what() const throw()
    {
	    return "Level data was invalid";
    }
};

class JSONWriteException : public std::exception
{
    virtual const char* what() const throw()
    {
	    return "Writing level as JSON failed.";
    }
};