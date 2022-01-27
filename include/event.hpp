namespace BdUI
{
    template <typename Return, typename... Param>
    class Event;

    template <typename Return, typename... Param>
    class Event<Return(Param...)> : public std::vector<Delegate<Return(Param...)>>
    {
    public:
        bool (*check)(const Param &...) = nullptr;
        void (*returnCallback)(const std::map<std::type_index, Return> &) = nullptr;
        using std::vector<Delegate<Return(Param...)>>::vector;
        using std::vector<Delegate<Return(Param...)>>::operator=;
        void operator()(Param... args)
        {
            if (check != nullptr)
            {
                if (!check(args...))
                    return;
            }
            else if (!Check(args...))
                return;
            std::map<std::type_index, Return> temp;
            for (auto iter = this->cbegin(); iter != this->cend(); iter++)
            {
                temp.insert(std::pair<std::type_index, Return>((*iter).target_type(), (*iter)(args...)));
            }
            if (returnCallback != nullptr)
                returnCallback(temp);
            else
                ReturnCallBack(temp);
        }
        Event<Return(Param...)> &operator+=(const Delegate<Return(Param...)> &d)
        {
            if (this->size() == 0 || (*(std::find(this->begin(), this->end(), d))) != d)
                this->push_back(d);
            return *this;
        }
        Event<Return(Param...)> &operator-=(const Delegate<Return(Param...)> &d)
        {
            if (this->size() == 0)
                return *this;
            auto iter = std::find(this->begin(), this->end(), d);
            if ((*iter) == d)
                this->erase(iter);
            return *this;
        }

    private:
        virtual bool Check(const Param &...) { return true; }
        virtual void ReturnCallBack(const std::map<std::type_index, Return> &) { return; }
    };

    template <typename... Param>
    class Event<void(Param...)> : public std::vector<Delegate<void(Param...)>>
    {
    public:
        bool (*check)(const Param &...) = nullptr;
        using std::vector<Delegate<void(Param...)>>::vector;
        using std::vector<Delegate<void(Param...)>>::operator=;
        void operator()(Param... args)
        {
            if (check != nullptr)
            {
                if (!check(args...))
                    return;
            }
            else if (!Check(args...))
                return;
            for (auto iter = this->begin(); iter != this->end(); iter++)
                (*iter)(args...);
        }
        Event<void(Param...)> &operator+=(const Delegate<void(Param...)> &d)
        {
            if (this->size() == 0 || (*(std::find(this->begin(), this->end(), d))) != d)
                this->push_back(d);
            return *this;
        }
        Event<void(Param...)> &operator-=(const Delegate<void(Param...)> &d)
        {
            if (this->size() == 0)
                return *this;
            auto iter = std::find(this->begin(), this->end(), d);
            if ((*iter) == d)
                this->erase(iter);
            return *this;
        }

    private:
        virtual bool Check(const Param &...) { return true; }
    };

    template <typename Return, typename... Param>
    class EventArray;

    template <typename Return, typename... Param>
    class EventArray<Return(Param...)> : public std::vector<Event<Return(Param...)>>
    {
    public:
        using std::vector<Event<Return(Param...)>>::vector;
        using std::vector<Event<Return(Param...)>>::operator=;
        void operator()(Param... args)
        {
            for (auto iter = this->begin(); iter != this->end(); iter++)
                (*iter)(args...);
        }
        EventArray<Return(Param...)> &operator+=(const Event<Return(Param...)> &e)
        {
            if (this->size() == 0 || (*(std::find(this->begin(), this->end(), e))) != e)
                this->push_back(e);
            return *this;
        }
        EventArray<Return(Param...)> &operator-=(const Event<Return(Param...)> &e)
        {
            if (this->size() == 0)
                return *this;
            auto iter = std::find(this->begin(), this->end(), e);
            if ((*iter) == e)
                this->erase(iter);
            return *this;
        }
    };
} // namespace BdUI
