#pragma once
#include "FieldsCls.hpp"


class FieldSolver
{
    private:
    double dt, dx, dy;
    size_t Nx, Ny;

    public:
    FieldSolver(): dt{0}, dx{0}, dy{0},
    Nx{0}, Ny{0} {}
    FieldSolver(double dt_, double dx_, double dy_, size_t Nx_, size_t Ny_):
        dt{dt_}, dx{dx_}, dy{dy_},
        Nx{Nx_}, Ny{Ny_}
    {}

    enum class BorderType { Periodic, Reflect, Damping };

    Vec3<double> MagnFields_HalfMove(const Vec3<double> &B0, const Vec3<double> &E00, const Vec3<double> &E01, const Vec3<double> &E02);
    Vec3<double> ElectrFields_Move(const Vec3<double> &E0, const Vec3<double> &B00, const Vec3<double> &B01, const Vec3<double> &B02, const Vec3<double> &J);
    void MagneticField_loop(FieldGrid &B1, const FieldGrid &B0, const FieldGrid &E0);
    void ElectricField_loop(FieldGrid &E1, const FieldGrid &B1, const FieldGrid &E0, const FieldGrid &J);
    void Periodic_Magnetic(FieldGrid &B1, const FieldGrid &B0, const FieldGrid &E0);
    void Periodic_Electric(FieldGrid &E1, const FieldGrid &B0, const FieldGrid &E0, const FieldGrid &J);
    void BoundaryConditions_Electric(const BorderType Btype, size_t width, FieldGrid &E1, const FieldGrid &B0, const FieldGrid &E0, const FieldGrid &J);
    void BoundaryConditions_Magnetic(const BorderType Btype, size_t width, FieldGrid &B1, const FieldGrid &B0, const FieldGrid &E0);
    void FieldsSwap(FieldGrid &A1, FieldGrid &A0);

};