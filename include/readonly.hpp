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
        ReadOnly(const ReadOnly<Data> &r){
            if(!r.Isempty) Value = r.Value;
            Isempty = false;
        }
        void Set(Data data){
            if(Isempty){
                if(set){
                    (*set)(Value,data);
                    Isempty = false;
                }
                else{
                    Value = data;
                    Isempty = false;
                }
            }
        }
        Data Get(){
            if (get) return (*get)(Value);
            else return Value;
        }
        operator Data(){
            if (get) return (*get)(Value);
            else return Value;
        }
        Data *operator->() const{
            return &Value;
        }
        bool empty(){
            return Isempty;
        }
        operator bool(){
            return Isempty;
        }
        ReadOnly<Data> &operator=(Data data){
            if(Isempty){
                if(set){
                    (*set)(Value,data);
                    Isempty = false;
                }
                else{
                    Value = data;
                    Isempty = false;
                }
            }
            return *this;
        }
        ReadOnly<Data> &operator=(const ReadOnly<Data> &r){
            if(Isempty && !r.Isempty){
                if(set){
                    (*set)(Value,r.Value);
                    Isempty = false;
                }
                else{
                    Value = r.Value;
                    Isempty = false;
                }
            }
            return *this;
        }
    private:
        Data Value;
        bool Isempty = true;
    };
}
#endif