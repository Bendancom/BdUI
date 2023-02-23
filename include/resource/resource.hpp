#ifndef BDUI_RESOURCE
#define BDUI_RESOURCE
#include <string>
#include <fstream>
#include <cstdint>
#include <algorithm>
#include <filesystem>

#include <error.hpp>

namespace BdUI{
    class Resource{
    public:
        enum ResourceType {
            Unknown = 0,
            Image = 1,
            Menu = 2,
            PopMenu = 3,
            FontType = 4,
        };
    protected:
        ResourceType Resource_Type = Unknown;
        std::filesystem::path FilePath;
        unsigned long long Size = 0;
        unsigned char* Data = nullptr;
    public:
        Resource() {}
        Resource(const std::filesystem::path& filename);
        Resource(const Resource&);
        ~Resource();

        ResourceType GetResourceType();
        void Malloc(unsigned long long size);
        void Copy(void* pos, unsigned long long size);
        void Clear();
        std::pair<void*, unsigned long long> getDataPointer();

        virtual void OpenFile(const std::filesystem::path& filename);
        virtual void LoadFromFile();
        virtual void LoadFromMemory(void* pos,unsigned long long size);
        virtual void SaveToFile();
        virtual std::pair<void*,unsigned long long> SaveToMemory();

        Resource &operator=(const Resource& resource);
    };
}
#endif