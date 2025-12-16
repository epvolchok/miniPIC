#include <iostream>
#include <filesystem>
#include <numbers>

#include <Fields/FieldSolver.hpp>
#include <Diagnostics/Diagnostics.hpp>

namespace fs = std::filesystem;


void SetCurrent_external(FieldGrid &J, const int Jtype, size_t Nx, size_t Ny, double dt, double it, double w, double J0, size_t t0)
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
    fs::path data_dir = cwd.parent_path() / "data";

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
    std::set<size_t> Xlines_diag{indX_diag};
    std::set<size_t> Ylines_diag{indY_diag};

    //std::cout<<"Xlines: "<<Xlines_diag.empty()<<std::endl;
    //std::cout<<"Ylines: "<<Ylines_diag.empty()<<std::endl;

    size_t pointdiag_ix = 10;
    size_t pointdiag_iy = 5;
    std::set<std::pair<size_t, size_t>> point_diag{{pointdiag_ix, pointdiag_iy}};

    size_t DiagnStep = 1;


    FieldGrid E0(Nx, Ny, (double)0.0); //create 3-component grid variable and initialize with 0
    FieldGrid B0(Nx, Ny, (double)0.0);
    FieldGrid J(Nx, Ny, (double)0.0);
    FieldSolver F(dt, dx, dy, Nx, Ny);

    FieldGrid B1(Nx, Ny, (double)0.0);
    FieldGrid E1(Nx, Ny, (double)0.0);
    FieldGrid B2(Nx, Ny, (double)0.0);

    Diagnostics Diag(data_dir, MaxTime, Nx, Ny, DiagnStep, point_diag, Xlines_diag, Ylines_diag,
                    true, true, true);

    //B0 - B^n
    //B1 - B^n+1/2
    //B2 - B^n+1

    //E0 - E^n
    //E1 - E^n+1
    //J(0, 5).x = 1.;

    Diag.run_2DFieldDiagnostic(E0, B0, J, 0);
    Diag.run_1DFieldDiagnostic(E0, B0, J, 0);
    Diag.run_pointFieldDiagnostic(E0, B0, J, 0);

    //Diag.FieldDiagnostic_Energy(E0, B0, 0);

    double t=0;
    for (size_t it=1; it<MaxTime; ++it)
    {
        std::cout<<"it = "<<it<<std::endl;
        t = it*dt;

        Diag.run_2DFieldDiagnostic(E0, B0, J, it-1);
        Diag.run_1DFieldDiagnostic(E0, B0, J, it-1);
        Diag.run_pointFieldDiagnostic(E0, B0, J, it-1);

        Diag.FieldDiagnostic_Energy(E0, B0, it-1);

        SetCurrent_external(J, 0, Nx, Ny, dt, t, w, J0, t0);
        

        F.MagneticField_loop(B1, B0, E0);
        F.BoundaryConditions_Magnetic(FieldSolver::BorderType::Periodic, 1, B1, B0, E0);

        /*std::cout<<"Bx(0, 4) = " << B1(0, 4).x << " By = " << B1(0, 4).y << " Bz = " << B1(0, 4).z << std::endl;
        std::cout<<"Bx(0, 5) = " << B1(0, 5).x << " By = " << B1(0, 5).y << " Bz = " << B1(0, 5).z << std::endl;
        std::cout<<"Bx(Nx-1, 5) = " << B1(Nx-1, 5).x << " By = " << B1(Nx-1, 5).y << " Bz = " << B1(Nx-1, 5).z << std::endl;
        std::cout<<"Bx(Nx-1, 4) = " << B1(Nx-1, 4).x << " By = " << B1(Nx-1, 4).y << " Bz = " << B1(Nx-1, 4).z << std::endl;
        std::cout<<"Bx(0, 6) = " << B1(0, 6).x << " By = " << B1(0, 6).y << " Bz = " << B1(0, 6).z << std::endl;
        std::cout<<"Bx(Nx-1, 6) = " << B1(Nx-1, 6).x << " By = " << B1(Nx-1, 6).y << " Bz = " << B1(Nx-1, 6).z << std::endl;*/

        F.ElectricField_loop(E1, B1, E0, J);
        F.BoundaryConditions_Electric(FieldSolver::BorderType::Periodic, 1, E1, B1, E0, J);

        /*std::cout<<"Ex (0, 4) = " << E1(0, 4).x << " Ey = " << E1(0, 4).y << " Ez = " << E1(0, 4).z << std::endl;
        std::cout<<"Ex (0, 5) = " << E1(0, 5).x << " Ey = " << E1(0, 5).y << " Ez = " << E1(0, 5).z << std::endl;
        std::cout<<"Ex (Nx-1, 5) = " << E1(Nx-1, 5).x << " Ey = " << E1(Nx-1, 5).y << " Ez = " << E1(Nx-1, 5).z << std::endl;
        std::cout<<"Ex (Nx-1, 4) = " << E1(Nx-1, 4).x << " Ey = " << E1(Nx-1, 4).y << " Ez = " << E1(Nx-1, 4).z << std::endl;
        std::cout<<"Ex(0, 6) = " << E1(0, 6).x << " Ey = " << E1(0, 6).y << " Ez = " << E1(0, 6).z << std::endl;
        std::cout<<"Ex(Nx-1, 6) = " << E1(Nx-1, 6).x << " Ey = " << E1(Nx-1, 6).y << " Ez = " << E1(Nx-1, 6).z << std::endl;*/

        F.MagneticField_loop(B2, B1, E1);
        F.BoundaryConditions_Magnetic(FieldSolver::BorderType::Periodic, 1, B2, B1, E1);

        /*std::cout<<"Bx (0, 4) = " << B2(0, 4).x << " By = " << B2(0, 4).y << " Bz = " << B2(0, 4).z << std::endl;
        std::cout<<"Bx (0, 5) = " << B2(0, 5).x << " By = " << B2(0, 5).y << " Bz = " << B2(0, 5).z << std::endl;
        std::cout<<"Bx (Nx-1, 5) = " << B2(Nx-1, 5).x << " By = " << B2(Nx-1, 5).y << " Bz = " << B2(Nx-1, 5).z << std::endl;
        std::cout<<"Bx (Nx-1, 4) = " << B2(Nx-1, 4).x << " By = " << B2(Nx-1, 4).y << " Bz = " << B2(Nx-1, 4).z << std::endl;
        std::cout<<"Bx(0, 6) = " << B2(0, 6).x << " By = " << B2(0, 6).y << " Bz = " << B2(0, 6).z << std::endl;
        std::cout<<"Bx(Nx-1, 6) = " << B2(Nx-1, 6).x << " By = " << B2(Nx-1, 6).y << " Bz = " << B2(Nx-1, 6).z << std::endl;*/

        F.FieldsSwap(B2, B0);
        F.FieldsSwap(E1, E0);
        
    }
    Diag.run_2DFieldDiagnostic(E0, B0, J, MaxTime-1);
    Diag.run_1DFieldDiagnostic(E0, B0, J, MaxTime-1);
    Diag.run_pointFieldDiagnostic(E0, B0, J, MaxTime-1);
    Diag.FieldDiagnostic_Energy(E0, B0, MaxTime-1);

    return 0;
}