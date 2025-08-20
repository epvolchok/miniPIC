#include <iostream>
#include "GridCLs.hpp"

int main()
{
    size_t Nx = 10;
    size_t Ny = 1;
    GridVar<double> A(10, 1, "rand");
    for (auto ix=0; ix<Nx; ++ix)
    {
        for (auto iy=0; iy<Ny; ++iy)
            std::cout<<A.get(ix, iy)<<" ";
    }
}