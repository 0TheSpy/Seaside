#pragma once

#include <memory>
#include <iostream>
#include <unordered_map>

#include "sdk/dt_recv.h"
#include "Singleton.hpp"

class NetvarSys
    : public Singleton<NetvarSys>
{
public:
    void Initialize();

    int GetOffset(const char* tableName, const char* propName);

    RecvTable* GetTable(const char* tableName);
    std::vector<RecvTable*>    m_tables;

private:
    int GetProp(const char* tableName, const char* propName, RecvProp** prop = 0);
    int GetProp(RecvTable* recvTable, const char* propName, RecvProp** prop = 0);

};


#define NETVAR_OFFSET2(funcname, class_name, var_name, offset, ...) \
auto funcname() -> std::add_lvalue_reference_t<__VA_ARGS__> \
{ \
    static int netvar = NetvarSys::Get().GetOffset(class_name, var_name);    \
    auto addr = std::uintptr_t(this) + offset + netvar;     \
	return *reinterpret_cast<std::add_pointer_t<__VA_ARGS__>>(addr); \
}

#define NETVAR2(funcname, class_name, var_name, ...) \
	NETVAR_OFFSET2(funcname, class_name, var_name, 0, __VA_ARGS__)
