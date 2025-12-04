#include "scanner.hpp"
#include "display.hpp"
#include "colors.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

void print_help() {
    std::cout << colors::bold_cyan("dirstat") << " - Ultra-fast directory analyzer\n\n";
    std::cout << colors::bold_white("USAGE:") << "\n";
    std::cout << "    dirstat [COMMAND] [OPTIONS] [PATH]\n\n";
    std::cout << colors::bold_white("COMMANDS:") << "\n";
    std::cout << "    " << colors::green("scan") << "     Scan directory and show statistics (default)\n";
    std::cout << "    " << colors::green("large") << "    Find largest files\n";
    std::cout << "    " << colors::green("tree") << "     Show directory tree structure\n";
    std::cout << "    " << colors::green("dupes") << "    Find potential duplicate files\n";
    std::cout << "    " << colors::green("types") << "    Show file type breakdown\n";
    std::cout << "    " << colors::green("help") << "     Show this help message\n\n";
    std::cout << colors::bold_white("OPTIONS:") << "\n";
    std::cout << "    " << colors::yellow("-H, --hidden") << "     Include hidden files\n";
    std::cout << "    " << colors::yellow("-d, --depth") << " N    Maximum depth (default: 0 = unlimited)\n";
    std::cout << "    " << colors::yellow("-c, --count") << " N    Number of items to show (default: 10)\n";
    std::cout << "    " << colors::yellow("-m, --min") << " N      Minimum file size in bytes (for dupes)\n";
    std::cout << "    " << colors::yellow("-h, --help") << "       Show help\n\n";
    std::cout << colors::bold_white("EXAMPLES:") << "\n";
    std::cout << "    dirstat                    # Scan current directory\n";
    std::cout << "    dirstat scan C:\\Users      # Scan specific path\n";
    std::cout << "    dirstat large -c 20        # Top 20 largest files\n";
    std::cout << "    dirstat tree -d 3          # Tree with depth 3\n";
    std::cout << "    dirstat types -c 15        # Top 15 file types\n";
}

int main(int argc, char* argv[]) {
    colors::enable_colors();
    
    std::vector<std::string> args(argv + 1, argv + argc);
    
    std::string command = "scan";
    fs::path path = ".";
    bool show_hidden = false;
    int depth = 0;
    size_t count = 10;
    uint64_t min_size = 1024;
    
    for (size_t i = 0; i < args.size(); ++i) {
        const std::string& arg = args[i];
        
        if (arg == "-h" || arg == "--help" || arg == "help") {
            print_help();
            return 0;
        } else if (arg == "-H" || arg == "--hidden") {
            show_hidden = true;
        } else if (arg == "-d" || arg == "--depth") {
            if (i + 1 < args.size()) {
                depth = std::stoi(args[++i]);
            }
        } else if (arg == "-c" || arg == "--count") {
            if (i + 1 < args.size()) {
                count = std::stoul(args[++i]);
            }
        } else if (arg == "-m" || arg == "--min") {
            if (i + 1 < args.size()) {
                min_size = std::stoull(args[++i]);
            }
        } else if (arg == "scan" || arg == "large" || arg == "tree" || arg == "dupes" || arg == "types") {
            command = arg;
        } else if (arg[0] != '-') {
            path = arg;
        }
    }
    
    std::cout << colors::bold_cyan("dirstat") << " - Ultra-fast directory analyzer\n" << std::endl;
    
    if (command == "scan") {
        scanner::scan_directory(path, show_hidden, depth);
    } else if (command == "large") {
        scanner::find_largest_files(path, count, show_hidden);
    } else if (command == "tree") {
        if (depth == 0) depth = 3;
        display::show_tree(path, depth, show_hidden);
    } else if (command == "dupes") {
        scanner::find_duplicates(path, min_size);
    } else if (command == "types") {
        scanner::show_file_types(path, count);
    }
    
    return 0;
}
