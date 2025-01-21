#ifndef REGISTER_H
#define REGISTER_H

template <typename T>
class Register {
private:
    T val;            

public:
    Register() : val() {}
    ~Register() {}

    T getVal();
    void setVal(T newVal);   
};

template <typename T>
T Register<T>::getVal() {
    return val;
}

template <typename T>
void Register<T>::setVal(T newVal) {
    val = newVal;
}

#endif // REGISTER_H