# Jetson_cam
plants scanner

Open your terminal in the same directory as the file and compile it using g++. This command links both OpenCV and the TaraXL SDK libraries:
g++ capture_scan.cpp -o capture_scan `pkg-config --cflags --libs opencv4` -ltaraxl -std=c++11

Start your turntable and immediately execute the compiled program:
./capture_scan


g++ capture_scan.cpp -o capture_scan -I/usr/local/taraxl-sdk/include -L/usr/local/taraxl-sdk/lib -L/usr/lib/libopencv_core.so.3.3 -lecon_taraxl -lopencv_core -lopencv_highgui -lopencv_imgcodecs -Wl,-rpath,/usr/lib/libopencv_core.so.3.3 -Wl,-rpath,/usr/local/taraxl-sdk/lib -std=c++11 -pthread


g++ capture_scan.cpp -o capture_scan -I/usr/local/taraxl-sdk/include -I/usr/local/taraxl-opencv/include -L/usr/local/taraxl-sdk/lib -L/usr/local/taraxl-opencv/lib -lecon_taraxl -lopencv_core -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -Wl,-rpath,/usr/local/taraxl-opencv/lib -Wl,-rpath,/usr/local/taraxl-sdk/lib -std=c++11 -pthread
