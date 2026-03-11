// global.hpp
#pragma once

// libs
#pragma comment(lib, "Crypt32.lib")

// win32
#include <Windows.h>
#include <TlHelp32.h>

// types
#include <cstdint>

// containers
#include <string>
#include <vector>

// io
#include <iostream>

// pattern matching
#include <regex>

// core
#include <include/core/discord/discord.hpp>
#include <include/core/memory/memory.hpp>

#define print( sym, fmt, ... ) std::printf( " [" sym "] " fmt "\n", ##__VA_ARGS__ )
#define space( ) std::printf("\n")

class global_c
{
public:
	core::discord_c m_discord;
	core::memory_c m_memory;
};
inline auto* const g = new global_c( );