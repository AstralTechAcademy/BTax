cmake_minimum_required(VERSION 3.1.0)

project(Broker)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(PROJECT_ROOT_PATH "../")

message(STATUS "------- HOST DETAILS -------")
cmake_host_system_information(RESULT OS_DETAILS QUERY OS_NAME)
message("Detected operating system: ${OS_DETAILS}")

#Avoid open windows terminal when the application is in Release Mode and deployed
if(${CMAKE_BUILD_TYPE} MATCHES Release)
    set(CMAKE_BUILD_TYPE "Release")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
    #add_definitions(-DOFICIAL)

    if(WIN)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-subsystem,windows") #Mingw Linker
    endif()
else()
set(CMAKE_BUILD_TYPE "Debug")
endif()
message("${CMAKE_BUILD_TYPE}")

if(MANUAL_TEST)
#################### QT TEST ####################
add_definitions(-DMANUAL_TEST)
endif()

if(LINUX)
    set(Qt5Core_VERSION "5.15")
    set(CMAKE_MODULE_PATH "~/Qt/5.15.2")
    set(CMAKE_PREFIX_PATH "~/Qt/5.15.2/gcc_64/lib/cmake/Qt5")
    set(Qt5_DIR "~/Qt/5.15.2/")
endif()

if(WIN)
    set(CMAKE_PREFIX_PATH "G:/Qt/6.5.0/mingw_64/lib/cmake/Qt6")
endif()

if(${OS_DETAILS} STREQUAL macOS)
    set(CMAKE_PREFIX_PATH "$ENV{Qt_DIR}/$ENV{Qt_Version}/macos/lib/cmake/Qt6")
endif()

message(STATUS "    CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}")

if(ANDROID)
    add_definitions( -DANDROID )
    set(ANDROID_PACKAGE_SOURCE_DIR /home/gabridc/Repositorio/neptuno-forked/App/android_files)
    set(ANDROID_ABI "arm64-v8a")
    set(ANDROID_ARM_NEON "ON")
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
    set(Qt5AndroidExtras_DIR "~/Qt/5.15.2/android/lib/cmake/Qt5AndroidExtras")
    set(Qt5Svg_DIR "~/Qt/5.15.2/android/lib/cmake/Qt5Svg")
endif()

if(ANDROID)
    find_package(Qt5 REQUIRED COMPONENTS Qml Quick Network Widgets Sql NetworkAuth AndroidExtras Svg Test)
else()
    find_package(Qt6 REQUIRED COMPONENTS Qml Quick Network Widgets Sql NetworkAuth Svg Test Core5Compat QuickControls2)
    qt_standard_project_setup()
endif()

if(LINUX)
include_directories(~/Qt/5.15.2/gcc_64/include/)
include_directories(~/Qt/5.15.2/gcc_64/include/QtNetworkAuth)
include_directories(~/Qt/5.15.2/gcc_64/include/QtNetwork)
include_directories(~/Qt/5.15.2/gcc_64/include/QtWidgets)
endif()

if(WIN)
include_directories(G:/Qt/6.5.0/mingw64/include/)
include_directories(G:/Qt/6.5.0/mingw64/include/QtNetworkAuth)
include_directories(G:/Qt/6.5.0/mingw64/include/QtNetwork)
include_directories(G:/Qt/6.5.0/mingw64/include/QtWidgets)
endif()

if(${OS_DETAILS} STREQUAL macOS)
    include_directories($ENV{Qt_DIR}/$ENV{Qt_Version})
endif()

include_directories(./)
include_directories(./IfExchanges)
include_directories(./IMarketData)
include_directories(./Logger)
include_directories(./Managers)

add_subdirectory(Logger)
add_subdirectory(Managers)

qt6_add_resources(RESOURCES ./qml.qrc)

set(APP_SOURCES  Wallet.cpp Wallet.h
        WalletOperation.cpp
        WalletOperation.h
        Managers/SQLManager.cpp
        Managers/SQLManager.h
        Managers/WalletsFilterBarManager.cpp
        Managers/WalletsFilterBarManager.h
        Host.cpp
        Host.h
        Config.cpp
        Config.h
        Broker.cpp
        Broker.h
        Host.cpp
        Host.h        
        OperationsModel.cpp
        OperationsModel.h
        Operation.cpp
        Operation.h
        Deposit.cpp
        Deposit.h
        Enumerations.h
        Enumerations.cpp
        Utils.h
        Utils.cpp
        Statistics.cpp
        WalletsPercModel.cpp WalletsPercModel.h
        Statistics.h WalletsModel.cpp WalletsModel.h
        UsersModel.cpp UsersModel.h
        CoinsModel.cpp CoinsModel.h
        ExchangesModel.cpp ExchangesModel.h
        AssetTypeModel.cpp AssetTypeModel.h
        IfExchanges/IExchange.h IfExchanges/IExchange.cpp
        IfExchanges/Cryptocom.cpp IfExchanges/Cryptocom.h 
        IfExchanges/B2m.cpp IfExchanges/B2m.h
        IfExchanges/B2mV2.cpp IfExchanges/B2mV2.h
        IfExchanges/Binance.cpp IfExchanges/Binance.h
        IfExchanges/ExchangeFactory.cpp IfExchanges/ExchangeFactory.h IfExchanges/Importer.cpp IfExchanges/Importer.h
        IMarketData/Coingecko.cpp IMarketData/Coingecko.h 
        IMarketData/YFinance.cpp IMarketData/YFinance.h
        Filters/SelectionMultipleFilter.h
        Filters/SelectionMultipleFilter.cpp
        Validators/VDate.cpp Validators/VDate.h
        Validators/VOperation.cpp Validators/VOperation.h Notifications/NotificationManager.cpp Notifications/NotificationManager.h)

if(ANDROID)
    add_library(Broker SHARED
            main.cpp
            ${APP_SOURCES}
            ${RESOURCES}
            ${MANAGER_SOURCES})
else()
add_executable(Broker
        main.cpp
        ${RESOURCES}
        ${APP_SOURCES}
        ${MANAGER_SOURCES})
endif()


target_link_libraries(Broker Qt6::Sql Qt6::Quick Qt6::QuickControls2 Qt6::Qml Qt6::Network Qt6::NetworkAuth Qt6::Widgets  Qt6::Core5Compat Logger)

if(WIN)
    if(${CMAKE_BUILD_TYPE} MATCHES Release)
        set_target_properties(Broker PROPERTIES
                                RUNTIME_OUTPUT_DIRECTORY ${PROJECT_ROOT_PATH}/release/${OS_DETAILS})
    else()
        set_target_properties(Broker PROPERTIES
                                RUNTIME_OUTPUT_DIRECTORY ${PROJECT_ROOT_PATH}/debug/${OS_DETAILS})
    endif()
endif()

if(${OS_DETAILS} STREQUAL macOS)
    if(${CMAKE_BUILD_TYPE} MATCHES Release)
        set_target_properties(Broker PROPERTIES
                                RUNTIME_OUTPUT_DIRECTORY ${PROJECT_ROOT_PATH}/release/${OS_DETAILS}
                                MACOSX_BUNDLE FALSE)
    else()
        set_target_properties(Broker PROPERTIES
                                RUNTIME_OUTPUT_DIRECTORY ${PROJECT_ROOT_PATH}/debug/${OS_DETAILS}
                                MACOSX_BUNDLE FALSE)
    endif()
endif()

if(GTEST)
#################### QT TEST ####################
add_definitions(-DGTEST)
enable_testing(true)

set(TEST_UTILS  ${PROJECT_ROOT_PATH}/Test/Test_Utils.h)


add_executable(BrokerManagerTest ${PROJECT_ROOT_PATH}/Test/BrokerManagerTest.cpp ${RESOURCES} ${TEST_UTILS} ${APP_SOURCES} ${MANAGER_SOURCES})
add_executable(BrokerTest ${PROJECT_ROOT_PATH}/Test/BrokerTest.cpp ${RESOURCES} ${TEST_UTILS}  ${APP_SOURCES} ${MANAGER_SOURCES})
add_executable(B2mTest ${PROJECT_ROOT_PATH}/Test/B2mTest.cpp ${RESOURCES} ${TEST_UTILS}  ${APP_SOURCES} ${MANAGER_SOURCES})
add_executable(B2mV2Test ${PROJECT_ROOT_PATH}/Test/B2mV2Test.cpp ${RESOURCES} ${TEST_UTILS}  ${APP_SOURCES} ${MANAGER_SOURCES})
add_executable(BinanceTest ${PROJECT_ROOT_PATH}/Test/BinanceTest.cpp ${RESOURCES} ${TEST_UTILS}  ${APP_SOURCES} ${MANAGER_SOURCES})
add_executable(ImporterTest ${PROJECT_ROOT_PATH}/Test/ImporterTest.cpp ${RESOURCES} ${TEST_UTILS}  ${APP_SOURCES} ${MANAGER_SOURCES})
add_executable(SQLManagerTest ${PROJECT_ROOT_PATH}/Test/SQLManagerTest.cpp ${RESOURCES} ${TEST_UTILS} ${APP_SOURCES} ${MANAGER_SOURCES})


set_target_properties(BrokerManagerTest PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_ROOT_PATH}/Test")
set_target_properties(BrokerTest PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_ROOT_PATH}/Test")
set_target_properties(B2mTest PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_ROOT_PATH}/Test")
set_target_properties(B2mV2Test PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_ROOT_PATH}/Test")
set_target_properties(BinanceTest PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_ROOT_PATH}/Test")
set_target_properties(ImporterTest PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_ROOT_PATH}/Test")
set_target_properties(SQLManagerTest PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_ROOT_PATH}/Test")


add_test(NAME BrokerTest COMMAND BrokerTest)
add_test(NAME BrokerManagerTest COMMAND BrokerManagerTest)
add_test(NAME B2mTest COMMAND B2mTest)
add_test(NAME B2mV2Test COMMAND B2mV2Test)
add_test(NAME BinanceTest COMMAND BinanceTest)
add_test(NAME ImporterTest COMMAND ImporterTest)
add_test(NAME SQLManagerTest COMMAND ImporterTest)


target_link_libraries(SQLManagerTest PRIVATE Qt6::Test Qt6::Sql Qt6::Qml Qt6::Network Qt6::NetworkAuth Logger)
target_link_libraries(BrokerManagerTest PRIVATE Qt6::Test Qt6::Sql Qt6::Qml Qt6::Network Qt6::NetworkAuth Logger)
target_link_libraries(BrokerTest PRIVATE Qt6::Test Qt6::Sql Qt6::Qml Qt6::Network Qt6::NetworkAuth Logger)
target_link_libraries(B2mTest PRIVATE Qt6::Test Qt6::Sql Qt6::Qml Qt6::Network Qt6::NetworkAuth Logger)
target_link_libraries(B2mV2Test PRIVATE Qt6::Test Qt6::Sql Qt6::Qml Qt6::Network Qt6::NetworkAuth Logger)
target_link_libraries(BinanceTest PRIVATE Qt6::Test Qt6::Sql Qt6::Qml Qt6::Network Qt6::NetworkAuth Logger)
target_link_libraries(ImporterTest PRIVATE Qt6::Test Qt6::Sql Qt6::Qml Qt6::Network Qt6::NetworkAuth Logger)
endif()

# Copy external libs and files

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${PROJECT_ROOT_PATH}/debug/${OS_DETAILS}/Astral
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${PROJECT_ROOT_PATH}/Astral ${PROJECT_ROOT_PATH}/debug/${OS_DETAILS}/Astral)

