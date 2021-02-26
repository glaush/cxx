@ECHO ON

conan install . -pr Conan/vs_debug_x86_64 -if Conan/ --build=missing
conan install . -pr Conan/vs_release_x86_64 -if Conan/ --build=missing

