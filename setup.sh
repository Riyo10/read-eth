#!/bin/bash
# Network Diagnostic Tool - Setup Script
# This script adds convenient shortcuts to your shell

PROJECT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SHELL_CONFIG=""

# Detect shell
if [ -n "$ZSH_VERSION" ]; then
    SHELL_CONFIG="$HOME/.zshrc"
elif [ -n "$BASH_VERSION" ]; then
    SHELL_CONFIG="$HOME/.bashrc"
else
    SHELL_CONFIG="$HOME/.bashrc"
fi

echo "Setting up Network Diagnostic Tool shortcuts..."
echo "Project directory: $PROJECT_DIR"
echo "Shell config: $SHELL_CONFIG"
echo ""

# Check if already configured
if grep -q "network-diagnostic-tool-alias" "$SHELL_CONFIG" 2>/dev/null; then
    echo "✓ Already configured!"
    echo ""
    echo "You can now use:"
    echo "  run port      - Start the server"
    echo "  run stop      - Stop the server"
    echo "  run status    - Check server status"
    exit 0
fi

# Add alias to shell config
cat >> "$SHELL_CONFIG" << 'EOF'

# Network Diagnostic Tool Aliases - network-diagnostic-tool-alias
alias run="$PROJECT_DIR/run"
EOF

# Replace $PROJECT_DIR with actual path in the config
sed -i "s|\$PROJECT_DIR|$PROJECT_DIR|g" "$SHELL_CONFIG"

echo "✓ Configuration added to $SHELL_CONFIG"
echo ""
echo "To start using aliases, run:"
echo "  source $SHELL_CONFIG"
echo ""
echo "Then you can use:"
echo "  run port      - Start the server"
echo "  run stop      - Stop the server"  
echo "  run status    - Check server status"
echo "  run rebuild   - Clean rebuild"
echo "  run restart   - Restart the server"
echo "  run help      - Show all commands"
