#ifndef BDUI_EVENT
#define BDUI_EVENT
#include <list>
#include <vector>
#include "delegate.hpp"

namespace BdUI{
    template<typename Return,typename... Param> class Event;
    template<typename Return,typename... Param>
    class Event<Return(Param...)> : public std::list<Delegate<Return(Param...)>*>{
    public:
        Delegate<bool(Param...)> Check;
        Delegate<void(std::vector<Return>)> ReturnCallBack;
        using std::list<Delegate<Return(Param...)>>::list;
        using std::list<Delegate<Return(Param...)>>::operator=;
        Event(const Delegate<Return(Param...)> &d) { this->push_back(d); }
        void operator()(Param... args){
            if(Check){
                if(!Check(args...)) return;
            }
            if (this->size() != 0){
                std::vector<Return>&& temp;
                for(auto i : *this){
                    temp.push_back(i(args...));
                }
                ReturnCallBack(temp);
            }
        }
        Event<Return(Param...)> &operator+=(const Delegate<Return(Param...)> &d){
            this->push_back(d);
            return *this;
        }
        Event<Return(Param...)> &operator-=(const Delegate<Return(Param...)> &d){
            if(this->size() == 0) return *this;
            this->remove(d);
            return *this;
        }
        bool operator==(const Event<Return(Param...)>& e) {
            if (e.Check == Check &&
                std::equal(this->operator[](0), this->operator[](this->size() - 1), e[0]))
                return true;
            else return false;
        }
        bool operator!=(const Event<Return(Param...)>& e) {
            if (e.Check != Check || std::equal(this->operator[](0), this->operator[](this->size() - 1), e[0]))
                return true;
            else return false;
        }
    };
    template<typename... Param>
    class Event<void(Param...)> : public std::list<Delegate<void(Param...)>>{
    public:
        Delegate<bool(Param...)> Check;
        using std::list<Delegate<void(Param...)>>::list;
        using std::list<Delegate<void(Param...)>>::operator=;
        Event(const Delegate<void(Param...)> &d) { this->push_back(d); }
        void operator()(Param... args){
            if (Check){
                if (!Check(args...)) return;
            }
            if(this->size() != 0){
                for(auto i : *this){
                    i(args...);
                }
            }
        }
        Event<void(Param...)> &operator+=(const Delegate<void(Param...)> &d){
            this->push_back(d);
            return *this;
        }
        Event<void(Param...)> &operator-=(const Delegate<void(Param...)> &d){
            if(this->size() == 0) return *this;
            auto iter = std::find(this->begin(),this->end(),d);
            if ((*iter) == d) this->erase(iter);
            return *this;
        }
        bool operator==(const Event<void(Param...)> &e){
            if (e.Check == Check &&
            std::equal(this->operator[](0),this->operator[](this->size()-1),e[0]))
                return true;
            else return false;
        }
        bool operator!=(const Event<void(Param...)> &e){
            if (e.Check != Check || std::equal(this->operator[](0),this->operator[](this->size()-1),e[0]))
                return true;
            else return false;
        }
    };

    template<typename Return,typename... Param> class EventArray;
    template<typename Return,typename... Param>
    class EventArray<Return(Param...)> : public std::list<Event<Return(Param...)>>{
    public:
        using std::list<Event<Return(Param...)>>::list;
        using std::list<Event<Return(Param...)>>::operator=;
        EventArray() {}
        EventArray(const Event<Return(Param...)> &e) { this->push_back(e); }
        void operator()(Param... args){
            if (this->size() != 0){
                for(auto i : *this){
                    i(args...);
                }
            }
        }
        EventArray<Return(Param...)> &operator+=(const Event<Return(Param...)> &e){
            this->push_back(e);
            return *this;
        }
        EventArray<Return(Param...)> &operator-=(const Event<Return(Param...)> &e){
            if(this->size() == 0) return *this;
            this->remove(e);
            return *this;
        }
    };
}
#endif