// UI module for updating the DOM

class UI {
    constructor() {
        this.elements = {
            // Network info
            ipv4: document.getElementById('ipv4'),
            ipv6: document.getElementById('ipv6'),
            gateway: document.getElementById('gateway'),
            dns1: document.getElementById('dns1'),
            dns2: document.getElementById('dns2'),
            interface: document.getElementById('interface'),

            // Speed test
            downloadSpeed: document.getElementById('downloadSpeed'),
            uploadSpeed: document.getElementById('uploadSpeed'),
            pingValue: document.getElementById('pingValue'),
            gaugeFill: document.getElementById('gaugeFill'),
            startSpeedTestBtn: document.getElementById('startSpeedTestBtn'),
            speedTestProgress: document.getElementById('speedTestProgress'),
            progressFill: document.getElementById('progressFill'),
            progressText: document.getElementById('progressText'),

            // ISP info
            ispName: document.getElementById('ispName'),
            country: document.getElementById('country'),
            city: document.getElementById('city'),
            timezone: document.getElementById('timezone'),
            coordinates: document.getElementById('coordinates'),

            // Stats
            bytesSent: document.getElementById('bytesSent'),
            bytesReceived: document.getElementById('bytesReceived'),

            // Loading
            loadingIndicator: document.getElementById('loadingIndicator'),

            // Buttons
            refreshNetworkBtn: document.getElementById('refreshNetworkBtn'),
            refreshISPBtn: document.getElementById('refreshISPBtn'),
            refreshStatsBtn: document.getElementById('refreshStatsBtn'),

            // Map
            map: document.getElementById('map'),
        };
    }

    /**
     * Update network information display
     */
    updateNetworkInfo(data) {
        if (data.ipv4) this.elements.ipv4.textContent = data.ipv4;
        if (data.ipv6) this.elements.ipv6.textContent = data.ipv6 || 'Not available';
        if (data.gateway) this.elements.gateway.textContent = data.gateway;
        if (data.dns1) this.elements.dns1.textContent = data.dns1;
        if (data.dns2) this.elements.dns2.textContent = data.dns2;
    }

    /**
     * Update interface information
     */
    updateInterfaceInfo(data) {
        if (data.interface) this.elements.interface.textContent = data.interface;
    }

    /**
     * Update speed test results
     */
    updateSpeedTest(data) {
        // Update speed values
        this.elements.downloadSpeed.textContent = Math.round(data.download_mbps * 10) / 10;
        this.elements.uploadSpeed.textContent = Math.round(data.upload_mbps * 10) / 10;
        this.elements.pingValue.textContent = Math.round(data.ping_ms * 10) / 10;

        // Update gauge
        const maxSpeed = 200; // 200 Mbps for full gauge
        const percentage = Math.min((data.download_mbps / maxSpeed) * 100, 100);
        const circumference = 251; // SVG arc circumference
        const dashOffset = circumference - (circumference * percentage / 100);
        this.elements.gaugeFill.style.strokeDasharray = `${circumference - dashOffset} ${circumference}`;
    }

    /**
     * Update ISP information
     */
    updateISPInfo(data) {
        this.elements.ispName.textContent = data.isp || 'Unknown';
        this.elements.country.textContent = data.country || 'Unknown';
        this.elements.city.textContent = data.city || 'Unknown';
        this.elements.timezone.textContent = data.timezone || 'Unknown';

        if (data.latitude && data.longitude && data.latitude !== 'N/A' && data.longitude !== 'N/A') {
            this.elements.coordinates.textContent = `${data.latitude}, ${data.longitude}`;
            this.updateMap(data.latitude, data.longitude);
        } else {
            this.elements.coordinates.textContent = 'N/A, N/A';
            this.elements.map.innerHTML = '<p style="padding: 20px; text-align: center; color: #a0a0a0;">Location data unavailable</p>';
        }
    }

    /**
     * Update statistics
     */
    updateStats(data) {
        this.elements.bytesSent.textContent = this.formatBytes(parseInt(data.bytes_sent));
        this.elements.bytesReceived.textContent = this.formatBytes(parseInt(data.bytes_received));
    }

    /**
     * Show loading indicator
     */
    showLoading(show = true) {
        if (show) {
            this.elements.loadingIndicator.classList.remove('hidden');
        } else {
            this.elements.loadingIndicator.classList.add('hidden');
        }
    }

    /**
     * Show speed test progress
     */
    showSpeedTestProgress(show = true, text = '') {
        if (show) {
            this.elements.speedTestProgress.classList.remove('hidden');
            if (text) this.elements.progressText.textContent = text;
        } else {
            this.elements.speedTestProgress.classList.add('hidden');
        }
    }

    /**
     * Update progress bar
     */
    updateProgress(percentage, text = '') {
        this.elements.progressFill.style.width = percentage + '%';
        if (text) this.elements.progressText.textContent = text;
    }

    /**
     * Disable/Enable speed test button
     */
    setSpeedTestButtonState(enabled = true) {
        this.elements.startSpeedTestBtn.disabled = !enabled;
        if (!enabled) {
            this.elements.startSpeedTestBtn.textContent = 'Testing...';
        } else {
            this.elements.startSpeedTestBtn.textContent = 'Start Speed Test';
        }
    }

    /**
     * Format bytes to human readable format
     */
    formatBytes(bytes) {
        if (bytes === 0) return '0 B';

        const k = 1024;
        const sizes = ['B', 'KB', 'MB', 'GB', 'TB'];
        const i = Math.floor(Math.log(bytes) / Math.log(k));

        return Math.round((bytes / Math.pow(k, i)) * 100) / 100 + ' ' + sizes[i];
    }

    /**
     * Update map with location (using embedded HTML)
     */
    updateMap(latitude, longitude) {
        try {
            // Use OpenStreetMap with Leaflet CDN (optional, or simple iframe)
            const mapHTML = `
                <iframe 
                    src="https://www.openstreetmap.org/export/embed.html?bbox=${longitude - 0.05},${latitude - 0.05},${longitude + 0.05},${latitude + 0.05}&layer=mapnik&marker=${latitude},${longitude}"
                    style="width: 100%; height: 100%; border: none; border-radius: 8px;"
                    loading="lazy">
                </iframe>
            `;
            this.elements.map.innerHTML = mapHTML;
        } catch (error) {
            console.error('Error updating map:', error);
            this.elements.map.innerHTML = '<p style="padding: 20px; text-align: center; color: #a0a0a0;">Map unavailable</p>';
        }
    }

    /**
     * Show error message
     */
    showError(message) {
        console.error(message);
        const errorDiv = document.createElement('div');
        errorDiv.className = 'error-notification';
        errorDiv.style.cssText = `
            position: fixed;
            top: 20px;
            right: 20px;
            background: #f44336;
            color: white;
            padding: 16px 20px;
            border-radius: 8px;
            box-shadow: 0 4px 12px rgba(244, 67, 54, 0.3);
            z-index: 1001;
            animation: slideIn 0.3s ease;
        `;
        errorDiv.textContent = 'Error: ' + message;
        document.body.appendChild(errorDiv);

        setTimeout(() => errorDiv.remove(), 5000);
    }

    /**
     * Show success message
     */
    showSuccess(message) {
        const successDiv = document.createElement('div');
        successDiv.className = 'success-notification';
        successDiv.style.cssText = `
            position: fixed;
            top: 20px;
            right: 20px;
            background: #4CAF50;
            color: white;
            padding: 16px 20px;
            border-radius: 8px;
            box-shadow: 0 4px 12px rgba(76, 175, 80, 0.3);
            z-index: 1001;
            animation: slideIn 0.3s ease;
        `;
        successDiv.textContent = message;
        document.body.appendChild(successDiv);

        setTimeout(() => successDiv.remove(), 3000);
    }
}

// Create a singleton instance
const ui = new UI();
