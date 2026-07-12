#pragma once

#include <string>
#include <vector>

namespace core::platform {

bool resourceExists(const std::string& path);
bool readResourceBytes(const std::string& path, std::vector<unsigned char>& bytes);

} // namespace core::platform
