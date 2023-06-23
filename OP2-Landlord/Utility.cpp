#include "Utility.h"


#include <stdexcept>
#include <memory>

#include <SDL2/SDL.h>


namespace
{
    struct SdlStringDeleter
    {
        void operator()(char* string)
        {
            SDL_free(string);
        }
    };

    using SdlString = std::unique_ptr<char, SdlStringDeleter>;
}


std::string getUserPrefPath(const std::string& appName, const std::string& organizationName)
{
    const auto pathPtr = SdlString{ SDL_GetPrefPath(organizationName.c_str(), appName.c_str()) };
    if (pathPtr.get() == nullptr)
    {
        throw std::runtime_error("Error getting PrefPath: " + std::string{ SDL_GetError() });
    }

    return pathPtr.get();
}
