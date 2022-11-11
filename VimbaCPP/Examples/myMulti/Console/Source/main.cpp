#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <string>
#include <sys/stat.h>

#include "VimbaCPP/Include/VimbaCPP.h"
#include "Common/StreamSystemInfo.h"
#include "Common/ErrorCodeToMessage.h"
#include "ApiController.h"
#include "Bitmap.h"
#include "Saver.h"

#define CAMERA_COUNT 3
#define LOG(x) std::cout<<x<<std::endl;

//some function declarations
std::string cmdParse(int argc, char* argv[]);
void printHelp(char* argv[]);
const std::string currentDateTime();

int main(int argc, char* argv[])
{
    VmbErrorType err; //initialize the variable used for error checking
    AVT::VmbAPI::Examples::ProgramConfig Config;
    std::string camID;
    AVT::VmbAPI::CameraPtr mainCam;
    //const char * pFileName = NULL;


    camID = cmdParse(argc, argv); //parse the command line arguments, should be a single camera name
    if(camID == "parse_error")
    {
        LOG("Program Ending");
        return 0;
    }
    else
    {

        AVT::VmbAPI::Examples::ApiController control;
        std::cout<<"Vimba C++ API Version: "<<control.GetVersion()<<std::endl; //print the current version
        
        Config.setCameraID(camID);
        std::cout<<"Setting Camera: "<<camID<<"\n";


        AVT::VmbAPI::CameraPtrVector cameras; //initialize the camera list
        VmbFrameStatusType status = VmbFrameStatusIncomplete;
        err = control.StartUp(); //start up vimba

        if(err ==  VmbErrorSuccess)
        {
            cameras = control.GetCameraList(); //fetch the cameras
            if(cameras.empty()) //end program if no cameras are detected
            {
                err = VmbErrorNotFound;
                LOG("ERROR: No cameras found");
                LOG("Program Ending");
                control.ShutDown();
                return 0;
            }
            bool checker = false;
            std::string tempID;
            for(int i = 0; i < cameras.size();i++) //compare the camID input over cmdline with those of detected cameras
            {
                err = cameras[i]->GetID(tempID);
                if(err != VmbErrorSuccess){LOG("ERROR: Couldnt read a cam ID");LOG("Program Ending");control.ShutDown();return 0;}
                if(camID == tempID)
                {
                    checker = true;
                    mainCam = cameras[i];
                }
            }
            if(checker)
            {
                LOG("Camera ID Matched") //confirm in log that the input camID matches a linked camera
                err = VmbErrorSuccess;
                Config.setCameraID(camID);
            }
            else{err = VmbErrorNotFound;}

            if(err == VmbErrorSuccess) //the loop will need to start in this scope
            {   
                std::string currDate = currentDateTime();

                for(int i = 0; i < currDate.size(); i++)
                {
                    if(currDate.at(i) == '.')
                    {
                        currDate.at(i) = '_';
                    } 
                    if(currDate.at(i)== ':')
                    {
                        currDate.at(i) = '-';
                    }
                }

                //create the unique folder to save the camera images in
                std::string sFolderName = camID + "_images_" + currDate;
                const char* folderName = sFolderName.c_str();
                if(mkdir(folderName,0777) == 0)
                {
                    LOG("Creating images folder for camera " + camID);
                }
                else {LOG("POTENTIAL ERROR: Folder not created. Not a problem if folder already exists");}


                std::cout<<"Opening camera with ID: "<<camID<<std::endl;
                control.setCameraID(camID);
                err = control.StartContinuousImageAcquisition(Config);

                if(VmbErrorSuccess == err)
                {
                    std::cout<< "Press <enter> to stop acquisition\n";
                    getchar();

                    control.StopContinuousImageAcquisition();
                }
            }
        }
        else{LOG("ERROR: Input ID did not match a detected camera");LOG("Program Ending");control.ShutDown();return 0;}

    
        control.ShutDown(); //end the vimba instance

        if ( VmbErrorSuccess == err )
        {
            std::cout<<"\nAcquisition stopped.\n" ;
        }
        else
        {
            std::string strError = control.ErrorCodeToMessage( err );
            std::cout<<"\nAn error occurred: " << strError << "\n";
        }

    } 
    return 0;
}

std::string cmdParse(int argc, char* argv[]) //function to get the camera ID from the command line
{
    std::string item;
    if(argc < 2){std::cout<<"ERROR: No arguments provided"<<std::endl;printHelp(argv);return "parse_error";}
    if(argc > 2){std::cout<<"ERROR: Too many arguments provided"<<std::endl;printHelp(argv);return "parse_error";}
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
    std::cout<<"Camera ID's look something like: \'DEV_000F314F3265\'. Only one ID can be accepted as an argument"<<std::endl<<std::endl;
}

const std::string currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

    return buf;
}