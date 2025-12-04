#pragma once
#include <filesystem>
#include <cstdint>

namespace fs = std::filesystem;

namespace scanner {

void scan_directory(const fs::path& path, bool show_hidden, int max_depth);
void find_largest_files(const fs::path& path, size_t count, bool show_hidden);
void find_duplicates(const fs::path& path, uint64_t min_size);
void show_file_types(const fs::path& path, size_t count);

} // namespace scanner
