#include <iostream>
#include <filesystem>
#include <numbers>

#include "GridCLs.hpp"
#include "FieldsCls.hpp"
#include "FieldSolver.hpp"
#include "Diagnostics.hpp"

namespace fs = std::filesystem;

template <typename T>
void SetCurrent_external(FieldGrid<T> &J, const int Jtype, size_t Nx, size_t Ny, T dt, T t, T w=T{}, T J0=T{})
{
    switch (Jtype)
    {
    case 0:
        for (auto iy=0; iy<Ny; ++iy)
        {
            for (auto ix=0; ix<Nx; ++ix)
            {
                J(ix, iy).x = J0 * sin(w * t);
            }
        }
        break;
    }
}


int main()
{
    fs::path cwd = fs::current_path();
    fs::path data_dir = cwd.parent_path() / "data" / "Fields";
    fs::path Fields2D_path = "2D";
    fs::path Fields1D_path = "1D";
    fs::path FieldsPoint_path = "Point";

    size_t Nx = 20;
    size_t Ny = 10;
    size_t MaxTime = 10;
    
    double dx = 0.5, dy=0.5;
    double dt = 0.25;
    double w = 2. * std::numbers::pi / 5. / dt;

    size_t indX_diag = 10;
    size_t indY_diag = 5;
    size_t pointdiag_ix = 10;
    size_t pointdiag_iy = 5;



    size_t DiagnStep = 1;


    FieldGrid<double> E0(Nx, Ny, (double)0.0); //create 3-component grid variable and initialize with 0
    FieldGrid<double> B0(Nx, Ny, (double)0.0);
    FieldGrid<double> J(Nx, Ny, (double)0.0);
    FieldSolver<double> F(dt, dx, dy, Nx, Ny);

    std::cout<<B0(0, 0).x<<std::endl;
    B0(0, 0).x =1;
    std::cout<<B0(0, 0).x<<std::endl;

    FieldGrid<double> B1(Nx, Ny, (double)0.0);
    FieldGrid<double> E1(Nx, Ny, (double)0.0);
    FieldGrid<double> B2(Nx, Ny, (double)0.0);

    Diagnostics<double> D(data_dir, MaxTime);

    //B0 - B^n
    //B1 - B^n+1/2
    //B2 - B^n+1

    //E0 - E^n
    //E1 - E^n+1
    double t=0;
    for (auto it=1; it<MaxTime; ++it)
    {
        t = it*dt;
        /*if (it % DiagnStep == 0)
        {
            D.run_2DFieldDiagnostic(B0, Fields2D_path, "FieldsB", it);
            D.run_2DFieldDiagnostic(E0, Fields2D_path, "FieldsE", it);
            D.run_2DFieldDiagnostic(J, Fields2D_path, "FieldsJ", it);
        }

        D.run_1DFieldDiagnostic(B0, Fields1D_path, "FieldsB", it, indX_diag, GridVar<double>::DiagnLine::X);
        D.run_1DFieldDiagnostic(E0, Fields1D_path, "FieldsE", it, indX_diag, GridVar<double>::DiagnLine::X);
        D.run_1DFieldDiagnostic(J, Fields1D_path, "FieldsJ", it, indX_diag, GridVar<double>::DiagnLine::X);

        D.run_1DFieldDiagnostic(B0, Fields1D_path, "FieldsB", it, indY_diag, GridVar<double>::DiagnLine::Y);
        D.run_1DFieldDiagnostic(E0, Fields1D_path, "FieldsE", it, indY_diag, GridVar<double>::DiagnLine::Y);
        D.run_1DFieldDiagnostic(J, Fields1D_path, "FieldsJ", it, indY_diag, GridVar<double>::DiagnLine::Y);

        D.run_pointFieldDiagnostic(B0, FieldsPoint_path, "PointFieldsB", pointdiag_ix, pointdiag_iy);
        D.run_pointFieldDiagnostic(E0, FieldsPoint_path, "PointFieldsE", pointdiag_ix, pointdiag_iy);*/

        SetCurrent_external(J, 0, Nx, Ny, dt, t, 1., 1.);
        D.run_pointFieldDiagnostic(J, FieldsPoint_path, "PointFieldsE", pointdiag_ix, pointdiag_iy);
        D.run_1DFieldDiagnostic(J, Fields1D_path, "FieldsJ", it, indY_diag, GridVar<double>::DiagnLine::Y);
        D.run_1DFieldDiagnostic(J, Fields1D_path, "FieldsJ", it, indX_diag, GridVar<double>::DiagnLine::X);
        D.run_2DFieldDiagnostic(J, Fields2D_path, "FieldsJ", it);
        /*F.MagneticField_loop(B1, B0, E0);
        F.BoundaryConditions_Magnetic(FieldSolver<double>::BorderType::Periodic, 1, B1, B0, E0);
        F.ElectricField_loop(E1, B1, E0, J);
        F.BoundaryConditions_Electric(FieldSolver<double>::BorderType::Periodic, 1, E1, B1, E0, J);
        F.MagneticField_loop(B2, B1, E1);
        F.BoundaryConditions_Magnetic(FieldSolver<double>::BorderType::Periodic, 1, B2, B1, E1);

        F.FieldsSwap(B2, B0);
        F.FieldsSwap(E1, E0);*/
    }

    return 0;
}