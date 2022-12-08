#ifndef KERNEL_H
#define KERNEL_H


#include <iostream>
#include <fstream>
#include <sstream>

#include <CL/opencl.hpp>
#include <GL/glew.h>
#include <GL/glx.h>
#include <SDL2/SDL.h>

#include "Define.hpp"
#include "Math.hpp"

struct KernelFunctionData {
    int kernel = -1;
    std::vector<int> interOn = std::vector<int>();
    std::vector<int> interOff = std::vector<int>();
    std::vector<float> floats = std::vector<float>();
};

class Kernel
{
public:
    std::vector<cl::Platform> platforms;
    std::vector<std::vector<cl::Device>> devices;
    cl::Platform default_platform;
    cl::Device default_device;
    cl::Context context;

    cl::NDRange globalWS;
    cl::NDRange localWS;
    size_t numParticles = 0;
    size_t maxParticles = 0;

    std::vector<GLuint> hosts; // host opengl object
    std::vector<cl::BufferGL> deviceInterObject; // device memory buffer interpolarity
    std::vector<cl::Buffer> deviceObject; // device memory buffer 
    std::vector<cl::Kernel> kernels;
    std::vector<KernelFunctionData> data;

    Kernel() {};
    ~Kernel() {};

    void Create();
    void Function(const struct KernelFunctionData& data);

private:
    std::string const getErrorString(cl_int error);
    void coutError(const unsigned int type, const std::string& string, cl_uint error = 0, const bool showMessage = true);
    
    void DefinePlatformDeviceParticles();
    void CreateCLContext();
    void AllocateInterpolarity();
    void AllocateCL();
    void BuildCLProgram();
    void CreateOpenCLFunctionData();

};
#endif
