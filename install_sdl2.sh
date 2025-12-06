#!/bin/bash

# SDL2 Installation Script for Helicopter Game

echo "========================================="
echo "  SDL2 Installation Helper"
echo "========================================="
echo ""

# Detect OS
if [ -f /etc/os-release ]; then
    . /etc/os-release
    OS=$ID
    echo "Detected OS: $PRETTY_NAME"
    echo ""
else
    echo "Cannot detect OS. Please install SDL2 manually."
    exit 1
fi

# Install based on OS
case $OS in
    ubuntu|debian|pop)
        echo "Installing SDL2 for Debian/Ubuntu..."
        echo "Running: sudo apt-get update && sudo apt-get install -y libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev libsdl2-mixer-dev"
        echo ""
        sudo apt-get update
        sudo apt-get install -y libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev libsdl2-mixer-dev
        ;;
    
    fedora|rhel|centos)
        echo "Installing SDL2 for Fedora/RHEL..."
        echo "Running: sudo dnf install -y SDL2-devel SDL2_image-devel SDL2_ttf-devel SDL2_mixer-devel"
        echo ""
        sudo dnf install -y SDL2-devel SDL2_image-devel SDL2_ttf-devel SDL2_mixer-devel
        ;;
    
    arch|manjaro)
        echo "Installing SDL2 for Arch Linux..."
        echo "Running: sudo pacman -S --noconfirm sdl2 sdl2_image sdl2_ttf sdl2_mixer"
        echo ""
        sudo pacman -S --noconfirm sdl2 sdl2_image sdl2_ttf sdl2_mixer
        ;;
    
    *)
        echo "Unsupported OS: $OS"
        echo ""
        echo "Please install SDL2 manually:"
        echo "  - SDL2"
        echo "  - SDL2_image"
        echo "  - SDL2_ttf"
        echo "  - SDL2_mixer"
        exit 1
        ;;
esac

echo ""
echo "========================================="
echo "  Installation complete!"
echo "========================================="
echo ""
echo "Verifying installation..."
pkg-config --modversion sdl2

if [ $? -eq 0 ]; then
    echo ""
    echo "✓ SDL2 successfully installed!"
    echo ""
    echo "You can now build the game with:"
    echo "  make"
    echo ""
    echo "And run it with:"
    echo "  ./helicopter_game"
else
    echo ""
    echo "✗ SDL2 installation may have failed."
    echo "  Please check the error messages above."
fi
