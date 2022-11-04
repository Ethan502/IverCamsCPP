from vimba import *
from statements import *
import sys


print(intro) #Print the first hello
with Vimba.get_instance() as vimba:
    cams = vimba.get_all_cameras()
    if len(cams) == 0:
        print("There are no cameras detected")
        sys.exit()
    counter = 1
    for cam in cams: #list all the cameras
        print(f"{counter}. {cam.get_id()}")
        counter+=1
    
    if len(cams) > 1:
        print(f"Which camera would you like to choose? Enter a number 1-{counter-1}")
        num = int(input())-1
        selected_cam = cams[num]
    else:
        
        selected_cam = cams[0]

    print(f"You selected: {selected_cam.get_id()}")
    


    















        # print(cam.get_id())
        # exposure_time = cam.ExposureTimeAbs
        # print(exposure_time.get())
        # exposure_time.set(12000)