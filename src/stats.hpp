#pragma once
#include <string>
#include <map>
#include <optional>
#include <filesystem>

namespace fs = std::filesystem;

struct DirStats {
    uint64_t total_files = 0;
    uint64_t total_dirs = 0;
    uint64_t total_size = 0;
    uint64_t largest_file_size = 0;
    std::optional<fs::path> largest_file_path;
    std::map<std::string, uint64_t> extensions;
};

// Format bytes to human readable
inline std::string format_size(uint64_t bytes) {
    const char* units[] = {"B", "KB", "MB", "GB", "TB"};
    int unit_index = 0;
    double size = static_cast<double>(bytes);
    
    while (size >= 1024.0 && unit_index < 4) {
        size /= 1024.0;
        unit_index++;
    }
    
    char buffer[32];
    if (unit_index == 0) {
        snprintf(buffer, sizeof(buffer), "%llu B", bytes);
    } else {
        snprintf(buffer, sizeof(buffer), "%.1f %s", size, units[unit_index]);
    }
    return std::string(buffer);
}
