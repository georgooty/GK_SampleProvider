mkdir -p build
cd build
cmake ..
make install
cd ../
echo "Registering library with cim server"
./provider-register.sh -t sfcb -r mof/GK_TestIndication.registration -m mof/GK_Test.mof

