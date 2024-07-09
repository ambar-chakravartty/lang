#ifndef VALUES_HPP
#define VALUES_HPP

#include <string>

enum class ValueType {
    NULLTYPE,
    NUMBER,
    STRVAL
};

class RuntimeVal{
    public:
        ValueType type;

        virtual ~RuntimeVal() = default; 

};

class NullVal : public RuntimeVal{
public:
    std::string value;

    NullVal(){
        value = "null";
        type = ValueType::NULLTYPE;
    }
};

class NumberVal : public RuntimeVal{
public:
    float value;

    NumberVal(float value): value(value) {
        type = ValueType::NUMBER;
    }
};

class StringValue : public RuntimeVal{
public:
    std::string value;

    StringValue(std::string value) : value(value) {
        type = ValueType::STRVAL;
    }
};

#endif //VALUES_HPP