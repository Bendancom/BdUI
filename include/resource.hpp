#ifndef BDUI_RESOURCE
#define BDUI_RESOURCE
#include <string>
#include <fstream>
#include <cstdint>
#ifdef _WIN32
#include <Windows.h>
#endif
#include "error.hpp"

namespace BdUI{
    class Resource{
    protected:
        std::string Filepath;
        std::fstream File;
    public:
        Resource() {}
        Resource(const std::string&);
        Resource(const Resource&);
        ~Resource();
        virtual void OpenFile(const std::string&);
        virtual void Load();
        virtual void Save();
        Resource &operator=(const Resource& resource);
        unsigned long long Size;
        unsigned char* Data = nullptr;
    };
}
#endif