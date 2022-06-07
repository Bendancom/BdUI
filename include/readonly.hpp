#ifndef BDUI_READONLY
#define BDUI_READONLY
namespace BdUI{
    template<typename Data>
    class ReadOnly{
    public:
        void (*set)(Data&,const Data&);
        Data (*get)(const Data&);
        ReadOnly() {}
        ReadOnly(Data data) : Value(data),Isempty(false) {}
        ReadOnly(const ReadOnly<Data>&);
        void Set(Data);
        Data Get();
        operator Data();
        Data *operator->() const;
        bool empty();
        operator bool();
        ReadOnly<Data> &operator=(Data);
        ReadOnly<Data> &operator=(const ReadOnly<Data>&);
    private:
        Data Value;
        bool Isempty = true;
    };
}
#endif