# Password Manager

A manager for passwords.

# Installation Instructions (Windows)

Password Manager requires [OpenSSL](https://github.com/openssl/openssl/releases/tag/openssl-3.3.2) and [wxWidgets](https://github.com/wxWidgets/wxWidgets/releases/tag/v3.2.6) to be installed and compiled, after which the following steps must be taken.

1. `libcrypto-3-x64.dll`, found where OpenSSL was installed, must be placed into the Password Manager directory.
2. `wxbase32ud_vc_custom.dll`, `wxbase32ud_xml_vc_custom.dll`, `wxmsw32ud_core_vc_custom.dll`, and `wxmsw32ud_qa_vc_custom.dll`, all found in `wxWidgets/lib/vc_x64_dll`, must be placed into the Password Manager directory.
3. Run `cmake -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH=[OpenSSL Folder];[wxWidgets Folder] [CMakeLists.txt Path]` wherever you want the build files to be placed.
4. Run `nmake` to compile the executable.