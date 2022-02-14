#ifndef BDUI_READONLY
#define BDUI_READONLY
namespace BdUI{
    template<typename Data>
    class ReadOnly{
    public:
        ReadOnly() {}
        ReadOnly(Data data) : Value(data),empty(false) {}
        ReadOnly(const ReadOnly<Data> &r){
            if(!r.empty) Value = r.Value;
            empty = false;
        }
        void Set(Data data){
            if(empty){
                Value = data;
                empty = false;
            }
        }
        Data Get(){
            return Value;
        }
        operator Data(){
            return Value;
        }
        Data *operator->() const{
            return &Value;
        }
        ReadOnly<Data> &operator=(Data data){
            if(empty) Value = data;
            return *this;
        }
        ReadOnly<Data> &operator=(const ReadOnly<Data> &r){
            if(empty && !r.empty) Value = r.Value;
            return *this;
        }
    private:
        Data Value;
        bool empty = true;
    };
}
#endif