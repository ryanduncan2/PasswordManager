cmake_minimum_required(VERSION 3.24.2)
project(PasswordManager VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

find_package(OpenSSL REQUIRED)
find_package(wxWidgets 3.2.1 REQUIRED COMPONENTS core base qa xml)

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
target_link_libraries(PasswordManager PRIVATE OpenSSL::Crypto ${wxWidgets_LIBRARIES})