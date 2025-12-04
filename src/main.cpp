#include "scanner.hpp"
#include "display.hpp"
#include "colors.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <sstream>

namespace fs = std::filesystem;

// Global options
struct Options {
    std::string command = "scan";
    fs::path path = ".";
    bool show_hidden = false;
    bool json_output = false;
    int depth = 0;
    size_t count = 10;
    uint64_t min_size = 1024;
    std::vector<std::string> exclude_patterns;
};

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
    std::cout << "    " << colors::yellow("-H, --hidden") << "       Include hidden files\n";
    std::cout << "    " << colors::yellow("-d, --depth") << " N      Maximum depth (default: 0 = unlimited)\n";
    std::cout << "    " << colors::yellow("-c, --count") << " N      Number of items to show (default: 10)\n";
    std::cout << "    " << colors::yellow("-m, --min") << " N        Minimum file size in bytes (for dupes)\n";
    std::cout << "    " << colors::yellow("-e, --exclude") << " PAT  Exclude patterns (comma-separated)\n";
    std::cout << "    " << colors::yellow("-j, --json") << "         Output as JSON\n";
    std::cout << "    " << colors::yellow("-h, --help") << "         Show help\n\n";
    std::cout << colors::bold_white("EXAMPLES:") << "\n";
    std::cout << "    dirstat                              # Scan current directory\n";
    std::cout << "    dirstat scan C:\\Users                # Scan specific path\n";
    std::cout << "    dirstat large -c 20                  # Top 20 largest files\n";
    std::cout << "    dirstat tree -d 3                    # Tree with depth 3\n";
    std::cout << "    dirstat -e node_modules,.git         # Exclude folders\n";
    std::cout << "    dirstat large --json                 # Output as JSON\n";
}

std::vector<std::string> split_string(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(s);
    std::string token;
    while (std::getline(ss, token, delimiter)) {
        if (!token.empty()) tokens.push_back(token);
    }
    return tokens;
}

int main(int argc, char* argv[]) {
    colors::enable_colors();
    
    std::vector<std::string> args(argv + 1, argv + argc);
    Options opts;
    
    for (size_t i = 0; i < args.size(); ++i) {
        const std::string& arg = args[i];
        
        if (arg == "-h" || arg == "--help" || arg == "help") {
            print_help();
            return 0;
        } else if (arg == "-H" || arg == "--hidden") {
            opts.show_hidden = true;
        } else if (arg == "-j" || arg == "--json") {
            opts.json_output = true;
        } else if (arg == "-d" || arg == "--depth") {
            if (i + 1 < args.size()) {
                opts.depth = std::stoi(args[++i]);
            }
        } else if (arg == "-c" || arg == "--count") {
            if (i + 1 < args.size()) {
                opts.count = std::stoul(args[++i]);
            }
        } else if (arg == "-m" || arg == "--min") {
            if (i + 1 < args.size()) {
                opts.min_size = std::stoull(args[++i]);
            }
        } else if (arg == "-e" || arg == "--exclude") {
            if (i + 1 < args.size()) {
                opts.exclude_patterns = split_string(args[++i], ',');
            }
        } else if (arg == "scan" || arg == "large" || arg == "tree" || arg == "dupes" || arg == "types") {
            opts.command = arg;
        } else if (!arg.empty() && arg[0] != '-') {
            opts.path = arg;
        }
    }
    
    if (!opts.json_output) {
        std::cout << colors::bold_cyan("dirstat") << " - Ultra-fast directory analyzer\n" << std::endl;
    }
    
    if (opts.command == "scan") {
        scanner::scan_directory(opts.path, opts.show_hidden, opts.depth, opts.exclude_patterns, opts.json_output);
    } else if (opts.command == "large") {
        scanner::find_largest_files(opts.path, opts.count, opts.show_hidden, opts.exclude_patterns, opts.json_output);
    } else if (opts.command == "tree") {
        if (opts.depth == 0) opts.depth = 3;
        display::show_tree(opts.path, opts.depth, opts.show_hidden, opts.exclude_patterns);
    } else if (opts.command == "dupes") {
        scanner::find_duplicates(opts.path, opts.min_size, opts.exclude_patterns, opts.json_output);
    } else if (opts.command == "types") {
        scanner::show_file_types(opts.path, opts.count, opts.exclude_patterns, opts.json_output);
    }
    
    return 0;
}
