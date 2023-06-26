#pragma once

#include <string>

std::string getUserPrefPath(const std::string& appName, const std::string& organizationName);
std::string trimWhitespace(std::string_view string);
