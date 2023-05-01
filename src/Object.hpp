
#pragma once

#include<string>
#include<vector>

#pragma once

struct Object{
    enum class Type {
        VAR, CONST, FUNCTION
    } type;

    std::string name;
    std::vector<Object*> args;

    Object() = default;
    ~Object();
    std::string toString() const;
    bool operator==(const Object& obj);
};

Object* Var(std::string name);
Object* Const(std::string name);
Object* Func(std::string name, std::vector<Object*> args);
