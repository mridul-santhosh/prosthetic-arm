# Prosthetic Arm

A comprehensive open-source project for the design, control, and implementation of a myoelectric prosthetic arm. This repository contains hardware schematics, embedded firmware, and supporting utilities for a functional, customizable prosthetic limb.

---

## Table of Contents

- [Project Overview](#project-overview)
- [Features](#features)
- [Hardware Requirements](#hardware-requirements)
- [Software Architecture](#software-architecture)
- [Getting Started](#getting-started)
  - [Prerequisites](#prerequisites)
  - [Installation & Build](#installation--build)
  - [Uploading Firmware](#uploading-firmware)
- [Usage](#usage)
- [Directory Structure](#directory-structure)
- [Contributing](#contributing)
- [License](#license)
- [Contact](#contact)
- [Acknowledgements](#acknowledgements)

---

## Project Overview

This project provides a cost-effective, customizable prosthetic arm controlled using EMG (muscle) signals. The design focuses on ease of assembly, modularity, and adaptability for research, education, or personal use.

**Key goals:**
- Restore fundamental hand movements to users.
- Provide a robust platform for research and development.
- Encourage community-driven enhancements and collaboration.

---

## Features

- Open-source hardware schematics and firmware
- EMG signal acquisition and real-time processing
- Multi-finger actuation (e.g., grip, pinch)
- Modular firmware in C/C++
- Support for various microcontrollers (Arduino, STM32, etc.)
- Optional sensor feedback (force, position)
- Calibration and setup tools (Python/JavaScript)
- Expandable to additional sensors or actuators

---

## Hardware Requirements

- **Microcontroller:** Arduino Uno/Nano, STM32, ESP32, or similar
- **EMG Sensors:** MyoWare or custom-built
- **Servos/Actuators:** As specified in mechanical design
- **Power Supply:** Battery (specification based on design)
- **Prosthetic Frame:** 3D-printed or purchased components
- **Wiring and Connectors**
- **(Optional) Additional Sensors:** Force, flex, IMU, etc.

> See the `/hardware` directory for schematics, recommended components, and 3D models.

---

## Software Architecture

- **Embedded Firmware:**  
  Written in C/C++. Handles EMG signal processing, actuator control, and feedback.
- **Calibration & Tools:**  
  Python/JavaScript scripts for EMG calibration and system diagnostics, located in `/tools`.
- **Interfaces:**  
  Serial/CLI or web-based setup for calibration and diagnostics.

---

## Getting Started

### Prerequisites

- [PlatformIO](https://platformio.org/) or Arduino IDE or STM32CubeIDE
- [Python 3.x](https://www.python.org/) (for tools/scripts)
- USB drivers for your microcontroller
- CMake (if using native builds)

### Installation & Build

1. **Clone the repository:**
   ```sh
   git clone https://github.com/mridul-santhosh/prosthetic-arm.git
   cd prosthetic-arm
Set up your environment:

For Arduino: Open the .ino file in Arduino IDE.
For STM32: Open the /firmware project in STM32CubeIDE.
For PlatformIO:
sh
platformio run
Install Python dependencies (if needed):

sh
cd tools
pip install -r requirements.txt
Uploading Firmware
Connect your microcontroller via USB.
Select the correct port and board in your IDE.
Upload the firmware (Upload or Flash).
Usage
Power the prosthetic arm.

Connect EMG electrodes to the target muscle groups.

Calibrate EMG signals:

sh
python tools/calibrate.py
Follow on-screen instructions to record rest/flex levels.

Operate the arm:

Use muscle contractions to control the actuators.
Optionally monitor status via serial output or web dashboard.
Directory Structure
Code
prosthetic-arm/
├── hardware/      # Schematics, PCB layouts, 3D models
├── firmware/      # Embedded C/C++ source code
├── tools/         # Calibration and utility scripts (Python/JS)
├── docs/          # Documentation, images
├── LICENSE
└── README.md
Contributing
Contributions are welcome! To get started:

Fork the repository.
Create a new branch: git checkout -b feature/your-feature
Make your changes and commit: git commit -am 'Add new feature'
Push to your fork: git push origin feature/your-feature
Open a Pull Request describing your changes.
Guidelines:

Follow code style conventions.
Document your changes.
Test thoroughly before submitting.
License
This project is licensed under the MIT License.

Contact
Maintainer: mridul-santhosh
Issues: Please use GitHub Issues for bug reports and feature requests.
Acknowledgements
Open-source prosthetics community
Contributors to open hardware/software projects
Users and testers for valuable feedback
