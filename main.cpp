#include <iostream>
#include <filesystem>

#include "GridCLs.hpp"

namespace fs = std::filesystem;

int main()
{
    fs::path cwd = fs::current_path();
    fs::path data_dir = cwd.parent_path() / "data" / "results";

    size_t Nx = 10;
    size_t Ny = 10;
    GridVar<double> A(Nx, Ny, "rand");
    for (auto ix=0; ix<Nx; ++ix)
    {
        for (auto iy=0; iy<Ny; ++iy)
            std::cout<<A.get(ix, iy)<<" ";
    }

    A.write_to_binary(data_dir, "test.bin");
}