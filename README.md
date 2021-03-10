# Morph
Currently all that's implemented is the animation of a julia fractal and the static generation of a mandelbrot fractal. The colors are randomly selected.


## Dependencies:
SDL2, SDL2_image, g++


Ubuntu 20.04.1: `sudo apt-get install libsdl2-dev libsdl2-image-dev g++ git && git clone https://github.com/foogolplex/Morph`


## Compile:
`g++ -o main main.cpp emerge.cpp -lSDL2 -lSDL2_image`

## TODO:
Implement: multithreading, UI, colors that correspond with eachother and depth, effects


![Alt text](morph_screenshot.png?raw=true)
