if [ ! -d android1 ] 
then 
mkdir android1 
fi

/home/gabridc/Qt/Tools/CMake/bin/cmake -DANDROID=1 \
 -S /home/gabridc/Repositorio/Broker \
 -B /home/gabridc/Repositorio/Broker/android1 \
-DCMAKE_TOOLCHAIN_FILE:PATH=/home/gabridc/Android/Sdk/ndk/build/cmake/android.toolchain.cmake \
-DANDROID_ABI:STRING=arm64-v8a \
-DANDROID_STL:STRING=c++_shared && \
cmake --build /home/gabridc/Repositorio/Broker/android1 --target all
