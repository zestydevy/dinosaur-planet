# Dinosaur Planet
A WIP decompilation of Dinosaur Planet for the Nintendo 64, as released by Forest of Illusion on Feb. 20, 2021.

<b>Note</b>: To use this repository, you must already have a ROM for the game.

```diff
- WARNING! -

The ROM this repository builds while it has a matching checksum cannot be 'shifted' due
to hardcoded pointers which have yet to be dumped. Thus this repository is currently
in an early research/development phase and cannot currently be used traditionally as a
source code base for general changes. It also cannot be run in an emulator or on console
due to the dongleCheck copy protection code.
```

# OS: 
Linux (Debian / Ubuntu)</br>
Windows 10 (WSL)

# Prerequisites:
`sudo apt install -y build-essential git binutils-mips-linux-gnu python3`</br>
`pip3 install -r requirements.txt`

# To use
1. Place the unmodified, uncracked Dinosaur Planet ROM into the root of the repository as `baserom.z64`.
2. Set up tools and extract the ROM: `make setup`
3. Re-build the ROM: `make`

# Contributing:
PRs are welcome, but please make sure your code can compile to a matching ROM succesfully
(you'll know if it does because the terminal will read ``build/dino.z64: OK`` if the resulting ROM matches) before submitting.
