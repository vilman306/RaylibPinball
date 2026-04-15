![Gameplay](readmeContent/Gameplay.gif)

A simple pinball game with its own physics engine, built in C++ with raylib.

[Full gameplay](https://www.reddit.com/r/raylib/comments/1s805cm/i_made_a_simple_pinball_game_with_raylib/?utm_source=share&utm_medium=web3x&utm_name=web3xcss&utm_term=1&utm_content=share_button)

# Features
* A custom physics engine
* A level prototype
* Flippers, bumpers, walls, ball
* Score
* Post-processing (bloom)
* Debug mode (enable it by uncommenting //#define DEBUG in Game.cpp)

# Controls
* Left/right arrow - rotate left/right flipper
* Spacebar - launch ball

# Build instructions (Linux)
1. git clone https://github.com/vilman306/RaylibPinball.git
2. cd RaylibPinball
3. mkdir build
4. cd build
5. cmake ..
6. make
7. ./RaylibPinball
