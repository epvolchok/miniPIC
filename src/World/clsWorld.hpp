#pragma once
#include <iostream>
#include <fstream>
#include <filesystem>
#include <cassert>
#include <variant>
#include <unordered_map>
#include <unordered_set>
namespace fs = std::filesystem;

class World
{
    private:
    size_t Nx, Ny;
    double dx, dy, dt;
    size_t MaxTime, DiagnStep;

    using ParamValue = std::variant<size_t, double, std::string>;
    using ParamDict  = std::unordered_map<std::string, ParamValue>;

    public:
    World():
        Nx{}, Ny{},
        dx{}, dy{}, dt{},
        MaxTime{}, DiagnStep{}
    {}

    World(size_t Nx_, size_t Ny_, double dx_, double dy_, double dt_,
            size_t MaxTime_, size_t DiagnStep_):
        Nx{Nx_}, Ny{Ny_},
        dx{dx_}, dy{dy_}, dt{dt_},
        MaxTime{MaxTime_}, DiagnStep{DiagnStep_}
    {}

    fs::path find_param_file();
    ParamDict read_param(fs::path path);




};