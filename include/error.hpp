#ifndef BDUI_ERROR
#define BDUI_ERROR
#include <exception>

#define ERRORTYPE(name,error_what) class name : public std::exception{public: virtual const char* what(){return string_what;} name(){} name(const char* s):string_what(s){} private: const char* string_what = error_what; }

namespace BdUI{
    namespace error{
        namespace File{
            ERRORTYPE(Open_Failed,"Can't find the file by this filename");
            ERRORTYPE(Type_Invalid, "The type of the file is invalid");
            ERRORTYPE(Const_Changed, "The const resource can't be changed.");
            ERRORTYPE(Data_Error, "The data is wrong");
        }
        namespace Class{
            ERRORTYPE(Uninitialize,"Haven't initialized");
            ERRORTYPE(Initialize_Failed, "Can't initialize the class");
        }
        namespace Function {
            ERRORTYPE(ParamError, "Input a error parameter");
            ERRORTYPE(CarryOut_Faild, "Faild to carry out the function");
        }
        namespace Log {
            ERRORTYPE(LayoutError, "Invaild Layout");
        }
        namespace OpenGL {
            ERRORTYPE(Initialization_Failed, "OpenGL Load Error");
            ERRORTYPE(UnInitialization, "UnLoad OpenGL");
        }
        namespace Math {
            ERRORTYPE(operator_error, "Unkonwn Operator");
            ERRORTYPE(variable_num_error, "The number of variables is error");
            ERRORTYPE(no_variable, "This doesn't have any variable");
            ERRORTYPE(variable_subscript_error, "The variable's subscript is invalid");
        }
    }
}
#undef ERRORTYPE
#endif