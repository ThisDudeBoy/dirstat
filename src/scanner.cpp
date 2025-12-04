#include "scanner.hpp"
#include "display.hpp"
#include "stats.hpp"
#include "colors.hpp"
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <functional>

namespace scanner {

// Check if entry should be skipped (hidden files)
bool should_skip(const fs::directory_entry& entry, bool show_hidden) {
    if (show_hidden) return false;
    std::string name = entry.path().filename().string();
    return !name.empty() && name[0] == '.';
}

// Recursive scan helper
void scan_recursive(const fs::path& path, DirStats& stats, bool show_hidden, int max_depth, int current_depth) {
    if (max_depth > 0 && current_depth > max_depth) return;
    
    std::error_code ec;
    for (const auto& entry : fs::directory_iterator(path, fs::directory_options::skip_permission_denied, ec)) {
        if (should_skip(entry, show_hidden)) continue;
        
        if (entry.is_regular_file(ec)) {
            uint64_t size = entry.file_size(ec);
            if (!ec) {
                stats.total_files++;
                stats.total_size += size;
                
                if (size > stats.largest_file_size) {
                    stats.largest_file_size = size;
                    stats.largest_file_path = entry.path();
                }
                
                // Track extension
                std::string ext = entry.path().extension().string();
                if (ext.empty()) {
                    ext = "(no ext)";
                } else {
                    ext = ext.substr(1);
                    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
                }
                stats.extensions[ext]++;
            }
        } else if (entry.is_directory(ec)) {
            stats.total_dirs++;
            scan_recursive(entry.path(), stats, show_hidden, max_depth, current_depth + 1);
        }
    }
}

void scan_directory(const fs::path& path, bool show_hidden, int max_depth) {
    std::error_code ec;
    fs::path abs_path = fs::absolute(path, ec);
    
    if (!fs::exists(abs_path, ec)) {
        std::cerr << colors::red("[X]") << " Cannot access path: " << path << std::endl;
        return;
    }
    
    std::cout << colors::yellow("[>]") << " Scanning: " << colors::cyan(abs_path.string()) << std::endl;
    std::cout << colors::dim("    Analyzing directory...") << std::endl;
    
    DirStats stats;
    scan_recursive(abs_path, stats, show_hidden, max_depth, 0);
    
    display::show_stats(stats, abs_path);
}

void find_largest_files(const fs::path& path, size_t count, bool show_hidden) {
    std::error_code ec;
    fs::path abs_path = fs::absolute(path, ec);
    
    if (!fs::exists(abs_path, ec)) {
        std::cerr << colors::red("[X]") << " Cannot access path: " << path << std::endl;
        return;
    }
    
    std::cout << colors::yellow("[>]") << " Finding " << colors::green(std::to_string(count)) 
              << " largest files in: " << colors::cyan(abs_path.string()) << std::endl;
    std::cout << colors::dim("    Scanning files...") << std::endl;
    
    std::vector<std::pair<uint64_t, fs::path>> files;
    
    std::function<void(const fs::path&)> collect_files = [&](const fs::path& dir) {
        std::error_code ec;
        for (const auto& entry : fs::directory_iterator(dir, fs::directory_options::skip_permission_denied, ec)) {
            if (should_skip(entry, show_hidden)) continue;
            
            if (entry.is_regular_file(ec)) {
                uint64_t size = entry.file_size(ec);
                if (!ec) {
                    files.emplace_back(size, entry.path());
                }
            } else if (entry.is_directory(ec)) {
                collect_files(entry.path());
            }
        }
    };
    
    collect_files(abs_path);
    
    std::sort(files.begin(), files.end(), [](const auto& a, const auto& b) {
        return a.first > b.first;
    });
    
    std::cout << std::endl;
    std::cout << colors::bold_cyan("[*] Largest Files:") << std::endl;
    std::cout << colors::dim(std::string(60, '-')) << std::endl;
    
    for (size_t i = 0; i < std::min(count, files.size()); ++i) {
        const auto& [size, file_path] = files[i];
        fs::path relative = fs::relative(file_path, abs_path, ec);
        if (ec) relative = file_path;
        
        std::cout << colors::yellow(std::to_string(i + 1) + ".") << " "
                  << colors::bold_green(format_size(size)) << " "
                  << colors::white(relative.string()) << std::endl;
    }
    
    if (files.empty()) {
        std::cout << colors::dim("  No files found.") << std::endl;
    }
}

void find_duplicates(const fs::path& path, uint64_t min_size) {
    std::error_code ec;
    fs::path abs_path = fs::absolute(path, ec);
    
    if (!fs::exists(abs_path, ec)) {
        std::cerr << colors::red("[X]") << " Cannot access path: " << path << std::endl;
        return;
    }
    
    std::cout << colors::yellow("[>]") << " Finding potential duplicates (min size: " 
              << colors::green(format_size(min_size)) << ")" << std::endl;
    std::cout << colors::dim("    Scanning files...") << std::endl;
    
    std::map<uint64_t, std::vector<fs::path>> size_map;
    
    std::function<void(const fs::path&)> collect_files = [&](const fs::path& dir) {
        std::error_code ec;
        for (const auto& entry : fs::directory_iterator(dir, fs::directory_options::skip_permission_denied, ec)) {
            std::string name = entry.path().filename().string();
            if (!name.empty() && name[0] == '.') continue;
            
            if (entry.is_regular_file(ec)) {
                uint64_t size = entry.file_size(ec);
                if (!ec && size >= min_size) {
                    size_map[size].push_back(entry.path());
                }
            } else if (entry.is_directory(ec)) {
                collect_files(entry.path());
            }
        }
    };
    
    collect_files(abs_path);
    
    std::vector<std::pair<uint64_t, std::vector<fs::path>>> duplicates;
    for (auto& [size, paths] : size_map) {
        if (paths.size() > 1) {
            duplicates.emplace_back(size, std::move(paths));
        }
    }
    
    std::sort(duplicates.begin(), duplicates.end(), [](const auto& a, const auto& b) {
        return a.first > b.first;
    });
    
    std::cout << std::endl;
    std::cout << colors::bold_cyan("[*] Potential Duplicates (same size):") << std::endl;
    std::cout << colors::dim(std::string(60, '-')) << std::endl;
    
    if (duplicates.empty()) {
        std::cout << colors::dim("  No potential duplicates found.") << std::endl;
        return;
    }
    
    size_t shown = 0;
    for (const auto& [size, paths] : duplicates) {
        if (shown++ >= 10) break;
        
        std::cout << std::endl;
        std::cout << colors::bold_green(format_size(size)) << " (" 
                  << colors::yellow(std::to_string(paths.size())) << " files):" << std::endl;
        
        size_t file_shown = 0;
        for (const auto& p : paths) {
            if (file_shown++ >= 5) {
                std::cout << colors::dim("    ... and " + std::to_string(paths.size() - 5) + " more...") << std::endl;
                break;
            }
            fs::path relative = fs::relative(p, abs_path, ec);
            if (ec) relative = p;
            std::cout << "    " << colors::white(relative.string()) << std::endl;
        }
    }
}

void show_file_types(const fs::path& path, size_t count) {
    std::error_code ec;
    fs::path abs_path = fs::absolute(path, ec);
    
    if (!fs::exists(abs_path, ec)) {
        std::cerr << colors::red("[X]") << " Cannot access path: " << path << std::endl;
        return;
    }
    
    std::cout << colors::yellow("[>]") << " Analyzing file types in: " 
              << colors::cyan(abs_path.string()) << std::endl;
    std::cout << colors::dim("    Scanning files...") << std::endl;
    
    std::map<std::string, std::pair<uint64_t, uint64_t>> ext_stats;
    
    std::function<void(const fs::path&)> analyze = [&](const fs::path& dir) {
        std::error_code ec;
        for (const auto& entry : fs::directory_iterator(dir, fs::directory_options::skip_permission_denied, ec)) {
            std::string name = entry.path().filename().string();
            if (!name.empty() && name[0] == '.') continue;
            
            if (entry.is_regular_file(ec)) {
                uint64_t size = entry.file_size(ec);
                if (!ec) {
                    std::string ext = entry.path().extension().string();
                    if (ext.empty()) {
                        ext = "(no ext)";
                    } else {
                        ext = ext.substr(1);
                        std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
                    }
                    ext_stats[ext].first++;
                    ext_stats[ext].second += size;
                }
            } else if (entry.is_directory(ec)) {
                analyze(entry.path());
            }
        }
    };
    
    analyze(abs_path);
    
    std::vector<std::pair<std::string, std::pair<uint64_t, uint64_t>>> sorted(
        ext_stats.begin(), ext_stats.end());
    std::sort(sorted.begin(), sorted.end(), [](const auto& a, const auto& b) {
        return a.second.second > b.second.second;
    });
    
    std::cout << std::endl;
    std::cout << colors::bold_cyan("[*] File Types by Size:") << std::endl;
    std::cout << colors::dim(std::string(60, '-')) << std::endl;
    
    printf("%s%-12s %10s %12s%s\n", 
           colors::BOLD_WHITE.c_str(), "Extension", "Count", "Total Size", colors::RESET.c_str());
    std::cout << colors::dim(std::string(60, '-')) << std::endl;
    
    for (size_t i = 0; i < std::min(count, sorted.size()); ++i) {
        const auto& [ext, data] = sorted[i];
        const auto& [file_count, total_size] = data;
        
        std::cout << colors::cyan("." + ext);
        for (size_t j = ext.length() + 1; j < 12; ++j) std::cout << ' ';
        
        std::string count_str = std::to_string(file_count);
        for (size_t j = count_str.length(); j < 10; ++j) std::cout << ' ';
        std::cout << colors::yellow(count_str);
        
        std::string size_str = format_size(total_size);
        for (size_t j = size_str.length(); j < 12; ++j) std::cout << ' ';
        std::cout << colors::green(size_str) << std::endl;
    }
}

} // namespace scanner
