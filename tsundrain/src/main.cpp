// main.cpp
#include <include/global.hpp>

int main( )
{
	::SetConsoleTitleA( "tsundrain" );

	if ( !g->m_memory.attach( L"Discord.exe" ) )
	{
		print( "!", "failed to find process" );
		std::cin.get( );
		return 0;
	}

	print( "+", "attached (pid: %u)", g->m_memory.pid( ) );
	print( "~", "brute forcing memory..." );

	const auto tokens = g->m_discord.find_tokens( );
	const auto usernames = g->m_discord.find_usernames( );

	std::printf( "\n" );

	for ( const auto& token : tokens ) {
		std::printf( "   > [token] %s\n", token.c_str( ) );
	}

	for ( const auto& username : usernames ) {
		std::printf( "   > [username] %s\n", username.c_str( ) );
	}

	std::printf( "\n" );
	print( "+", "found %zu token(s)", tokens.size( ) );
	print( "+", "found %zu username(s)", usernames.size( ) );

	std::cin.get( );
	return 0;
}