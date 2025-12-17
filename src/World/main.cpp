#include <iostream>
#include <filesystem>
#include "clsWorld.hpp"
namespace fs = std::filesystem;

int main()
{
    World world;

    fs::path param_path = world.find_param_file();
    std::cout<<param_path<<std::endl;
    world.read_param(param_path);

    return 0;

}