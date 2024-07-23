#include <iostream>

template<typename T, class Friend>
struct Property {
    Property(const T& value) : _value(value) {}
    Property() : _value{} {}

    operator const T& () const { return _value; }
private:
    Property& operator= (const T& newValue) { _value = newValue; return *this; }
    Property& operator= (T&& newValue) { _value = std::move(newValue); return *this; }
    T _value;
    friend Friend;
};

class TestClass {
public:
    Property<int, TestClass> Prop = 42;

    void setPropToSeven() {
        Prop = 7;
    }
};

int main(void) {

    TestClass tc;
    std::cout << "Getter initial value: " << tc.Prop << std::endl;

    // tc.Prop = 7;      // Not possible
    std::cout << "Cannot update Getter from outside: " << tc.Prop << std::endl;

    tc.setPropToSeven();
    std::cout << "Set property to 7 from inside of class: " << tc.Prop << std::endl;

    return 0;
}