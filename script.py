import time
import cv2
import numpy as np
import os

# Import the e-con Systems TaraXL Python module. 
# (Ensure the python-bindings library from the SDK is in your PYTHONPATH)
import taraxl 

def capture_360_scan():
    output_dir = "plant_scan_data"
    os.makedirs(output_dir, exist_ok=True)
    
    # 1. Initialize the TaraXL Camera instance
    cam = taraxl.TaraXLCam()
    
    if not cam.connect():
        print("Error: Could not connect to the TaraXL camera.")
        return
        
    # Optional: Configure the camera for High Accuracy depth mapping
    # cam.set_accuracy_mode(taraxl.ACCURACY_HIGH)
    
    num_frames = 60
    delay_between_frames = 1.0 # 1 second per frame
    
    print(f"Starting 360-degree scan... Ensure the turntable is rotating.")
    
    for i in range(num_frames):
        start_time = time.time()
        
        # 2. Capture synchronized frames
        # The SDK returns numpy arrays for RGB (8-bit) and Depth (16-bit)
        success, rgb_frame, depth_frame = cam.get_frames()
        
        if success:
            # 3. Save RGB as standard PNG
            rgb_filename = f"{output_dir}/rgb_{i:03d}.png"
            cv2.imwrite(rgb_filename, rgb_frame)
            
            # 4. Save Depth as 16-bit PNG
            # It is critical to cast to uint16 so the millimeter depth values are not truncated
            depth_filename = f"{output_dir}/depth_{i:03d}.png"
            cv2.imwrite(depth_filename, depth_frame.astype(np.uint16))
            
            print(f"Captured frame {i+1}/{num_frames}")
        else:
            print(f"Warning: Failed to capture frame {i+1}")
            
        # 5. Wait for the next position (accounting for the time it took to capture/save)
        elapsed = time.time() - start_time
        sleep_time = max(0, delay_between_frames - elapsed)
        time.sleep(sleep_time)
        
    # Clean up
    cam.disconnect()
    print("Scan complete! RGB and Depth frames saved.")

if __name__ == "__main__":
    capture_360_scan()
