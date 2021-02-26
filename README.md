# Dinosaur Planet
A WIP decompilation of Dinosaur Planet for the Nintendo 64

<b>Note</b>: To use this repository, you must already have a ROM for the game.

# OS: 
Linux (Debian / Ubuntu)</br>
Windows 10 (WSL)

# Prerequisites:
`sudo apt install -y build-essential git binutils-mips-linux-gnu python3`</br>
`pip3 install -r requirements.txt`

# To use
1. Place the Dinosaur Planet ROM into the root of the repository as `baserom.z64`.
2. Set up tools and extract the ROM: `make setup`
3. Re-build the ROM: `make`

test