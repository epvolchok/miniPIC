#pragma once
#include "GridCLs.hpp"
#include "FieldsCls.hpp"

template <typename T>
class FieldSolver
{
    private:
    T dt, dx, dy;
    size_t Nx, Ny;

    public:
    FieldSolver(): dt{0}, dx{0}, dy{0},
    Nx{0}, Ny{0} {}
    FieldSolver(T dt_, T dx_, T dy_, size_t Nx_, size_t Ny_):
        dt{dt_}, dx{dx_}, dy{dy_},
        Nx{Nx_}, Ny{Ny_}
    {}

    enum class BorderType { Periodic, Reflect, Damping };

    Vec3<T> MagnFields_HalfMove(const Vec3<T> &B0, const Vec3<T> &E00, const Vec3<T> &E01, const Vec3<T> &E02)
    {
        Vec3<T> B1;
        //B0=B0(ix, iy), E00=E0(ix, iy), E01=E0(ix-1, iy), E02=E0(ix, iy-1)
        
        B1.x = B0.x - 0.5 * dt * (E00.z - E02.z)/dy;
        B1.y = B0.y + 0.5 * dt * (E00.z - E01.z)/dx;
        B1.z = B0.z + 0.5 * dt * (E00.x - E02.x)/dy -
                        0.5 * dt * (E00.y - E01.y)/dx;
        return B1;
    }


    Vec3<T> ElectrFields_Move(const Vec3<T> &E0, const Vec3<T> &B00, const Vec3<T> &B01, const Vec3<T> &B02, const Vec3<T> &J)
    {
        Vec3<T> E1;
        //E0=E0(ix, iy), B00=B0(ix, iy), B01=B0(ix+1, iy), B02=B0(ix, iy+1), J=J(ix, iy)
        E1.x = E0.x - dt * J.x + dt * (B02.z - B00.z)/dy;
        E1.y = E0.y - dt * J.y - dt * (B01.z - B00.z)/dx;
        E1.z = E0.z - dt * J.z - dt * (B02.x - B00.x)/dy +
                        dt * (B01.y - B00.y)/dx;
        return E1;
    }

    void MagneticField_loop(FieldGrid<T> &B1, const FieldGrid<T> &B0, const FieldGrid<T> &E0)
    {
        for (size_t iy=1; iy<Ny; ++iy)
        {
            for (size_t ix=1; ix<Nx; ++ix)
            {
                B1(ix, iy) = MagnFields_HalfMove(B0(ix, iy), E0(ix, iy), E0(ix-1, iy), E0(ix, iy-1)); //B^n+1/2
            }
        }
    }

    void ElectricField_loop(FieldGrid<T> &E1, const FieldGrid<T> &B1, const FieldGrid<T> &E0, const FieldGrid<T> &J)
    {
        for (size_t iy=0; iy<Ny-1; ++iy)
        {
            for (size_t ix=0; ix<Nx-1; ++ix)
            {
                E1(ix, iy) = ElectrFields_Move(E0(ix, iy), B1(ix, iy), B1(ix+1, iy), B1(ix, iy+1), J(ix, iy));
            }
        }
    }

    void Periodic_Magnetic(FieldGrid<T> &B1, const FieldGrid<T> &B0, const FieldGrid<T> &E0)
    {
        for(size_t ix=1, iy=0; ix<Nx; ++ix)
        {
            B1(ix, iy).x = B0(ix, iy).x - 0.5 * dt * (E0(ix, iy).z - E0(ix, Ny-1).z)/dy;
            B1(ix, iy).y = B0(ix, iy).y + 0.5 * dt * (E0(ix, iy).z - E0(ix-1, iy).z)/dx;
            B1(ix, iy).z = B0(ix, iy).z + 0.5 * dt * (E0(ix, iy).x - E0(ix, Ny-1).x)/dy -
                        0.5 * dt * (E0(ix, iy).y - E0(ix-1, iy).y)/dx;
        }
        for(size_t ix=0, iy=1; iy<Ny; ++iy)
        {
            B1(ix, iy).x = B0(ix, iy).x - 0.5 * dt * (E0(ix, iy).z - E0(ix, iy-1).z)/dy;
            B1(ix, iy).y = B0(ix, iy).y + 0.5 * dt * (E0(ix, iy).z - E0(Nx-1, iy).z)/dx;
            B1(ix, iy).z = B0(ix, iy).z + 0.5 * dt * (E0(ix, iy).x - E0(ix, iy-1).x)/dy -
                        0.5 * dt * (E0(ix, iy).y - E0(Nx-1, iy).y)/dx;
        }
        size_t iy=0, ix=0;
            B1(ix, iy).x = B0(ix, iy).x - 0.5 * dt * (E0(ix, iy).z - E0(ix, Ny-1).z)/dy;
            B1(ix, iy).y = B0(ix, iy).y + 0.5 * dt * (E0(ix, iy).z - E0(Nx-1, iy).z)/dx;
            B1(ix, iy).z = B0(ix, iy).z + 0.5 * dt * (E0(ix, iy).x - E0(ix, Ny-1).x)/dy -
                        0.5 * dt * (E0(ix, iy).y - E0(Nx-1, iy).y)/dx;
    }

    void Periodic_Electric(FieldGrid<T> &E1, const FieldGrid<T> &B0, const FieldGrid<T> &E0, const FieldGrid<T> &J)
    {
        for(size_t ix=0, iy=Ny-1; ix<Nx-1; ++ix)
        {
            E1(ix, iy).x = E0(ix, iy).x - dt * J(ix, iy).x + dt * (B0(ix, 0).z - B0(ix, iy).z)/dy;
            E1(ix, iy).y = E0(ix, iy).y - dt * J(ix, iy).y - dt * (B0(ix+1, iy).z - B0(ix, iy).z)/dx;
            E1(ix, iy).z = E0(ix, iy).z - dt * J(ix, iy).z - dt * (B0(ix, 0).x - B0(ix, iy).x)/dy +
                        dt * (B0(ix+1, iy).y - B0(ix, iy).y)/dx;
        }
        for(size_t ix=Nx-1, iy=0; iy<Ny-1; ++iy)
        {
            E1(ix, iy).x = E0(ix, iy).x - dt * J(ix, iy).x + dt * (B0(ix, iy+1).z - B0(ix, iy).z)/dy;
            E1(ix, iy).y = E0(ix, iy).y - dt * J(ix, iy).y - dt * (B0(0, iy).z - B0(ix, iy).z)/dx;
            E1(ix, iy).z = E0(ix, iy).z - dt * J(ix, iy).z - dt * (B0(ix, iy+1).x - B0(ix, iy).x)/dy +
                        dt * (B0(0, iy).y - B0(ix, iy).y)/dx;
        }
        size_t ix=Nx-1, iy=Ny-1;
            E1(ix, iy).x = E0(ix, iy).x - dt * J(ix, iy).x + dt * (B0(ix, 0).z - B0(ix, iy).z)/dy;
            E1(ix, iy).y = E0(ix, iy).y - dt * J(ix, iy).y - dt * (B0(0, iy).z - B0(ix, iy).z)/dx;
            E1(ix, iy).z = E0(ix, iy).z - dt * J(ix, iy).z - dt * (B0(ix, 0).x - B0(ix, iy).x)/dy +
                        dt * (B0(0, iy).y - B0(ix, iy).y)/dx;
    }


    void BoundaryConditions_Electric(const BorderType Btype, size_t width, FieldGrid<T> &E1, const FieldGrid<T> &B0, const FieldGrid<T> &E0, const FieldGrid<T> &J)
    {
        
        switch (Btype)
        {
        case BorderType::Periodic:
            Periodic_Electric(E1, B0, E0, J);
            break;
        }
    }
    void BoundaryConditions_Magnetic(const BorderType Btype, size_t width, FieldGrid<T> &B1, const FieldGrid<T> &B0, const FieldGrid<T> &E0)
    {
        
        switch (Btype)
        {
        case BorderType::Periodic:
            Periodic_Magnetic(B1, B0, E0);
            break;
        }
    }

    void FieldsSwap(FieldGrid<T> &A1, FieldGrid<T> &A0)
    {
        Vec3<T> Temp;
        for (size_t iy=0; iy<Ny-1; ++iy)
        {
            for (size_t ix=0; ix<Nx-1; ++ix)
            {
                Temp = A1(ix, iy);


                A0(ix, iy) = Temp;

            }
        }
    }
};