#pragma once
#include <cassert>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <filesystem>
#include "GridCLs.hpp"


class FieldGrid
{
    /*
    Class for electromagnetic fields and currents. 
    Contains three components each as an array on a grid,
    but access is as vector<Vec3<T>>
     */
    private:
    size_t Nx, Ny;
    GridVar<double> Ax, Ay, Az;

    using FieldRef = FieldRefTpl<double>;
    using ConstFieldRef = FieldRefTpl<const double>;


    
    public:
    FieldGrid(): 
        Nx{0}, Ny{0},
        Ax{}, Ay{}, Az{}
    {}

    explicit FieldGrid(size_t Nx_, size_t Ny_):
        Nx{Nx_}, Ny{Ny_},
        Ax{Nx_, Ny_, GridVar<double>::FillType::Const, 0},
        Ay{Nx_, Ny_, GridVar<double>::FillType::Const, 0},
        Az{Nx_, Ny_, GridVar<double>::FillType::Const, 0}
    {}

    FieldGrid(size_t Nx_, size_t Ny_, double val_):
        Nx{Nx_}, Ny{Ny_},
        Ax{Nx_, Ny_, GridVar<double>::FillType::Const, val_},
        Ay{Nx_, Ny_, GridVar<double>::FillType::Const, val_},
        Az{Nx_, Ny_, GridVar<double>::FillType::Const, val_}
        {}

    double getX(size_t ix, size_t iy) const;
    double getY(size_t ix, size_t iy) const;
    void setX(double val, size_t ix, size_t iy);
    void setY(double val, size_t ix, size_t iy);
    void setZ(double val, size_t ix, size_t iy);

    FieldRef operator()(size_t ix, size_t iy);
    ConstFieldRef operator()(size_t ix, size_t iy) const;
    void write_to_binary(const std::filesystem::path& path, const std::string &filename) const;
    void write_on_line(const std::filesystem::path& path, const std::string &filename, size_t ind, GridVar<double>::DiagnLine line_type) const;
    void write_at_point(const std::filesystem::path& path, const std::string &filename, size_t ix, size_t iy) const;
};