cmake_minimum_required(VERSION 3.24.2)
project(PasswordManager VERSION 1.1.0 LANGUAGES CXX)

option(PM_BUILD_STATIC OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

find_package(OpenSSL REQUIRED)
find_package(wxWidgets 3.2.6 REQUIRED COMPONENTS core base qa xml)

if (PM_BUILD_STATIC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    set(wxWidgets_USE_STATIC ON)
    set(OPENSSL_USE_STATIC_LIBS TRUE)
endif()

set(SRC_FILES
    src/EntryPoint.cpp
    src/Application.cpp

    src/Data/CharacterMap.cpp
    src/Data/Account.cpp
    src/Data/AccountManager.cpp
    src/Data/Encryptor.cpp
    src/Data/SecureString.cpp

    src/GUI/AccountViewer.cpp
    src/GUI/AccountEditor.cpp
    src/GUI/RandomiserDialog.cpp
    src/GUI/ListEntry.cpp
)

add_executable(PasswordManager WIN32 ${SRC_FILES} PasswordManager.rc)
include("${wxWidgets_USE_FILE}")
target_include_directories(PasswordManager PRIVATE "${CMAKE_SOURCE_DIR}/include")

if (PM_BUILD_STATIC)
    target_link_libraries(PasswordManager PRIVATE OpenSSL::Crypto ${wxWidgets_LIBRARIES} Crypt32.lib)
else()
    target_link_libraries(PasswordManager PRIVATE OpenSSL::Crypto ${wxWidgets_LIBRARIES})
endif()