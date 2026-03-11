// memory.hpp
#pragma once

namespace core {

	class memory_c
	{
	public:
		~memory_c( );

	public:
		[[nodiscard]] bool attach( const wchar_t* name );

		bool read_virtual( const void* address, void* buffer, const std::size_t size ) const;
		bool query_memory( const void* address, MEMORY_BASIC_INFORMATION& memory_info ) const;

		[[nodiscard]] auto pid( ) const { return this->m_pid; }
		[[nodiscard]] auto handle( ) const { return this->m_handle; }

	private:
		[[nodiscard]] std::uint32_t find_pid( const wchar_t* name ) const;

	private:
		std::uint32_t m_pid{ 0 };
		HANDLE m_handle{ nullptr };
	};

} // namespace core