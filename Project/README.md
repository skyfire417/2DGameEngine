# Instructions

## Build Game

Build on macOS:

- Run Project/Game/build.sh to build the game.
- Make sure SDL2 frameworks are not installed on system, or build willfail. SDL2 will be installed 
by build script using HomeBrew.
- Run either 'build.sh' (debug) or 'build.sh -s' (release). 
  - Both generate executable(s) in 'build' directory, while the latteralso copies the main executable to 
  'GameBuild' directory.
- Run 'build.sh -c': clean (all cmake-generated files) before building. If you find any problems when building, try adding this option first
 
 ## Run Game
 
 - Run executable Project/Game/build/main for debug version of our game.
 - Run executable Project/Game/GameBuild/main for release version of our game.
 
 ## Run Game Editor
 
 - install python3 in Mac
 - pip install pyqt5
 - Go to Project/Editor
 - Run `python GameEditor.py`
