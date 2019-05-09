#!/usr/bin/env bash
PROG="$(basename "$0")"
## Parse options
USAGE="Usage: $PROG [-h|--help] [-s] [-c] DIRECTORY
Build Breakout game with Debug build type. Currently works on macOS only.
...

Options:
-s
    Build with CMAKE_BUILD_TYPE set to Release, and put executable in
    GameBuild/ directory
-c  Clean before build
-d  Generate documentation without building
-h, --help
    Display this help and exit
...
"
OPTIONS=:hscd

if [[ "$OSTYPE" != "darwin"* ]]; then
    >&2 echo "Not on macOS. Exited."
    exit 1
fi

BUILD_TYPE="Debug"
GEN_DOC="no"
while getopts "$OPTIONS" FLAG; do
  case $FLAG in
    h)
      echo "$USAGE"
      exit 0
      ;;
    s)
      BUILD_TYPE="Release"
      ;;
    c)
      CLEAN="yes"
      ;;
    d)
      GEN_DOC="yes"
      ;;
    ?) # unrecognized option - show help
      echo -e \\n"Option -$OPTARG not allowed."
      echo "$USAGE"
      exit 1
      ;;
  esac
done

if ! command -v brew >/dev/null 2>&1; then
    echo "HomeBrew not found. Installing..."
    xcode-select --install
    /usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
fi

echo "Installing required system packages..."
brew bundle || \
    { echo >&2 "Error occured when installing packages using HomeBrew bundle. Quit." && exit 1; }
if ! command -v git >/dev/null 2>&1; then
    brew install git || { echo >&2 "Error occured when installing git using HomeBrew. Quit." && exit 1; }
fi

script_dir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd "$script_dir"

## Clean if specified
if [ "$CLEAN" = "yes" ]; then
    rm -rf build/ lib/ temp/
fi

## Generate cmake files
[[ -d build ]] || mkdir build
cmake -Bbuild -D CMAKE_BUILD_TYPE="$BUILD_TYPE" . || \
    { echo >&2 "Error occured when configuring. Quit." && exit 1; }

## Generate documentation if specified
if [ "$GEN_DOC" = "yes" ]; then
    cmake --build build --target doxygen || \
        { echo >&2 "Error occurred when generating doc. Quit." && exit 1; }
else
    ## Begin building
    cmake --build build -j "$(sysctl -n hw.ncpu)" || \
        { echo >&2 "Error occured when building. Quit" && exit 1; }
    if [ "$BUILD_TYPE" = "Debug" ]; then
        echo "Built Debug version. You can now run executables in build directory."
    elif [ "$BUILD_TYPE" = "Release" ]; then
        echo "Built Release version. You can now run the game in GameBuild directory."
    fi
fi

exit 0
