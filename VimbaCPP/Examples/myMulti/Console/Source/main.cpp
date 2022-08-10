#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <string>

#include "VimbaCPP/Include/VimbaCPP.h"
#include "Common/StreamSystemInfo.h"
#include "Common/ErrorCodeToMessage.h"
#include "ApiController.h"

#define CAMERA_COUNT 3

int main()
{
    VmbErrorType err; //initialize the variable used for error checking
    AVT::VmbAPI::Examples::ApiController apicontrol; 
    AVT::VmbAPI::Examples::ProgramConfig Config0;
    AVT::VmbAPI::Examples::ProgramConfig Config1;
    AVT::VmbAPI::Examples::ProgramConfig Config2;

    AVT::VmbAPI::CameraPtrVector cameras; //initialize the camera list

    std::cout<<"Vimba C++ API Version: "<<apicontrol.GetVersion()<<std::endl; //print the current version
    std::cout<<CAMERA_COUNT<<std::endl;


    err = apicontrol.StartUp(); //start up vimba
    
    if(VmbErrorSuccess == err)
    {
        if(Config0.getCameraID().empty() &&
           Config1.getCameraID().empty() &&
           Config2.getCameraID().empty() ) //if no cams are listed
        {
            cameras = apicontrol.GetCameraList(); //get the cams
            if(cameras.empty()) //if the cameras cannot be found
            {
                err = VmbErrorNotFound;
            }
        }
    }
    if(err == VmbErrorNotFound)
    {
        std::cout<<"No cameras found"<<std::endl;
        return 0;
    }
    else if (err == VmbErrorSuccess)
    {
        std::cout<<"Starting the cams";
        std::string strCameraID;

        if(cameras.size() == CAMERA_COUNT) //make sure that 3 cameras are being used
        {
            err = cameras[0]->GetID(strCameraID);
            if(VmbErrorSuccess == err)
            {
                Config0.setCameraID(strCameraID);
            }
            else{std::cout<<"Camera 0 not found. ID number " << cameras[0]->GetID(strCameraID) << std::endl;}
            err = cameras[1]->GetID(strCameraID);
            if(VmbErrorSuccess == err)
            {
                Config1.setCameraID(strCameraID);
            }
            else{std::cout<<"Camera 1 not found. ID number " << cameras[1]->GetID(strCameraID) << std::endl;}
            err = cameras[2]->GetID(strCameraID);
            if(VmbErrorSuccess == err)
            {
                Config2.setCameraID(strCameraID);
            }
            else{std::cout<<"Camera 2 not found. ID number " << cameras[2]->GetID(strCameraID) << std::endl;}
        }
        else {std::cout<<"Wrong number of cameras detected"<<std::endl;}

        //the apicontroller class and the camera class can both grab single images with frame pointers.
        // need to find how to make a frame pointer and save it

    }


    

    





    return 0;
}
