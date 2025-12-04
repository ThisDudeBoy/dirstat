#pragma once
#include <filesystem>
#include <cstdint>
#include <vector>
#include <string>

namespace fs = std::filesystem;

namespace scanner {

void scan_directory(const fs::path& path, bool show_hidden, int max_depth, 
                   const std::vector<std::string>& exclude, bool json_output);
void find_largest_files(const fs::path& path, size_t count, bool show_hidden,
                       const std::vector<std::string>& exclude, bool json_output);
void find_duplicates(const fs::path& path, uint64_t min_size,
                    const std::vector<std::string>& exclude, bool json_output);
void show_file_types(const fs::path& path, size_t count,
                    const std::vector<std::string>& exclude, bool json_output);

} // namespace scanner
