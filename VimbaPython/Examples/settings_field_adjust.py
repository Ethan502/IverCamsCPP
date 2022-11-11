from vimba import *
import sys
from typing import Optional


with Vimba.get_instance() as vimba:
    cams = vimba.get_all_cameras()

    args = sys.argv[1:]
    argc = len(args)

    isCam = False
    theCamera = cams[0]
    settings_file = args[1]

    if(args[0] == "-h" and argc == 1):
        print("Enter the command as follows: settings_field_adjust.py [cameraID] [fileName]")
        print("Listing the cameras")
        for cam in cams:
            print(f"    {cam.get_id()}")
        sys.exit()
    elif(argc > 2):
        print("Incorrect arguments. Abort")
        sys.exit()
    else:
        for cam in cams:
            if(args[0] == cam.get_id()):
                isCam = True
                theCamera = cam


    if(isCam == False):
        print("Not a valid camera ID, or the camera is not detected. Abort")
        sys.exit()

    with cams[0] as cam:
        cam.load_settings(settings_file, PersistType.All)
        print("Loaded the settings")

            

        
    


    


    













