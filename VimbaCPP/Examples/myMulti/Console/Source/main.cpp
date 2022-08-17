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


std::string cmdParse(int argc, char* argv[]);
void printHelp(char* argv[]);

int main(int argc, char* argv[])
{
    VmbErrorType err; //initialize the variable used for error checking
    AVT::VmbAPI::Examples::ProgramConfig Config;
    std::string camID;


    camID = cmdParse(argc, argv);
    if(camID == "parse_error")
    {
        std::cout<<"Program ending, error thrown"<<"\n";
        return 0;
    }
    else
    {
        AVT::VmbAPI::Examples::ApiController control;
        std::cout<<"Vimba C++ API Version: "<<control.GetVersion()<<std::endl; //print the current version
        
        Config.setCameraID(camID);
        std::cout<<"Setting Camera: "<<camID<<"\n";

        
        err = control.StartUp(); //start up vimba

    }

    
    
    
        
    return 0;
}

std::string cmdParse(int argc, char* argv[]) //function to get the camera ID from the command line
{
    std::string item;
    if(argc < 2){std::cout<<"Error: No arguments provided"<<std::endl;printHelp(argv);return "parse_error";}
    if(argc > 2){std::cout<<"Error: Too many arguments provided"<<std::endl;printHelp(argv);return "parse_error";}
    if(argc == 2)
    {
        item = argv[1];
        return item;
    }
}

void printHelp(char* argv[])
{
    std::cout<<"To use the executable " << argv[0] << ", enter the command in like this:"<<std::endl<<std::endl;
    std::cout<<"\t\t ./FILE_NAME.exe <cam_id>"<<std::endl<<std::endl;
    std::cout<<"Camera ID's look something like: \'DEV_000F314F3265\'. Only one ID can be accepted as an argument"<<std::endl;
}