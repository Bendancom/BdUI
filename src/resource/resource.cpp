#include "resource/resource.hpp"

namespace BdUI
{
    Resource::Resource(const std::string &s)
    {
        OpenFile(s);
    }
    
    Resource::Resource(const Resource &resource)
    {
        Size = resource.Size;
        Data = new unsigned char[Size];
        memcpy(Data, resource.Data, Size);
    }

    Resource::~Resource()
    {
        delete[] Data;
    }

    void Resource::OpenFile(const std::string &s)
    {
        FilePath = s;
        std::string&& file_ext = FilePath.substr(FilePath.find_last_of('.') + 1);
        std::transform(file_ext.begin(), file_ext.end(), file_ext.begin(), std::tolower);
        File_Ext = file_ext;
    }

    void Resource::Malloc(unsigned long long size) {
        Clear();
        Data = new unsigned char[size];
        Size = size;
    }

    void Resource::Copy(void* data, unsigned long long size) {
        Clear();
        Data = new unsigned char[size];
        memcpy(Data, data, size);
        Size = size;
    }

    void Resource::Clear() {
        free(Data);
        Data = nullptr;
    }

    Resource::ResourceType Resource::GetResourceType() {
        return Resource_Type;
    }

    std::pair<void*, unsigned long long> Resource::getDataPointer() {
        if (Data == nullptr) throw error::Class::Uninitialize();
        return std::pair<void*, unsigned long long>(Data, Size);
    }

    void Resource::LoadFromFile()
    {
        Clear();
        if (FilePath.empty() == true) throw error::File::Open_Failed("Haven't opened the File");
        std::fstream fstream(FilePath,std::ios::binary|std::ios::in);
        Size = fstream.rdbuf()->in_avail();
        Data = new unsigned char[Size];
        fstream.read((char *)Data, Size);
    }
    void Resource::LoadFromMemory(void* pos,unsigned long long size) {
        Clear();
        Copy(pos, size);
    }

    void Resource::SaveToFile()
    {
        if (FilePath.empty() == true) throw error::File::Open_Failed("Haven't opened the File");
        std::fstream fstream(FilePath, std::ios::out || std::ios::binary);
        fstream.write((char*)Data, Size);
    }
    std::pair<void*,unsigned long long> Resource::SaveToMemory() {
        void* dst = malloc(Size);
        memcpy(dst, Data, Size);
        return { dst,Size };
    }

    Resource &Resource::operator=(const Resource &resource)
    {
        delete[] Data;
        Size = resource.Size;
        Data = new unsigned char[Size];
        if(resource.Data != nullptr) memcpy(Data, resource.Data, Size);
        return *this;
    }
}