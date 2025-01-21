# MSYS2 for Decomp Setup Guide
This decompilation makes use of [GNU GCC](https://gcc.gnu.org/), [GNU Binutils](https://www.gnu.org/software/binutils/) targeting MIPS, and various Linux tools typically provided by the `base-devel` (or similar) package. For Windows users, these can be installed through [MSYS2](https://www.msys2.org/) and its package manager. The exception to this is Binutils with a MIPS target, which is not provided by the MSYS2 package repository and instead must be compiled and installed manually.

## 1. Install and Run MSYS2
The MSYS2 installer can be found [here](https://www.msys2.org/#installation). It is not important where you install it to.

Once installed, open the MSYS2 terminal. The rest of this guide will be done from this terminal and within the MSYS2 drive.

## 2. Install GCC and base-devel
First, update preinstalled packages and the local package index (this can be skipped if already up to date):
```bash
pacman -Syu
```

Next, install both GCC and the `base-devel` package:
```bash
pacman -S base-devel gcc
```

## 3. Building and Installing Binutils
As mentioned above, the MSYS2 package repository does not provide a build of Binutils that targets MIPS, so we'll compile it manually.

First, download the latest version of Binutils and extract it. You can [find the builds here](https://www.gnu.org/software/binutils/).
```bash
# Example (replace this with the latest version!)
wget "https://ftp.gnu.org/gnu/binutils/binutils-2.43.1.tar.gz"
tar -xvzf binutils-2.43.1.tar.gz
cd binutils-2.43.1
```

Next, configure the build to target `mips-linux-gnu` by running the following **within the extracted directory**:
```bash
./configure --target mips-linux-gnu --disable-multilib
```

Now it can be built:
```bash
# Set the value of j to the number of logical processors your
# cpu has. This is an example for a 16-core processor. Running with
# -j will run the build multithreaded and save a lot of time.
make -j16
```

Finally, install the built executables. This will make each program available under the MSYS2 `/usr/bin` directory with a prefix of `mips-linux-gnu` (e.g. `mips-linux-gnu-ld.exe`).
```bash
make install
```

## 4. Add MSYS2 /usr/bin to your Windows $PATH
The decomp project will not detect your MSYS2 installation and instead assumes that all programs installed above are available on your path. This can be done by simply adding the MSYS2 `/usr/bin` directory to your Windows PATH environment variable. The exact directory path will depend on where you installed MSYS2 to, but the default will be at `C:\msys64\usr\bin`.

## 5. Done
That's everything that must be installed through MSYS2 to run this decomp on Windows! Feel free to delete the downloaded Binutils archive and extracted directory the build was ran from, they are no longer needed.
