#ifndef BDUI_ERROR
#define BDUI_ERROR
#include <exception>

#define ERRORTYPE(name,error_what) class name : public std::exception{public: virtual const char* what(){return error_what;} }

namespace BdUI{
    namespace error{
        namespace File{
            ERRORTYPE(Open_Failed,"Can't find the file by this filename");
            ERRORTYPE(Type_Invalid, "The type of the file is invalid");
            ERRORTYPE(Const_Changed, "The const resource can't be changed.");
        }
        namespace Class{
            ERRORTYPE(Uninitialize,"Haven't initialized");
            ERRORTYPE(Initialize_Failed, "Can't initialize the class");
        }
        namespace Function {
            ERRORTYPE(ParamError, "Input a error parameter");
        }
        namespace Log {
            ERRORTYPE(LayoutError, "Invaild Layout");
        }
        namespace OpenGL {
            ERRORTYPE(Initialization_Failed, "OpenGL Load Error");
        }
    }
}
#endif