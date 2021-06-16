#include "NetVarManager.hpp"
#include <cctype>

#ifdef DUMP_NETVARS
#define IF_DUMPING(...) __VA_ARGS__
#else
#define IF_DUMPING(...)
#endif

IF_DUMPING(static FILE* s_fp;)

netvar_manager::netvar_manager()
{
	IF_DUMPING(fopen_s(&s_fp, "netvar_dump.txt", "w");)
		for (auto clazz = iff.g_pClient->GetAllClasses(); clazz; clazz = clazz->m_pNext)
			if (clazz->m_pRecvTable)
				dump_recursive(clazz->m_pNetworkName, clazz->m_pRecvTable, 0);
	IF_DUMPING(fclose(s_fp);)
}

auto netvar_manager::dump_recursive(const char* base_class, RecvTable* table, const std::uint16_t offset) -> void
{
	for (auto i = 0; i < table->m_nProps; ++i)
	{
		const auto prop_ptr = &table->m_pProps[i];

		if (!prop_ptr || isdigit(prop_ptr->m_pVarName[0]))
			continue;

		if (fnv::hash_runtime(prop_ptr->m_pVarName) == FNV("baseclass"))
			continue;

		if (prop_ptr->m_RecvType == DPT_DataTable &&
			prop_ptr->m_pDataTable != nullptr &&
			prop_ptr->m_pDataTable->m_pNetTableName[0] == 'D')    
		{
			dump_recursive(base_class, prop_ptr->m_pDataTable, std::uint16_t(offset + prop_ptr->m_Offset));
		}

		char hash_name[256];

		strcpy_s(hash_name, base_class);
		strcat_s(hash_name, "->");
		strcat_s(hash_name, prop_ptr->m_pVarName);

		const auto hash = fnv::hash_runtime(hash_name);
		const auto total_offset = std::uint16_t(offset + prop_ptr->m_Offset);

		IF_DUMPING(fprintf(s_fp, "%s\t0x%04X\t%s\n", base_class, total_offset, prop_ptr->m_pVarName);)

			m_props[hash] =
		{
			prop_ptr,
			total_offset
		};
	}
}