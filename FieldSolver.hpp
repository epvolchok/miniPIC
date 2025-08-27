#include <tuple>

#include "GridCLs.hpp"
#include "FieldsCls.hpp"

template <typename T>
class FieldSolver
{
    private:
    T dt, dx, dy;

    public:
    FieldSolver(): dt{0}, dx{0}, dy{0} {}
    FieldSolver(T dt_, T dx_, T dy_):
        dt{dt_}, dx{dx_}, dy{dy_}
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
        E1.x = E0.x - dt * J + dt * (B02.z - B00.z)/dy;
        E1.y = E0.y - dt * J - dt * (B01.z - B00.z)/dx;
        E1.z = E0.z - dt * J - dt * (B02.x - B00.x)/dy +
                        dt * (B01.y - B00.y)/dx;
        return E1;
    }

    void MagneticField_loop(FieldGrid<T> &B1, const FieldGrid<T> &B0, const FieldGrid<T> &E0)
    {
        for (auto iy=1; iy<Ny; ++iy)
        {
            for (auto ix=1; ix<Nx; ++ix)
            {
                //Currents = Currents::CalcCurrent(); //J^n+1/2
                B1(ix, iy) = MagnFields_HalfMove(B0(ix, iy), E0(ix, iy), E0(ix-1, iy), E0(ix, iy-1)); //B^n+1/2
                
                //Diagnostics();
            }
        }
    }

    void ElectricField_loop(FieldGrid<T> &E1, const FieldGrid<T> &B1, const FieldGrid<T> &E0, const FieldGrid<T> &J)
    {
        for (auto iy=0; iy<Ny-1; ++iy)
        {
            for (auto ix=0; ix<Nx-1; ++ix)
            {
                E1(ix, iy) = ElectrFields_Move(E0(ix, iy), B1(ix, iy), B1(ix+1, iy), B1(ix, iy+1), J(ix, iy));
            }
        }
    }

    void Periodic_boundaries(FieldGrid<T> &B1, FieldGrid<T> &E1, const FieldGrid<T> &B0, const FieldGrid<T> &E0)
    {
        //for electric fields
        for(size_t ix=0, iy=Ny-1;x<Nx-1;++ix)
        {

        }
        for(size_t x=0, y=Ny-1;x<Nx-1;++x)
        {

        }
        //x=Nx-1,y=Ny-1
        //for magnetic fields
        for(size_t x=1, y=0;x<Nx;++x)
        {

        }
        for(size_t x=0, y=1;y<Ny;++y)
        {

        }
        //size_t x=0,y=0; 

    }

    void Boundary_conditions(const BorderType Btype, size_t width, FieldGrid<T> &B1, FieldGrid<T> &E1, const FieldGrid<T> &B0, const FieldGrid<T> &E0)
    {
        
        switch (Btype)
        {
        case BorderType::Periodic:
            Periodic_boundaries(B1, E1, B0, E0);
            break;
        }
    }
};