#include "./resource/cursor.hpp"

namespace BdUI{
    Cursor::Cursor(const std::string& s) { LoadFromFile(s); }
    Cursor::Cursor(int id) { LoadFromExecution(id); }
    Cursor::Cursor(const Cursor& c){

    }
    void Cursor::LoadFromExecution(int id) {
        #ifdef _WIN32
        hCursor = LoadCursor(GetModuleHandle(NULL), MAKEINTRESOURCE(id));
        #endif
    }
    void Cursor::LoadFromFile(const std::string& s) {
        FilePath = s;
        std::fstream File(s, std::ios::in | std::ios::binary);
        #ifdef _WIN32
        struct _stat info;
        _stat(s.c_str(), &info);
        FileSize = info.st_size;
        #endif
    }
    void Cursor::SaveInFile(const std::string& s) {

    }
    void Cursor::SaveInExecution(int id){

    }
}