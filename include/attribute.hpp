#ifndef BDUI_ATTRIBUTE
#define BDUI_ATTRIBUTE
#include <mutex>
#include "event.hpp"

namespace BdUI{
    template<typename... T> class Attribute; 

    template<typename Data,typename GetData,typename SetData>
    class Attribute<Data,GetData,SetData>{
    public:
        EventArray<void(Data)> Changed;
        GetData (*get)(const Data&);
        bool (*set)(Data&,const SetData&);
        Attribute(GetData (*getptr)(const Data&),bool (*setptr)(Data&,const SetData&)) : get(getptr),set(setptr) {}
        Attribute(const Data &t,GetData (*getptr)(const Data&),bool (*setptr)(Data&,const SetData&)) :
        Value(t),get(getptr),set(setptr) {}
        Attribute(const EventArray<void(Data)> &e,GetData (*getptr)(const Data&),bool (*setptr)(Data&,const SetData&)) :
        Changed(e),get(getptr),set(setptr) {}
        Attribute(const Data &t,const EventArray<void(Data)> &e,GetData (*getptr)(const Data&),bool (*setptr)(Data&,const SetData&)) :
        Value(t),Changed(e),get(getptr),set(setptr) {}
        Attribute(const Attribute<Data,GetData,SetData>&) = delete;
        operator GetData() {
            Mutex.lock();
            GetData &&data = (*get)(Value);
            Mutex.unlock();
            return data;
        }
        operator Data(){
            return Value;
        }
        GetData Get(){
            Mutex.lock();
            GetData &&data = (*get)(Value);
            Mutex.unlock();
            return data;
        }
        void Set(SetData value){
            Mutex.lock();
            if (!(*set)(Value,value)){
                Mutex.unlock();
                return;
            }
            Changed(Value);
            Mutex.unlock();
        }
        Attribute<Data,GetData,SetData> &operator=(SetData value){
            Mutex.lock();
            if (!(*set)(Value,value)){
                Mutex.unlock();
                return *this; 
            }
            Changed(Value);
            Mutex.unlock();
            return *this;
        }
        Attribute<Data,GetData,SetData> &operator=(const Attribute<Data,GetData,SetData>&) = delete;
    private:
        Data Value;
        std::mutex Mutex;
    };
    template<typename Data>
    class Attribute<Data>{
    public:
        EventArray<void(Data)> Changed;
        Data (*get)(const Data&);
        bool (*set)(Data&,const Data&);
        Attribute(): get(nullptr),set(nullptr) {}
        Attribute(Data (*getptr)(const Data&),bool (*setptr)(Data&,const Data&)) : get(getptr),set(setptr) {}
        Attribute(Data (*getptr)(const Data&)) : get(getptr),set(nullptr) {}
        Attribute(bool (*setptr)(Data&,const Data&)) : set(setptr),get(nullptr) {}
        Attribute(const Data &t,Data (*getptr)(const Data&),bool (*setptr)(Data&,const Data&)) :
        Value(t),get(getptr),set(setptr) {}
        Attribute(const Data &t,Data (*getptr)(const Data&)) :
        Value(t),get(getptr),set(nullptr) {}
        Attribute(const Data &t,bool (*setptr)(Data&,const Data&)) :
        Value(t),set(setptr),get(nullptr) {}
        Attribute(const EventArray<void(Data)> &e,Data (*getptr)(const Data&),bool (*setptr)(Data&,const Data&)) :
        Changed(e),get(getptr),set(setptr) {}
        Attribute(const EventArray<void(Data)> &e,Data (*getptr)(const Data&)) :
        Changed(e),get(getptr),set(nullptr) {}
        Attribute(const EventArray<void(Data)> &e,bool (*setptr)(Data&,const Data&)) :
        Changed(e),set(setptr),get(nullptr) {}
        Attribute(const Data &t,const EventArray<void(Data)> &e,Data (*getptr)(const Data&),bool (*setptr)(Data&,const Data&)) :
        Value(t),Changed(e),get(getptr),set(setptr) {}
        Attribute(const Data &t,const EventArray<void(Data)> &e,Data (*getptr)(const Data&)) :
        Value(t),Changed(e),get(getptr),set(nullptr) {}
        Attribute(const Data &t,const EventArray<void(Data)> &e,bool (*setptr)(Data&,const Data&)) :
        Value(t),Changed(e),set(setptr),get(nullptr) {}
        Attribute(const Attribute<Data>&) = delete;
        operator Data() {
            Data data;
            Mutex.lock();
            if (get != nullptr) data = get(Value);
            else data = Value;
            Mutex.unlock();
            return data;
        }
        Data Get() {
            Data data;
            Mutex.lock();
            if (get != nullptr) data = get(Value);
            else data = Value;
            Mutex.unlock();
            return data;
        }
        void Set(Data value){
            Mutex.lock();
            if (set != nullptr){ 
                if (!(*set)(Value,value)) {
                    Mutex.unlock();
                    return; 
                }
            }
            else Value = value;
            Changed(Value);
            Mutex.unlock();
        }
        Attribute<Data> &operator=(Data value){
            Mutex.lock();
            if (set != nullptr){ 
                if (!(*set)(Value,value)) {
                    Mutex.unlock();
                    return *this;
                }
            }
            else Value = value;
            Changed(Value);
            Mutex.unlock();
            return *this;
        }
        Attribute<Data> &operator=(const Attribute<Data>&) = delete;
    private:
        Data Value;
        std::mutex Mutex;
    };
}
#endif