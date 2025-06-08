#include <windows.h>
#include <filesystem>
#include <iostream>
#include <string>

#include <torchcs/properties/properties.hpp>

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        try {
            torchcs::properties server_properties;

            server_properties.load_file("server.properties");

            if (!server_properties.exists()) {
                std::cerr << "server.properties not found." << std::endl;
                return FALSE;
            }

            server_properties.load();

            std::string runtime_path_str = server_properties.get("shared-runtime-path");
            std::filesystem::path runtime_path(runtime_path_str);

            if (!runtime_path.is_absolute()) {
                CHAR module_path[MAX_PATH] = {0};
                if (!GetModuleFileNameA(hModule, module_path, MAX_PATH)) {
                    std::cerr << "Failed to get module file name." << std::endl;
                    return FALSE;
                }

                std::filesystem::path base_path(module_path);
                base_path = base_path.parent_path();

                runtime_path = base_path / runtime_path;
            }

            runtime_path = std::filesystem::absolute(runtime_path);

            if (!std::filesystem::exists(runtime_path)) {
                std::cerr << "Runtime path does not exist: " << runtime_path << std::endl;
                return FALSE;
            }

            if (!LoadLibraryA(runtime_path.string().c_str())) {
                std::cerr << "Failed to load DLL: " << runtime_path << std::endl;
                return FALSE;
            }

        } catch (const std::exception& ex) {
            std::cerr << "Exception in DllMain: " << ex.what() << std::endl;
            return FALSE;
        }
    }

    return TRUE;
}
