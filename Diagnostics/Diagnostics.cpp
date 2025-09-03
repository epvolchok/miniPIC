#include "Diagnostics.hpp"

std::string Diagnostics::make_filename(const std::string &filename, size_t ind, const std::string &filetype)
{
    assert(ind < MaxTime);
    const auto width = std::to_string(MaxTime).size();
    return std::format("{}{:0{}}.{}",
                   filename, ind, width, filetype);
}

std::string Diagnostics::line_type_str(const GridVar<double>::DiagnLine line_type) 
{
    switch(line_type) {
        case GridVar<double>::DiagnLine::X: return "X";
        case GridVar<double>::DiagnLine::Y: return "Y";
    }
    return "?";
}

void Diagnostics::run_2DFieldDiagnostic(const FieldGrid &A, std::filesystem::path &subfolder, const std::string &filename_,
                                size_t ind_time)
{
    std::filesystem::create_directories(folder_name / subfolder);
    std::string filename = make_filename(filename_, ind_time, "bin");
    A.write_to_binary(folder_name / subfolder, filename);
}

void Diagnostics::run_1DFieldDiagnostic(const FieldGrid &A, const std::filesystem::path &subfolder, const std::string &filename_,
                                const size_t ind_time, const size_t Coord_ind, const GridVar<double>::DiagnLine line_type)
{
    std::filesystem::create_directories(folder_name / subfolder);
    std::string full_name = filename_ + "_" + line_type_str(line_type) + std::to_string(Coord_ind);
    std::string filename = make_filename(full_name, ind_time, "txt");
    A.write_on_line(folder_name / subfolder, filename, Coord_ind, line_type);
}

void Diagnostics::run_pointFieldDiagnostic(const FieldGrid &A, const std::filesystem::path &subfolder, const std::string &filename_,
                                    const size_t ix, const size_t iy)
{
    std::filesystem::path path = folder_name / subfolder;
    std::filesystem::create_directories(path);
    std::string full_name = filename_ + "_XY" + std::to_string(ix) + "_" + std::to_string(iy) + ".txt";
    A.write_at_point(path, full_name, ix, iy);
}