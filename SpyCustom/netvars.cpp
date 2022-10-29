#include "netvars.hpp"

#include <fstream>
#include <utility>

#include "interfaces.hpp"

void NetvarSys::Initialize()
{

    m_tables.clear();

    ClientClass* clientClass = iff.g_pClient->GetAllClasses();
    if (!clientClass)
        return;

    while (clientClass)
    {
        RecvTable* recvTable = clientClass->m_pRecvTable;
        m_tables.push_back(recvTable);

        clientClass = clientClass->m_pNext;
    }

}


#undef GetProp

int NetvarSys::GetOffset(const char* tableName, const char* propName)
{
    int offset = this->GetProp(tableName, propName);
    if (!offset)
    { 
        printfdbg("%s not found!\n", propName);
        return 0;
    }
    printfdbg("%s: 0x%02X\n", propName, offset);

    return offset;
}


int NetvarSys::GetProp(const char* tableName, const char* propName, RecvProp** prop)
{
    RecvTable* recvTable = GetTable(tableName);
    if (!recvTable)
        return 0;

    int offset = this->GetProp(recvTable, propName, prop);
    if (!offset)
        return 0;

    return offset;
}


int NetvarSys::GetProp(RecvTable* recvTable, const char* propName, RecvProp** prop)
{
    int extraOffset = 0;
    for (int i = 0; i < recvTable->m_nProps; ++i)
    {
        RecvProp* recvProp = &recvTable->m_pProps[i];
        RecvTable* child = recvProp->m_pDataTable;

        if (child && (child->m_nProps > 0))
        {
            int tmp = this->GetProp(child, propName, prop);
            if (tmp)
                extraOffset += (recvProp->m_Offset + tmp);
        }

        if (_stricmp(recvProp->m_pVarName, propName))
            continue;

        if (prop)
            *prop = recvProp;

        return (recvProp->m_Offset + extraOffset);
    }

    return extraOffset;
}

RecvTable* NetvarSys::GetTable(const char* tableName)
{
    if (m_tables.empty())
        return 0;

    for (const auto& table : m_tables)
    {
        if (!table)
            continue;

        if (_stricmp(table->m_pNetTableName, tableName) == 0)
            return table;
    }

    return 0;
}







