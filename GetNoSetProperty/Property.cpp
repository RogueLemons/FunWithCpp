#include <iostream>

struct Get {};
struct Set {};
template<typename T, class Friend, typename PropertyType = Get>
struct Property {};

template<typename T, class Friend>
struct Property<T, Friend, Get> {
    Property(const T& value) : _value(value) {}
    Property() : _value{} {}

    operator const T& () const { return _value; }
private:
    Property& operator= (const T& newValue) { _value = newValue; return *this; }
    Property& operator= (T&& newValue) { _value = std::move(newValue); return *this; }
    T _value;
    friend Friend;
};

template<typename T, class Friend>
struct Property<T, Friend, Set> {
    Property(const T& value) : _value(value) {}
    Property() : _value{} {}

    Property& operator= (const T& newValue) { _value = newValue; return *this; }
    Property& operator= (T&& newValue) { _value = std::move(newValue); return *this; }
    operator const T& () const { return _value; }
private:
    T _value;
    friend Friend;
};

class TestClass {
public:
    Property<int, TestClass, Get> Prop_Getter = 42;
    Property<int, TestClass, Set> Prop_GetSet = 43;

    void setPropsToZero() {
        Prop_Getter = 0;
        Prop_GetSet = 0;
    }
};

int main(void) {

    TestClass tc;
    std::cout << "Getter initial value: " << tc.Prop_Getter << std::endl;
    std::cout << "GetSet initial value: " << tc.Prop_GetSet << std::endl;
    std::cout << std::endl;

    // tc.Prop_Getter = 8;      // Not possible
    std::cout << "Cannot update Getter from outside: " << tc.Prop_Getter << std::endl;
    tc.Prop_GetSet = 7;
    std::cout << "GetSet new value: " << tc.Prop_GetSet << std::endl;
    std::cout << std::endl;

    std::cout << "Set properties to zero via class method:" << std::endl;
    tc.setPropsToZero();
    std::cout << "Getter value: " << tc.Prop_Getter << std::endl;
    std::cout << "GetSet value: " << tc.Prop_GetSet << std::endl;

    return 0;
}