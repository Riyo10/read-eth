// Main application logic

class NetworkDiagnosticApp {
    constructor() {
        this.isTestingSpeed = false;
        this.init();
    }

    /**
     * Initialize the application
     */
    async init() {
        this.setupEventListeners();
        await this.loadInitialData();
    }

    /**
     * Setup event listeners
     */
    setupEventListeners() {
        ui.elements.startSpeedTestBtn.addEventListener('click', () => this.startSpeedTest());
        ui.elements.refreshNetworkBtn.addEventListener('click', () => this.loadNetworkInfo());
        ui.elements.refreshISPBtn.addEventListener('click', () => this.loadISPInfo());
        ui.elements.refreshStatsBtn.addEventListener('click', () => this.loadInterfaceStats());
    }

    /**
     * Load all initial data
     */
    async loadInitialData() {
        try {
            ui.showLoading(true);

            // Load all data in parallel
            await Promise.all([
                this.loadNetworkInfo(),
                this.loadISPInfo(),
                this.loadInterfaceStats(),
            ]);

            ui.showLoading(false);
        } catch (error) {
            console.error('Error loading initial data:', error);
            ui.showError('Failed to load initial data: ' + error.message);
            ui.showLoading(false);
        }
    }

    /**
     * Load network information
     */
    async loadNetworkInfo() {
        try {
            const data = await api.getNetworkInfo();
            ui.updateNetworkInfo(data);
        } catch (error) {
            console.error('Error loading network info:', error);
            ui.showError('Failed to load network info: ' + error.message);
        }
    }

    /**
     * Load ISP information
     */
    async loadISPInfo() {
        try {
            const data = await api.getISPInfo();
            ui.updateISPInfo(data);
            ui.showSuccess('ISP info updated');
        } catch (error) {
            console.error('Error loading ISP info:', error);
            ui.showError('Failed to load ISP info: ' + error.message);
        }
    }

    /**
     * Load interface statistics
     */
    async loadInterfaceStats() {
        try {
            const data = await api.getInterfaceStats();
            ui.updateInterfaceInfo(data);
            ui.updateStats(data);
        } catch (error) {
            console.error('Error loading interface stats:', error);
            ui.showError('Failed to load interface stats: ' + error.message);
        }
    }

    /**
     * Start speed test with progress simulation
     */
    async startSpeedTest() {
        if (this.isTestingSpeed) return;

        this.isTestingSpeed = true;
        ui.setSpeedTestButtonState(false);
        ui.showSpeedTestProgress(true, 'Initializing test...');
        ui.updateProgress(0);

        try {
            // Simulate test progress
            const progressInterval = setInterval(() => {
                const current = parseInt(ui.elements.progressFill.style.width) || 0;
                if (current < 90) {
                    ui.updateProgress(current + Math.random() * 15);
                }
            }, 300);

            // Make the actual request
            const data = await api.performSpeedTest();

            // Clear progress simulation
            clearInterval(progressInterval);
            ui.updateProgress(100, 'Test complete!');

            // Update UI with results
            ui.updateSpeedTest(data);

            // Show completion message
            setTimeout(() => {
                ui.showSpeedTestProgress(false);
                ui.showSuccess('Speed test completed successfully!');
                this.isTestingSpeed = false;
                ui.setSpeedTestButtonState(true);
            }, 1500);

        } catch (error) {
            console.error('Error during speed test:', error);
            ui.showError('Speed test failed: ' + error.message);
            ui.showSpeedTestProgress(false);
            this.isTestingSpeed = false;
            ui.setSpeedTestButtonState(true);
        }
    }
}

// Initialize app when DOM is ready
document.addEventListener('DOMContentLoaded', () => {
    window.app = new NetworkDiagnosticApp();
});

// Auto-refresh data every 30 seconds
setInterval(() => {
    if (window.app && !window.app.isTestingSpeed) {
        window.app.loadNetworkInfo();
        window.app.loadInterfaceStats();
    }
}, 30000);
