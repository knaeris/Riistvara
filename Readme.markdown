# I237 2018 fall semester lab project template

## Introduction

This is a Microchip ATmega2560 lab project template for I237-2018 fall semester. All programs run examples are meant to be run in project directory.

## Repository structure and directories

Repository structure is inline with `Makefile`. Changes to repository structure require `Makefile` modifications.

Project structure:

`bin/` - built usercode (firmware)

`doc/` - documentation

`lib/` - libraries (third party components)

`src/` - project source code

`tooling` - scripts, tools and so on.

Empty directories contain file `.gitkeep`. Do not remove those files. Git can not track empty directories.

`.gitignore` ignores `astyle` formatter changed source code files, `bin` directory, C object files and other intermediate files. Also common ignored files for editors and operating systems are ignored based on [GitHub templates](https://github.com/github/gitignore).


## Toolchain set-up

### Needed programs to compile and deploy the firmware project

```
avr-binutils
avr-gcc
avrdude
GNU/Make
```
[AVR Libc](https://www.nongnu.org/avr-libc/) version 2.0.0 shall be used. See [AVR Libc Toolchain Overview](https://www.nongnu.org/avr-libc/user-manual/overview.html) for tools descriptions and purpose.

In addition following programs are required to develop and test lab software project.

```
gedit, including gedit-plugins gedit-developer-plugins gedit-source-code-browser-plugin
git
astyle
GNU screen
```

Toolchain installation for Ubuntu includes those. 

### Ubuntu 18.04

Use apt or any other package manager of your choice.

```
$ sudo apt-get install binutils-avr gcc-avr avr-libc avrdude make \
> gedit gedit-common gedit-developer-plugins gedit-plugins \
> gedit-source-code-browser-plugin git astyle screen
```

**NB! in order to deploy(upload) firmware to the microcontroller your user shall be part of dialout group.**

To add your user:

```
$ sudo usermod -a -G dialout $USER
```
Logout is eventualy required in desktop environment.


### MacOS 10.13

Most convenient way is to use [Homebrew](https://brew.sh) package manager. Alternative could be compiling toolchain from source.

**NB! Command Line Tools for Xcode are eventually required** Easiest way to install those is to install Xcode from App store. To only install minimum of required tools, see instructions in [homebrew-avr Installing homebrew-avr formulae](https://github.com/osx-cross/homebrew-avr#installing-homebrew-avr-formulae).

1. Install Homebrew according to instructions provided in project homepage.

    ```
    $ /usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
    ```

2. Create avr-cross tap

    See tap project [homebrew-avr](https://github.com/osx-cross/homebrew-avr) for more detailed instructions and clarifications.

    ```
    $ brew tap osx-cross/avr
    ```

3. Install compiler

    ```
    $ brew install avr-gcc
    ```

    **NB! avr-binutils and avr-libc are installed via dependencies. There is no need to install those separately.**


4. Install avrdude

    ```
    $ brew install avrdude
    ```

5. Install astyle

    ```
    $ brew install astyle
    ```

### Windows

**TODO**

### Others and compiling toolchain from source

See [AVR Libc Building and Installing the GNU Tool Chain](https://www.nongnu.org/avr-libc/user-manual/install_tools.html).

**NB! Instructions in Building the Toolchain for Windows are outdated and applicable only for historic Windows operating systems.**

## Make targets

Following targets are in use:

- default target (run make without target) - build project. Firmware files are built into the directory `bin`.

    For example:

    ```
    $ make
    ```

- `clean` target - deletes built firmware and object files.

    For example:

    ```
    $ make clean
    ```

- `dist-clean` - in addition to clean code formatter preserved original files are deleted.

- `install` - uploads built firmware via `avrdude`.

    **NB! default device file is /dev/ttyACM0. Use environment variable `ARDUINO` to define other device file.

    For example:

    ```
    $ export ARDUINO=/dev/ttyACM1
    ```

- `erase` - Uploads "blank" firmware in `tooling/atemega2560-blank-user-code/bin/atmega2560-user-code-blank.ihx` to the device via `avrdude` See Readme.txt for details.

- `format` - formats C source files in `src/` directory via script in `format-code.sh`.

- `size` - prints user code size via `avr-size` in AVR format.
