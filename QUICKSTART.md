# Quick Start Guide - Network Diagnostic Tool

## 5-Minute Setup

### Step 1: Install Dependencies (1 minute)

**Ubuntu/Debian:**
```bash
sudo apt-get update
sudo apt-get install -y build-essential libcurl4-openssl-dev
```

**Fedora:**
```bash
sudo dnf groupinstall "Development Tools"
sudo dnf install curl-devel
```

**macOS:**
```bash
brew install curl
```

### Step 2: Build the Application (1 minute)

```bash
cd ~/Documents/read-eth
make clean && make
```

**Or use the build script:**
```bash
chmod +x build.sh
./build.sh run
```

### Step 3: Open in Browser (1 minute)

The server starts automatically on **http://localhost:8080**

Open your browser and navigate to: `http://localhost:8080`

---

## Using the Application

### Dashboard Overview

The web interface shows four main sections:

#### 1. Network Information (Top Left)
Shows your current network configuration:
- **IPv4 Address**: Your local IP address
- **IPv6 Address**: IPv6 if available
- **Gateway**: Your router's IP
- **DNS Servers**: Primary and secondary DNS
- **Interface**: Active network interface name

**Auto-refreshes every 30 seconds**

#### 2. Speed Test (Center)
Measure your internet connection:
1. Click **"Start Speed Test"** button
2. Test runs automatically (15-30 seconds)
3. Results show:
   - Download speed (Mbps)
   - Upload speed (Mbps)
   - Ping latency (ms)
   - Visual gauge for download speed

#### 3. ISP & Location (Bottom Right)
Your internet provider and location info:
- **ISP Name**: Your internet service provider
- **Country**: Where your connection originates
- **City**: Approximate city
- **Timezone**: Local timezone
- **Coordinates**: GPS location (if available)
- **Interactive Map**: Shows your location

#### 4. Network Statistics (Bottom Left)
Real-time network data:
- **Bytes Sent**: Total data uploaded
- **Bytes Received**: Total data downloaded
- Auto-formatted in B, KB, MB, GB

---

## Command Line Options

### Basic Usage

```bash
# Start with default port (8080)
./build/network-diagnostic

# Start with custom port
./build/network-diagnostic -p 9000

# Show version
./build/network-diagnostic --version

# Show help
./build/network-diagnostic --help
```

### Access Different Ports

```bash
# Default (port 8080)
http://localhost:8080

# Custom port 9000
http://localhost:9000

# Remote server
http://192.168.1.100:8080
```

---

## API Examples

### Using curl to Test Endpoints

```bash
# Get network information
curl http://localhost:8080/api/network-info

# Get speed test results
curl http://localhost:8080/api/speed-test

# Get ISP information
curl http://localhost:8080/api/isp-info

# Get network statistics
curl http://localhost:8080/api/interface-stats
```

### Response Examples

**Network Info:**
```json
{
    "ipv4":"192.168.1.100",
    "ipv6":"fe80::1",
    "gateway":"192.168.1.1",
    "dns1":"8.8.8.8",
    "dns2":"8.8.4.4"
}
```

**Speed Test:**
```json
{
    "download_mbps":85.5,
    "upload_mbps":45.2,
    "ping_ms":15.3,
    "test_time":1704067200
}
```

---

## Troubleshooting

### Port Already in Use

**Error:** "Address already in use"

**Solution:** Use a different port
```bash
./build/network-diagnostic -p 8081
```

Or find and stop the process:
```bash
lsof -i :8080
# Kill the process if needed
kill -9 <PID>
```

### Speed Test Not Working

**Possible causes:**
- No internet connection
- Firewall blocking outbound connections
- External API temporarily down

**Solution:**
- Check internet connection: `ping google.com`
- Try again in a few moments
- Check your firewall settings

### Cannot Build

**Error:** "curl.h: No such file or directory"

**Solution:**
```bash
# Ubuntu
sudo apt-get install libcurl4-openssl-dev

# Fedora
sudo dnf install curl-devel

# macOS
brew install curl
```

### Browser Shows "Cannot Connect"

**Problem:** Browser can't reach localhost:8080

**Solution:**
```bash
# Make sure server is running
./build/network-diagnostic

# Check if port is open
netstat -tlnp | grep 8080

# Try explicit localhost
curl http://127.0.0.1:8080
```

---

## Common Use Cases

### 1. Check Network Configuration

```bash
# Start server
./build/network-diagnostic

# Open browser
# http://localhost:8080
```
Instantly see all network details and DNS configuration.

### 2. Test Internet Speed

Click **"Start Speed Test"** button on the web UI.

Results display:
- Download speed
- Upload speed
- Latency (ping)

### 3. Identify Your ISP

Navigate to **"ISP & Location"** section to see:
- ISP name
- Country and city
- Map location

### 4. Monitor Network Usage

Check **"Network Statistics"** for:
- Total bytes sent/received
- Network interface info
- Real-time data

---

## Advanced Usage

### Run on Remote Server

1. Upload to server:
```bash
scp -r ~/Documents/read-eth user@server.com:/home/user/
```

2. Build on server:
```bash
ssh user@server.com
cd read-eth
make clean && make
```

3. Run on custom port:
```bash
./build/network-diagnostic -p 8080 &
```

4. Access from client:
```
http://server.com:8080
```

### Run as Background Service

```bash
# Start in background
./build/network-diagnostic > app.log 2>&1 &

# Check if running
pgrep -f network-diagnostic

# View logs
tail -f app.log

# Stop
pkill network-diagnostic
```

### Integrate with Monitoring

```bash
# Get JSON API data programmatically
curl -s http://localhost:8080/api/network-info | jq

# Store in database
curl -s http://localhost:8080/api/speed-test > speed_test.json

# Use in scripts
if [ $(curl -s http://localhost:8080/api/network-info | jq -r '.ipv4') = "192.168.1.100" ]; then
    echo "Network OK"
fi
```

---

## Performance Tips

### Optimize for Speed

1. **Run locally first**:
   ```bash
   ./build/network-diagnostic -p 8080
   ```

2. **Use Chrome/Firefox for best performance**

3. **Clear browser cache** for fresh load:
   - Chrome: Ctrl+Shift+Delete
   - Firefox: Ctrl+Shift+Delete

### Reduce Server Load

1. **Disable auto-refresh** (modify `main.js`)
2. **Run fewer concurrent tests**
3. **Use custom port** if default is busy

---

## File Structure Reference

```
read-eth/
├── build/
│   └── network-diagnostic    ← Run this!
├── web/
│   ├── index.html           ← Opens in browser
│   └── static/
│       ├── css/style.css    ← Styling
│       └── js/
│           ├── api.js       ← API calls
│           ├── ui.js        ← UI updates
│           └── main.js      ← Logic
├── src/
│   ├── main.c               ← Entry point
│   ├── network.c            ← Network ops
│   ├── server.c             ← HTTP server
│   └── json.c               ← JSON utilities
├── Makefile                 ← Build system
└── README.md                ← Full docs
```

---

## Keyboard Shortcuts

| Action | Shortcut |
|--------|----------|
| Refresh Network Info | Click refresh button or wait 30s |
| Start Speed Test | Click "Start Speed Test" |
| Refresh ISP Info | Click refresh button |
| Refresh Stats | Click refresh button |
| Hard reload page | Ctrl+Shift+R (Chrome) |
| | Cmd+Shift+R (macOS) |

---

## Tips & Tricks

### 1. Fastest Speed Test
- Close other browser tabs
- Disconnect VPN if using one
- Test multiple times for average

### 2. Accurate Location
- Your ISP's location may differ from actual location
- Usually accurate within city level
- Uses IP geolocation database

### 3. Monitor Over Time
- Run tests at different times of day
- Compare with ISP's promised speeds
- Document patterns for support calls

### 4. Share Results
- Screenshot the web page
- Export JSON from API
- Share network info easily

---

## What's Happening Behind the Scenes

### When You Load the Page

1. Browser requests `http://localhost:8080/`
2. Server returns `index.html`
3. Browser loads CSS and JavaScript
4. JavaScript makes API calls
5. Page displays results

### When You Run Speed Test

1. JavaScript calls `/api/speed-test`
2. C backend tests actual download speed
3. Measures upload speed
4. Measures latency (ping)
5. Returns results as JSON
6. JavaScript displays results

### Network Information Updates

1. System reads `/proc/net/` files
2. Gets IPv4/IPv6 from system interfaces
3. Reads DNS from `/etc/resolv.conf`
4. Returns all info as JSON
5. Updates every 30 seconds automatically

---

## Getting Help

### If Something Breaks

1. **Check logs**:
   ```bash
   # Terminal output shows errors
   ```

2. **Restart server**:
   ```bash
   pkill network-diagnostic
   ./build/network-diagnostic
   ```

3. **Check browser console**:
   - Press F12
   - Click "Console" tab
   - Look for errors

4. **Read documentation**:
   - `README.md` - Full documentation
   - `INSTALL.md` - Installation help
   - `PROJECT_SUMMARY.md` - Architecture details

---

## Next Steps

1. ✅ Build the application
2. ✅ Start the server
3. ✅ Open in browser
4. ✅ Run a speed test
5. 📖 Read detailed docs in `README.md`
6. 🔧 Customize for your needs
7. 🚀 Deploy to production

---

**Congratulations! Your Network Diagnostic Tool is ready to use! 🎉**

For detailed documentation, see [README.md](README.md)
