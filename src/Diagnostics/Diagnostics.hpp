#pragma once
#include <filesystem>
#include <cassert>
#include <string>
#include <format>
#include <set>

#include <FieldsCls.hpp>

namespace fs = std::filesystem;

class Diagnostics
{
    private:
    size_t MaxTime; //will most likely be presented differently
    size_t Nx, Ny; //will most likely be presented differently
    std::filesystem::path folder_name;
    size_t DiagnStep;
    std::set<std::pair<size_t, size_t>> point_diag; //pairs of (x, y), in cells
    std::set<size_t> Xlines_diag; //coordinates of fixed x to itterate through y, in cells
    std::set<size_t> Ylines_diag; //coordinates of fixed y to itterate through x, in cells

    fs::path field_dir;
    fs::path Fields2D_path = "2D";
    fs::path Fields2D_Electric = "ElectricFields";
    fs::path Fields2D_Magnetic = "MagneticFields";
    fs::path Fields2D_Current = "Current";
    fs::path Fields1D_path = "1D";
    fs::path Fields1D_Electric = "ElectricFields";
    fs::path Fields1D_Magnetic = "MagneticFields";
    fs::path Fields1D_Current = "Current";
    fs::path FieldsPoint_path = "Point";
    fs::path FieldsEnergy_path = "Energy";

    public:
    Diagnostics(): 
    MaxTime{0}, folder_name{}, DiagnStep{}, point_diag{}, Xlines_diag{}, Ylines_diag{} {};

    Diagnostics(const std::filesystem::path &folder_name_, size_t MaxTime_, size_t Nx_, size_t Ny_, size_t DiagnStep_,
                std::set<std::pair<size_t, size_t>> point_diag_={},
                std::set<size_t> Xlines_diag_={},
                std::set<size_t> Ylines_diag_={},
                bool clear2D=false, bool clear1D=false, bool clearPoint=false): 
        MaxTime{MaxTime_}, Nx{Nx_}, Ny{Ny_},
        folder_name{folder_name_},
        DiagnStep{DiagnStep_},
        point_diag{std::move(point_diag_)},
        Xlines_diag{std::move(Xlines_diag_)},
        Ylines_diag{std::move(Ylines_diag_)}
    {
        //std::cout<<"Xlines in Diag: "<<Xlines_diag.empty()<<std::endl;
        //std::cout<<"Ylines in Diag: "<<Ylines_diag.empty()<<std::endl;
        

        fs::create_directories(folder_name);
        field_dir = folder_name / "Fields";
        create_field_dirs(field_dir, clear2D, clear1D, clearPoint);

        initialise_point_diag(field_dir / FieldsPoint_path);
        initialise_energy_diag(field_dir / FieldsEnergy_path);
    }

    void clear_create_directory(const fs::path& dir, bool clear);
    void create_field_dirs(const fs::path& dir, bool clear2D, bool clear1D, bool clearPoint);
    void initialise_point_diag(const fs::path& dir);
    void Diagnostics::initialise_energy_diag(const fs::path& dir);
    
    std::string make_filename(size_t ind, size_t MaxSize);
    std::string line_type_str(const GridVar<double>::DiagnLine line_type); 


    void run_2DFieldDiagnostic(const FieldGrid &E, const FieldGrid &B, const FieldGrid &J, size_t ind_time);
    void FieldDiagnostic2D(const FieldGrid &A, std::filesystem::path &subfolder, const std::string &filename_,
                            size_t ind_time);

    void FieldDiagnostic1D(const FieldGrid &A, const std::filesystem::path &subfolder, const std::string &filename_,
                                const size_t ind_time, const size_t Coord_ind, const GridVar<double>::DiagnLine line_type);
    void run_1DFieldDiagnostic(const FieldGrid &E, const FieldGrid &B, const FieldGrid &J, const size_t ind_time);

    void run_pointFieldDiagnostic(const FieldGrid &E,const FieldGrid &B, const FieldGrid &J, const size_t ind_time);
    void FieldDiagnostic_point(const FieldGrid &A, const std::string &filename_,
                                    const size_t ix, const size_t iy, const size_t ind_time);
    void Diagnostics::FieldDiagnostic_Energy(const FieldGrid &E, const FieldGrid &B, size_t ind_time);


};