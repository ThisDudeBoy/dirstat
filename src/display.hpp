#pragma once
#include "stats.hpp"
#include <filesystem>
#include <vector>
#include <string>

namespace fs = std::filesystem;

namespace display {

void show_stats(const DirStats& stats, const fs::path& path);
void show_tree(const fs::path& path, int max_depth, bool show_hidden, const std::vector<std::string>& exclude);

} // namespace display
