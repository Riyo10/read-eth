# Network Diagnostic Tool

A full-scale, production-ready network diagnostic application built in C with a modern, responsive web-based UI. Monitor your internet speed, check your IP address, ISP information, and network statistics in real-time.

![Version](https://img.shields.io/badge/version-1.0.0-blue)
![License](https://img.shields.io/badge/license-MIT-green)
![Platform](https://img.shields.io/badge/platform-Linux%20%7C%20macOS%20%7C%20Unix-lightgrey)

## Features

✨ **Core Features**
- 🚀 **Internet Speed Test** - Measure download/upload speeds and ping latency
- 📍 **IP Address Detection** - Display both IPv4 and IPv6 addresses
- 🌍 **ISP & Location Info** - Get ISP name, country, city, timezone, and coordinates
- 📊 **Network Statistics** - View bytes sent/received on active interfaces
- 🎯 **Real-time Monitoring** - Auto-refresh network data every 30 seconds
- 🗺️ **Interactive Map** - View location on embedded OpenStreetMap

## Technical Stack

### Backend
- **Language**: C (POSIX)
- **Libraries**: 
  - `libcurl` - HTTP requests and speed testing
  - `pthreads` - Multi-threaded HTTP server
  - Standard C libraries for network operations
- **Architecture**: Multi-threaded HTTP server on port 8080
- **Network APIs**: 
  - System calls for local network info (`ifaddrs`, `/proc/net/`)
  - ipapi.co for ISP/location information

### Frontend
- **HTML5** - Semantic markup
- **CSS3** - Modern styling with Flexbox/Grid
- **Vanilla JavaScript** - No dependencies, ~300 lines total
- **Design**: Dark theme, glassmorphism, responsive layout

## Project Structure

```
read-eth/
├── src/
│   ├── main.c              # Application entry point
│   ├── network.c           # Network utilities implementation
│   ├── server.c            # HTTP server implementation
│   └── json.c              # JSON builder utilities
├── include/
│   ├── network.h           # Network utilities header
│   ├── server.h            # Server header
│   └── json.h              # JSON utilities header
├── web/
│   ├── index.html          # Main UI
│   └── static/
│       ├── css/
│       │   └── style.css   # Styling
│       └── js/
│           ├── api.js      # API client
│           ├── ui.js       # UI state manager
│           └── main.js     # Application logic
├── CMakeLists.txt          # CMake build configuration
├── Makefile                # Alternative build system
└── README.md               # This file
```

## Installation

### Prerequisites

**Linux (Ubuntu/Debian):**
```bash
sudo apt-get update
sudo apt-get install -y build-essential libcurl4-openssl-dev
```

**macOS:**
```bash
brew install curl
```

**Other Linux distributions:**
```bash
# Fedora/RHEL
sudo dnf install gcc curl-devel

# Arch
sudo pacman -S base-devel curl
```

### Build with Make (Recommended)

```bash
cd ~/Documents/read-eth

# Build the project
make

# Or build and run directly
make run

# Clean build artifacts
make clean
```

### Build with CMake

```bash
cd ~/Documents/read-eth

# Create build directory
mkdir -p build && cd build

# Configure and build
cmake ..
cmake --build . --config Release

# Run the application
./network-diagnostic
```

### Manual Compilation

```bash
cd ~/Documents/read-eth

# Compile all source files
gcc -Wall -Wextra -O2 -std=c11 -I./include \
    src/main.c src/network.c src/server.c src/json.c \
    -o build/network-diagnostic \
    -lcurl -lpthread -lm

# Run
./build/network-diagnostic
```

## Usage

### Start the Server

```bash
# Run with default port (8080)
./build/network-diagnostic

# Run with custom port
./build/network-diagnostic -p 9000

# Show help
./build/network-diagnostic --help

# Show version
./build/network-diagnostic --version
```

### Access the Web UI

Open your browser and navigate to:
```
http://localhost:8080
```

The application will:
1. Load and display current network information
2. Show ISP and location details
3. Display network statistics
4. Allow you to run speed tests

### API Endpoints

The backend exposes RESTful JSON APIs:

```
GET /api/network-info
{
    "ipv4": "192.168.1.100",
    "ipv6": "fe80::1",
    "gateway": "192.168.1.1",
    "dns1": "8.8.8.8",
    "dns2": "8.8.4.4"
}

GET /api/isp-info
{
    "isp": "AS1234 ISP Name",
    "country": "United States",
    "city": "San Francisco",
    "latitude": "37.7749",
    "longitude": "-122.4194",
    "timezone": "America/Los_Angeles"
}

GET /api/interface-stats
{
    "interface": "eth0",
    "bytes_sent": "1024000000",
    "bytes_received": "2048000000"
}

GET /api/speed-test
{
    "download_mbps": 85.5,
    "upload_mbps": 45.2,
    "ping_ms": 15.3,
    "test_time": 1704067200
}
```

## Features in Detail

### Network Information
- **IPv4 Address**: Your local IPv4 address
- **IPv6 Address**: Your local IPv6 address (if available)
- **Gateway**: Default gateway IP
- **DNS Servers**: Primary and secondary DNS servers
- **Interface**: Active network interface name

### Speed Test
- **Download Speed**: Measured in Mbps
- **Upload Speed**: Measured in Mbps
- **Ping**: Latency in milliseconds
- **Interactive Gauge**: Visual representation of download speed
- **Progress Tracking**: Real-time test progress display

### ISP & Location
- **ISP Name**: Internet Service Provider information
- **Country**: Geographic location (country)
- **City**: City name
- **Timezone**: Local timezone
- **Coordinates**: Latitude/Longitude for map display
- **Interactive Map**: Embedded OpenStreetMap showing your location

### Network Statistics
- **Bytes Sent**: Total bytes transmitted on active interface
- **Bytes Received**: Total bytes received on active interface
- **Auto-refresh**: Updates every 30 seconds

## Code Architecture

### Backend Architecture

**Main Server Loop** (`server.c`):
- Accepts incoming HTTP connections
- Creates thread pool for handling multiple clients
- Routes requests to appropriate handlers
- Sends JSON responses with proper CORS headers

**Network Module** (`network.c`):
- System-level network information gathering
- CURL-based speed testing
- ISP detection via external API
- Interface statistics from `/proc/net/`

**JSON Utilities** (`json.c`):
- Lightweight JSON builder (no external dependencies)
- String escaping and formatting
- Efficient memory management

### Frontend Architecture

**API Module** (`api.js`):
- Fetch wrapper with timeout handling
- Four main endpoints: network-info, isp-info, interface-stats, speed-test
- Error handling and response parsing

**UI Module** (`ui.js`):
- DOM element management
- Display update logic
- Error/success notifications
- Byte formatting utilities

**Main App** (`main.js`):
- Event listener setup
- Data loading orchestration
- Speed test workflow
- Auto-refresh timer

## Performance Considerations

- **Threading**: Multi-threaded server handles concurrent requests
- **Lightweight**: No external web frameworks, minimal dependencies
- **Caching**: Network info cached in browser (30-second refresh)
- **Speed Test**: Non-blocking progress simulation for UX
- **Memory**: Efficient buffer management in C backend

## Security Features

- **CORS Headers**: Proper cross-origin support
- **Input Validation**: Safe HTTP parsing
- **Error Handling**: Graceful error responses
- **No SQL/Database**: No injection vulnerabilities
- **Read-only Operations**: No state modification from HTTP

## Troubleshooting

### Connection Refused
```
Error: Failed to fetch from localhost:8080
```
- Ensure the server is running: `./build/network-diagnostic`
- Check if port 8080 is available: `lsof -i :8080`
- Try custom port: `./build/network-diagnostic -p 9000`

### Missing Dependencies
```
error: curl.h: No such file or directory
```
- Install libcurl development files (see Prerequisites)

### Build Errors
```
undefined reference to 'curl_*'
```
- Make sure `pkg-config` is installed
- Verify libcurl is properly installed

### Speed Test Timeout
- Ensure internet connectivity
- Check firewall rules
- Try again - external API might be temporarily unavailable

## Development

### Adding New Features

1. **Add C API endpoint**:
   - Add function in `include/network.h`
   - Implement in `src/network.c`
   - Add handler in `src/server.c`

2. **Add UI component**:
   - Add HTML in `web/index.html`
   - Add styles in `web/static/css/style.css`
   - Update `web/static/js/main.js` for logic

3. **Rebuild**:
   ```bash
   make clean && make run
   ```

### Testing

Test individual endpoints:
```bash
curl http://localhost:8080/api/network-info
curl http://localhost:8080/api/isp-info
curl http://localhost:8080/api/interface-stats
curl http://localhost:8080/api/speed-test
```

## Limitations

- Speed test reliability depends on external API availability
- ISP detection requires internet connectivity
- IPv6 support depends on system configuration
- Speed test measured against single server (not geographically optimized)

## Future Enhancements

- [ ] Multi-server speed test (average)
- [ ] Historical data tracking
- [ ] Network alerts and notifications
- [ ] Dark/Light theme toggle
- [ ] Connection quality analysis
- [ ] Mobile app (React Native/Flutter)
- [ ] Database for historical data
- [ ] Advanced packet analysis
- [ ] VPN detection
- [ ] Network latency graph

## Performance Metrics

- **Startup Time**: < 100ms
- **Network Info Load**: < 500ms
- **ISP Info Load**: 1-3s (API dependent)
- **Speed Test**: 15-30s
- **UI Response**: < 100ms
- **Memory Usage**: ~10-20 MB

## License

MIT License - See LICENSE file for details

## Contributing

Contributions are welcome! Please:
1. Fork the repository
2. Create feature branch (`git checkout -b feature/amazing-feature`)
3. Commit changes (`git commit -m 'Add amazing feature'`)
4. Push to branch (`git push origin feature/amazing-feature`)
5. Open Pull Request

## Author

Built with ❤️ as a full-scale production C application

## Support

For issues, questions, or suggestions:
- Open an issue on GitHub
- Check existing documentation
- Review troubleshooting section

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

### MIT License
- ✅ Commercial use
- ✅ Modification
- ✅ Distribution
- ✅ Private use
- ⚠️ Liability
- ⚠️ Warranty

## Acknowledgments

- libcurl - HTTP client library
- POSIX standards for system programming
- ipapi.co - Free IP geolocation API
- OpenStreetMap - Map data provider

---

**Version 1.0.0** | Last Updated: 2025-12-31 | Licensed under [MIT](LICENSE)
