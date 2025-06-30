cmake_minimum_required(VERSION 3.16)

project(Car_Dashboard VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick Network SerialPort Mqtt)

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(appCar_Dashboard
    main.cpp
)

qt_add_qml_module(appCar_Dashboard
    URI Car_Dashboard
    VERSION 1.0
    QML_FILES
        Main.qml
        QML_FILES UI/WelcomePage/WelcomeScreen.qml
        RESOURCES UI/Assets/30.gif
        RESOURCES UI/Assets/CAR-215-ASURT.png
        RESOURCES UI/Assets/power.png
        QML_FILES UI/WelcomePage/MyButton.qml
        RESOURCES UI/Assets/powerButton.png
        QML_FILES UI/WelcomePage/WaitingScreen.qml
        SOURCES Controllers/udpclient.h Controllers/udpclient.cpp
        RESOURCES UI/Assets/formulalogo.jpeg
        RESOURCES UI/Assets/car3_white.png UI/Assets/road2.png
        QML_FILES UI/StatusBar/StatusBar.qml
        RESOURCES UI/Assets/back-button.png
        QML_FILES UI/InformationPage/Information.qml
        QML_FILES UI/InformationPage/WheelSpeed.qml
        QML_FILES UI/InformationPage/TemperatureIndicator.qml
        RESOURCES UI/Assets/AI_car_transparent.png
        RESOURCES UI/Assets/AI_car_transparent.png UI/Assets/batteryIcon.png UI/Assets/batteryIcon_blue.png UI/Assets/marker.png UI/Assets/thermometer.png
        RESOURCES UI/Assets/racinglogo.png
        QML_FILES UI/InformationPage/SteeringWheel.qml
        RESOURCES UI/Assets/Steering_wheel.png
        QML_FILES UI/InformationPage/Speedometer.qml
        QML_FILES UI/InformationPage/RpmMeter.qml
        QML_FILES UI/InformationPage/GpsPlotter.qml
        QML_FILES UI/InformationPage/AcceleratorPedal.qml
        QML_FILES UI/InformationPage/BrakePadel.qml
        QML_FILES UI/InformationPage/BatteryLevelIndicator.qml
        QML_FILES UI/InformationPage/EulerGauges.qml
        QML_FILES UI/InformationPage/EulerVisual.qml
        RESOURCES UI/Assets/GG_Diagram.png UI/Assets/point.png
        SOURCES Controllers/udpreceiverworker.h Controllers/udpreceiverworker.cpp
        SOURCES Controllers/udpparserworker.h Controllers/udpparserworker.cpp
        SOURCES
        SOURCES
        SOURCES
        SOURCES
        SOURCES Controllers/serialmanager.cpp Controllers/serialmanager.h Controllers/serialparserworker.cpp Controllers/serialparserworker.h Controllers/serialreceiverworker.cpp Controllers/serialreceiverworker.h
        SOURCES Controllers/communicationmanager.h Controllers/communicationmanager.cpp
        RESOURCES UI/Assets/Trial1.jpg
        RESOURCES UI/Assets/Car1.png
        RESOURCES UI/Assets/Car2.png
        SOURCES Controllers/mqttclient.h Controllers/mqttclient.cpp
        SOURCES Controllers/mqttreceiverworker.h Controllers/mqttreceiverworker.cpp
        SOURCES Controllers/mqttparserworker.h Controllers/mqttparserworker.cpp
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appCar_Dashboard PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appCar_Dashboard
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appCar_Dashboard
    PRIVATE Qt6::Quick Qt6::Network Qt6::SerialPort  Qt6::Mqtt
)

include(GNUInstallDirs)
install(TARGETS appCar_Dashboard
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
