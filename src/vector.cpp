template <typename Type>
class vector{
    const int cluster_size = 100;
    int capacity = 100;
    int size = 0;
    Type* data;
    public:
    vector(){
        this->data = new Type[capacity];
    }

    ~vector(){
        delete[] this->data;
    }

    Type& operator[](int i){
        return this->data[i];
    }

    const Type& operator[](int i) const{
        return this->data[i];
    }

    void add(Type value){
        if(this->size == this->capacity){
            this->expand();
        }
        this->data[this->size] = value;
        this->size += 1;
    }

    void expand(){
        this->capacity += this->cluster_size;
        Type* newData = new Type[capacity];
        for (int i = 0; i < size; ++i)
            newData[i] = data[i];

        delete[] data;
        data = newData;
    }
};