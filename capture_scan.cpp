#include <iostream>
#include <thread>
#include <chrono>
#include <opencv2/opencv.hpp>

#include "TaraXLCam.h"
#include "TaraXLDepth.h"

using namespace std;
using namespace cv;
// Updated namespace based on compiler notes
using namespace TaraXLSDK; 

int main() {
    TaraXLCam cam;
    
    if (cam.connect() != TARAXL_SUCCESS) {
        cout << "Failed to connect to TaraXL camera." << endl;
        return -1;
    }

    cam.enableDepth(true);
    
    system("mkdir -p plant_scan_data");
    int num_frames = 60;
    
    cout << "Starting 360-degree scan... Ensure turntable is rotating." << endl;
    
    for (int i = 0; i < num_frames; i++) {
        Mat leftImage, rightImage, depthImage;
        
        if (cam.getMap(leftImage, rightImage, depthImage) == TARAXL_SUCCESS) {
            char rgb_filename[256], depth_filename[256];
            sprintf(rgb_filename, "plant_scan_data/rgb_%03d.png", i);
            sprintf(depth_filename, "plant_scan_data/depth_%03d.png", i);
            
            imwrite(rgb_filename, leftImage);
            imwrite(depth_filename, depthImage);
            
            cout << "Captured frame " << i + 1 << "/" << num_frames << endl;
        } else {
            cout << "Failed to capture frame " << i + 1 << endl;
        }
        
        // Explicitly use std::this_thread and std::chrono
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
    cam.disconnect();
    cout << "Scan complete!" << endl;
    return 0;
}
