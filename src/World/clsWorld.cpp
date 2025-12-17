#include "clsWorld.hpp"

fs::path World::find_param_file()
{
    fs::path cwd = fs::current_path();
    fs::path param_path = {};
    for (auto & p : fs::directory_iterator(cwd))
    {   //std::cout<<p.path()<<std::endl;
        if (p.path().filename().string().starts_with("Param"))
        {
            //std::cout<<p.path()<<std::endl;
            param_path = p.path();
            break;
        }
    }
    if (fs::is_empty(param_path))
    {
        throw std::runtime_error("Parameter file can not be found!");
    }
    return param_path;
}

World::ParamDict World::read_param(fs::path path)
{
    World::ParamDict Parameters;
    std::ifstream fin(path);
    if (!fin.is_open()) {
        throw std::runtime_error("Can not  open Parameter file!");
    }
    std::string folder_name;
    std::getline(fin, folder_name);
    

    if (folder_name.empty()) {
        throw std::runtime_error("Can not find a folder name in Parameter file");
    }
    Parameters["Folder"] = folder_name;
    std::cout << std::get<std::string>(Parameters.at("Folder")) << std::endl;
    
    std::string line;
    std::string param;
    std::string value;
    const std::unordered_set<std::string> sizet_params = {"Nx", "Ny", "MaxTime", "DiagnStep"};
    const std::unordered_set<std::string> double_params = {"dx", "dy", "dt"};
    while (std::getline(fin, line))
    {   
        std::istringstream iss(line);
        iss >> param;
        if (sizet_params.contains(param))
        {
            size_t temp;
            iss >> temp;
            Parameters[param] = temp;

            std::cout<<"size_t "<<param<<" "<<temp<<std::endl;

        }
        else if (double_params.contains(param))
        {
            double temp;
            iss >> temp;
            Parameters[param] = temp;

            std::cout<<"double "<<param<<" "<<temp<<std::endl;
        }
    }
    fin.close();

    return Parameters;
}