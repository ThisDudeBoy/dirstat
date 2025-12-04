#include "display.hpp"
#include "colors.hpp"
#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>

namespace display {

void show_stats(const DirStats& stats, const fs::path& path) {
    std::cout << std::endl;
    std::cout << colors::bold_cyan("[*] Directory Statistics") << std::endl;
    std::cout << colors::dim(std::string(50, '-')) << std::endl;
    
    std::cout << "  " << colors::white("Path:") << " " << colors::cyan(path.string()) << std::endl;
    std::cout << "  " << colors::white("Files:") << " " << colors::bold_green(std::to_string(stats.total_files)) << std::endl;
    std::cout << "  " << colors::white("Directories:") << " " << colors::yellow(std::to_string(stats.total_dirs)) << std::endl;
    std::cout << "  " << colors::white("Total Size:") << " " << colors::bold_green(format_size(stats.total_size)) << std::endl;
    
    if (stats.largest_file_path.has_value()) {
        std::cout << std::endl;
        std::cout << colors::bold_cyan("[*] Largest File:") << std::endl;
        
        std::error_code ec;
        fs::path relative = fs::relative(stats.largest_file_path.value(), path, ec);
        if (ec) relative = stats.largest_file_path.value();
        
        std::cout << "    " << colors::white(relative.string()) 
                  << " (" << colors::green(format_size(stats.largest_file_size)) << ")" << std::endl;
    }
    
    if (!stats.extensions.empty()) {
        std::cout << std::endl;
        std::cout << colors::bold_cyan("[*] Top File Types:") << std::endl;
        
        std::vector<std::pair<std::string, uint64_t>> sorted_ext(
            stats.extensions.begin(), stats.extensions.end());
        std::sort(sorted_ext.begin(), sorted_ext.end(), 
            [](const auto& a, const auto& b) { return a.second > b.second; });
        
        for (size_t i = 0; i < std::min(size_t(5), sorted_ext.size()); ++i) {
            const auto& [ext, count] = sorted_ext[i];
            
            size_t bar_len = static_cast<size_t>(
                static_cast<double>(count) / stats.total_files * 20.0);
            bar_len = std::max(bar_len, size_t(1));
            std::string bar(bar_len, '#');
            
            std::cout << "    " << colors::cyan("." + ext);
            for (size_t j = ext.length() + 1; j < 10; ++j) std::cout << ' ';
            
            std::string count_str = std::to_string(count);
            for (size_t j = count_str.length(); j < 6; ++j) std::cout << ' ';
            std::cout << colors::yellow(count_str) << " " << colors::green(bar) << std::endl;
        }
    }
    
    std::cout << std::endl;
    std::cout << colors::dim(std::string(50, '-')) << std::endl;
    std::cout << colors::green("[OK] Scan complete!") << std::endl;
}

// Helper to check exclusions
bool is_excluded_display(const fs::path& path, const std::vector<std::string>& exclude) {
    std::string name = path.filename().string();
    for (const auto& pattern : exclude) {
        if (name == pattern || name.find(pattern) != std::string::npos) {
            return true;
        }
    }
    return false;
}

void show_tree(const fs::path& path, int max_depth, bool show_hidden, const std::vector<std::string>& exclude) {
    std::error_code ec;
    fs::path abs_path = fs::absolute(path, ec);
    
    if (!fs::exists(abs_path, ec)) {
        std::cerr << colors::red("[X]") << " Cannot access path: " << path << std::endl;
        return;
    }
    
    std::cout << colors::yellow("[>]") << " Directory Tree: " << colors::cyan(abs_path.string()) << std::endl;
    std::cout << std::endl;
    
    std::string root_name = abs_path.filename().string();
    if (root_name.empty()) root_name = abs_path.string();
    std::cout << colors::bold_cyan(root_name) << std::endl;
    
    std::function<void(const fs::path&, const std::string&, int)> print_tree;
    print_tree = [&](const fs::path& dir, const std::string& prefix, int depth) {
        if (max_depth > 0 && depth > max_depth) return;
        
        std::vector<fs::directory_entry> entries;
        for (const auto& entry : fs::directory_iterator(dir, fs::directory_options::skip_permission_denied, ec)) {
            std::string name = entry.path().filename().string();
            if (!show_hidden && !name.empty() && name[0] == '.') continue;
            if (is_excluded_display(entry.path(), exclude)) continue;
            entries.push_back(entry);
        }
        
        std::sort(entries.begin(), entries.end(), [](const auto& a, const auto& b) {
            return a.path().filename() < b.path().filename();
        });
        
        for (size_t i = 0; i < entries.size(); ++i) {
            const auto& entry = entries[i];
            bool is_last = (i == entries.size() - 1);
            std::string connector = is_last ? "+-- " : "|-- ";
            std::string name = entry.path().filename().string();
            
            if (entry.is_directory(ec)) {
                std::cout << colors::dim(prefix + connector) << colors::bold_blue(name + "/") << std::endl;
                std::string new_prefix = prefix + (is_last ? "    " : "|   ");
                print_tree(entry.path(), new_prefix, depth + 1);
            } else {
                uint64_t size = entry.file_size(ec);
                std::string size_str = ec ? "" : " (" + format_size(size) + ")";
                std::cout << colors::dim(prefix + connector) << colors::white(name) 
                          << colors::dim(size_str) << std::endl;
            }
        }
    };
    
    print_tree(abs_path, "", 1);
    std::cout << std::endl;
}

} // namespace display
