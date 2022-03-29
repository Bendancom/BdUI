#include "resource.hpp"

namespace BdUI
{
    Resource::Resource(const std::string &s)
    {
        filePath = s;
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
        filePath = s;
    }

    void Resource::Load()
    {
        std::fstream file;
        file.open(filePath, std::ios::in | std::ios::binary);
        if (!file.is_open())
            throw error::bad_data_request();
        file.seekg(std::ios::end);
        Size = file.tellg();
        file.seekg(std::ios::beg);
        Data = new unsigned char[Size];
        file.read((char *)Data, Size);
    }

    void Resource::Save()
    {
        if (Size == 0 || Data == nullptr)
            throw error::value_error();
        std::fstream file;
        file.open(filePath, std::ios::out | std::ios::binary);
        file.write((char *)Data, Size);
    }

    Resource &Resource::operator=(const Resource &resource)
    {
        Size = resource.Size;
        Data = new unsigned char[Size];
        memcpy(Data, resource.Data, Size);
        return *this;
    }
}