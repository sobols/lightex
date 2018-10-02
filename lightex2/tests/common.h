#pragma once

#include <string>
#include <vector>

void CheckOK(const std::string& tex, const std::vector<std::string>& expectedElems);
void CheckError(const std::string& tex);
