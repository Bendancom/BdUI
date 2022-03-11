#ifndef BDUI_RESOURCE
#define BDUI_RESOURCE
#include <string>
#include <iostream>
#include <fstream>
#ifdef _WIN32
#include <Windows.h>
#endif
#include "../error.hpp"

namespace BdUI{
    class IResource{
    public:
        IResource() {}
        virtual ~IResource() = 0;
        virtual void LoadFromFile(const std::string&) = 0;
        virtual void LoadFromExecution(int) = 0;
        virtual void SaveInFile(const std::string&) = 0;
        virtual void SaveInExecution(int) = 0;
        std::string FilePath;
        unsigned long long FileSize;
    };
}
#endif