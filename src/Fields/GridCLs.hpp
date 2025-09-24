#pragma once
#include <cassert>
#include <iostream>
#include <random>
#include <algorithm>
#include <fstream>
#include <filesystem>

template <typename T>
class GridVar;


template <typename T>
class GridVar
{
    private:
    size_t Nx, Ny;
    std::vector<T> data;

    public:
    enum class FillType { Rand, Const};
    enum class DiagnLine { X, Y };
    //empty constructor
    GridVar(): Nx{0}, Ny{0} {}
    //default constructor
    explicit GridVar(size_t Nx_, size_t Ny_):
        Nx{Nx_}, Ny{Ny_},
        data(Nx_*Ny_)
    {}
    //with filling by a constant or random number from normal distribution
    GridVar(size_t Nx_, size_t Ny_, const FillType fill_type = FillType::Rand, T value = T{}):
        Nx{Nx_}, Ny{Ny_},
        data(Nx_*Ny_)
    {
        switch (fill_type)
        {
        case FillType::Rand:
        {    std::random_device rnd_device;
            std::mt19937 mersenne_engine {rnd_device()};
            std::normal_distribution<T> distribution(0.,1.0);

            auto gen = [&](){ return distribution(mersenne_engine); };
            std::generate(data.begin(), data.end(), gen);
            break;
        }
        case FillType::Const:
        {
            std::fill(data.begin(), data.end(), value);
            break;
        }
        
        }
    }



    std::pair<size_t, size_t> get_size() const
    {
        std::pair<size_t, size_t> Size={Nx, Ny};
        return Size;
    }

    void print_size() const
    {
        std::cout<<"Nx = "<<Nx<<", Ny = "<<Ny<<std::endl;
    }

    __inline__ size_t lincoord(size_t ix, size_t iy)
    {
        return iy*Nx + ix;
    }

    __inline__ const size_t lincoord(size_t ix, size_t iy) const
    {
        return iy*Nx + ix;
    }

    T get(size_t ix, size_t iy) const
    {
        
        assert(ix < Nx && iy < Ny);
        return data[lincoord(ix, iy)];
    }

    void set(T val, size_t ix, size_t iy)
    {
        assert(ix < Nx && iy < Ny);
        data[lincoord(ix, iy)] = val;
    }

    T& operator()(size_t ix, size_t iy) &
    {
        assert(ix < Nx && iy < Ny);
        return data[lincoord(ix, iy)];
    }

    const T& operator()(size_t ix, size_t iy) const& 
    {
        assert(ix < Nx && iy < Ny);
        return data[lincoord(ix, iy)];
    }

    void write_to_binary(std::ofstream &wf) const
    {
        wf.write(reinterpret_cast<const char*>(data.data()), static_cast<std::streamsize>(data.size() * sizeof(T)));
    }

    void write_on_line(const std::filesystem::path& path, const std::string& filename, size_t ind, const DiagnLine line_type)
    {
        std::filesystem::create_directories(path);
        std::ofstream wf(path / filename);
        if (!wf.is_open())
        {
        throw std::runtime_error("Error openning the file: " + (path / filename).string());
           }
        switch (line_type)
        {
            case DiagnLine::X:
            {
                for (auto iy=0; iy<Ny; ++iy)
                    wf<<data[lincoord(ind, iy)]<<std::endl;
                break;
            }
            case DiagnLine::Y:
            {
                for (auto ix=0; ix<Nx; ++ix)
                    wf<<data[lincoord(ix, ind)]<<std::endl;
                break;
            }
        }
        wf.close();
        if (wf.fail()) {
        throw std::runtime_error("Error writing to file: " + (path / filename).string());
           }
    }

};


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