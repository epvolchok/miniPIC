#pragma once
#include <cassert>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <filesystem>


template <typename T>
struct Vec3
{
    T x, y, z;

    Vec3& operator=(const Vec3& other) = default;
    Vec3 operator+(const Vec3& other) const
    {
        return {x+other.x, y+other.y, z+other.z};
    }


};

template <typename U>
struct FieldRefTpl {
    U& x;
    U& y;
    U& z;

    // присвоение из Vec3
    FieldRefTpl& operator=(const Vec3<std::remove_const_t<U>>& v) {
        x = v.x; y = v.y; z = v.z;
        return *this;
    }

    // присвоение из другого FieldRefTpl
    FieldRefTpl& operator=(const FieldRefTpl<std::remove_const_t<U>>& other) {
        x = other.x; y = other.y; z = other.z;
        return *this;
    }

    // преобразование в Vec3 (всегда можно скопировать)
    operator Vec3<std::remove_const_t<U>>() const {
        return {x, y, z};
    }
};


template <typename T>
class FieldGrid
{
    /*
    Class for electromagnetic fields and currents. 
    Contains three components each as an array on a grid,
    but access is as vector<Vec3<T>>
     */
    private:
    size_t Nx, Ny;
    GridVar<T> Ax, Ay, Az;

    using FieldRef = FieldRefTpl<T>;
    using ConstFieldRef = FieldRefTpl<const T>;


    
    public:
    FieldGrid(): 
        Nx{0}, Ny{0},
        Ax{}, Ay{}, Az{}
    {}

    explicit FieldGrid(size_t Nx_, size_t Ny_):
        Nx{Nx_}, Ny{Ny_},
        Ax{Nx_, Ny_},
        Ay{Nx_, Ny_},
        Az{Nx_, Ny_}
    {}

    FieldGrid(size_t Nx_, size_t Ny_, T val_):
        Nx{Nx_}, Ny{Ny_},
        Ax{Nx_, Ny_, GridVar<T>::FillType::Const, val_},
        Ay{Nx_, Ny_, GridVar<T>::FillType::Const, val_},
        Az{Nx_, Ny_, GridVar<T>::FillType::Const, val_}
        {}

    T getX(size_t ix, size_t iy) const
    {
        return Ax.get(ix, iy);
    }
    T getY(size_t ix, size_t iy) const
    {
        return Ay.get(ix, iy);
    }
    T getZ(size_t ix, size_t iy) const
    {
        return Az.get(ix, iy);
    }

    void setX(size_t ix, size_t iy)
    {
        Ax.set(ix, iy);
    }
    void setY(size_t ix, size_t iy)
    {
        Ay.set(ix, iy);
    }
    void setZ(size_t ix, size_t iy)
    {
        Az.set(ix, iy);
    }

    FieldRef operator()(size_t ix, size_t iy)
    {
        assert(ix < Nx && iy < Ny);
        return { Ax(ix,iy), Ay(ix,iy), Az(ix,iy) };
    }
    

    ConstFieldRef operator()(size_t ix, size_t iy) const
    {
        assert(ix < Nx && iy < Ny);
        return { Ax(ix,iy), Ay(ix,iy), Az(ix,iy) };
    }

    

    void write_to_binary(const std::filesystem::path& path, const std::string &filename) const
    {
        std::filesystem::create_directories(path);
        std::ofstream wf(path / filename, std::ios::out | std::ios::binary);
        if (!wf.is_open())
        {
        throw std::runtime_error("Error openning the file: " + (path / filename).string());
           }
        //sizes
        wf.write(reinterpret_cast<const char*>(&Nx), sizeof(Nx));
        wf.write(reinterpret_cast<const char*>(&Ny), sizeof(Ny));

        //data as 3 blocks

        Ax.write_to_binary(wf);
        Ay.write_to_binary(wf);
        Az.write_to_binary(wf);

        
        wf.close();
        if (wf.fail()) {
        throw std::runtime_error("Error writing to file: " + (path / filename).string());
           }
    }

    void write_on_line(const std::filesystem::path& path, const std::string &filename, size_t ind, GridVar<T>::DiagnLine line_type) const
    {
        std::filesystem::create_directories(path);
        std::ofstream wf(path / filename);
        if (!wf.is_open())
        {
        throw std::runtime_error("Error openning the file: " + (path / filename).string());
           }
        wf<<"#Ax    Ay    Az"<<std::endl;
        switch (line_type)
        {
            case GridVar<T>::DiagnLine::X:
            {
                for (auto iy=0; iy<Ny; ++iy)
                    wf<<Ax(ind, iy)<<"\t"<<Ay(ind, iy)<<"\t"<<Az(ind, iy)<<std::endl;
                break;
            }
            case GridVar<T>::DiagnLine::Y:
            {
                for (auto ix=0; ix<Nx; ++ix)
                    wf<<Ax(ix, ind)<<"\t"<<Ay(ix, ind)<<"\t"<<Az(ix, ind)<<std::endl;
                break;
            }
        }
        wf.close();
        if (wf.fail()) {
        throw std::runtime_error("Error writing to file: " + (path / filename).string());
           }
    }
    void write_at_point(const std::filesystem::path& path, const std::string &filename, size_t ix, size_t iy) const
    {
        std::filesystem::create_directories(path);
        std::ofstream wf(path / filename, std::ios::out | std::ios::app);
        if (!wf.is_open())
        {
        throw std::runtime_error("Error openning the file: " + (path / filename).string());
           }

        wf<<Ax(ix, iy)<<"\t"<<Ay(ix, iy)<<"\t"<<Az(ix, iy)<<std::endl;
                wf.close();
        if (wf.fail()) {
        throw std::runtime_error("Error writing to file: " + (path / filename).string());
           }
    }

};