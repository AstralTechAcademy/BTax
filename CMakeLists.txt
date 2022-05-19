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

#add_definitions(-DOFICIAL)

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
    set(ANDROID_PACKAGE_SOURCE_DIR /home/gabridc/Repositorio/Broker/android_files)
    set(ANDROID_ABI "arm64-v8a")

    set(CMAKE_TOOLCHAIN_FILE "/home/gabridc/Android/Sdk/ndk/build/cmake/android.toolchain.cmake")
    set(CMAKE_BUILD_TYPE "Debug")
    set(CMAKE_PROJECT_INCLUDE_BEFORE "/home/gabridc/Qt/Tools/QtCreator/share/qtcreator/package-manager/auto-setup.cmake")
    set(QT_QMAKE_EXECUTABLE "/home/gabridc/Qt/5.15.2/android/bin/qmake")
    set(CMAKE_PREFIX_PATH "~/Qt/5.15.2/android/lib/cmake/Qt5,/home/gabridc/Qt/5.15.2/android")
    set(ANDROID_NDK "/home/gabridc/Android/Sdk/ndk")
    set(CMAKE_TOOLCHAIN_FILE "/home/gabridc/Android/Sdk/ndk/build/cmake/android.toolchain.cmake")
    set(ANDROID_STL "c++_shared")
    set(CMAKE_FIND_ROOT_PATH "/home/gabridc/Qt/5.15.2/android")
    set(QT_HOST_PATH "/home/gabridc/Qt/5.15.2/android")
    set(ANDROID_SDK_ROOT "/home/gabridc/Android/Sdk")
    set(JAVA_HOME "/usr/lib/jvm/java-1.8.0-openjdk-amd64")
    set(QT_ANDROID_SDK_ROOT "/home/gabridc/Android/Sdk")
    set(ANDROID_DEPLOY_QT "/home/gabridc/Qt/5.15.2/android/bin/android1")

    set(Qt5Core_VERSION "5.15")
    set(CMAKE_MODULE_PATH "~/Qt/5.15.2")
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
            DBCredential_NotPush.h
            Statistics.h WalletsModel.cpp WalletsModel.h BrokerManager.cpp BrokerManager.h DBRemote.cpp DBRemote.h UsersModel.cpp UsersModel.h
            WalletsPercModel.cpp WalletsPercModel.h
            CoinsModel.cpp CoinsModel.h
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
        DBCredential_NotPush.h
        WalletsPercModel.cpp WalletsPercModel.h
        Statistics.h WalletsModel.cpp WalletsModel.h BrokerManager.cpp BrokerManager.h DBRemote.cpp DBRemote.h UsersModel.cpp UsersModel.h
        CoinsModel.cpp CoinsModel.h
        IfExchanges/IMarketData.cpp IfExchanges/IExchange.h IfExchanges/Cryptocom.cpp IfExchanges/Cryptocom.h IfExchanges/B2m.cpp IfExchanges/B2m.h
        IfExchanges/Binance.cpp IfExchanges/Binance.h
        IfExchanges/ExchangeFactory.cpp IfExchanges/MarketDataFactory.h IfExchanges/Importer.cpp IfExchanges/Importer.h IMarketData/Coingecko.cpp IMarketData/Coingecko.h)
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