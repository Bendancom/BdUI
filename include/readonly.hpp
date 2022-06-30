#ifndef BDUI_READONLY
#define BDUI_READONLY
namespace BdUI{
    template<typename Data>
    class ReadOnly{
    public:
        void (*set)(Data&,const Data&);
        Data (*get)(const Data&);
        ReadOnly() {}
        ReadOnly(Data data) : Value(data),Isexist(true) {}
        ReadOnly(const ReadOnly<Data>& r) {
            if (r.Isexist)
                Value = r.Value;
            Isexist = true;
        }
        void Set(Data data) {
            if (!Isexist) {
                if (set) {
                    (*set)(Value, data);
                    Isexist = true;
                }
                else {
                    Value = data;
                    Isexist = true;
                }
            }
        }
        Data Get() {
            if (get)
                return (*get)(Value);
            else
                return Value;
        }
        operator Data() {
            if (get)
                return (*get)(Value);
            else
                return Value;
        }
        const Data* operator->() {
            return &Value;
        }
        bool exist() {
            return Isexist;
        }
        operator bool() {
            return Isexist;
        }
        ReadOnly<Data>& operator=(Data data) {
            if (!Isexist) {
                if (set) {
                    (*set)(Value, data);
                    Isexist = true;
                }
                else {
                    Value = data;
                    Isexist = true;
                }
            }
        }
        ReadOnly<Data>& operator=(const ReadOnly<Data>&) {
            if (r.Isexist && Isexist == false)
                Value = r.Value;
            Isexist = true;
        }
    private:
        Data Value;
        bool Isexist = false;
    };
}
#endif