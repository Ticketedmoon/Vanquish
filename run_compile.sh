# g++ compiler for cpp files.
# -o to specify the output binary file name.
# -I to specify where the headers are (By convention, these are usually found in `include/`.
# -l to link to external libraries (SFML in this case)
# SFML usage requires `graphics`, `window` and `system` to be linked by default.
g++ -std=c++20 \
    ./src/*.cpp \
    ./src/user_interface/*.cpp \
    -o vanquish.exe \
    -I./include \
    -I./include/user_interface \
    -I./libs/headers \
    -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
