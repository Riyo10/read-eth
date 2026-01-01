// API module for making requests to the backend

class API {
    constructor(baseUrl = '') {
        this.baseUrl = baseUrl;
        this.timeout = 30000;
    }

    /**
     * Make a fetch request with timeout
     */
    async request(endpoint, options = {}) {
        const url = `${this.baseUrl}/api${endpoint}`;
        const controller = new AbortController();
        const timeoutId = setTimeout(() => controller.abort(), this.timeout);

        try {
            const response = await fetch(url, {
                ...options,
                signal: controller.signal,
                headers: {
                    'Content-Type': 'application/json',
                    ...options.headers,
                }
            });

            clearTimeout(timeoutId);

            if (!response.ok) {
                throw new Error(`API Error: ${response.status} ${response.statusText}`);
            }

            return await response.json();
        } catch (error) {
            clearTimeout(timeoutId);
            if (error.name === 'AbortError') {
                throw new Error('Request timeout');
            }
            throw error;
        }
    }

    /**
     * Get network information
     */
    async getNetworkInfo() {
        return this.request('/network-info');
    }

    /**
     * Get ISP information
     */
    async getISPInfo() {
        return this.request('/isp-info');
    }

    /**
     * Get interface statistics
     */
    async getInterfaceStats() {
        return this.request('/interface-stats');
    }

    /**
     * Perform speed test
     */
    async performSpeedTest() {
        return this.request('/speed-test');
    }
}

// Create a singleton instance
const api = new API();
