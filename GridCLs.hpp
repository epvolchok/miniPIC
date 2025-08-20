#pragma once
#include <cassert>
#include <iostream>
#include <memory>
#include <random>
#include <algorithm>
template <typename T>
class GridVar;


template <typename T>
class GridVar
{
    private:
    size_t Nx, Ny;
    std::vector<T> data;

    public:
    //empty constructor
    GridVar(): Nx{0}, Ny{0} {}
    //default constructor
    explicit GridVar(size_t Nx_, size_t Ny_):
        Nx{Nx_}, Ny{Nx_},
        data(Nx_*Ny_)
    {}

    GridVar(size_t Nx_, size_t Ny_, const std::string& fill_type = "rand", T value = T{}):
        Nx{Nx_}, Ny{Nx_},
        data(Nx_*Ny_)
    {
        if (fill_type == "rand")
            {
                std::random_device rnd_device;
                std::mt19937 mersenne_engine {rnd_device()};
                std::normal_distribution<T> distribution(0.,1.0);

                auto gen = [&](){ return distribution(mersenne_engine); };
                std::generate(data.begin(), data.end(), gen);
            }
        else if (fill_type == "const")
        {
            std::fill(data.begin(), data.end(), value);
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

    T get(size_t ix, size_t iy) const
    {
        return data[iy*Nx + ix];
    }

    void set(T val, size_t ix, size_t iy)
    {
        assert(ix < Nx && iy < Ny);
        data[iy*Nx + ix] = val;
    }

    //friend std::ostream& operator<<<T>(std::ostream& os, GridVar& v);

};


/* template <typename T>
std::ostream& operator<<(std::ostream& os, GridVar<T>& v)
{
	os << "(";
	for (unsigned i=0; i<(v.size-1); ++i)
		os << (v.data[i]) << ", ";
	os << (v.data[v.size-1]) << ")";
	return os;
} */