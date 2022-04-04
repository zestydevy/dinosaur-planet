![Dinosaur Planet Decompilation](docs/banner.png)

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
- Other systems may work, see `packages.txt` for the list of required APT packages

**APT packages**<br/>
`sudo apt install -y $(cat packages.txt)`

**Python 3 packages**<br/>
`pip3 install -r requirements.txt`

### Setup
1. Place an unmodified Dinosaur Planet ROM into the root of the repository as `baserom.z64` (MD5: `49f7bb346ade39d1915c22e090ffd748`).
2. Set up tools and extract the ROM:
    - `make setup`
3. Build the ROM:
    - `make -j`

### Rebuilding
1. If symbols or the splat config were changed, re-extract the ROM and rebuild the linker script:
    - `make split`
2. Rebuild the ROM:
    - `make -j`

### Docker
If you prefer to develop inside of a Docker container instead of installing everything in your local environment, use the provided Dockerfile in the root of the repository.

Example usage:
```bash
# Create container
docker build -t dino .

# Enter a bash prompt
docker run --rm -it -v $(pwd):/dino dino bash

# Run a one-off command
docker run --rm -it -v $(pwd):/dino dino make -j
```


## Contributing
PRs are welcome. Please make sure that the ROM builds and matches successfully before submitting a non-draft PR; the CI system will also verify this.

Also please see our [contribution guide](./CONTRIBUTING.md) for more information on how this project works and the ways to contribute.
