#ifndef EVENT_H
#define EVENT_H

#include "Shader.hpp"
#include "Kernel.hpp"
#include <map>

class Event
{
public:
    SDL_Event event = {};
    Shader* shader;
    Kernel* kernel;
    SDL_Window* window;

    std::map<double, void(Event::*)()> recorder;

    unsigned int selectedFunction = KERNEL_PARTICLES_TO_CUBE;
    unsigned int selectedScript = NO_SCRIPT;

    vec3 cameraPos = { 0.0f, 0.0f, 0.0f };
    vec3 cameraFront = { 0.0f, 0.0f, 1.0f };
    const vec3 cameraUp = { 0.0f, 1.0f, 0.0f };
    float yaw = 90.0f;
    float pitch = 0.0f;
    const float sensitivity = -0.1f;

    float gravityPower = 1.0f;
    vec3 gravityPoint = cameraFront * 1000.0f;

    bool cameraController = false;
    bool gravityPointToCursor = false; 
    bool eventMouse = false;

    int windowWidth = 0;
    int windowHeight = 0;
    int windowHalfWidth = 0;
    int windowHalfHeight = 0;

    bool isRunning = true;
    double timeDiff = 0.0;

    int statement[4] = { NO_KERNEL, NO_KERNEL, NO_SCRIPT, NO_SCRIPT };

	Event() {};
	~Event() {};

    void Create(SDL_Window* window, Shader* shader, Kernel* kernel, const int windowWidth, const int windowHeight)
    {
        this->window = window;
        this->shader = shader;
        this->kernel = kernel;

        this->windowWidth = windowWidth;
        this->windowHeight = windowHeight;
        this->windowHalfWidth = windowWidth / 2;
        this->windowHalfHeight = windowHeight / 2;
    };

    void Run();
    void FrameRate();
private:
    void Quit();

    void MouseMotion();
    void MouseButtonDownRight();
    void MouseButtonDownLeft();
    void MouseButtonUpLeft();
    void MouseWheelFront();
    void MouseWheelBack();

    void KeyDown();
    void KeyDownSpace();
    void KeyDown1();
    void KeyDown2();
    void KeyDown3();
    void KeyDown4();
    void KeyDown5();
    void KeyDown6();
    void KeyDownNumericKeyPadMultiply();
    void KeyDownNumericKeyPadDivide();
    void KeyDownNumericKeyPadPlus();
    void KeyDownNumericKeyPadMinus();
    void KeyDownP();
    void KeyDownO();
    void KeyDownR();
    void KeyDownN();

    void CameraControl();
    void GravityPointToCursor();
    void GravityPointToScript();
    void Mouse();





};

#endif

