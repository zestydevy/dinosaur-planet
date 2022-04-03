<img src=https://media.discordapp.net/attachments/383113459581321217/817157532346351626/banner_out.png></img>


A WIP decompilation of Dinosaur Planet for the Nintendo 64, as released by Forest of Illusion on Feb. 20, 2021.

<b>Note</b>: To use this repository, you must already have a ROM for the game.

```diff
- WARNING! -

This codebase is in an early research/development phase and the ROM this repository
builds is not 'shiftable', so cannot be used yet as a source code base for general changes.
```

# OS: 
Linux (Debian / Ubuntu)</br>
Windows 10 (WSL)

# Install prerequisites:
```bash
# APT packages
sudo apt install -y $(cat packages.txt)

# Python packages
pip3 install -r requirements.txt
```

# To use:
1. Place an unmodified Dinosaur Planet ROM into the root of the repository as `baserom.z64` (MD5: `49f7bb346ade39d1915c22e090ffd748`)
2. Set up tools and extract the ROM: `make setup`
3. Re-build the ROM: `make`

# Contributing:
PRs are welcome. Please make sure that the ROM builds and matches successfully before submitting a non-draft PR; the CI system will also verify this.

Also please see our [contribution guide](./CONTRIBUTING.md) for more information on how this project works and the ways to contribute.
