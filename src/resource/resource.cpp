#include "resource/resource.hpp"

namespace BdUI
{
    Resource::Resource(const std::string &s)
    {
        IsProcess = false;
        FileStream.open(s,std::ios::in|std::ios::out|std::ios::binary);
        if(!FileStream) FileStream.open(s,std::ios::out|std::ios::binary);
        if(!FileStream.good()) throw error::File::Open_Failed();
        FilePath = s;
    }
    
    Resource::Resource(const Resource &resource)
    {
        IsProcess = false;
        delete[] Data;
        Size = resource.Size;
        Data = new unsigned char[Size];
        Source = resource.Source;
        FileStream.open(resource.FilePath);
        FilePath = resource.FilePath;
        memcpy(Data, resource.Data, Size);
        Process();
    }

    Resource::~Resource()
    {
        delete[] Data;
    }

    void Resource::OpenFile(const std::string &s)
    {
        FileStream.open(s,std::ios::in|std::ios::out|std::ios::binary);
        if(!FileStream) FileStream.open(s,std::ios::out|std::ios::binary);
        if(!FileStream.good()) throw error::File::Open_Failed();
        FilePath = s;
    }

    void Resource::OpenMemory(void* data, unsigned long long size) {
        IsProcess = false;
        delete[] Data;
        Data = reinterpret_cast<unsigned char*>(data);
        Size = size;
        Source = Where::Memory;
        Process();
    }

    void Resource::MemoryCopy(void* data, unsigned long long size) {
        IsProcess = false;
        delete[] Data;
        Data = new unsigned char[size];
        memcpy(Data, data, size);
        Size = size;
        Source = Where::Memory;
        Process();
    }

    std::pair<void*, unsigned long long> Resource::getData() {
        if (Data == nullptr) throw error::Class::Uninitialize();
        return std::pair<void*, unsigned long long>(Data, Size);
    }

    void Resource::LoadFile()
    {
        IsProcess = false;
        delete[] Data;
        Size = FileStream.rdbuf()->in_avail();
        Data = new unsigned char[Size];
        FileStream.read((char *)Data, Size);
        Source = Where::File;
        Process();
    }

    void Resource::SaveFile()
    {
        FileStream.write((char *)Data, Size);
    }

    Resource &Resource::operator=(const Resource &resource)
    {
        IsProcess = false;
        delete[] Data;
        Size = resource.Size;
        Data = new unsigned char[Size];
        Source = resource.Source;
        memcpy(Data, resource.Data, Size);
        Process();
        return *this;
    }
}