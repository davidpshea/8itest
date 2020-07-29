# README #

### How do I get set up? ###

## Prerequsites
* QT installed.
* Cmake installed.
* Git installed

## Install
* Clone the remote git repository,

  `git clone https://davidpshea@bitbucket.org/davidpshea/8itest`
* Checkout the master branch.

  `cd 8itest`
`git checkout master`
* If not already done, let cmake know where the QT cmake package configuration is by setting the
environment variable `CMAKE_PREFIX_PATH`
For instance, for my Mac, this would be

  `export CMAKE_PREFIX_PATH=/Users/davidshea/source/qt/5.15.0/clang_64`
* Initialise the build using cmake

  `cmake .`
* Build the app, for example, this will build the release version.

  `cmake --build . --config release`
* You should then have an executable `8i`

## Example Output
![alt text](./givenfiles/Barbie.png "Title")
