#pragma once
#include "stats.hpp"
#include <filesystem>

namespace fs = std::filesystem;

namespace display {

void show_stats(const DirStats& stats, const fs::path& path);
void show_tree(const fs::path& path, int max_depth, bool show_hidden);

} // namespace display
