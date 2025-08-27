# Real-Time Vehicle Telemetry Dashboard

A robust, real-time vehicle telemetry dashboard built using Qt/QML and UDP. This project receives and processes data from a vehicle (or simulation) including speed, RPM, GPS coordinates, wheel speeds, and other parameters, and displays them dynamically on an interactive UI.

## Table of Contents
- [Introduction](#introduction)
- [Key Features](#key-features)
- [Advanced Architecture](#advanced-architecture)
- [Technologies Used](#technologies-used)
- [Installation and Setup](#installation-and-setup)
- [Configuration](#configuration)
- [Usage](#usage)
- [Screenshots and Demonstrations](#screenshots-and-demonstrations)
- [Video](#Video)
- [Testing](#testing)
- [Project Structure](#project-structure)
- [Contributing](#contributing)
- [Contact Information](#contact-information)

## Introduction
The Real-Time Vehicle Telemetry Dashboard is a sophisticated desktop application designed to monitor and display various vehicle parameters in real time. Built with a multi-threaded architecture, it ensures efficient data processing and smooth UI updates, making it suitable for high-frequency telemetry data. The application features:

- Multi-threaded data processing for optimal performance
- Real-time UDP data transmission and processing
- Modern, responsive Qt/QML user interface
- Advanced visualization including GG diagrams and wheel speed indicators
- Comprehensive session management and data validation

## Key Features

### High-Performance Multi-Threading
- **Dedicated Receiver Thread:** Handles UDP packet reception without blocking the UI
- **Thread Pool for Data Parsing:** Multiple parser threads process incoming data concurrently
- **Non-blocking UI Updates:** Smooth interface updates even under heavy data loads
- **Efficient Resource Utilization:** Optimized thread management based on system capabilities

### Real-Time Data Processing
- High-frequency data reception and processing (up to thousands of packets per second)
- Zero-copy data handling for maximum performance
- Thread-safe data synchronization using atomic operations

### Advanced Telemetry Visualization
- **Dynamic GG Diagram:** Real-time visualization of lateral and longitudinal G-forces
- **Interactive Wheel Speed Display:** Individual speed monitoring for all four wheels
- **Comprehensive Dashboard:**
  - Vehicle speed and RPM gauges
  - Accelerator and brake pedal positions
  - Steering encoder angle visualization
  - Temperature and battery level monitoring
  - IMU parameters display
  - GPS route tracking with real-time updates

### Intelligent Session Management
- Secure session initialization and configuration
- Automatic port and connection management
- Data validation and error handling
- Smooth transition between session states

## Advanced Architecture

### Multi-Threaded Core
```cpp
class UdpClient {
    QThread m_receiverThread;            // Dedicated thread for UDP reception
    QThreadPool m_parserPool;           // Thread pool for parallel data parsing
    QList<UdpParserWorker *> m_parsers; // Multiple parser workers
    // ... other components
};
```

### Thread-Safe Data Handling
- Atomic operations for all telemetry data updates
- Lock-free algorithms for maximum performance
- Smart pointer usage for memory safety
- Event-driven communication between threads

### UDP Communication
- **UdpReceiverWorker:** Dedicated worker for efficient packet reception
- **UdpParserWorker:** Multiple workers for parallel data parsing
- **Thread Pool Management:** Dynamic scaling based on system load

### User Interface Layer
- **QML/C++ Integration:** Seamless binding between backend and frontend
- **Custom Components:** Specialized widgets for telemetry visualization
- **Responsive Design:** Dynamic layouts that adapt to any screen size

## Technologies Used
- **Qt 6+:** Modern C++ framework for cross-platform development
- **QML:** Declarative UI design language
- **C++17/20:** Advanced language features for robust implementation
- **Multi-threading:** Parallel processing for optimal performance
- **UDP Networking:** High-speed data transmission
- **QtLocation & QtPositioning:** GPS and mapping capabilities
- **Git:** Version control and collaboration

## Installation and Setup

### Prerequisites
- **Qt 6+** (with QtLocation and QtPositioning modules)
- **C++ Compiler:** Compatible with your Qt installation.
- **Git:** For cloning the repository.

### Cloning the Repository
```bash
git clone https://github.com/Mohamedkhaled687/CarDashboard_ASURT.git
```

## Configuration

### UDP Settings
- **UDP Client:** Configure the listening port in the UdpClient component.
- **UDP Sender:** Set the destination IP address and port in the UDP Sender screen.

### Map Configuration
- **Map Plugin:** This project uses the OpenStreetMap (OSM) plugin. Ensure your network connection is available to load map tiles.
- **GPS Mapping:** Adjust mapping boundaries in the `gps.qml` file if necessary.

### Telemetry Data Format
UDP datagrams should be formatted as follows:
```
speed,rpm,accPedal,brakePedal,encoderAngle,temperature,batteryLevel,gpsLongitude,gpsLatitude,speedFL,speedFR,speedBL,speedBR
```

## Usage

### Running the Dashboard
- Start the application.
- Enter the destination IP and port on the UDP Sender screen.
- Use the random slider to simulate telemetry data.
- Click **"Send Data"** to transmit the data over UDP.
- The dashboard updates in real time as new data is received.

### Session Management
- **Session Creation:** Enter session details such as session name and port.
- **Waiting Screen:** After validation, the application transitions to a waiting screen.

### Map Interaction
- Use the zoom in/out buttons to adjust the map's zoom level.
- The map automatically centers on the latest GPS coordinate received.
- A blue line represents the vehicle's route on the map.

## Screenshots and Demonstrations
- **Session Setup:**
   <img width="1748" height="1017" alt="Image" src="https://github.com/user-attachments/assets/bb477943-48df-4a66-ae03-63c54abb7488" />
- **Dashboard Overview:**
    <img width="1749" height="1002" alt="Image" src="https://github.com/user-attachments/assets/92189e08-f3ba-4dd9-8892-9164fabb31f8" />
## Video
https://github.com/user-attachments/assets/76593ab2-9330-4a89-a245-a9e5cc24e4df
## Testing

### Unit Testing
- Unit tests are included for critical components.
- Run tests using Qt Creator's test framework.

### Manual Testing
- Verify UDP connectivity using a UDP packet generator.
- Test UI functionality by simulating random telemetry data via the provided slider.

## Project Structure
```
CarDashboard/
├── README.md
├── main.cpp
├── CMAKELists.txt
├── src/
│   ├── udpclient.h
│   ├── udpclient.cpp
├── qml/
│   ├── main.qml
│   ├── gps.qml
│   ├── wheelsBox.qml
│   ├── WaitingScreen.qml
│   └── ... (other QML files)
└── assets/
    ├── marker.png
    ├── AI_car_transparent.png
    ├── CAR-215-ASURT.png
    └── ... (other asset files)
```

## Contributing
Contributions are welcome! To contribute:
1. Fork the repository.
2. Create a feature branch:
   ```bash
   git checkout -b feature/your-feature-name
   ```
3. Commit your changes:
   ```bash
   git commit -am 'Add new feature'
   ```
4. Push to your branch:
   ```bash
   git push origin feature/your-feature-name
   ```
5. Open a Pull Request.

## Contact Information
For any questions, suggestions, or bug reports, please contact:

- **Name:** Mohamed Khaled Ahmed
- **Email:** mohamedkhaled47526@gmail.com
- **GitHub:** https://github.com/Mohamedkhaled687
- **LinkedIn:** [https://www.linkedin.com/in/mohammedkhaaledd/]
