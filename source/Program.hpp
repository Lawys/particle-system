#ifndef PROGRAM_H
#define PROGRAM_H

#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>
#include <cmath>

#include <GL/glew.h> 
#include <SDL2/SDL.h>

#include "Math.hpp"
#include "Shader.hpp"
#include "Kernel.hpp"
#include "Event.hpp"


class Program
{
public:
    Program();
    ~Program() {};
private:
    SDL_Window* window = nullptr;
    SDL_GLContext GLcontext;
    Shader shader;
    Kernel kernel;
    const int windowWidth = 1920;
    const int windowHeight = 1080;
    const int windowHalfWidth = windowWidth / 2;
    const int windowHalfHeight = windowHeight / 2;
    Event event;

    // error functions
    void coutError(const unsigned int type, const std::string& string, const cl_uint error = 0, const bool showMessage = true);
    // start functions
    void SDL_GL();
    void GLEW();
    void DataStart();
    void GetInformations();
    // loop functions
    void SetColors();
    void SetOpacity();
};

#endif