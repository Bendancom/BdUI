#ifndef BDUI_ERROR
#define BDUI_ERROR
#include <exception>

#define ERRORTYPE(name,error_what) class name : public std::exception{public: virtual const char* what(){return error_what;} }

namespace BdUI{
    namespace error{
        namespace File{
            ERRORTYPE(Open_Failed,"Can't find the file by this filename.");
            ERRORTYPE(Type_Invalid, "The type of the file is invalid");
        }
        namespace Class{
            ERRORTYPE(Uninitialize,"Haven't initialized");
        }
    }
}
#endif