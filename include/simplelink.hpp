#ifndef BDUI_SIMPLELINK
#define BDUI_SIMPLELINK
namespace BdUI{
    template<typename Data>
    class SimpleLink{
        SimpleLink();
        SimpleLink(const std::initializer_list<Data> &list){
            Size = list.size();
            Root.data = new Data(list[0]);
            Root.next = new LinkNode();
            LinkNode *current = Root.next;
            for(int i = 1;i<list.size();i++){
                current->data = list[i];
                current->next = new LinkNode();
                Current = current;
                current = current->next;
            }
            IsRoot = true;
        }
        ~SimpleLink(){
            delete Root.data;
            LinkNode *current = Root.next;
            while (current != nullptr){
                LinkNode *next = current->next;
                delete current;
                current = next;
            }
        }
        int size(){ return Size; }
        Data &operator[](int i){
            if (i == 0) return Root.data;
            LinkNode *current = Root.next;
            for(int j = 1;j<i;j++){
                current = current->next;
            }
            return *current->data;
        }
        SimpleLink<Data> &operator=(const SimpleLink<Data> &s){

        }
        SimpleLink<Data> &add(const Data &d){
            if (IsRoot){
                LinkNode
            }
        }
    private:
        struct LinkNode{
            Data data;
            LinkNode *next;
        };
        struct LinkRoot{
            Data *data;
            LinkNode *next;
        }Root;
        LinkNode *Current;
        bool IsRoot = false;
        int Size = 0;
    }
}
#endif