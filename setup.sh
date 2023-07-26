# Linux (Replace with CMake?)
sudo apt-get remove libsfml-dev -y
sudo apt-get install libsfml-dev -y

# jsoncpp
mkdir -p libs/
cd libs
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.sh
./vcpkg integrate install
./vcpkg install jsoncpp
