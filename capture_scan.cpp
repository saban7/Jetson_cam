// New v 23:00
#include <iostream>
#include <unistd.h> 
#include <sys/stat.h>
#include <string>
#include <opencv2/opencv.hpp>

// Include TaraXL SDK headers
#include "TaraXL.h"
#include "TaraXLCam.h"
#include "TaraXLDepth.h"

using namespace std;
using namespace cv;
using namespace TaraXLSDK; 

// Helper function to check if a directory exists
bool dirExists(const string& path) {
    struct stat info;
    if (stat(path.c_str(), &info) != 0) return false;
    return (info.st_mode & S_IFDIR) != 0;
}

int main() {
    TaraXL taraxl;
    TaraXLCam selectedCam;
    TaraXLCamList taraxlCamList;
    TaraXLDepth *taraxlDepth;

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

    taraxlDepth = new TaraXLDepth(selectedCam);
    
    // --- Directory Creation Logic ---
    string base_dir = "plant_scan_data";
    string current_dir = base_dir;
    int dir_counter = 2;

    while (dirExists(current_dir)) {
        current_dir = base_dir + "_" + to_string(dir_counter);
        dir_counter++;
    }

    string mkdir_cmd = "mkdir -p " + current_dir;
    system(mkdir_cmd.c_str());
    // --------------------------------
    
    int num_frames = 60;
    cout << "Saving scan to directory: " << current_dir << endl;
    cout << "Starting 360-degree scan... Ensure turntable is rotating." << endl;
    
    for (int i = 0; i < num_frames; i++) {
        Mat leftImage, rightImage, grayDisp, depthImage;
        
        if (taraxlDepth->getMap(leftImage, rightImage, grayDisp, true, depthImage, true) == TARAXL_SUCCESS) {
            char left_filename[256], right_filename[256], disp_filename[256], depth_filename[256];
            
            // Save using the dynamic current_dir variable
            sprintf(left_filename, "%s/left_%03d.png", current_dir.c_str(), i);
            sprintf(right_filename, "%s/right_%03d.png", current_dir.c_str(), i);
            sprintf(disp_filename, "%s/disp_%03d.png", current_dir.c_str(), i);
            sprintf(depth_filename, "%s/depth_%03d.png", current_dir.c_str(), i);
            
            imwrite(left_filename, leftImage);
            imwrite(right_filename, rightImage);
            imwrite(disp_filename, grayDisp);
            imwrite(depth_filename, depthImage);
            
            cout << "Captured frame " << i + 1 << "/" << num_frames << endl;
        } else {
            cout << "Failed to capture frame " << i + 1 << endl;
        }
        
        sleep(1);
    }
    
    selectedCam.disconnect();
    cout << "Scan complete!" << endl;
    return 0;
}
