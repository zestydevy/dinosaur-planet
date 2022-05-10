![Dinosaur Planet Decompilation](docs/banner.png)

[![](https://img.shields.io/badge/Discord-Dinosaur%20Planet%20Community-5865F2?logo=discord)](https://discord.gg/H6WGkznZBc) 
![](https://img.shields.io/endpoint?url=https%3A%2F%2Fraw.githubusercontent.com%2FFrancessco121%2Fdino-status%2Fgh-pages%2Ftotal.shield.json) 
![](https://img.shields.io/endpoint?url=https%3A%2F%2Fraw.githubusercontent.com%2FFrancessco121%2Fdino-status%2Fgh-pages%2Fcore.shield.json)
![](https://img.shields.io/endpoint?url=https%3A%2F%2Fraw.githubusercontent.com%2FFrancessco121%2Fdino-status%2Fgh-pages%2Fdlls.shield.json)

A WIP decompilation of Dinosaur Planet for the Nintendo 64, as released by Forest of Illusion on Feb. 20, 2021.

**Note**: To use this repository, you must already have a ROM for the game.

```diff
- WARNING! -

This codebase is in an early research/development phase and the ROM this repository
builds is not 'shiftable', so cannot be used yet as a source code base for general changes.
```

## Development

### Prerequisites
**Operating system**
- Linux (Debian / Ubuntu)
- Windows 10/11 WSL (Ubuntu)
    - **Note**: The repository must be cloned with Unix line endings!
    - It is highly recommended to clone this repository to the WSL Linux partition. Builds are considerably slower when done on a Windows partition.
- Other systems may work, see `packages.txt` for the list of required APT packages

**APT packages**<br/>
`sudo apt install -y $(cat packages.txt)`

**Python 3 packages**<br/>
`pip3 install -r requirements.txt`

> Note: This repository uses Git submodules. Don't forget to update them with `./dino.py submodules` or `git submodule update --recursive` after pulling changes to a submodule ref.

### Setup
1. Place an unmodified Dinosaur Planet ROM into the root of the repository as `baserom.z64` (MD5: `49f7bb346ade39d1915c22e090ffd748`).
2. Set up tools and extract the ROM:
    - `./dino.py setup`
3. Build the ROM:
    - `./dino.py build`

### Rebuilding
1. If any symbols or the splat config were changed, re-extract the ROM and rebuild the linker script:
    - `./dino.py extract`
2. If any source files were added, removed, or renamed, re-configure the build script:
    - `./dino.py configure`
2. Rebuild the ROM:
    - `./dino.py build`

### Documentation
Please see the [docs](./docs) directory for project documentation such as:
- [an overview of the project](./docs/Overview.md)
- [a break-down of the ROM extraction and build system](./docs/Recompilation.md)
- [an introductory guide to decomp](./docs/Guide.md)
- and more!

### Tools
The repository comes with a bunch of tools for decompilation and managing the repository:
- `dino.py` - An all in one script for working with the repository. Some of the common commands:
    - `build` - Builds the ROM and verifies that it matches.
    - `configure` - Re-configures the build script.
    - `extract` - Splits the ROM and unpacks DLLs.
    - `clean` - Removes all extracted files, build artifacts, and build scripts for a fresh state.
    - `diff` - Diff a function/address between the base and re-built ROM.
    - `context` - Create a context file that can be used for mips2c/decomp.me.
- `tools/first-diff.py` - Find the first N differences between the base and re-built ROM.
- `tools/first_bin_diff.py` - Find the first N differences between two binary files.
- `tools/dlldump.py` - Dump headers, relocation tables, and assembly from a Dinosaur Planet DLL.

### Docker
If you prefer to develop inside of a Docker container instead of installing everything in your local environment, use the provided Dockerfile in the root of the repository.

Example usage:
```bash
# Create image
docker build -t dpdecomp --build-arg login=$USER --build-arg uid=$UID .

# Enter a bash prompt
docker run --rm -it -v $(pwd):/dino dpdecomp bash

# Run a one-off command
docker run --rm -it -v $(pwd):/dino dpdecomp dino build
```

## Contributing
Pull requests are welcome! Please see our [contribution guide](./CONTRIBUTING.md) for more information on how this project works and how to contribute.

If you're interested in contributing, please also consider joining us on Discord over at the [Dinosaur Planet Community server](https://discord.gg/H6WGkznZBc) in the `#decompilation` channel!
