# Password Manager

A manager for passwords.

Capable of securely encrypting and storing account credentials that can later be accessed and searched. OpenSSL and wxWidgets were used to create this program and must be independently compiled if Password Manager is built from the source code. Alternatively, a statically-compiled executable is available for download.

# Compiling from Source (Windows)

Password Manager requires [OpenSSL](https://github.com/openssl/openssl/releases/tag/openssl-3.3.2) and [wxWidgets](https://github.com/wxWidgets/wxWidgets/releases/tag/v3.2.6) to be installed and compiled, after which the following steps must be taken to compile a release build.

1. Run `cmake -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH=[OpenSSL Build Folder];[wxWidgets Root Folder] -DwxWidgets_LIB_DIR=[wxWidgets Build Folder] [CMakeLists.txt Path]` wherever you want the build files to be placed.
2. Run `nmake` to compile the executable.
3. `libcrypto-3-x64.dll`, found where OpenSSL was installed, must be placed into the Password Manager directory.
4. `wxbase32u_vc_custom.dll`, `wxbase32u_xml_vc_custom.dll`, `wxmsw32u_core_vc_custom.dll`, and `wxmsw32u_qa_vc_custom.dll`, all found in `wxWidgets/lib/vc_x64_dll`, must be placed into the Password Manager directory.
5. Run `PasswordManager.exe` to start the program.

### Compiling Debug Version

To compile a debug version, set `-DCMAKE_BUILD_TYPE` to `Debug`, ensure that debug versions of OpenSSL and wxWidgets are available, and replace the libraries with their corresponding debug versions, e.g., `wxbase32u-vc_custom.dll` becomes `wxbase32ud_vc_custom.dll`.

### Supported CMake Flags

Password Manager supports the `PM_BUILD_STATIC` flag, which may be `ON` or `OFF`. If `ON`, Password Manager will attempt to compile statically, which requires static versions of wxWidgets and OpenSSL to be available at the specified locations. 