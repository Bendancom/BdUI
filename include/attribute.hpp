#include <mutex>
#include "event.hpp"

#ifndef BDUI_ATTRIBUTE
#define BDUI_ATTRIBUTE
namespace BdUI{
    template<typename Data,typename GetData = Data,typename SetData = Data>
    class Attribute{
    public:
        EventArray<void(Data)> EventList;
        GetData (*get)(const Data&);
        bool (*set)(Data&,const SetData&);
        Attribute(GetData (*getptr)(const Data&) = nullptr,bool (*setptr)(Data&,const SetData&) = nullptr) :
        get(getptr),set(setptr) {}
        Attribute(const Data &t,GetData (*getptr)(const Data&) = nullptr,bool (*setptr)(Data&,const SetData&) = nullptr) :
        Value(t),get(getptr),set(setptr) {}
        Attribute(const Data &t,const EventArray<void(Data)> &e,GetData (*getptr)(const Data&) = nullptr,bool (*setptr)(Data&,const SetData&) = nullptr) :
        Value(t),EventList(e),get(getptr),set(setptr) {}
        Attribute(const EventArray<void(Data)> &e,GetData (*getptr)(const Data&) = nullptr,bool (*setptr)(Data&,const SetData&) = nullptr) :
        EventList(e),get(getptr),set(setptr) {}
        Attribute(const Attribute<Data,GetData,SetData> &a) : Value(a.Value),set(a.set),get(a.get),EventList(a.EventList) {}
        operator GetData() {
            Mutex.lock();
            GetData data;
            if (get != nullptr) data = get(Value);
            else data = Get(Value);
            Mutex.unlock();
            return data;
        }
        const Data *operator->() { return &Value; }
        Attribute<Data,GetData,SetData> &operator=(SetData value){
            Mutex.lock();
            if (set != nullptr) { if (!set(Value,value)) { return *this; } }
            else { if (!Set(Value,value)) { return *this; } }
            Mutex.unlock();
            EventList(Value);
            return *this;
        }
        Attribute<Data,GetData,SetData> &operator=(const Attribute<Data,GetData,SetData> &a){
            Mutex.lock();
            Value = a.Value;
            get = a.get;
            set = a.set;
            EventList = a.EventList;
            Mutex.unlock();
        }
    private:
        Data Value;
        std::mutex Mutex;
        virtual GetData Get(const Data &t) {}
        virtual bool Set(Data&,const SetData&) { return true; };
    };
    template<typename Data,typename GetData>
    class Attribute<Data,GetData,GetData>{
    public:
        EventArray<void(Data)> EventList;
        Data (*get)(const Data&);
        bool (*set)(Data&,const GetData&);
        Attribute(GetData (*getptr)(const Data&) = nullptr,bool (*setptr)(Data&,const GetData&) = nullptr) :
        get(getptr),set(setptr) {}
        Attribute(const Data &t,GetData (*getptr)(const Data&) = nullptr,bool (*setptr)(Data&,const GetData&) = nullptr) :
        Value(t),get(getptr),set(setptr) {}
        Attribute(const Data &t,const EventArray<void(Data)> &e,GetData (*getptr)(const Data&) = nullptr,bool (*setptr)(Data&,const GetData&) = nullptr) :
        Value(t),EventList(e),get(getptr),set(setptr) {}
        Attribute(const EventArray<void(Data)> &e,GetData (*getptr)(const Data&) = nullptr,bool (*setptr)(Data&,const GetData&) = nullptr) :
        EventList(e),get(getptr),set(setptr) {}
        Attribute(const Attribute<Data,GetData,GetData> &a) {
            Mutex.lock();
            Value = a.Value;
            set = a.set;
            get = a.get;
            EventList = a.EventList;
            Mutex.unlock();
        }
        operator GetData() { 
            if (get != nullptr) return get(Value);
            else return Get(Value);
        }
        const Data *operator->() { return &Value; }
        Attribute<Data,GetData,GetData> &operator=(Data value){
            Mutex.lock();
            if (set != nullptr) { if (!set(Value,value)) return *this; }
            else { if (!Set(Value,value)) return *this; }
            Mutex.unlock();
            EventList(value);
            return *this;
        }
        Attribute<Data,GetData,GetData> &operator=(const Attribute<Data,GetData,GetData> &a){
            Mutex.lock();
            Value = a.Value;
            get = a.get;
            set = a.set;
            EventList = a.EventList;
            Mutex.unlock();
        }
    private:
        Data Value;
        std::mutex Mutex;
        virtual GetData Get(const Data &t) {}
        virtual bool Set(Data&,const GetData&) { return true; };
    };

    template<typename Data,typename GetData>
    class Attribute<Data&,GetData,GetData>{
        public:
        EventArray<void(Data)> EventList;
        GetData (*get)(const Data&);
        bool (*set)(Data&,const GetData&);
        Attribute(Data &t,GetData (*getptr)(const Data&) = nullptr,bool (*setptr)(Data&,const GetData&) = nullptr) :
        Value(t),get(getptr),set(setptr) {}
        Attribute(Data &t,const EventArray<void(Data)> &e,GetData (*getptr)(const Data&) = nullptr,bool (*setptr)(Data&,const GetData&) = nullptr) :
        Value(t),EventList(e),get(getptr),set(setptr) {}
        Attribute(const Attribute<Data&,GetData,GetData> &a) : Value(a.Value),set(a.set),get(a.get),EventList(a.EventList) {}
        operator GetData() {
            Mutex.lock();
            GetData data;
            if (get != nullptr) data = get(Value);
            else data = Get(Value);
            Mutex.unlock();
            return data;
        }
        const Data *operator->() { return &Value; }
        Attribute<Data&,GetData,GetData> &operator=(GetData value){
            Mutex.lock();
            if (set != nullptr) { if (!set(Value,value)) { return *this; } }
            else { if (!Set(Value,value)) { return *this; } }
            Mutex.unlock();
            EventList(Value);
            return *this;
        }
        Attribute<Data&,GetData,GetData> &operator=(const Attribute<Data&,GetData,GetData> &a){
            Mutex.lock();
            Value = a.Value;
            get = a.get;
            set = a.set;
            EventList = a.EventList;
            Mutex.unlock();
        }
    private:
        Data &Value;
        std::mutex Mutex;
        virtual GetData Get(const Data &t) {}
        virtual bool Set(Data&,const GetData&) { return true; };
    };
    template<typename Data,typename GetData,typename SetData>
    class Attribute<Data&,GetData,SetData>{
        public:
        EventArray<void(Data)> EventList;
        GetData (*get)(const Data&);
        bool (*set)(Data&,const SetData&);
        Attribute(Data &t,GetData (*getptr)(const Data&) = nullptr,bool (*setptr)(Data&,const SetData&) = nullptr) :
        Value(t),get(getptr),set(setptr) {}
        Attribute(Data &t,const EventArray<void(Data)> &e,GetData (*getptr)(const Data&) = nullptr,bool (*setptr)(Data&,const SetData&) = nullptr) :
        Value(t),EventList(e),get(getptr),set(setptr) {}
        Attribute(const Attribute<Data&,GetData,SetData> &a) : Value(a.Value),set(a.set),get(a.get),EventList(a.EventList) {}
        operator GetData() {
            Mutex.lock();
            GetData data;
            if (get != nullptr) data = get(Value);
            else data = Get(Value);
            Mutex.unlock();
            return data;
        }
        const Data *operator->() { return &Value; }
        Attribute<Data&,GetData,SetData> &operator=(SetData value){
            Mutex.lock();
            if (set != nullptr) { if (!set(Value,value)) { return *this; } }
            else { if (!Set(Value,value)) { return *this; } }
            Mutex.unlock();
            EventList(Value);
            return *this;
        }
        Attribute<Data&,GetData,SetData> &operator=(const Attribute<Data&,GetData,SetData> &a){
            Mutex.lock();
            Value = a.Value;
            get = a.get;
            set = a.set;
            EventList = a.EventList;
            Mutex.unlock();
        }
    private:
        Data &Value;
        std::mutex Mutex;
        virtual GetData Get(const Data &t) {}
        virtual bool Set(Data&,const SetData&) { return true; };
    };
    template<typename Data>
    class Attribute<Data&,Data,Data>{
        public:
        EventArray<void(Data)> EventList;
        Data (*get)(const Data&);
        bool (*set)(Data&,const Data&);
        Attribute(Data &t,Data (*getptr)(const Data&) = nullptr,bool (*setptr)(Data&,const Data&) = nullptr) :
        Value(t),get(getptr),set(setptr) {}
        Attribute(Data &t,const EventArray<void(Data)> &e,Data (*getptr)(const Data&) = nullptr,bool (*setptr)(Data&,const Data&) = nullptr) :
        Value(t),EventList(e),get(getptr),set(setptr) {}
        Attribute(const Attribute<Data&,Data,Data> &a) : Value(a.Value),set(a.set),get(a.get),EventList(a.EventList) {}
        operator Data() {
            Mutex.lock();
            Data data;
            if (get != nullptr) data = get(Value);
            else data = Get(Value);
            Mutex.unlock();
            return data;
        }
        const Data *operator->() { return &Value; }
        Attribute<Data&,Data,Data> &operator=(Data value){
            Mutex.lock();
            if (set != nullptr) { if (!set(Value,value)) { return *this; } }
            else { if (!Set(Value,value)) { return *this; } }
            Mutex.unlock();
            EventList(Value);
            return *this;
        }
        Attribute<Data&,Data,Data> &operator=(const Attribute<Data&,Data,Data> &a){
            Mutex.lock();
            Value = a.Value;
            get = a.get;
            set = a.set;
            EventList = a.EventList;
            Mutex.unlock();
        }
    private:
        Data &Value;
        std::mutex Mutex;
        virtual Data Get(const Data &t) {}
        virtual bool Set(Data&,const Data&) { return true; };
    };
}
#endif