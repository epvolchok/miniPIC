#include <Fields/FieldsCls.hpp>

double FieldGrid::getX(size_t ix, size_t iy) const
{
        return Ax.get(ix, iy);
    }
double FieldGrid::getY(size_t ix, size_t iy) const
{
        return Ay.get(ix, iy);
}
void FieldGrid::setX(double val, size_t ix, size_t iy)
{
    Ax.set(val, ix, iy);
}
void FieldGrid::setY(double val, size_t ix, size_t iy)
{
    Ay.set(val, ix, iy);
}
void FieldGrid::setZ(double val, size_t ix, size_t iy)
{
    Az.set(val, ix, iy);
}

FieldGrid::FieldRef FieldGrid::operator()(size_t ix, size_t iy)
{
    assert(ix < Nx && iy < Ny);
    return { Ax(ix,iy), Ay(ix,iy), Az(ix,iy) };
}
    

FieldGrid::ConstFieldRef FieldGrid::operator()(size_t ix, size_t iy) const
{
    assert(ix < Nx && iy < Ny);
    return { Ax(ix,iy), Ay(ix,iy), Az(ix,iy) };
}

    

void FieldGrid::write_to_binary(const std::filesystem::path& path, const std::string &filename) const
{

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

void FieldGrid::write_on_line(const std::filesystem::path& path, const std::string &filename, size_t ind, GridVar<double>::DiagnLine line_type) const
{

    std::ofstream wf(path / filename);
    if (!wf.is_open())
    {
    throw std::runtime_error("Error openning the file: " + (path / filename).string());
       }
    wf<<"#Ax    Ay    Az"<<std::endl;
    switch (line_type)
    {
        case GridVar<double>::DiagnLine::X:
        {
            for (auto iy=0; iy<Ny; ++iy)
                wf<<Ax(ind, iy)<<"\t"<<Ay(ind, iy)<<"\t"<<Az(ind, iy)<<std::endl;
            break;
        }
        case GridVar<double>::DiagnLine::Y:
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
void FieldGrid::write_at_point(const std::filesystem::path& path, const std::string &filename, size_t ix, size_t iy, size_t it) const
{
    std::ofstream wf(path / filename, std::ios::out | std::ios::app);
    if (!wf.is_open())
    {
    throw std::runtime_error("Error openning the file: " + (path / filename).string());
        }

    wf<<it<<"\t"<<Ax(ix, iy)<<"\t"<<Ay(ix, iy)<<"\t"<<Az(ix, iy)<<std::endl;
            wf.close();
    if (wf.fail()) {
    throw std::runtime_error("Error writing to file: " + (path / filename).string());
       }
}

double FieldGrid::Energy() const
{
    double En = 0.0;
    for (size_t iy=0; iy<Ny; ++iy)
    {
        for (size_t ix=0; ix<Nx; ++ix)
        {
            En += Ax(ix, iy) * Ax(ix, iy) + Ay(ix, iy) * Ay(ix, iy) + Az(ix, iy) * Az(ix, iy);
        }
    }
    //std::cout<<"energy 1 "<<En<<std::endl;
    //std::cout<<"dx "<<dx<<"dy "<<dy<<std::endl;
    En = En * 0.5 * dx * dy;
    //std::cout<<"energy 2 "<<En<<std::endl;
    return En;
}