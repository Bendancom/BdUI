#ifndef BDUI_EVENT
#define BDUI_EVENT
#include <list>
#include <mutex>
#include "delegate.hpp"

namespace BdUI{
    template<typename Return,typename... Param> class Event;
    template<typename Return,typename... Param>
    class Event<Return(Param...)>{
    public:
        Delegate<void(std::vector<Return>)> ReturnCallBack;
        Event(Delegate<void(std::vector<Return>)> rd, std::initializer_list<Delegate<Return(Param...)>>&& d) : Delegate_list(d), ReturnCallBack(rd) {}
        void operator()(Param... args){
            std::lock_guard<std::mutex> lock(Mutex);
            if (Delegate_list.size() != 0){
                std::vector<Return>&& temp;
                for(auto i : Delegate_list){
                    temp.push_back(i(args...));
                }
                ReturnCallBack(temp);
            }
        }
        Event<Return(Param...)> &operator+=(const Delegate<Return(Param...)> &d){
            std::lock_guard<std::mutex> lock(Mutex);
            if (Delegate_list.size() != 0)
                if (std::find(Delegate_list.begin(), Delegate_list.end(), d) == Delegate_list.end())
                    Delegate_list.push_back();
            else Delegate_list.push_back(d);
            return *this;
        }
        Event<Return(Param...)> &operator-=(const Delegate<Return(Param...)> &d){
            std::lock_guard<std::mutex> lock(Mutex);
            if (Delegate_list.size() != 0) {
                auto i = std::find(Delegate_list.begin(), Delegate_list.end(), d);
                if (i != Delegate_list.end()) Delegate_list.erase(i);
            }
            return *this;
        }
        bool operator==(const Event<Return(Param...)>& e) {
            std::lock_guard<std::mutex> lock(Mutex);
            for (auto iter = Delegate_list.begin(); iter != Delegate_list.end(); iter++)
                if (std::find(e.Delegate_list.begin(), e.Delegate_list.end(), *iter) == e.Delegate_list.end()) return false;
            return true;
        }
        bool operator!=(const Event<Return(Param...)>& e) {
            std::lock_guard<std::mutex> lock(Mutex);
            for (auto iter = Delegate_list.begin(); iter != Delegate_list.end(); iter++)
                if (std::find(e.Delegate_list.begin(), e.Delegate_list.end(), *iter) == e.Delegate_list.end()) return true;
            return false;
        }
    private:
        std::list<Delegate<Return(Param...)>> Delegate_list;
        std::mutex Mutex;
    };
    template<typename... Param>
    class Event<void(Param...)> {
    public:
        Event(std::initializer_list<Delegate<void(Param...)>> init_list) : Delegate_list(init_list){}
        void operator()(Param... args){
            if(Delegate_list.size() != 0){
                for(auto i : Delegate_list){
                    i(args...);
                }
            }
        }
        Event<void(Param...)>& operator+=(const Delegate<void(Param...)>& d) {
            std::lock_guard<std::mutex> lock(Mutex);
            if (Delegate_list.size() != 0)
                if (std::find(Delegate_list.begin(), Delegate_list.end(), d) == Delegate_list.end())
                    Delegate_list.push_back();
                else Delegate_list.push_back(d);
            return *this;
        }
        Event<void(Param...)>& operator-=(const Delegate<void(Param...)>& d) {
            std::lock_guard<std::mutex> lock(Mutex);
            if (Delegate_list.size() != 0) {
                auto i = std::find(Delegate_list.begin(), Delegate_list.end(), d);
                if (i != Delegate_list.end()) Delegate_list.erase(i);
            }
            return *this;
        }
        bool operator==(const Event<void(Param...)>& e) {
            std::lock_guard<std::mutex> lock(Mutex);
            for (auto iter = Delegate_list.begin(); iter != Delegate_list.end(); iter++)
                if (std::find(e.Delegate_list.begin(), e.Delegate_list.end(), *iter) == e.Delegate_list.end()) return false;
            return true;
        }
        bool operator!=(const Event<void(Param...)>& e) {
            std::lock_guard<std::mutex> lock(Mutex);
            for (auto iter = Delegate_list.begin(); iter != Delegate_list.end(); iter++)
                if (std::find(e.Delegate_list.begin(), e.Delegate_list.end(), *iter) == e.Delegate_list.end()) return true;
            return false;
        }
    private:
        std::list<Delegate<void(Param...)>> Delegate_list;
        std::mutex Mutex;
    };
}
#endif