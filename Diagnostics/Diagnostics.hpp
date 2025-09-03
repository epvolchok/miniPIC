#pragma once
#include <filesystem>
#include <cassert>
#include <string>
#include <format>

#include "FieldsCls.hpp"


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

    std::string make_filename(const std::string &filename, size_t ind, const std::string &filetype);
    std::string line_type_str(const GridVar<double>::DiagnLine line_type); 


    void run_2DFieldDiagnostic(const FieldGrid &A, std::filesystem::path &subfolder, const std::string &filename_,
                                size_t ind_time);
    void run_1DFieldDiagnostic(const FieldGrid &A, const std::filesystem::path &subfolder, const std::string &filename_,
                                const size_t ind_time, const size_t Coord_ind, const GridVar<double>::DiagnLine line_type);
    void run_pointFieldDiagnostic(const FieldGrid &A, const std::filesystem::path &subfolder, const std::string &filename_,
                                    const size_t ix, const size_t iy);


};