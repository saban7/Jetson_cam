#include <iostream>
#include <thread>
#include <chrono>
#include <opencv2/opencv.hpp>

// Include TaraXL SDK headers
#include "TaraXL.h"
#include "TaraXLCam.h"
#include "TaraXLDepth.h"

using namespace std;
using namespace cv;
using namespace TaraXLSDK; 

int main() {
    TaraXL taraxl;
    TaraXLCam selectedCam;
    TaraXLCamList taraxlCamList;
    TaraXLDepth *taraxlDepth;

    // 1. Enumerate and connect to the camera
    taraxl.enumerateDevices(taraxlCamList);
    if (taraxlCamList.size() == 0) {
        cout << "No TaraXL camera found." << endl;
        return -1;
    }
    
    selectedCam = taraxlCamList.at(0);
    if (selectedCam.connect() != TARAXL_SUCCESS) {
        cout << "Failed to connect to camera." << endl;
        return -1;
    }

    // 2. Initialize the separate Depth module required by this SDK version
    taraxlDepth = new TaraXLDepth(selectedCam);
    
    system("mkdir -p plant_scan_data");
    int num_frames = 60;
    
    cout << "Starting 360-degree scan... Ensure turntable is rotating." << endl;
    
    for (int i = 0; i < num_frames; i++) {
        Mat leftImage, rightImage, grayDisp, depthImage;
        
        // 3. Grab frames from the depth object
        if (taraxlDepth->getMap(leftImage, rightImage, grayDisp, true, depthImage, true) == TARAXL_SUCCESS) {
            char rgb_filename[256], depth_filename[256];
            sprintf(rgb_filename, "plant_scan_data/rgb_%03d.png", i);
            sprintf(depth_filename, "plant_scan_data/depth_%03d.png", i);
            
            imwrite(rgb_filename, leftImage);
            imwrite(depth_filename, depthImage);
            
            cout << "Captured frame " << i + 1 << "/" << num_frames << endl;
        } else {
            cout << "Failed to capture frame " << i + 1 << endl;
        }
        
        // Native C++11 timing
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
    selectedCam.disconnect();
    cout << "Scan complete!" << endl;
    return 0;
}
