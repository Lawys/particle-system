#include "Program.hpp"

Program::Program()
{   
    SDL_GL();
    GLEW();
    shader.Create();
    kernel.Create();
    event.Create(window, &shader, &kernel, windowWidth, windowHeight);   
    GetInformations();
    
    DataStart();
    while (event.isRunning)
    {
        event.FrameRate();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        shader.setMat4("view", LookAt(event.cameraPos, (event.cameraPos + event.cameraFront), event.cameraUp));

        SetColors();
        SetOpacity();

        glDrawArrays(GL_POINTS, 0, kernel.maxParticles);
        
        SDL_GL_SwapWindow(window);
        glFlush();

        kernel.Function(kernel.data[event.selectedFunction]);
        event.Run();
    }
}

void Program::coutError(const unsigned int type, const std::string& string, cl_uint error, const bool showMessage) {
    if (type == GL) {
        if (glGetError() != GL_NO_ERROR)
            throw std::runtime_error("[ERROR] " + string + " " + std::to_string(glGetError()));
        else if (showMessage)
            std::cout << "[OK] " << string << "\n";
    }
    else if (type == SDL) {
        std::string errorMessage = SDL_GetError();
        if (errorMessage.size() != 0)
            throw std::runtime_error("[ERROR] " + string + " " + errorMessage);
        else if (showMessage)
            std::cout << "[OK] " << string << "\n";
    }
}

void Program::SDL_GL() {
    SDL_Init(SDL_INIT_EVERYTHING);
    coutError(SDL, "SDL_Init");
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    coutError(SDL, "SDL_GL_SetAttribute");
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    coutError(SDL, "SDL_GL_SetAttribute");
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    coutError(SDL, "SDL_GL_SetAttribute");
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    coutError(SDL, "SDL_GL_SetAttribute");
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24); /*32 not working with glew*/
    coutError(SDL, "SDL_GL_SetAttribute");
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    coutError(SDL, "SDL_GL_SetAttribute");
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
    coutError(SDL, "SDL_GL_SetAttribute");
    window = SDL_CreateWindow("An SDL2 window",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        windowWidth, windowHeight,
        SDL_WINDOW_OPENGL /*| SDL_WINDOW_FULLSCREEN_DESKTOP*/);
    coutError(SDL, "SDL_CreateWindow");
    GLcontext = SDL_GL_CreateContext(window);
    coutError(SDL, "SDL_GL_CreateContext");
    SDL_GL_SetSwapInterval(SDL_TRUE);
    coutError(SDL, "SDL_GL_SetSwapInterval");
    glEnable(GL_DEPTH_TEST);
    coutError(GL, "glEnable");
    glEnable(GL_BLEND);
    coutError(GL, "glEnable");
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    coutError(GL, "glBlendFunc");
}

void Program::GLEW() {
    glewExperimental = GL_TRUE;
    if (GLenum error = glewInit() != GLEW_OK || !GLEW_VERSION_2_1)
        throw std::runtime_error("[ERROR] glewInit " + (std::string)((char*)glewGetErrorString(error)));
}

void Program::DataStart()
{
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    SDL_WarpMouseInWindow(window, windowHalfWidth, windowHalfHeight);
    SDL_GetRelativeMouseState(nullptr, nullptr);
    SDL_GetRelativeMouseState(nullptr, nullptr);
    SDL_WarpMouseInWindow(window, mouseX, mouseY);
    shader.setMat4("projection", Perspective(45.0f, (float)windowWidth / (float)windowHeight, 0.1f, 1000000000.0f));
    shader.setVec3("gPoint", { 0.0f, 0.0f, 1000.0f });
    //kernel.Function(kernel.data[KERNEL_START]);
    event.FrameRate();
}

void Program::GetInformations() {
    std::string string;
    cl_uint value;
    std::vector<size_t> valueList;

    const std::vector<std::string> pANames = { "Name", "Vendor", "Version", "Profile" }; //, "Extensions" };
    const cl_platform_info platformAttribute[] = { CL_PLATFORM_NAME, CL_PLATFORM_VENDOR, CL_PLATFORM_VERSION, CL_PLATFORM_PROFILE }; //, CL_PLATFORM_EXTENSIONS };

    const unsigned int dAReturnType[] = { 0, 0, 0, 0, 1, 2, 2, 1 };
    const std::vector<std::string> dANames = { "Name", "Hardware version", "Software version", "OpenCL C version", "Parallel compute units", "Maximum work item sizes", "Maximum work group size", "Maximum work item dimensions" };
    const cl_device_info deviceAttribute[] = { CL_DEVICE_NAME, CL_DEVICE_VERSION, CL_DRIVER_VERSION, CL_DEVICE_OPENCL_C_VERSION, CL_DEVICE_MAX_COMPUTE_UNITS, CL_DEVICE_MAX_WORK_ITEM_SIZES, CL_DEVICE_MAX_WORK_GROUP_SIZE, CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS };

    size_t pACount = pANames.size(), dACount = dANames.size();
    std::cout << "Platforms available: " << kernel.platforms.size() << "\n" << "\n";
    for (size_t i = 0; i < kernel.platforms.size(); i++) { // platforms
        std::cout << " > Platform" << "\n";
        for (size_t k = 0; k < pACount; k++)
            kernel.platforms[i].getInfo(platformAttribute[k], &string),
            std::cout << "   " << pANames[k] << ": " << string << "\n";
        for (size_t j = 0; j < kernel.devices[i].size(); j++) { // devices
            std::cout << "   > Device" << "\n";
            for (size_t k = 0; k < dACount; k++) {
                if (dAReturnType[k] == 0)
                    kernel.devices[i][j].getInfo(deviceAttribute[k], &string),
                    std::cout << "     " << dANames[k] << ": " << string << "\n";
                else if (dAReturnType[k] == 1)
                    kernel.devices[i][j].getInfo(deviceAttribute[k], &value),
                    std::cout << "     " << dANames[k] << ": " << value << "\n";
                else if (dAReturnType[k] == 2) {
                    kernel.devices[i][j].getInfo(deviceAttribute[k], &valueList);
                    std::cout << "     " << dANames[k] << ": ";
                    for (size_t v : valueList) std::cout << v << " ";
                    std::cout << "\n";
                }
            }
        } std::cout << "\n";
    }
    SDL_version v; SDL_GetVersion(&v);
    std::cout << "Using SDL version: " << (int)v.major << "." << (int)v.minor << " " << (int)v.patch << "\n";
    std::cout << "Using OpenGL version: " << glGetString(GL_VERSION) << "\n";
    std::cout << "Using Glew version: " << glewGetString(GLEW_VERSION) << "\n";
    std::cout << "Using platform: " << kernel.default_platform.getInfo<CL_PLATFORM_NAME>() << "\n";
    std::cout << "Using device: " << kernel.default_device.getInfo<CL_DEVICE_NAME>() << "\n";
    std::cout << "Using particles: " << kernel.numParticles << "\n" << "\n";
}

void Program::SetColors()
{
    static vec3 dColor = { 0.0f, 200.0f, 400.0f };
    static vec3 iColor = { 1.0f, 1.0f, 1.0f };

    dColor.x += iColor.x;
    if (dColor.x >= 400.0f) iColor.x = -1.0f;
    else if (dColor.x <= 0.0f) iColor.x = 1.0f;

    dColor.y += iColor.y;
    if (dColor.y >= 400.0f) iColor.y = -1.0f;
    else if (dColor.y <= 0.0f) iColor.y = 1.0f;

    dColor.z += iColor.z;
    if (dColor.z >= 400.0f) iColor.z = -1.0f;
    else if (dColor.z <= 0.0f) iColor.z = 1.0f;

    shader.setVec3("dColor", dColor);
}

void Program::SetOpacity()
{
    static float dOpacity = 200.0f;
    static float iOpacity = 1.0f;

    dOpacity += 1.0f * iOpacity;
    if (dOpacity >= 200.0f) iOpacity = -1.0f;
    else if (dOpacity <= 0.0f) iOpacity = 1.0f;

    shader.setFloat("dOpacity", dOpacity);
}

