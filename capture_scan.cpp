#include <iostream>
#include <thread>
#include <chrono>
#include <opencv2/opencv.hpp>

// Include TaraXL SDK headers
#include "TaraXLCam.h"
#include "TaraXLDepth.h"

using namespace std;
using namespace cv;
using namespace taraxl;

int main() {
    TaraXLCam cam;
    
    if (cam.connect() != TARAXL_SUCCESS) {
        cout << "Failed to connect to TaraXL camera." << endl;
        return -1;
    }

    // Enable depth map generation
    cam.enableDepth(true);
    
    system("mkdir -p plant_scan_data");
    int num_frames = 60;
    
    cout << "Starting 360-degree scan... Ensure turntable is rotating." << endl;
    
    for (int i = 0; i < num_frames; i++) {
        Mat leftImage, rightImage, depthImage;
        
        // Grab frames from the SDK
        if (cam.getMap(leftImage, rightImage, depthImage) == TARAXL_SUCCESS) {
            char rgb_filename[256], depth_filename[256];
            sprintf(rgb_filename, "plant_scan_data/rgb_%03d.png", i);
            sprintf(depth_filename, "plant_scan_data/depth_%03d.png", i);
            
            // Save RGB (Left Camera)
            imwrite(rgb_filename, leftImage);
            // Save Depth (16-bit PNG)
            imwrite(depth_filename, depthImage);
            
            cout << "Captured frame " << i + 1 << "/" << num_frames << endl;
        } else {
            cout << "Failed to capture frame " << i + 1 << endl;
        }
        
        // Wait 1 second before the next capture
        this_thread::sleep_for(chrono::seconds(1));
    }
    
    cam.disconnect();
    cout << "Scan complete!" << endl;
    return 0;
}
