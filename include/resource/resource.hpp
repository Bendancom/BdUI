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
        std::string FilePath;
        std::fstream FileStream;
        enum class Where {
            Unknow = 0,
            File = 1,
            Memory = 2,
        }Source = Where::Unknow;
        unsigned long long Size;
        unsigned char* Data = nullptr;
        bool IsProcess = false;
    public:
        Resource() {}
        Resource(const std::string&);
        Resource(const Resource&);
        ~Resource();
        void OpenFile(const std::string&);
        void OpenMemory(void*,unsigned long long);
        void MemoryCopy(void*, unsigned long long);
        std::pair<void*, unsigned long long> getData();
        void LoadFile();
        virtual void SaveFile();
        virtual void Process() {}
        Resource &operator=(const Resource& resource);
    };
}
#endif