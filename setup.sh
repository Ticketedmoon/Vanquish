# Linux (Replace with CMake?)
sudo apt-get remove libsfml-dev -y
sudo apt-get install libsfml-dev -y

rm -rf libs/
mkdir libs/
cd libs
mkdir headers/
cd headers
wget https://raw.githubusercontent.com/nlohmann/json/develop/single_include/nlohmann/json.hpp
