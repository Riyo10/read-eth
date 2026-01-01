# Network Diagnostic Tool - Installation & Setup Guide

## Quick Start (5 minutes)

### 1. Install Dependencies

**Ubuntu/Debian:**
```bash
sudo apt-get update
sudo apt-get install -y build-essential libcurl4-openssl-dev git
```

**Fedora/RHEL:**
```bash
sudo dnf groupinstall "Development Tools"
sudo dnf install curl-devel
```

**macOS:**
```bash
brew install curl
```

### 2. Clone/Enter Project Directory

```bash
cd ~/Documents/read-eth
```

### 3. Build the Project

**Option A: Using build script (easiest)**
```bash
chmod +x build.sh
./build.sh run
```

**Option B: Using Make**
```bash
make run
```

**Option C: Using CMake**
```bash
mkdir -p build && cd build
cmake .. && cmake --build .
./network-diagnostic
```

### 4. Access the Web UI

Open in your browser: **http://localhost:8080**

## Detailed Build Instructions

### Prerequisites Check

Verify you have all required packages:

```bash
# Check GCC
gcc --version

# Check libcurl
curl --version

# Check pkg-config
pkg-config --version

# Verify libcurl dev files
pkg-config --cflags --libs libcurl
```

### Complete Installation Steps

#### Linux (Ubuntu 20.04+)

```bash
# 1. Update package manager
sudo apt-get update

# 2. Install essential packages
sudo apt-get install -y \
    build-essential \
    libcurl4-openssl-dev \
    pkg-config \
    git \
    cmake

# 3. Navigate to project
cd ~/Documents/read-eth

# 4. Build
make clean && make

# 5. Run
./build/network-diagnostic
```

#### Linux (Fedora 35+)

```bash
# 1. Install packages
sudo dnf groupinstall "Development Tools"
sudo dnf install \
    gcc \
    curl-devel \
    pkgconfig \
    cmake \
    git

# 2. Navigate to project
cd ~/Documents/read-eth

# 3. Build with CMake
mkdir -p build && cd build
cmake .. && cmake --build .

# 4. Run
./network-diagnostic
```

#### macOS

```bash
# 1. Install Homebrew (if not installed)
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# 2. Install packages
brew install curl cmake git

# 3. Navigate to project
cd ~/Documents/read-eth

# 4. Build
make clean && make

# 5. Run
./build/network-diagnostic
```

### Troubleshooting Build Issues

#### Issue: "curl.h: No such file or directory"

**Solution:**
```bash
# Ubuntu/Debian
sudo apt-get install libcurl4-openssl-dev

# Fedora
sudo dnf install curl-devel

# macOS
brew install curl
```

#### Issue: "cannot find -lpthread"

**Solution:**
```bash
# Usually pre-installed, but if missing:
# Ubuntu
sudo apt-get install libc6-dev

# Fedora
sudo dnf install glibc-devel
```

#### Issue: "cmake not found"

**Solution:**
```bash
# Use Makefile instead
make run

# Or install CMake
# Ubuntu
sudo apt-get install cmake

# Fedora
sudo dnf install cmake
```

#### Issue: Permission denied when running script

**Solution:**
```bash
chmod +x build.sh
chmod +x build/network-diagnostic
```

## Compilation Details

### Full Build Command (Manual)

```bash
gcc -Wall -Wextra -O2 -std=c11 \
    -I./include \
    -o build/network-diagnostic \
    src/main.c \
    src/network.c \
    src/server.c \
    src/json.c \
    -lcurl -lpthread -lm
```

### Compiler Flags Explained

- `-Wall -Wextra`: Show all warnings
- `-O2`: Optimization level 2
- `-std=c11`: Use C11 standard
- `-I./include`: Include header files
- `-lcurl`: Link libcurl library
- `-lpthread`: Link pthread library
- `-lm`: Link math library

## Running the Application

### Default Port (8080)

```bash
./build/network-diagnostic
```

Navigate to: **http://localhost:8080**

### Custom Port

```bash
./build/network-diagnostic -p 9000
```

Navigate to: **http://localhost:9000**

### Command-line Options

```bash
./build/network-diagnostic --help
./build/network-diagnostic --version
./build/network-diagnostic -p <PORT>
```

### Running in Background

```bash
# Run in background
./build/network-diagnostic &

# Run with nohup (survives terminal close)
nohup ./build/network-diagnostic > app.log 2>&1 &

# Check if running
pgrep -f network-diagnostic
```

## System Requirements

### Minimum Requirements
- CPU: Any modern processor (x86, ARM)
- RAM: 100 MB
- Storage: 5 MB
- OS: Linux, macOS, or Unix-like system

### Recommended
- CPU: Dual-core or better
- RAM: 512 MB+
- Storage: 20 MB (with web assets)
- OS: Ubuntu 20.04+, Fedora 35+, macOS 10.15+

## Network Requirements

- **Internet Connection**: Yes (for speed test and ISP info)
- **Ports**: 8080 (configurable)
- **Outbound Access**: 
  - ipapi.co (ISP information)
  - speedtest servers (speed test)

## Development Setup

### IDE Configuration

**VS Code:**
```json
{
    "c_cpp_properties": {
        "includePath": [
            "${workspaceFolder}/include"
        ]
    },
    "launch": {
        "program": "${workspaceFolder}/build/network-diagnostic"
    }
}
```

**CLion:**
- Open project directory
- CMake should auto-configure

### Debugging

**With GDB:**
```bash
# Compile with debug symbols
gcc -g -I./include -o build/network-diagnostic \
    src/main.c src/network.c src/server.c src/json.c \
    -lcurl -lpthread -lm

# Run with debugger
gdb ./build/network-diagnostic
```

**With LLDB (macOS):**
```bash
lldb ./build/network-diagnostic
```

## Installation as System Service

### Creating a systemd Service (Linux)

Create `/etc/systemd/system/network-diagnostic.service`:

```ini
[Unit]
Description=Network Diagnostic Tool
After=network.target

[Service]
Type=simple
User=www-data
WorkingDirectory=/opt/network-diagnostic
ExecStart=/usr/local/bin/network-diagnostic -p 8080
Restart=on-failure
RestartSec=10

[Install]
WantedBy=multi-user.target
```

Then:
```bash
# Copy executable
sudo cp build/network-diagnostic /usr/local/bin/

# Enable and start service
sudo systemctl daemon-reload
sudo systemctl enable network-diagnostic
sudo systemctl start network-diagnostic

# Check status
sudo systemctl status network-diagnostic

# View logs
sudo journalctl -u network-diagnostic -f
```

## Docker Deployment (Optional)

Create `Dockerfile`:

```dockerfile
FROM ubuntu:22.04

RUN apt-get update && apt-get install -y \
    build-essential \
    libcurl4-openssl-dev

WORKDIR /app
COPY . .

RUN make clean && make

EXPOSE 8080

CMD ["./build/network-diagnostic"]
```

Build and run:
```bash
docker build -t network-diagnostic .
docker run -p 8080:8080 network-diagnostic
```

## Next Steps

1. **Open the web UI**: http://localhost:8080
2. **Run a speed test**: Click "Start Speed Test" button
3. **Check network info**: View all network details
4. **Read the docs**: See README.md for full documentation

## Getting Help

If you encounter issues:

1. Check the **Troubleshooting** section above
2. Verify dependencies: `make help` or `./build.sh`
3. Check build output for errors
4. Ensure port 8080 is not in use: `lsof -i :8080`
5. Try a clean rebuild: `make clean && make`

## Performance Optimization

For production use:

1. **Build with optimizations**:
   ```bash
   gcc -O3 -march=native -I./include ...
   ```

2. **Enable compiler optimizations**:
   ```bash
   CFLAGS="-O3 -march=native" make
   ```

3. **Run with proper resource limits**:
   ```bash
   ulimit -n 65536  # Max file descriptors
   ```

## Uninstallation

```bash
# Clean build
make clean

# Remove installed binary
sudo rm -f /usr/local/bin/network-diagnostic

# Remove service (if installed)
sudo systemctl disable network-diagnostic
sudo systemctl stop network-diagnostic
sudo rm -f /etc/systemd/system/network-diagnostic.service
sudo systemctl daemon-reload
```

---

**Need help?** Check the main README.md or visit the project repository.
