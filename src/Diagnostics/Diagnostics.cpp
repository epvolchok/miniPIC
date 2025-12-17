#include <Diagnostics/Diagnostics.hpp>

void Diagnostics::clear_create_directory(const fs::path& dir, bool clear)
{
    if (fs::exists(dir) && clear)
    {
        fs::remove_all(dir);
    }
    fs::create_directories(dir);
}

void Diagnostics::create_field_dirs(const fs::path& dir, bool clear2D, bool clear1D, bool clearPoint)
{
    fs::create_directories(field_dir);
    fs::create_directories(field_dir / Fields2D_path);
    clear_create_directory(field_dir / Fields2D_path / Fields2D_Electric, clear2D);
    clear_create_directory(field_dir / Fields2D_path / Fields2D_Magnetic, clear2D);
    clear_create_directory(field_dir / Fields2D_path / Fields2D_Current, clear2D);
    

    fs::create_directories(field_dir / Fields1D_path);
    clear_create_directory(field_dir / Fields1D_path / Fields1D_Electric, clear1D);
    clear_create_directory(field_dir / Fields1D_path / Fields1D_Magnetic, clear1D);
    clear_create_directory(field_dir / Fields1D_path / Fields1D_Current, clear1D);

    clear_create_directory(field_dir / FieldsPoint_path, clearPoint);
    clear_create_directory(field_dir / FieldsEnergy_path, clearPoint);
}

void Diagnostics::initialise_point_diag(const fs::path& dir)
{
    if (!point_diag.empty())
    {
        for (const auto& pair:point_diag)
        {
            std::string filename = "PointFieldB_X" + make_filename(pair.first, Nx) + "_Y" + make_filename(pair.second, Ny) + ".txt";
            fs::path file = dir / filename;
            std::ofstream wf1(file, std::ios::out | std::ios::trunc);
            wf1 << "#it    Bx    By    Bz" << std::endl;
            wf1.close();
            filename = "PointFieldE_X" + make_filename(pair.first, Nx) + "_Y" + make_filename(pair.second, Ny) + ".txt";
            file = dir / filename;
            std::ofstream wf2(file, std::ios::out | std::ios::trunc);
            wf2 << "#it    Ex    Ey    Ez" << std::endl;
            wf2.close();
            filename = "PointFieldJ_X" + make_filename(pair.first, Nx) + "_Y" + make_filename(pair.second, Ny) + ".txt";
            file = dir / filename;
            std::ofstream wf3(file, std::ios::out | std::ios::trunc);
            wf3 << "#it    Jx    Jy    Jz" << std::endl;
            wf3.close();
        }
    }

}

void Diagnostics::initialise_energy_diag(const fs::path& dir)
{
    std::string filename = "EBFields_energy.txt";
    fs::path file = dir / filename;
    std::ofstream wf1(file, std::ios::out | std::ios::trunc);
    wf1 << "#it    E    B" << std::endl;
    wf1.close();
}

std::string Diagnostics::make_filename(size_t ind, size_t MaxSize)
{
    assert(ind < MaxSize);
    const auto width = std::to_string(MaxSize).size();
    return std::format("{:0{}}", ind, width);
}

std::string Diagnostics::line_type_str(const GridVar<double>::DiagnLine line_type) 
{
    switch(line_type) {
        case GridVar<double>::DiagnLine::X: return "X";
        case GridVar<double>::DiagnLine::Y: return "Y";
    }
    return "?";
}

void Diagnostics::FieldDiagnostic2D(const FieldGrid &A, std::filesystem::path &subfolder, const std::string &filename_,
                                size_t ind_time)
{
    std::string filename = filename_+ make_filename(ind_time, MaxTime) + ".bin";
    A.write_to_binary(field_dir / Fields2D_path/ subfolder, filename);
}

void Diagnostics::run_2DFieldDiagnostic(const FieldGrid &E, const FieldGrid &B, const FieldGrid &J, size_t ind_time)
{
    if (ind_time % DiagnStep == 0)
    {
        FieldDiagnostic2D(B, Fields2D_Magnetic, "FieldB", ind_time);
        FieldDiagnostic2D(E, Fields2D_Electric, "FieldE", ind_time);
        FieldDiagnostic2D(J, Fields2D_Current, "FieldJ", ind_time);
    }
}

void Diagnostics::FieldDiagnostic1D(const FieldGrid &A, const std::filesystem::path &subfolder, const std::string &filename_,
                                 size_t ind_time, const size_t Coord_ind, const GridVar<double>::DiagnLine line_type)
{
    size_t MaxSize;
    switch (line_type)
    {
    case GridVar<double>::DiagnLine::X:
        MaxSize = Nx;
        break;
    
    case GridVar<double>::DiagnLine::Y:
        MaxSize = Ny;
        break;
    }

    std::string full_name = filename_ + "_" + line_type_str(line_type) + make_filename(Coord_ind, MaxSize) +
                            "_Time" + make_filename(ind_time, MaxTime) + ".txt";
    
    //std::cout<<field_dir / Fields1D_path / subfolder<<std::endl;
    A.write_on_line(field_dir / Fields1D_path / subfolder, full_name, Coord_ind, line_type);
}

void Diagnostics::run_1DFieldDiagnostic(const FieldGrid &E, const FieldGrid &B, const FieldGrid &J, 
                                        size_t ind_time)
{
    //std::cout<<"empty? "<<Xlines_diag.empty()<<std::endl;
    if (!Xlines_diag.empty())
    {
        for (const auto& Xline: Xlines_diag)
        {
            FieldDiagnostic1D(E, Fields1D_Electric, "FieldE", ind_time, Xline, GridVar<double>::DiagnLine::X);
            FieldDiagnostic1D(B, Fields1D_Magnetic, "FieldB", ind_time, Xline, GridVar<double>::DiagnLine::X);
            FieldDiagnostic1D(J, Fields1D_Current, "FieldJ", ind_time, Xline, GridVar<double>::DiagnLine::X);
        }
    }
    //std::cout<<Ylines_diag.empty()<<std::endl;
    if (!Ylines_diag.empty())
    {
        for (const auto& Yline: Ylines_diag)
        {
            FieldDiagnostic1D(E, Fields1D_Electric, "FieldE", ind_time, Yline, GridVar<double>::DiagnLine::Y);
            FieldDiagnostic1D(B, Fields1D_Magnetic, "FieldB", ind_time, Yline, GridVar<double>::DiagnLine::Y);
            FieldDiagnostic1D(J, Fields1D_Current, "FieldJ", ind_time, Yline, GridVar<double>::DiagnLine::Y);
        }
    }
}

void Diagnostics::FieldDiagnostic_point(const FieldGrid &A, const std::string &filename_,
                                    const size_t ix, const size_t iy, size_t ind_time)
{
    std::filesystem::path path = field_dir / FieldsPoint_path;
    std::string full_name = filename_ + "_X" + make_filename(ix, Nx) + "_Y" + make_filename(iy, Ny) + ".txt";
    A.write_at_point(path, full_name, ix, iy, ind_time);
}
void Diagnostics::run_pointFieldDiagnostic(const FieldGrid &E,const FieldGrid &B, const FieldGrid &J, size_t ind_time)
{
    if (!point_diag.empty())
    {
        for (const auto& pair: point_diag)
        {
            FieldDiagnostic_point(E, "PointFieldE", pair.first, pair.second, ind_time);
            FieldDiagnostic_point(B, "PointFieldB", pair.first, pair.second, ind_time);
            FieldDiagnostic_point(J, "PointFieldJ", pair.first, pair.second, ind_time);
        }
    }
}

void Diagnostics::FieldDiagnostic_Energy(const FieldGrid &E, const FieldGrid &B, size_t ind_time)
{
    std::filesystem::path path = field_dir / FieldsEnergy_path;
    std::string filename = "EBFields_energy.txt";
    std::ofstream wf(path / filename, std::ios::out | std::ios::app);
    double E_en=0.0;
    double B_en=0.0;
    if (!wf.is_open())
    {
    throw std::runtime_error("Error openning the file: " + (path / filename).string());
        }
    E_en = E.Energy();
    B_en = B.Energy();
    //std::cout<<"ind "<<ind_time<<" E energy "<<E_en<<std::endl;
    //std::cout<<"ind "<<ind_time<<" B energy "<<B_en<<std::endl;
    wf<<ind_time<<"\t"<<E_en<<"\t"<<B_en<<std::endl;
    wf.close();
    if (wf.fail()) {
    throw std::runtime_error("Error writing to file: " + (path / filename).string());
       }
}