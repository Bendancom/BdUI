#include "resource.hpp"

namespace BdUI
{
    Resource::Resource(const std::string &s)
    {
        File.open(s,std::ios::in|std::ios::out|std::ios::binary);
        if(!File) File.open(s,std::ios::out|std::ios::binary);
        if(!File.good()) throw error::File::Open_Failed();
        Filepath = s;
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
        File.open(s,std::ios::in|std::ios::out|std::ios::binary);
        if(!File) File.open(s,std::ios::out|std::ios::binary);
        if(!File.good()) throw error::File::Open_Failed();
        Filepath = s;
    }

    void Resource::Load()
    {
        Size = File.rdbuf()->in_avail();
        Data = new unsigned char[Size];
        File.read((char *)Data, Size);
    }

    void Resource::Save()
    {
        File.write((char *)Data, Size);
    }

    Resource &Resource::operator=(const Resource &resource)
    {
        Size = resource.Size;
        Data = new unsigned char[Size];
        memcpy(Data, resource.Data, Size);
        return *this;
    }
}