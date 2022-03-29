#ifndef BDUI_ERROR
#define BDUI_ERROR
#include <exception>

#define ERRORTYPE(name,error_what,free) class name : public std::exception{public: virtual char const* what() const{return error_what;} }

namespace BdUI{
    namespace error{
        ERRORTYPE(data_error,"The Data is invaild",false);
        ERRORTYPE(format_error,"The Format is invaild",false);
        ERRORTYPE(bad_data_request,"The Data Requestion has error",false);
        ERRORTYPE(data_available_error,"The Data isn't available",false);
        ERRORTYPE(value_error,"The Value is invaild",false);
    }
}
#endif