# ⚡ dirstat

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://isocpp.org/)
[![Windows](https://img.shields.io/badge/Platform-Windows-lightgrey.svg)](https://www.microsoft.com/windows)
[![PRs Welcome](https://img.shields.io/badge/PRs-welcome-brightgreen.svg)](http://makeapullrequest.com)

> **Lightning-fast directory analyzer** - Know exactly what's eating your disk space in milliseconds.

Ever wondered where all your disk space went? **dirstat** scans directories at blazing speed and gives you instant insights about file sizes, types, and potential duplicates.

<p align="center">
  <img src="https://img.shields.io/badge/Speed-Blazing%20Fast-orange?style=for-the-badge" />
  <img src="https://img.shields.io/badge/Size-273KB-green?style=for-the-badge" />
  <img src="https://img.shields.io/badge/Dependencies-Zero-blue?style=for-the-badge" />
</p>

---

## 🎯 Features

- 📊 **Directory Statistics** - Total files, folders, and size at a glance
- 📏 **Find Large Files** - Instantly locate space hogs
- 🌳 **Tree View** - Beautiful ASCII directory tree
- 🔍 **Duplicate Finder** - Find potential duplicates by size
- 📋 **File Types Analysis** - See which extensions consume the most space
- 🎨 **Colored Output** - Easy-to-read terminal output
- ⚡ **Zero Dependencies** - Single binary, no runtime needed
- 🚀 **Instant Results** - Scans 100,000+ files in under a second

---

## 📦 Installation

### Download Binary (Recommended)

Download the latest release from [Releases](https://github.com/ThisDudeBoy/dirstat/releases).

### Build from Source

```bash
git clone https://github.com/ThisDudeBoy/dirstat.git
cd dirstat
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

The binary will be at `build/dirstat.exe`

---

## 🚀 Usage

### Quick Scan
```bash
# Scan current directory
dirstat

# Scan specific path
dirstat scan C:\Users\MyFolder
```

### Find Largest Files
```bash
# Top 10 largest files (default)
dirstat large

# Top 20 largest files
dirstat large -c 20
```

### Directory Tree
```bash
# Show tree (depth 3 by default)
dirstat tree

# Custom depth
dirstat tree -d 5

# Include hidden files
dirstat tree -H
```

### Find Duplicates
```bash
# Find potential duplicates (files with same size)
dirstat dupes

# Set minimum file size (1MB)
dirstat dupes -m 1000000
```

### File Type Breakdown
```bash
# Analyze file types
dirstat types

# Top 20 types
dirstat types -c 20
```

---

## 📸 Example Output

```
dirstat - Ultra-fast directory analyzer

[>] Scanning: C:\Projects\MyApp
    Analyzing directory...

[*] Directory Statistics
--------------------------------------------------
  Path: C:\Projects\MyApp
  Files: 1,234
  Directories: 89
  Total Size: 256.7 MB

[*] Largest File:
    assets/video.mp4 (45.2 MB)

[*] Top File Types:
    .js           423 ############
    .json         201 ######
    .md            89 ###
    .ts            45 ##
    .css           34 #

--------------------------------------------------
[OK] Scan complete!
```

---

## ⚙️ Options

| Option | Description |
|--------|-------------|
| `-H, --hidden` | Include hidden files and directories |
| `-d, --depth N` | Maximum scan depth (0 = unlimited) |
| `-c, --count N` | Number of items to display |
| `-m, --min N` | Minimum file size in bytes (for dupes) |
| `-h, --help` | Show help message |

---

## 🔧 Commands

| Command | Description |
|---------|-------------|
| `scan` | Scan directory and show statistics (default) |
| `large` | Find largest files |
| `tree` | Show directory tree structure |
| `dupes` | Find potential duplicate files |
| `types` | Show file type breakdown |
| `help` | Show help message |

---

## 💡 Why dirstat?

| Feature | dirstat | Other tools |
|---------|---------|-------------|
| Speed | ⚡ Instant | 🐢 Slow |
| Size | 273 KB | 10+ MB |
| Dependencies | None | Many |
| Install | Download & run | Complex setup |

---

## 🛠️ Tech Stack

- **Language**: C++17
- **Build System**: CMake
- **Libraries**: Standard Library only (no external deps)

---

## 📝 License

MIT License - see [LICENSE](LICENSE) for details.

---

## 🤝 Contributing

Contributions are welcome! Feel free to:

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing`)
5. Open a Pull Request

---

## ⭐ Support

If you find this tool useful, please consider giving it a **star**! ⭐

It helps others discover this project and motivates continued development.

---

<p align="center">
  Made with ❤️ in C++
</p>
