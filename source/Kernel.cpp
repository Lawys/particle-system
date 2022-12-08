#include "Kernel.hpp"

void Kernel::Create()
{
    DefinePlatformDeviceParticles();
    CreateCLContext();
    AllocateInterpolarity();
    AllocateCL();
    BuildCLProgram();
    CreateOpenCLFunctionData();
}

std::string const Kernel::getErrorString(cl_int error)
{
    switch (error) {
        // run-time and JIT compiler errors
    case 0: return "CL_SUCCESS";
    case -1: return "CL_DEVICE_NOT_FOUND";
    case -2: return "CL_DEVICE_NOT_AVAILABLE";
    case -3: return "CL_COMPILER_NOT_AVAILABLE";
    case -4: return "CL_MEM_OBJECT_ALLOCATION_FAILURE";
    case -5: return "CL_OUT_OF_RESOURCES";
    case -6: return "CL_OUT_OF_HOST_MEMORY";
    case -7: return "CL_PROFILING_INFO_NOT_AVAILABLE";
    case -8: return "CL_MEM_COPY_OVERLAP";
    case -9: return "CL_IMAGE_FORMAT_MISMATCH";
    case -10: return "CL_IMAGE_FORMAT_NOT_SUPPORTED";
    case -11: return "CL_BUILD_PROGRAM_FAILURE";
    case -12: return "CL_MAP_FAILURE";
    case -13: return "CL_MISALIGNED_SUB_BUFFER_OFFSET";
    case -14: return "CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST";
    case -15: return "CL_COMPILE_PROGRAM_FAILURE";
    case -16: return "CL_LINKER_NOT_AVAILABLE";
    case -17: return "CL_LINK_PROGRAM_FAILURE";
    case -18: return "CL_DEVICE_PARTITION_FAILED";
    case -19: return "CL_KERNEL_ARG_INFO_NOT_AVAILABLE";

        // compile-time errors
    case -30: return "CL_INVALID_VALUE";
    case -31: return "CL_INVALID_DEVICE_TYPE";
    case -32: return "CL_INVALID_PLATFORM";
    case -33: return "CL_INVALID_DEVICE";
    case -34: return "CL_INVALID_CONTEXT";
    case -35: return "CL_INVALID_QUEUE_PROPERTIES";
    case -36: return "CL_INVALID_COMMAND_QUEUE";
    case -37: return "CL_INVALID_HOST_PTR";
    case -38: return "CL_INVALID_MEM_OBJECT";
    case -39: return "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR";
    case -40: return "CL_INVALID_IMAGE_SIZE";
    case -41: return "CL_INVALID_SAMPLER";
    case -42: return "CL_INVALID_BINARY";
    case -43: return "CL_INVALID_BUILD_OPTIONS";
    case -44: return "CL_INVALID_PROGRAM";
    case -45: return "CL_INVALID_PROGRAM_EXECUTABLE";
    case -46: return "CL_INVALID_KERNEL_NAME";
    case -47: return "CL_INVALID_KERNEL_DEFINITION";
    case -48: return "CL_INVALID_KERNEL";
    case -49: return "CL_INVALID_ARG_INDEX";
    case -50: return "CL_INVALID_ARG_VALUE";
    case -51: return "CL_INVALID_ARG_SIZE";
    case -52: return "CL_INVALID_KERNEL_ARGS";
    case -53: return "CL_INVALID_WORK_DIMENSION";
    case -54: return "CL_INVALID_WORK_GROUP_SIZE";
    case -55: return "CL_INVALID_WORK_ITEM_SIZE";
    case -56: return "CL_INVALID_GLOBAL_OFFSET";
    case -57: return "CL_INVALID_EVENT_WAIT_LIST";
    case -58: return "CL_INVALID_EVENT";
    case -59: return "CL_INVALID_OPERATION";
    case -60: return "CL_INVALID_GL_OBJECT";
    case -61: return "CL_INVALID_BUFFER_SIZE";
    case -62: return "CL_INVALID_MIP_LEVEL";
    case -63: return "CL_INVALID_GLOBAL_WORK_SIZE";
    case -64: return "CL_INVALID_PROPERTY";
    case -65: return "CL_INVALID_IMAGE_DESCRIPTOR";
    case -66: return "CL_INVALID_COMPILER_OPTIONS";
    case -67: return "CL_INVALID_LINKER_OPTIONS";
    case -68: return "CL_INVALID_DEVICE_PARTITION_COUNT";

        // extension errors
    case -1000: return "CL_INVALID_GL_SHAREGROUP_REFERENCE_KHR";
    case -1001: return "CL_PLATFORM_NOT_FOUND_KHR";
    case -1002: return "CL_INVALID_D3D10_DEVICE_KHR";
    case -1003: return "CL_INVALID_D3D10_RESOURCE_KHR";
    case -1004: return "CL_D3D10_RESOURCE_ALREADY_ACQUIRED_KHR";
    case -1005: return "CL_D3D10_RESOURCE_NOT_ACQUIRED_KHR";
    default: return "Unknown OpenCL error";
    }
}

void Kernel::Function(const struct KernelFunctionData& data)
{
    if (data.kernel != -1) {
        cl_int error = 0, argCount = 0;
        static std::vector<cl::Memory> memories;
        static cl::Event event;
        static cl::CommandQueue cmdQueue = cl::CommandQueue(context, default_device, 0, &error);
        coutError(CL, "CommandQueue", error, true);
        if (data.interOn.size())
        {
            for (auto object : data.interOn)
            {
                error = kernels[data.kernel].setArg(argCount++, sizeof(cl::BufferGL), &deviceInterObject[object]);
                coutError(CL, "setArg", error, true);
                memories.push_back(deviceInterObject[object]);
            }
        }
        if (data.interOff.size())
        {
            for (auto object : data.interOff)
            {
                error = kernels[data.kernel].setArg(argCount++, sizeof(cl::Buffer), &deviceObject[object]);
                coutError(CL, "setArg", error, true);
            }
        }
        if (data.floats.size())
        {
            for (auto object : data.floats)
            {
                error = kernels[data.kernel].setArg(argCount++, sizeof(float), &object);
                coutError(CL, "setArg", error, true);
            }
        } 
        glFinish();
        if (data.interOn.size())
        {   
            error = cmdQueue.enqueueAcquireGLObjects(&memories, NULL, &event);
            coutError(CL, "enqueueAcquireGLObjects", error, true);
            error = event.wait();
            coutError(CL, "wait", error, true);
        }
        error = cmdQueue.enqueueNDRangeKernel(kernels[data.kernel], 0, globalWS, cl::NullRange, NULL, &event);
        coutError(CL, "enqueueNDRangeKernel [" + std::to_string(data.kernel) + "] ", error, true);
        error = event.wait();
        coutError(CL, "wait", error, true);
        if (data.interOn.size())
        {
            error = cmdQueue.enqueueReleaseGLObjects(&memories, NULL, &event);
            coutError(CL, "enqueueReleaseGLObjects", error, true);
            error = event.wait();
            coutError(CL, "wait", error, true);
        }
        error = cmdQueue.finish();
        coutError(CL, "finish", error, true);
        glFinish();
    }
}

void Kernel::coutError(const unsigned int type, const std::string& string, cl_uint error, const bool showMessage)
{
    if (type == CL) {
        if (error != CL_SUCCESS)
            throw std::runtime_error("[ERROR] " + string + " " + getErrorString(error));
        else if (showMessage)
            std::cout << "[OK] " << string << "\n";
    }
    else if (type == GL) {
        if (glGetError() != GL_NO_ERROR)
            throw std::runtime_error("[ERROR] " + string + " " + std::to_string(glGetError()));
        else if (showMessage)   
            std::cout << "[OK] " << string << "\n";
    }
}

void Kernel::DefinePlatformDeviceParticles()
{
    cl::Platform::get(&platforms);
    if (platforms.size() == 0) throw std::runtime_error("[ERROR] no platforms");
    for (size_t i = 0; i < platforms.size(); i++) {
        devices.push_back(std::vector<cl::Device>());
        platforms[i].getDevices(CL_DEVICE_TYPE_ALL, &devices[i]);
        if (i == 0 && devices[i].size() == 0) throw std::runtime_error("[ERROR] no devise");
    }
    default_platform = platforms[0];
    default_device = devices[0][0];

    numParticles = 1024;
    globalWS = { numParticles, 1, 1 };
    
    std::cout << default_device.getInfo<CL_DEVICE_LOCAL_MEM_SIZE>() << std::endl;
    if (default_device.getInfo<CL_DEVICE_EXTENSIONS>().find("cl_khr_gl_sharing") == std::string::npos) {
        throw std::runtime_error("[ERROR] a requested extension is missing (cl_khr_gl_sharing)");
    }
    else std::cout << "[OK] device extension cl_khr_gl_sharing" << "\n";
    maxParticles = numParticles;
}

void Kernel::CreateCLContext()
{
    cl_int error = 0;
    cl_context_properties contextProperties[] = {
        CL_GL_CONTEXT_KHR, (cl_context_properties)glXGetCurrentContext(),
        CL_GLX_DISPLAY_KHR, (cl_context_properties)glXGetCurrentDisplay(),
        CL_CONTEXT_PLATFORM, (cl_context_properties)default_platform(),
        0
    };   
    context = cl::Context(default_device, contextProperties, nullptr, nullptr, &error);
    coutError(CL, "cl::Context", error);
}

void Kernel::AllocateInterpolarity()
{
    cl_int error = 0;
    hosts.resize(1);
    glGenBuffers(1, &hosts[0]);
    coutError(GL, "glGenBuffers");
    glBindBuffer(GL_ARRAY_BUFFER, hosts[0]);
    coutError(GL, "glBindBuffer");
    glBufferData(GL_ARRAY_BUFFER, 4 * maxParticles * sizeof(float), NULL, GL_DYNAMIC_DRAW);
    coutError(GL, "glGenBuffers");
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    //coutError(GL, "glBufferData");
    glVertexPointer(4, GL_FLOAT, 0, (void*)0);
    coutError(GL, "glVertexPointer");
    glEnableClientState(GL_VERTEX_ARRAY);
    coutError(GL, "glBinglEnableClientStatedBuffer");
    deviceInterObject.push_back(cl::BufferGL(context, CL_MEM_READ_WRITE, hosts[0], &error));
    coutError(CL, "cl::BufferGL", error);
}

void Kernel::AllocateCL()
{
    cl_int error = 0;
    deviceObject.push_back(cl::Buffer(context, CL_MEM_READ_WRITE, 4 * maxParticles * sizeof(float), 0, &error));
    coutError(CL, "cl::Buffer", error);
    deviceObject.push_back(cl::Buffer(context, CL_MEM_READ_WRITE, 1 * maxParticles * sizeof(float), 0, &error));
    coutError(CL, "cl::Buffer", error);
}

void Kernel::BuildCLProgram()
{
    cl_int error = 0;
    std::string kernelCode;
    try {
        std::ifstream kernelFile;
        kernelFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        kernelFile.open("./kernel/kernel.cl");
        std::stringstream kernelStream;
        kernelStream << kernelFile.rdbuf();
        kernelFile.close();
        kernelCode = kernelStream.str();
    }
    catch (std::ifstream::failure& e) {
        throw std::runtime_error("[ERROR] kernelCode " + (std::string)e.what());
    }
    cl::Program program = cl::Program(context, kernelCode, true, &error);
    coutError(CL, "cl::Program\n" + program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(default_device), error);
    error = program.createKernels(&kernels);
    coutError(CL, "createKernels", error);
}

void Kernel::CreateOpenCLFunctionData()
{
    data.push_back({ KERNEL_START, { GLOBAL_PARTICLE_POINTS_OBJECT } });
    data[KERNEL_START].floats = {0.0f, 0.0f, 1000.0f};

    data.push_back({ KERNEL_PARTICLES_TO_CUBE, { GLOBAL_PARTICLE_POINTS_OBJECT}, { GLOBAL_PARTICLE_VECTORS_OBJECT } });
    data[KERNEL_PARTICLES_TO_CUBE].floats = {0.0f, 0.0f, 1000.0f, 1.0f};

    data.push_back({ KERNEL_PARTICLES_TO_CUBE_FILLED, { GLOBAL_PARTICLE_POINTS_OBJECT}, { GLOBAL_PARTICLE_VECTORS_OBJECT } });
    data[KERNEL_PARTICLES_TO_CUBE_FILLED].floats = {0.0f, 0.0f, 1000.0f, 1.0f};

    data.push_back({ KERNEL_PARTICLES_TO_SPHERE, { GLOBAL_PARTICLE_POINTS_OBJECT}, { GLOBAL_PARTICLE_VECTORS_OBJECT } });
    data[KERNEL_PARTICLES_TO_SPHERE].floats = {0.0f, 0.0f, 1000.0f, 1.0f};

    data.push_back({ KERNEL_PARTICLES_TO_SPHERE_FILLED, { GLOBAL_PARTICLE_POINTS_OBJECT}, { GLOBAL_PARTICLE_VECTORS_OBJECT } });
    data[KERNEL_PARTICLES_TO_SPHERE_FILLED].floats = {0.0f, 0.0f, 1000.0f, 1.0f};

    data.push_back({ KERNEL_PARTICLES_CLEAN_TIMERS, { }, { GLOBAL_PARTICLE_TIMERS_OBJECT } });

    data.push_back({ KERNEL_PARTICLES_FOUNTAIN, { GLOBAL_PARTICLE_POINTS_OBJECT}, { GLOBAL_PARTICLE_VECTORS_OBJECT, GLOBAL_PARTICLE_TIMERS_OBJECT } });
    data[KERNEL_PARTICLES_FOUNTAIN].floats = {0.0f, 0.0f, 1000.0f, 1.0f};

    data.push_back({ KERNEL_PARTICLES_GRAVITY, { GLOBAL_PARTICLE_POINTS_OBJECT}, { GLOBAL_PARTICLE_VECTORS_OBJECT } });
    data[KERNEL_PARTICLES_GRAVITY].floats = {0.0f, 0.0f, 1000.0f, 1.0f};

    data.push_back(KernelFunctionData()); // NO KERNEL
}
