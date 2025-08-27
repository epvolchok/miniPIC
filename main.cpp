#include <iostream>
#include <filesystem>
#include <tuple>

#include "GridCLs.hpp"
#include "FieldsCls.hpp"
#include "FieldSolver.hpp"

namespace fs = std::filesystem;

template <typename T>
void SetCurrent_external(FieldGrid<T> &J, const std::string Jtype, T dt, T w=T{}, T J0=T{})
{
    switch (Jtype)
    {
    case "sin":
        for (auto iy=1; iy<Ny-1; ++iy)
        {
            for (auto ix=1; ix<Nx-1; ++ix)
            {
                J(ix, iy) = J0 * sin(w * dt);
            }
        }
        break;
    
    default:
        for (auto iy=1; iy<Ny-1; ++iy)
        {
            for (auto ix=1; ix<Nx-1; ++ix)
            {
                J(ix, iy) = J0;
            }
        }
        break;
    }
}


int main()
{
    fs::path cwd = fs::current_path();
    fs::path data_dir = cwd.parent_path() / "data" / "results";

    size_t Nx = 10;
    size_t Ny = 20;
    size_t MaxTime = 10;
    
    double dx = 0.5, dy=0.5;
    double dt = 0.25;


    FieldGrid<double> E0(Nx, Ny, (double)0.0); //create 3-component grid variable and initialize with 0
    FieldGrid<double> B0(Nx, Ny, (double)0.0);
    FieldGrid<double> J(Nx, Ny, (double)0.0);
    FieldSolver<double> F(dt, dx, dy);


    FieldGrid<double> B1(Nx, Ny, (double)0.0);
    FieldGrid<double> E1(Nx, Ny, (double)0.0);
    FieldGrid<double> B2(Nx, Ny, (double)0.0);

    //B0 - B^n
    //B1 - B^n+1/2
    //B2 - B^n+1

    //E0 - E^n
    //E1 - E^n+1

    for (auto it=1; it<MaxTime; ++it)
    {
        SetCurrent_external(J, "sin", dt, 1., 1.);
        F.MagneticField_loop(B1, B0, E0);
        F.ElectricField_loop(E1, B1, E0, J);
        F.MagneticField_loop(B2, B1, E1);

        F.Boundary_conditions(FieldSolver<double>::BorderType::Periodic, 1, B1, E1, B0, E0);
    }


}