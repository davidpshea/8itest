# README #

### How do I get set up? ###

## Prerequsites
* QT installed.
* Cmake installed.
* Git installed

## Install
* Clone the remote git repository, this should also give you the master branch.

  `git clone https://github.com/davidpshea/8itest.git`


* Switch to the project directory.

  `cd 8itest`

* If not already done, let cmake know where the QT cmake package configuration is by setting the
environment variable `CMAKE_PREFIX_PATH`
For instance, for my Mac, this would be

  `export CMAKE_PREFIX_PATH=/Users/davidshea/source/qt/5.15.0/clang_64`

* Initialise the build using cmake

  `cmake . -B build`

* Build the app, for example, this will build the release version.

  `cmake --build build --config release`

* You should then have an executable `8i` in the build directory. You should run this in the `8itest` directory (where the source files are) so the app knows where the background image is, as it looks for the folder `givenfiles`

  `./build/8i`

## Example Output
! [COMING SOON](./givenfiles/Barbie.png "Title")
