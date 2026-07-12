#include "core/platform/resource_io.h"

#include <filesystem>
#include <fstream>
#include <limits>

#if defined(__OHOS__)
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_iostream.h>
#endif

namespace core::platform {

bool resourceExists(const std::string& path) {
    if (path.empty()) {
        return false;
    }

    std::error_code error;
    if (std::filesystem::exists(path, error) && !error) {
        return true;
    }

#if defined(__OHOS__)
    if (std::filesystem::path(path).is_absolute()) {
        return false;
    }
    SDL_IOStream* stream = SDL_IOFromFile(path.c_str(), "rb");
    if (stream == nullptr) {
        SDL_ClearError();
        return false;
    }
    SDL_CloseIO(stream);
    return true;
#else
    return false;
#endif
}

bool readResourceBytes(const std::string& path, std::vector<unsigned char>& bytes) {
    bytes.clear();
    if (path.empty()) {
        return false;
    }

#if defined(__OHOS__)
    size_t size = 0;
    void* data = SDL_LoadFile(path.c_str(), &size);
    if (data != nullptr) {
        const auto* begin = static_cast<const unsigned char*>(data);
        bytes.assign(begin, begin + size);
        SDL_free(data);
        return !bytes.empty();
    }
    SDL_ClearError();
#endif

    std::ifstream file(path, std::ios::binary);
    if (!file.good()) {
        return false;
    }
    file.seekg(0, std::ios::end);
    const std::streamoff streamSize = file.tellg();
    if (streamSize <= 0 || static_cast<unsigned long long>(streamSize) >
                         static_cast<unsigned long long>(std::numeric_limits<std::size_t>::max())) {
        return false;
    }
    file.seekg(0, std::ios::beg);
    bytes.resize(static_cast<std::size_t>(streamSize));
    file.read(reinterpret_cast<char*>(bytes.data()), streamSize);
    if (!file.good() && !file.eof()) {
        bytes.clear();
        return false;
    }
    return true;
}

} // namespace core::platform
