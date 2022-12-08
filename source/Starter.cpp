#include "Program.hpp"

int main(int argc, char** argv)
{
    try
    {
        Program program;
    }
    catch (std::runtime_error& e)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "runtime_error", e.what(), NULL);
        std::cerr << e.what() << "\n";

        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
