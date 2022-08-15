#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <string>
#include <thread>

#include "VimbaCPP/Include/VimbaCPP.h"
#include "Common/StreamSystemInfo.h"
#include "Common/ErrorCodeToMessage.h"
#include "ApiController.h"

#define CAMERA_COUNT 3


//void grabber(AVT::VmbAPI::Examples::ApiController &control,
//               AVT::VmbAPI::Examples::ProgramConfig &fig);

int main()
{
    VmbErrorType err; //initialize the variable used for error checking
    AVT::VmbAPI::Examples::ApiController apicontrol; 
    AVT::VmbAPI::Examples::ProgramConfig Config0;
    AVT::VmbAPI::Examples::ProgramConfig Config1;
    AVT::VmbAPI::Examples::ProgramConfig Config2;
   //std::vector<AVT::VmbAPI::Examples::ProgramConfig> figs(Config0,Config1,Config2);

    AVT::VmbAPI::CameraPtrVector cameras; //initialize the camera list

    std::cout<<"Vimba C++ API Version: "<<apicontrol.GetVersion()<<std::endl; //print the current version

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
    else{std::cout<<"Could not start up apicontroller"<<std::endl;}
    
    if(err == VmbErrorNotFound)
    {
        std::cout<<"No cameras found"<<std::endl;
        return 0;
    }
    else if (err == VmbErrorSuccess)
    {
        std::cout<<"Starting the cams"<<std::endl;
        std::string strCameraID;

        std::vector<bool> truths;
        if(cameras.size() == CAMERA_COUNT) //make sure that 3 cameras are being used
        {
            std::cout<<"Got the right number"<<std::endl;
            err = cameras[0]->GetID(strCameraID);
            if(VmbErrorSuccess == err)
            {
                Config0.setCameraID(strCameraID);
                std::cout<<"Got the cam0 set"<<std::endl;
                truths.push_back(true);
            }
            else{std::cout<<"Camera 0 not found. ID number " << cameras[0]->GetID(strCameraID) << std::endl;}
            err = cameras[1]->GetID(strCameraID);
            if(VmbErrorSuccess == err)
            {
                Config1.setCameraID(strCameraID);
                std::cout<<"Got the cam1 set"<<std::endl;
                truths.push_back(true);
            }
            else{std::cout<<"Camera 1 not found. ID number " << cameras[1]->GetID(strCameraID) << std::endl;}
            err = cameras[2]->GetID(strCameraID);
            if(VmbErrorSuccess == err)
            {
                Config2.setCameraID(strCameraID);
                std::cout<<"Got the cam2 set"<<std::endl;
                truths.push_back(true);
            }
            else{std::cout<<"Camera 2 not found. ID number " << cameras[2]->GetID(strCameraID) << std::endl;}
        }
        else {std::cout<<"Wrong number of cameras detected"<<std::endl;}

        //make sure that all the camera ID's were set
        if(truths.at(0) == true && truths.at(1) == true && truths.at(2) == true)
        {
            err = VmbErrorSuccess;
            std::cout<<"all cam ID's verified"<<std::endl;
        }
        else
        {
            err=VmbErrorNotFound;
            std::cout<<"Not all cam ID's were set"<<std::endl;
        }

        if(err = VmbErrorSuccess)   //this section should start the image aquisition process
        {
           //threading with the apiconrol.startcontinurous aquisition wont work

           //this might work?
           AVT::VmbAPI::Examples::FrameObserver* m_pFrameObserver = new AVT::VmbAPI::Examples::FrameObserver( 
            cameras[0], Config0.getFrameInfos(), Config0.getColorProcessing(), Config0.getRGBValue() );

            /*
           err = cameras[0]->StartContinuousImageAcquisition(3,AVT::VmbAPI::IFrameObserverPtr(m_pFrameObserver),
           Config0.getAllocAndAnnounce() ? AVT::VmbAPI::FrameAllocation_AllocAndAnnounceFrame : AVT::VmbAPI::FrameAllocation_AnnounceFrame);
            */

            std::thread t1(cameras[0]->StartContinuousImageAcquisition,3,AVT::VmbAPI::IFrameObserverPtr(m_pFrameObserver),
            Config0.getAllocAndAnnounce() ? AVT::VmbAPI::FrameAllocation_AllocAndAnnounceFrame : AVT::VmbAPI::FrameAllocation_AnnounceFrame);

            t1.join();

            //ok, as of right now, this method and the apicontrol methond return the error of: invalid use of non-static member function
            //going to need to fix that for anything to work, it seems
        }
        apicontrol.ShutDown();
    }

    

    





    return 0;
}



// void grabber(AVT::VmbAPI::Examples::ApiController &control,
//                  AVT::VmbAPI::Examples::ProgramConfig &fig)
// {
//     VmbErrorType err;
//     err = control.StartContinuousImageAcquisition(fig);
//     if(VmbErrorSuccess == err)
//     {
//         std::cout<<"Press to stop"<<std::endl;
//         getchar();

//         control.StopContinuousImageAcquisition();
//     }
// }
