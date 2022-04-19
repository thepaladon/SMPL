#!/bin/bash

echo "Hello World!"

# update apt
sudo apt update
sudo apt upgrade

# VNC
sudo apt-get install realvnc-vnc-server
# remember to enable it through raspi-config

# GLM Math Library
git clone https://github.com/g-truc/glm.git

# GLES
sudo apt-get install libgles2-mesa-dev -y

# X11
sudo apt-get install libxrandr-dev -y

# Assimp (if you're using it)
sudo apt install assimp-utils libassimp-dev libassimp5
