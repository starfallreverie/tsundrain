# tsundrain

external Discord token grabber via process memory scanning.

attaches to `Discord.exe` and brute forces every committed memory region, scanning for anything that matches Discord's token format or JSON profile fields like `username`. Tokens are validated by base64 decoding the first segment and checking for a numeric user ID.

## Why memory scanning?

most token grabbers read Discord's LevelDB files on disk under %appdata%. This works but it's well known at this point, antivirus's and Discord both watch those paths, and tokens on disk may be encrypted.

memory scanning avoids all of this. Discord needs the token in memory to make requests, so it's always sitting there in plaintext regardless of how it's stored on disk. The tradeoff is Discord has to be running, but that's rarely an issue in practice.
## Usage

self explanatory, just run tsundrain while Discord is open.

### Console output

```
 [+] attached (pid: 12196)
 [~] brute forcing memory...

   > [token] MTIzNDU2Nzg5MDEyMzQ1Njc4.G1xF2w.aB3cD4eF5gH6iJ7kL8mN9oP0qR1sT2uV3w
   > [username] reverie77_

 [+] found 1 token(s)
 [+] found 1 username(s)
```

## Structure

```
include/
├── global.hpp
└── core/
    ├── discord/
    │   └── discord.hpp
    └── memory/
        └── memory.hpp

src/
├── main.cpp
└── core/
    ├── discord/
    │   └── discord.cpp
    └── memory/
        └── memory.cpp
```

## Disclaimer

This project is a proof of concept for educational purposes only. It is not intended for malicious use. The author is not responsible for any misuse of this software.
