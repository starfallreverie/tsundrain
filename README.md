# tsundrain

all this does is attache to `Discord.exe` and brute forces all of its memory for Discord's token format or JSON profile fields like `username`, tokens are validated by base64 decoding the first segment and checking for a numeric user ID

## Why memory scanning?

most token grabbers read Discord's LevelDB files on disk under %appdata%. This works but it's well known at this point, antivirus's and Discord both watch those paths, and tokens on disk may be encrypted

memory scanning avoids all of this. Discord needs the token in memory to make requests, so it's always sitting there in plaintext regardless of how it's stored on disk, the tradeoff is Discord has to be running, but that rarely matters in practice

## Disclaimer

This project is a proof of concept built for educational purposes and personal interest in malware development techniques. It is not intended for deployment or malicious use. You assume full responsibility for how you use this software. The author accepts no liability for misuse.

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
