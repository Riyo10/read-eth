#!/bin/bash

# Build script for Network Diagnostic Tool

set -e

VERSION="1.0.0"
PROJECT_NAME="Network Diagnostic Tool"

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Functions
print_header() {
    echo -e "${BLUE}========================================${NC}"
    echo -e "${BLUE}  $PROJECT_NAME - Build Script${NC}"
    echo -e "${BLUE}========================================${NC}"
    echo ""
}

print_success() {
    echo -e "${GREEN}✓ $1${NC}"
}

print_error() {
    echo -e "${RED}✗ $1${NC}"
}

print_info() {
    echo -e "${YELLOW}ℹ $1${NC}"
}

check_dependencies() {
    print_info "Checking dependencies..."
    
    if ! command -v gcc &> /dev/null; then
        print_error "GCC compiler not found. Please install build-essential."
        exit 1
    fi
    print_success "GCC found"
    
    if ! pkg-config --exists libcurl; then
        print_error "libcurl not found. Please install libcurl4-openssl-dev."
        exit 1
    fi
    print_success "libcurl found"
    
    if ! pkg-config --exists libcurl; then
        print_error "pthread not available"
        exit 1
    fi
    print_success "pthread found"
}

build_project() {
    print_info "Building project..."
    
    mkdir -p build
    cd build
    
    # Try CMake first, fall back to Make
    if command -v cmake &> /dev/null; then
        print_info "Using CMake..."
        cmake .. > /dev/null 2>&1 || true
        cmake --build . --config Release
    else
        print_info "Using Make..."
        cd ..
        make clean > /dev/null 2>&1 || true
        make
        cd build
    fi
    
    if [ -f "network-diagnostic" ] || [ -f "Release/network-diagnostic.exe" ]; then
        print_success "Build completed successfully"
        return 0
    else
        print_error "Build failed"
        return 1
    fi
}

run_project() {
    print_info "Starting $PROJECT_NAME..."
    echo ""
    
    cd ..
    
    if [ -f "build/network-diagnostic" ]; then
        ./build/network-diagnostic
    elif [ -f "build/Release/network-diagnostic.exe" ]; then
        ./build/Release/network-diagnostic.exe
    else
        print_error "Executable not found"
        exit 1
    fi
}

show_usage() {
    cat << EOF
Usage: ./build.sh [OPTION]

Options:
    build       Build the project (default)
    run         Build and run the project
    clean       Clean build artifacts
    help        Show this help message

Examples:
    ./build.sh              # Build
    ./build.sh run          # Build and run
    ./build.sh clean        # Clean

EOF
}

# Main script
main() {
    print_header
    
    case "${1:-build}" in
        build)
            check_dependencies
            build_project
            print_success "Build complete! Run with: ./build/network-diagnostic"
            ;;
        run)
            check_dependencies
            build_project && run_project
            ;;
        clean)
            print_info "Cleaning build artifacts..."
            make clean 2>/dev/null || rm -rf build
            print_success "Clean complete"
            ;;
        help)
            show_usage
            ;;
        *)
            print_error "Unknown option: $1"
            show_usage
            exit 1
            ;;
    esac
    
    echo ""
}

main "$@"
