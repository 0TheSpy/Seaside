#ifndef NETVARMGR
#define NETVARMGR
#pragma once

#include "sdk/cdll_int.h"
#include "sdk/dt_recv.h"
#include "sdk/client_class.h"
#include <vector>

#include "Interfaces.hpp"


#include <cstdlib>

namespace detail
{
	template <typename Type, Type OffsetBasis, Type Prime>
	struct size_dependant_data
	{
		using type = Type;
		constexpr static auto k_offset_basis = OffsetBasis;
		constexpr static auto k_prime = Prime;
	};

	template <size_t Bits>
	struct size_selector;

	template <>
	struct size_selector<32>
	{
		using type = size_dependant_data<std::uint32_t, 0x811c9dc5ul, 16777619ul>;
	};

	template <>
	struct size_selector<64>
	{
		using type = size_dependant_data<std::uint64_t, 0xcbf29ce484222325ull, 1099511628211ull>;
	};

	template <std::size_t Size>
	class fnv_hash
	{
	private:
		using data_t = typename size_selector<Size>::type;

	public:
		using hash = typename data_t::type;

	private:
		constexpr static auto k_offset_basis = data_t::k_offset_basis;
		constexpr static auto k_prime = data_t::k_prime;

	public:
		template <std::size_t N>
		static __forceinline constexpr auto hash_constexpr(const char(&str)[N], const std::size_t size = N) -> hash
		{
			return static_cast<hash>(1ull * (size == 1
				? (k_offset_basis ^ str[0])
				: (hash_constexpr(str, size - 1) ^ str[size - 1])) * k_prime);
		}

		static auto __forceinline hash_runtime(const char* str) -> hash
		{
			auto result = k_offset_basis;
			do
			{
				result ^= *str++;
				result *= k_prime;
			} while (*(str - 1) != '\0');

			return result;
		}
	};
}

namespace fnv2 {
	constexpr uint32_t offsetBasis = 0x811c9dc5;
	constexpr uint32_t prime = 0x1000193;

	constexpr uint32_t hash(const char* str, const uint32_t value = offsetBasis) noexcept
	{
		return *str ? hash(str + 1, (value ^ *str) * static_cast<unsigned long long>(prime)) : value;
	}

	constexpr uint32_t hashRuntime(const char* str) noexcept
	{
		auto value = offsetBasis;

		while (*str) {
			value ^= *str++;
			value *= prime;
		}
		return value;
	}
}

using fnv = ::detail::fnv_hash<sizeof(void*) * 8>;

#define FNV(str) (std::integral_constant<fnv::hash, fnv::hash_constexpr(str)>::value)


#include <map>
#include <type_traits>

class netvar_manager
{
private:
	struct stored_data
	{
		RecvProp* prop_ptr;
		std::uint16_t class_relative_offset;
	};

public:
	static auto get() -> const netvar_manager&
	{
		static netvar_manager instance;
		return instance;
	}

	auto get_offset(const fnv::hash hash) const -> std::uint16_t
	{
		return m_props.at(hash).class_relative_offset;
	}

	auto get_prop(const fnv::hash hash) const -> RecvProp*
	{
		return m_props.at(hash).prop_ptr;
	}

	__declspec(noinline) static auto get_offset_by_hash(const fnv::hash hash) -> std::uint16_t
	{
		return get().get_offset(hash);
	}

	template<fnv::hash Hash>
	static auto get_offset_by_hash_cached() -> std::uint16_t
	{
		static auto offset = std::uint16_t(0);
		if (!offset)
			offset = get_offset_by_hash(Hash);
		return offset;
	}

private:
	netvar_manager();
	auto dump_recursive(const char* base_class, RecvTable* table, std::uint16_t offset) -> void;

private:
	std::map<fnv::hash, stored_data> m_props;
};


#define PNETVAR_OFFSET(funcname, class_name, var_name, offset, ...) \
auto funcname() -> std::add_pointer_t<__VA_ARGS__> \
{ \
	constexpr auto hash = fnv::hash_constexpr(class_name "->" var_name); \
	const auto addr = std::uintptr_t(this) + offset + netvar_manager::get_offset_by_hash_cached<hash>(); \
	return reinterpret_cast<std::add_pointer_t<__VA_ARGS__>>(addr); \
}

#define PNETVAR(funcname, class_name, var_name, ...) \
	PNETVAR_OFFSET(funcname, class_name, var_name, 0, __VA_ARGS__)

#define NETVAR_OFFSET(funcname, class_name, var_name, offset, ...) \
auto funcname() -> std::add_lvalue_reference_t<__VA_ARGS__> \
{ \
	constexpr auto hash = fnv::hash_constexpr(class_name "->" var_name); \
	const auto addr = std::uintptr_t(this) + offset + netvar_manager::get_offset_by_hash_cached<hash>(); \
	return *reinterpret_cast<std::add_pointer_t<__VA_ARGS__>>(addr); \
}

#define NETVAR(funcname, class_name, var_name, ...) \
	NETVAR_OFFSET(funcname, class_name, var_name, 0, __VA_ARGS__)

#define NETPROP(funcname, class_name, var_name) \
static auto funcname() ->  RecvProp* \
{ \
	constexpr auto hash = fnv::hash_constexpr(class_name "->" var_name); \
	static RecvProp* prop_ptr; \
	if(!prop_ptr) prop_ptr = netvar_manager::get().get_prop(hash); \
	return prop_ptr; \
}

class recv_prop_hook
{
public:
	recv_prop_hook(RecvProp* prop, const RecvVarProxyFn proxy_fn) :
		m_property(prop),
		m_original_proxy_fn(prop->m_ProxyFn)
	{
		set_proxy_function(proxy_fn);
	}

	~recv_prop_hook()
	{
		m_property->m_ProxyFn = m_original_proxy_fn;
	}

	auto get_original_function() const -> RecvVarProxyFn
	{
		return m_original_proxy_fn;
	}

	auto set_proxy_function(const RecvVarProxyFn proxy_fn) const -> void
	{
		m_property->m_ProxyFn = proxy_fn;
	}

private:
	RecvProp* m_property;
	RecvVarProxyFn m_original_proxy_fn;
};



#endif
