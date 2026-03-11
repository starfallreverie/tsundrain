// memory.cpp
#include <include/global.hpp>

namespace core {

	memory_c::~memory_c( )
	{
		::CloseHandle( this->m_handle );
	}

	bool memory_c::attach( const wchar_t* name )
	{
		const auto pid = this->find_pid( name );
		if ( !pid ) {
			return false;
		}

		this->m_pid = pid;

		const auto handle = ::OpenProcess( PROCESS_VM_READ | PROCESS_QUERY_INFORMATION, 0, pid );
		if ( !handle or handle == INVALID_HANDLE_VALUE ) {
			return false;
		}

		this->m_handle = handle;

		return true;
	}

	bool memory_c::read_virtual( const void* address, void* buffer, const std::size_t size ) const
	{
		return ::ReadProcessMemory( this->m_handle, address, buffer, size, nullptr );
	}

	bool memory_c::query_memory( const void* address, MEMORY_BASIC_INFORMATION& memory_info ) const
	{
		return ::VirtualQueryEx( this->m_handle, address, &memory_info, sizeof( memory_info ) ) == sizeof( memory_info );
	}

	std::uint32_t memory_c::find_pid( const wchar_t* name ) const
	{
		PROCESSENTRY32 entry{ };
		entry.dwSize = sizeof( PROCESSENTRY32 );

		auto snapshot = ::CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
		if ( snapshot == INVALID_HANDLE_VALUE ) {
			return 0;
		}

		if ( !Process32First( snapshot, &entry ) )
		{
			::CloseHandle( snapshot );
			return 0;
		}

		do
		{
			if ( ::_wcsicmp( name, entry.szExeFile ) == 0 )
			{
				::CloseHandle( snapshot );
				return entry.th32ProcessID;
			}
		} while ( Process32Next( snapshot, &entry ) );

		::CloseHandle( snapshot );
		return 0;
	}


} // namespace core