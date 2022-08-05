#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <string>

#include "VimbaCPP/Include/VimbaCPP.h"
#include "Common/StreamSystemInfo.h"
#include "Common/ErrorCodeToMessage.h"
#include "ApiController.h"

int main()
{
    VmbErrorType err; //initialize the variable used for error checking
    AVT::VmbAPI::Examples::ApiController apicontrol; 
    AVT::VmbAPI::Examples::ProgramConfig Config1;
    AVT::VmbAPI::Examples::ProgramConfig Config2;
    AVT::VmbAPI::Examples::ProgramConfig Config3;



    std::cout<<"Vimba C++ API Version: "<<apicontrol.GetVersion()<<std::endl; //print the current version

    err = apicontrol.StartUp(); //start up vimba
    if(VmbErrorSuccess == err)
    {
        if(Config1.getCameraID().empty() &&
           Config1.getCameraID().empty() &&
           Config1.getCameraID().empty() ) //if no cams are listed
        {
            AVT::VmbAPI::CameraPtrVector cameras = apicontrol.GetCameraList(); //get the cams
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
        std::cout<<"Starting the cam"
    }
   




    return 0;
}
