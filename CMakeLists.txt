cmake_minimum_required(VERSION 3.1.0)

if(LINUX)
    set(CMAKE_C_COMPILER "/usr/bin/clang")
    set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
endif()

project(Broker)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(LINUX)
    set(Qt5Core_VERSION "5.15")
    set(CMAKE_MODULE_PATH "~/Qt/5.15.2")
    set(CMAKE_PREFIX_PATH "~/Qt/5.15.2/gcc_64/lib/cmake/Qt5")
    set(Qt5_DIR "~/Qt/5.15.2/")
endif()

if(WIN)
    set(CMAKE_PREFIX_PATH "C:/Qt/5.15.2/mingw81_64/lib/cmake/Qt5")
    set(Qt5_DIR "C:/Qt/5.15.2/mingw81_64/bin")
    set(Qt5Core_VERSION "5.15")
endif()

if(ANDROID)
    set(ANDROID_ABIS "armeabi-v7a")
    #set(ANDROID_ABIS "arm64-v8a")
    set(ANDROID_ARM_NEON "ON")
    set(Qt5Core_VERSION "5.15")
    set(CMAKE_MODULE_PATH "~/Qt/5.15.2")
    set(CMAKE_PREFIX_PATH "~/Qt/5.15.2/android/lib/cmake/Qt5")
    set(Qt5_DIR "~/Qt/5.15.2/android/lib/cmake/Qt5")
    set(Qt5Qml_DIR "~/Qt/5.15.2/android/lib/cmake/Qt5Qml")
    set(Qt5Network_DIR "~/Qt/5.15.2/android/lib/cmake/Qt5Network")
    set(Qt5Core_DIR "~/Qt/5.15.2/android/lib/cmake/Qt5Core")
    set(Qt5Quick_DIR "~/Qt/5.15.2/android/lib/cmake/Qt5Quick")
    set(Qt5QmlModels_DIR "~/Qt/5.15.2/android/lib/cmake/Qt5QmlModels")
    set(Qt5Gui_DIR "~/Qt/5.15.2/android/lib/cmake/Qt5Gui")
    set(Qt5Widgets_DIR "~/Qt/5.15.2/android/lib/cmake/Qt5Widgets")
    set(Qt5Sql_DIR "~/Qt/5.15.2/android/lib/cmake/Qt5Sql")
    set(Qt5NetworkAuth_DIR "~/Qt/5.15.2/android/lib/cmake/Qt5NetworkAuth")
endif()

find_package(Qt5 REQUIRED COMPONENTS Qml Quick Network Widgets Sql NetworkAuth)

include_directories(~/Qt/5.15.2/gcc_64/include/)
include_directories(~/Qt/5.15.2/gcc_64/include/QtNetworkAuth)
include_directories(~/Qt/5.15.2/gcc_64/include/QtNetwork)
include_directories(~/Qt/5.15.2/gcc_64/include/QtWidgets)
include_directories(./)

qt5_add_resources(RESOURCES ./qml.qrc)

if(ANDROID)
    add_library(Broker SHARED
            main.cpp
            Broker.cpp
            Broker.h
            Wallet.cpp
            Wallet.h
            SQLManager.cpp
            SQLManager.h
            DBLocal.h
            DBLocal.cpp
            ${RESOURCES})
else()
add_executable(Broker
        main.cpp
        ${RESOURCES}
        Wallet.cpp
        Wallet.h
        SQLManager.cpp
        SQLManager.h
        DBLocal.cpp
        DBLocal.h
        Broker.cpp
        Broker.h
        OperationsModel.cpp
        OperationsModel.h
        Operation.cpp
        Operation.h
        Deposit.cpp
        Deposit.h
        Statistics.cpp
        Statistics.h WalletsModel.cpp WalletsModel.h BrokerManager.cpp BrokerManager.h)
endif()

if(LINUX)
    target_link_libraries(Broker Qt5::Sql Qt5::Quick Qt5::Qml Qt5::Network Qt5::NetworkAuth Qt5::Widgets)
endif()

if(ANDROID)
    target_link_libraries(Broker Qt5::Sql Qt5::Quick Qt5::Qml Qt5::Network Qt5::NetworkAuth Qt5::Widgets)
endif()


if(WIN)
    target_link_libraries(Broker Qt5::Sql Qt5::Quick Qt5::Qml Qt5::Network Qt5::NetworkAuth Qt5::Widgets)
endif()