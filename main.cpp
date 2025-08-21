#include <iostream>
#include <filesystem>

#include "GridCLs.hpp"

namespace fs = std::filesystem;

int main()
{
    fs::path cwd = fs::current_path();
    fs::path data_dir = cwd.parent_path() / "data" / "results";

    size_t Nx = 10;
    size_t Ny = 20;
    GridVar<double> A(Nx, Ny, GridVar<double>::FillType::Rand);
    for (auto iy=0; iy<Ny; ++iy)
    {
        for (auto ix=0; ix<Nx; ++ix)
            std::cout<<A.get(ix, iy)<<" ";
        std::cout<<std::endl;
    }

    std::cout<<"ix = 2, iy = 3: "<<A(2, 3)<<std::endl;

    A.write_to_binary(data_dir, "test.bin");

    A.write_on_line(data_dir, "test_line.txt", 5, GridVar<double>::DiagnLine::X);
}