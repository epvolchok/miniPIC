#pragma once
#include <filesystem>
#include <cassert>
#include <string>
#include <format>

#include "FieldsCls.hpp"

template <typename T>
class Diagnostics
{
    private:
    size_t MaxTime;
    std::filesystem::path folder_name;

    public:
    Diagnostics(): MaxTime{0}, folder_name{} {};
    Diagnostics(const std::filesystem::path &folder_name_, size_t MaxTime_): 
        MaxTime{MaxTime_},
        folder_name{folder_name_}
    {
        std::filesystem::create_directories(folder_name);
    }

    std::string make_filename(const std::string &filename, size_t ind, const std::string &filetype)
    {
        assert(ind < MaxTime);
        const auto width = std::to_string(MaxTime).size();
        return std::format("{}{:0{}}.{}",
                       filename, ind, width, filetype);
    }

    std::string line_type_str(const GridVar<T>::DiagnLine line_type) 
    {
        switch(line_type) {
            case GridVar<T>::DiagnLine::X: return "X";
            case GridVar<T>::DiagnLine::Y: return "Y";
        }
        return "?";
    }

    void run_2DFieldDiagnostic(const FieldGrid<T> &A, std::filesystem::path &subfolder, const std::string &filename_,
                                size_t ind_time)
    {
        std::filesystem::create_directories(folder_name / subfolder);
        std::string filename = make_filename(filename_, ind_time, "bin");
        A.write_to_binary(folder_name / subfolder, filename);
    }

    void run_1DFieldDiagnostic(const FieldGrid<T> &A, const std::filesystem::path &subfolder, const std::string &filename_,
                                const size_t ind_time, const size_t Coord_ind, const GridVar<T>::DiagnLine line_type)
    {
        std::filesystem::create_directories(folder_name / subfolder);
        std::string full_name = filename_ + "_" + line_type_str(line_type) + std::to_string(Coord_ind);
        std::string filename = make_filename(full_name, ind_time, "txt");
        A.write_on_line(folder_name / subfolder, filename, Coord_ind, line_type);
    }

    void run_pointFieldDiagnostic(const FieldGrid<T> &A, const std::filesystem::path &subfolder, const std::string &filename_,
                                    const size_t ix, const size_t iy)
    {
        std::filesystem::path path = folder_name / subfolder;
        std::filesystem::create_directories(path);
        std::string full_name = filename_ + "_XY" + std::to_string(ix) + "_" + std::to_string(iy) + ".txt";
        A.write_at_point(path, full_name, ix, iy);
    }

};