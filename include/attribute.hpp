#ifndef BDUI_ATTRIBUTE
#define BDUI_ATTRIBUTE
#include <mutex>
#include "event.hpp"
#include "delegate.hpp"

namespace BdUI{
    template<typename... T> class Attribute; 

    template<typename Data,typename GetData,typename SetData>
    class Attribute<Data,GetData,SetData>{
    public:
        EventArray<void(Data)> Changed;
        Delegate<GetData(Data)> get;
        Delegate<bool(SetData,Data&)> set;
        Attribute() {}
        Attribute(const Data& v) : Value(v) {}
        Attribute(const Delegate<GetData(Data)>& g) : get(g) {}
        Attribute(const Data &v, const Delegate<GetData(Data)>& g) : get(g),Value(v) {}
        Attribute(const Delegate<bool(SetData,Data&)>& s) : set(s) {}
        Attribute(const Data &v, const Delegate<bool(SetData,Data&)>& s) : set(s),Value(v) {}
        Attribute(const Delegate<GetData(Data)>& g , const Delegate<bool(SetData,Data&)>& s) : get(g),set(s) {}
        Attribute(const Data &v, const Delegate<GetData(Data)>& g , const Delegate<bool(SetData,Data&)>& s) : get(g),set(s),Value(v) {}
        Attribute(const Attribute<Data,GetData,SetData>&) = delete;
        operator GetData() {
            return get(Value);
        }
        operator Data(){
            return Value;
        }
        operator bool(){
            return _exist;
        }
        bool exist(){
            return _exist;
        }
        Attribute<Data,GetData,SetData> &operator=(SetData value){
            Mutex.lock();
            if(set.exist()){
                if(set(value,Value)) Changed(Value);
            }
            else{
                Value = value;
                Changed(Value)
            }
            _exist = true;
            Mutex.unlock();
            return *this;
        }
        Attribute<Data,GetData,SetData> &operator=(const Attribute<Data,GetData,SetData>&) = delete;
    private:
        Data Value;
        bool _exist = false;
        std::mutex Mutex;
    };
    template<typename Data>
    class Attribute<Data>{
    public:
        EventArray<void(Data)> Changed;
        Delegate<Data(Data)> get;
        Delegate<bool(Data,Data&)> set;
        Attribute() {}
        Attribute(const Data& v) : Value(v) {}
        Attribute(const Delegate<Data(Data)>& g) : get(g) {}
        Attribute(const Data& v, const Delegate<Data(Data)>& g) : get(g),Value(v) {}
        Attribute(const Delegate<bool(Data,Data&)>& s) : set(s) {}
        Attribute(const Data& v, const Delegate<bool(Data,Data&)>& s) : set(s),Value(v) {}
        Attribute(const Delegate<Data(Data)>& g, const Delegate<bool(Data,Data&)>& s) : get(g),set(s) {}
        Attribute(const Attribute<Data>&) = delete;
        operator Data() {
            if(get) return get(Value);
            else return Value;
        }
        bool exist(){
            return _exist;
        }
        Attribute<Data> &operator=(Data value){
            Mutex.lock();
            if (set.exist()){
                if(set(value,Value)) Changed(Value);
            }
            else{
                Value = value;
                Changed(Value);
            }
            Mutex.unlock();
            return *this;
        }
        Attribute<Data> &operator=(const Attribute<Data>&) = delete;
    private:
        Data Value;
        bool _exist;
        std::mutex Mutex;
    };
}
#endif