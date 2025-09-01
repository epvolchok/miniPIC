#include <iostream>
#include <filesystem>
#include <numbers>

#include "GridCLs.hpp"
#include "FieldsCls.hpp"
#include "FieldSolver.hpp"
#include "Diagnostics.hpp"

namespace fs = std::filesystem;

template <typename T>
void SetCurrent_external(FieldGrid<T> &J, const int Jtype, size_t Nx, size_t Ny, T dt, T it, T w=T{}, T J0=T{}, size_t t0=T{})
{
    switch (Jtype)
    {
    case 0:
        if (it < t0)
        {
                J(10, 5).x = J0 * sin(w * it*dt);
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
    size_t MaxTime = 20;
    
    double dx = 0.5, dy=0.5;
    double dt = 0.25;
    double w = 2. * std::numbers::pi / 40. / dt;
    double J0 = 1.;
    size_t t0 = 10;

    size_t indX_diag = 10;
    size_t indY_diag = 5;
    size_t pointdiag_ix = 10;
    size_t pointdiag_iy = 5;



    size_t DiagnStep = 1;


    FieldGrid<double> E0(Nx, Ny, (double)0.0); //create 3-component grid variable and initialize with 0
    FieldGrid<double> B0(Nx, Ny, (double)0.0);
    FieldGrid<double> J(Nx, Ny, (double)0.0);
    FieldSolver<double> F(dt, dx, dy, Nx, Ny);

    FieldGrid<double> B1(Nx, Ny, (double)0.0);
    FieldGrid<double> E1(Nx, Ny, (double)0.0);
    FieldGrid<double> B2(Nx, Ny, (double)0.0);

    Diagnostics<double> D(data_dir, MaxTime);
    std::string filename = "PointFieldsB_XY" + std::to_string(pointdiag_ix) + "_" + std::to_string(pointdiag_iy) + ".txt";
    fs::path file = data_dir / FieldsPoint_path / filename;
    std::ofstream wf1(file, std::ios::out | std::ios::trunc);
    wf1 << "#Ax    Ay    Az" << std::endl;
    wf1.close();
    filename = "PointFieldsE_XY" + std::to_string(pointdiag_ix) + "_" + std::to_string(pointdiag_iy) + ".txt";
    file = data_dir / FieldsPoint_path / filename;
    std::ofstream wf2(file, std::ios::out | std::ios::trunc);
    wf2 << "#Ax    Ay    Az" << std::endl;
    wf2.close();

    //B0 - B^n
    //B1 - B^n+1/2
    //B2 - B^n+1

    //E0 - E^n
    //E1 - E^n+1
    //J(0, 5).x = 1.;
    double t=0;
    for (size_t it=1; it<MaxTime; ++it)
    {
        std::cout<<"it = "<<it<<std::endl;
        t = it*dt;
        if (it % DiagnStep == 0)
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
        D.run_pointFieldDiagnostic(E0, FieldsPoint_path, "PointFieldsE", pointdiag_ix, pointdiag_iy);

        SetCurrent_external(J, 0, Nx, Ny, dt, t, w, J0, t0);
        

        F.MagneticField_loop(B1, B0, E0);
        F.BoundaryConditions_Magnetic(FieldSolver<double>::BorderType::Periodic, 1, B1, B0, E0);

        /*std::cout<<"Bx(0, 4) = " << B1(0, 4).x << " By = " << B1(0, 4).y << " Bz = " << B1(0, 4).z << std::endl;
        std::cout<<"Bx(0, 5) = " << B1(0, 5).x << " By = " << B1(0, 5).y << " Bz = " << B1(0, 5).z << std::endl;
        std::cout<<"Bx(Nx-1, 5) = " << B1(Nx-1, 5).x << " By = " << B1(Nx-1, 5).y << " Bz = " << B1(Nx-1, 5).z << std::endl;
        std::cout<<"Bx(Nx-1, 4) = " << B1(Nx-1, 4).x << " By = " << B1(Nx-1, 4).y << " Bz = " << B1(Nx-1, 4).z << std::endl;
        std::cout<<"Bx(0, 6) = " << B1(0, 6).x << " By = " << B1(0, 6).y << " Bz = " << B1(0, 6).z << std::endl;
        std::cout<<"Bx(Nx-1, 6) = " << B1(Nx-1, 6).x << " By = " << B1(Nx-1, 6).y << " Bz = " << B1(Nx-1, 6).z << std::endl;*/

        F.ElectricField_loop(E1, B1, E0, J);
        F.BoundaryConditions_Electric(FieldSolver<double>::BorderType::Periodic, 1, E1, B1, E0, J);

        /*std::cout<<"Ex (0, 4) = " << E1(0, 4).x << " Ey = " << E1(0, 4).y << " Ez = " << E1(0, 4).z << std::endl;
        std::cout<<"Ex (0, 5) = " << E1(0, 5).x << " Ey = " << E1(0, 5).y << " Ez = " << E1(0, 5).z << std::endl;
        std::cout<<"Ex (Nx-1, 5) = " << E1(Nx-1, 5).x << " Ey = " << E1(Nx-1, 5).y << " Ez = " << E1(Nx-1, 5).z << std::endl;
        std::cout<<"Ex (Nx-1, 4) = " << E1(Nx-1, 4).x << " Ey = " << E1(Nx-1, 4).y << " Ez = " << E1(Nx-1, 4).z << std::endl;
        std::cout<<"Ex(0, 6) = " << E1(0, 6).x << " Ey = " << E1(0, 6).y << " Ez = " << E1(0, 6).z << std::endl;
        std::cout<<"Ex(Nx-1, 6) = " << E1(Nx-1, 6).x << " Ey = " << E1(Nx-1, 6).y << " Ez = " << E1(Nx-1, 6).z << std::endl;*/

        F.MagneticField_loop(B2, B1, E1);
        F.BoundaryConditions_Magnetic(FieldSolver<double>::BorderType::Periodic, 1, B2, B1, E1);

        /*std::cout<<"Bx (0, 4) = " << B2(0, 4).x << " By = " << B2(0, 4).y << " Bz = " << B2(0, 4).z << std::endl;
        std::cout<<"Bx (0, 5) = " << B2(0, 5).x << " By = " << B2(0, 5).y << " Bz = " << B2(0, 5).z << std::endl;
        std::cout<<"Bx (Nx-1, 5) = " << B2(Nx-1, 5).x << " By = " << B2(Nx-1, 5).y << " Bz = " << B2(Nx-1, 5).z << std::endl;
        std::cout<<"Bx (Nx-1, 4) = " << B2(Nx-1, 4).x << " By = " << B2(Nx-1, 4).y << " Bz = " << B2(Nx-1, 4).z << std::endl;
        std::cout<<"Bx(0, 6) = " << B2(0, 6).x << " By = " << B2(0, 6).y << " Bz = " << B2(0, 6).z << std::endl;
        std::cout<<"Bx(Nx-1, 6) = " << B2(Nx-1, 6).x << " By = " << B2(Nx-1, 6).y << " Bz = " << B2(Nx-1, 6).z << std::endl;*/

        F.FieldsSwap(B2, B0);
        F.FieldsSwap(E1, E0);
        
    }

    return 0;
}