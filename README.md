## Dependencies:
SDL2, SDL2_image, g++

Ubuntu 20.04.1: `sudo apt-get install libsdl2-dev libsdl2-image-dev g++ git`

## Download:
`cd ./download/location && git clone https://github.com/foogolplex/morph.git`

## Compile:
After verifying you are in the /morph/src/ directory: `g++ -o main main.cpp morph.cpp -lSDL2 -lSDL2_image`

## TODO:
Implement: multithreading, UI, colors that correspond with eachother and depth, effects

![Alt text](morph_screenshot.png?raw=true)
