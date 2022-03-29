#ifndef BDUI_RESOURCE
#define BDUI_RESOURCE
#include <string>
#include <fstream>
#ifdef _WIN32
#include <Windows.h>
#endif
#include "error.hpp"

namespace BdUI{
    class Resource{
    protected:
        std::string filePath;
        int Id = -1;// id must less than 2^((size(int)*8))-1)
    public:
        Resource() {}
        Resource(const std::string&);
        Resource(const Resource&);
        ~Resource();
        void OpenFile(const std::string&);
        void Load();
        void Save();
        Resource &operator=(const Resource& resource);
        unsigned long long Size;
        unsigned char* Data = nullptr;
    };
}
#endif