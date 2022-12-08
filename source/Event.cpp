#include "Event.hpp"

void Event::Run() {

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE))
        {
            Quit();
        }
        else if (event.type == SDL_MOUSEMOTION)
        {
            MouseMotion();
        }
        else if (event.type == SDL_MOUSEBUTTONDOWN)
        {
            if (event.button.button == SDL_BUTTON_RIGHT)
                MouseButtonDownRight();
            else if (event.button.button == SDL_BUTTON_LEFT)
                MouseButtonDownLeft();
        }
        else if (event.type == SDL_MOUSEBUTTONUP)
        {
            if (event.button.button == SDL_BUTTON_LEFT)
                MouseButtonUpLeft();
        }
        else if (event.type == SDL_MOUSEWHEEL)
        {
            if (event.wheel.y > 0)
                MouseWheelFront();
            else if (event.wheel.y < 0)
                MouseWheelBack();
        }
        else if (event.type == SDL_KEYDOWN)
            KeyDown();
    }
    if (selectedScript != NO_SCRIPT)
    {
        GravityPointToScript();
    }
    if (cameraController)
    {
        CameraControl();
        if (eventMouse)
            Mouse();
    }
    if (gravityPointToCursor)
    {
        GravityPointToCursor();
    }
}

void Event::Quit()
{
    isRunning = false;
}

void Event::MouseMotion()
{
    eventMouse = true;
}

void Event::MouseButtonDownRight()
{
    //recorder.insert(std::pair<double, void(Event::*)()>(timeDiff, Event::this));
    static int mouseX, mouseY;
    cameraController = !cameraController;
    if (cameraController)
        SDL_GetMouseState(&mouseX, &mouseY),
        SDL_WarpMouseInWindow(window, windowHalfWidth, windowHalfHeight),
        SDL_SetRelativeMouseMode(SDL_TRUE);
    else
        SDL_SetRelativeMouseMode(SDL_FALSE),
        SDL_WarpMouseInWindow(window, mouseX, mouseY);
}

void Event::MouseButtonDownLeft()
{
    gravityPointToCursor = true;
}

void Event::MouseButtonUpLeft()
{
    gravityPointToCursor = false;
}

void Event::MouseWheelFront()
{
    vec3 vector = gravityPoint - cameraPos;
    float distance = sqrt(vector * vector);
    if (distance < 8000.0f)
        gravityPoint = cameraPos + vector * 2.0f,
        kernel->data[selectedFunction].floats = { gravityPoint.x, gravityPoint.y, gravityPoint.z, gravityPower },
        shader->setVec3("gPoint", gravityPoint);
}

void Event::MouseWheelBack()
{
    vec3 vector = gravityPoint - cameraPos;
    float distance = sqrt(vector * vector);
    if (distance > 125.0f)
        gravityPoint = cameraPos + vector / 2.0f,
        kernel->data[selectedFunction].floats = { gravityPoint.x, gravityPoint.y, gravityPoint.z, gravityPower },
        shader->setVec3("gPoint", gravityPoint);
}

void Event::KeyDownSpace()
{
    if (statement[0] == NO_KERNEL)
        statement[0] = statement[1],
        selectedFunction = NO_KERNEL,
        statement[2] = statement[3],
        selectedScript = NO_SCRIPT;
    else
        selectedFunction = statement[0],
        statement[0] = NO_KERNEL,
        selectedScript = statement[2],
        statement[2] = NO_SCRIPT;
}

void Event::KeyDown1()
{
    statement[0] = NO_KERNEL;
    statement[1] = KERNEL_PARTICLES_TO_CUBE;
    statement[2] = NO_SCRIPT;
    statement[3] = NO_SCRIPT;
    selectedFunction = KERNEL_PARTICLES_TO_CUBE;
}

void Event::KeyDown2()
{
    statement[0] = NO_KERNEL;
    statement[1] = KERNEL_PARTICLES_TO_CUBE_FILLED;
    statement[2] = NO_SCRIPT;
    statement[3] = NO_SCRIPT;
    selectedFunction = KERNEL_PARTICLES_TO_CUBE_FILLED;
}

void Event::KeyDown3()
{
    statement[0] = NO_KERNEL;
    statement[1] = KERNEL_PARTICLES_TO_SPHERE;
    statement[2] = NO_SCRIPT;
    statement[3] = NO_SCRIPT;
    selectedFunction = KERNEL_PARTICLES_TO_SPHERE;
}

void Event::KeyDown4()
{
    statement[0] = NO_KERNEL;
    statement[1] = KERNEL_PARTICLES_TO_SPHERE_FILLED;
    statement[2] = NO_SCRIPT;
    statement[3] = NO_SCRIPT;
    selectedFunction = KERNEL_PARTICLES_TO_SPHERE_FILLED;
}

void Event::KeyDown5()
{
    kernel->Function(kernel->data[KERNEL_PARTICLES_CLEAN_TIMERS]);
    statement[0] = NO_KERNEL;
    statement[1] = KERNEL_PARTICLES_FOUNTAIN;
    statement[2] = NO_SCRIPT;
    statement[3] = NO_SCRIPT;
    selectedFunction = KERNEL_PARTICLES_FOUNTAIN;
}

void Event::KeyDown6()
{
    statement[0] = NO_KERNEL;
    statement[1] = KERNEL_PARTICLES_GRAVITY;
    statement[2] = NO_SCRIPT;
    statement[3] = NO_SCRIPT;
    selectedFunction = KERNEL_PARTICLES_GRAVITY;
}

void Event::KeyDownNumericKeyPadMultiply()
{
    if (kernel->maxParticles * 2 < kernel->numParticles)
        kernel->maxParticles *= 2;
}

void Event::KeyDownNumericKeyPadDivide()
{
    if (kernel->maxParticles / 2 > 1)
        kernel->maxParticles /= 2;
}

void Event::KeyDownNumericKeyPadPlus()
{
    if (gravityPower + 0.5f < 2.5f)
        gravityPower += 0.5f,
        kernel->data[selectedFunction].floats[3] = gravityPower;
}

void Event::KeyDownNumericKeyPadMinus()
{
    if (gravityPower - 0.5f > 0.0f)
        gravityPower -= 0.5f,
        kernel->data[selectedFunction].floats[3] = gravityPower;
}

void Event::KeyDownP()
{
    cameraPos = { 0.0f, 0.0f, 0.0f };
    cameraFront = { 0.0f, 0.0f, 1.0f };
    yaw = 90.0f;
    pitch = 0.0f;
}

void Event::KeyDownO()
{
    gravityPower = 1.0f;
    gravityPoint = { 0.0f, 0.0f, 1000.0f };
    kernel->data[selectedFunction].floats = { gravityPoint.x, gravityPoint.y, gravityPoint.z, gravityPower };
    shader->setVec3("gPoint", gravityPoint);
}

void Event::KeyDownR()
{
    statement[0] = NO_KERNEL;
    statement[1] = KERNEL_PARTICLES_TO_CUBE;
    selectedFunction = KERNEL_PARTICLES_TO_CUBE;
    selectedScript = NO_SCRIPT;
    cameraController = false;
    cameraPos = { 0.0f, 0.0f, 0.0f };
    cameraFront = { 0.0f, 0.0f, 1.0f };
    yaw = 90.0f;
    pitch = 0.0f;
    SDL_SetRelativeMouseMode(SDL_FALSE);
    gravityPower = 1.0f;
    gravityPoint = { 0.0f, 0.0f, 1000.0f };
    kernel->data[selectedFunction].floats = { gravityPoint.x, gravityPoint.y, gravityPoint.z, gravityPower };
    shader->setVec3("gPoint", gravityPoint);
}

void Event::KeyDownN()
{
    statement[0] = NO_KERNEL;
    statement[1] = KERNEL_PARTICLES_FOUNTAIN;
    statement[2] = NO_SCRIPT;
    statement[3] = SCRIPT_FOUNTAIN_REBOUND;
    selectedFunction = KERNEL_PARTICLES_FOUNTAIN;
    selectedScript = SCRIPT_FOUNTAIN_REBOUND;
    cameraPos = { 0.0f, 0.0f, 0.0f };
    cameraFront = { 0.0f, 0.0f, 1.0f };
    yaw = 90.0f;
    pitch = 0.0f;
    gravityPower = 0.5f;
    kernel->data[selectedFunction].floats = { gravityPower };
}



void Event::KeyDown()
{
    // KERNEL FUNCTION
    if (event.key.keysym.scancode == SDL_SCANCODE_SPACE)
        KeyDownSpace();
    else if (event.key.keysym.scancode == SDL_SCANCODE_1)
        KeyDown1();
    else if (event.key.keysym.scancode == SDL_SCANCODE_2)
        KeyDown2();
    else if (event.key.keysym.scancode == SDL_SCANCODE_3)
        KeyDown3();
    else if (event.key.keysym.scancode == SDL_SCANCODE_4)
        KeyDown4();
    else if (event.key.keysym.scancode == SDL_SCANCODE_5)
        KeyDown5();
    else if (event.key.keysym.scancode == SDL_SCANCODE_6)
        KeyDown6();
    // KERNEL PARAMS
    else if (event.key.keysym.scancode == SDL_SCANCODE_KP_MULTIPLY)
        KeyDownNumericKeyPadMultiply();
    else if (event.key.keysym.scancode == SDL_SCANCODE_KP_DIVIDE)
        KeyDownNumericKeyPadDivide();
    else if (event.key.keysym.scancode == SDL_SCANCODE_KP_PLUS)
        KeyDownNumericKeyPadPlus();
    else if (event.key.keysym.scancode == SDL_SCANCODE_KP_MINUS)
        KeyDownNumericKeyPadMinus();
    // RESET CAMERA
    else if (event.key.keysym.scancode == SDL_SCANCODE_P)
        KeyDownP();
    // RESET GRAVITY
    else if (event.key.keysym.scancode == SDL_SCANCODE_O)
        KeyDownO();
    // RESET ALL
    else if (event.key.keysym.scancode == SDL_SCANCODE_R)
        KeyDownR();
    // SCRIPT STARTER
    else if (event.key.keysym.scancode == SDL_SCANCODE_N)
        KeyDownN();
}

void Event::CameraControl()
{
    const Uint8* keyStates = SDL_GetKeyboardState(NULL);
    const float value = (float)timeDiff * 1000.0f;
    const vec3 cameraFrontCrossUpNormalize = (((cameraFront ^ cameraUp)).Normalize());

    if (keyStates[SDL_SCANCODE_W]) this->cameraPos = (cameraPos + (cameraFront * value)); // front
    if (keyStates[SDL_SCANCODE_S]) this->cameraPos = (cameraPos - (cameraFront * value)); // back
    if (keyStates[SDL_SCANCODE_A]) this->cameraPos = (cameraPos + cameraFrontCrossUpNormalize * value); // left
    if (keyStates[SDL_SCANCODE_D]) this->cameraPos = (cameraPos - cameraFrontCrossUpNormalize * value); // right
    if (keyStates[SDL_SCANCODE_Q]) this->cameraPos = (cameraPos + (cameraUp * value)); // top
    if (keyStates[SDL_SCANCODE_E]) this->cameraPos = (cameraPos - (cameraUp * value)); // bottom
}

void Event::GravityPointToCursor()
{
    static const float coef = tanf(RADIAN(22.5f)); // half angle of view (45.0)
    int mouseX, mouseY; SDL_GetMouseState(&mouseX, &mouseY);
    float X = (float)(mouseX - windowHalfWidth) / windowHalfHeight * coef;
    float Y = -(float)(mouseY - windowHalfHeight) / windowHalfHeight * coef;

    vec3 cameraRight = (cameraUp ^ cameraFront).Normalize();
    vec3 cameraTop = (cameraFront ^ cameraRight).Normalize();
    vec3 directionByCursor = cameraFront + cameraTop * Y + cameraRight * X;
    gravityPoint = GetLinePlaneIntersection(cameraPos, directionByCursor, gravityPoint, cameraFront);
    kernel->data[selectedFunction].floats = { gravityPoint.x, gravityPoint.y, gravityPoint.z, gravityPower };
    shader->setVec3("gPoint", gravityPoint);
}

void Event::GravityPointToScript()
{
    if (selectedScript == SCRIPT_FOUNTAIN_REBOUND)
    {
        static float timer = 0;
        if ((int)timer == 0) {
            selectedFunction = KERNEL_PARTICLES_TO_SPHERE_FILLED;
            timer = 1.5f;
        }
        if ((int)timer == 2) {
            selectedFunction = KERNEL_PARTICLES_FOUNTAIN;
            timer = 2.5f;
        }
        else if ((int)timer == 3) {
            selectedFunction = KERNEL_PARTICLES_GRAVITY;
            timer = 4.0f;
        }
        else if ((int)timer == 6) {
            timer = 0.0f;
        }
        timer += timeDiff;

        static float a = 0; a += 4;
        float t = cos(RADIAN(a / 1.5f)) * 385.0f;
        float x = cos(RADIAN(a)) - sin(RADIAN(a));
        float y = sin(RADIAN(a)) + cos(RADIAN(a));
        gravityPoint.x = x * 200.0f + t;
        gravityPoint.y = y * 200.0f;

        kernel->data[selectedFunction].floats = { gravityPoint.x, gravityPoint.y, gravityPoint.z, gravityPower };

        shader->setVec3("gPoint", { gravityPoint.x, gravityPoint.y, this->gravityPoint.z });
    }
}


void Event::Mouse()
{
    static int prevx = 0, prevy = 0;

    int x, y; SDL_GetRelativeMouseState(&x, &y);

    if (x != 0 || y != 0)
    {
        static double xoffset = -(x + prevx), yoffset = -(y + prevy);

        yaw += (float)(xoffset = (xoffset + x + prevx) * sensitivity);
        pitch += (float)(yoffset = (yoffset + y + prevy) * sensitivity);
        if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;

        vec3 direction;
        direction.x = (float)(cos(RADIAN((double)yaw)) * cos(RADIAN((double)pitch)));
        direction.y = (float)(sin(RADIAN((double)pitch)));
        direction.z = (float)(sin(RADIAN((double)yaw)) * cos(RADIAN((double)pitch)));
        cameraFront = direction.Normalize();

        SDL_WarpMouseInWindow(window, windowHalfWidth, windowHalfHeight);
    }
    prevx = x, prevy = y;
}

void Event::FrameRate()
{
    static double prevTime = 0.0;
    static double crntTime = 0.0;

    crntTime = static_cast<double>(SDL_GetTicks() / 1000.0);
    timeDiff = crntTime - prevTime;
    prevTime = crntTime;
    SDL_SetWindowTitle(window, ("[" + std::to_string(1.0 / timeDiff) + "]").c_str());
}

