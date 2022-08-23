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
#include "Bitmap.h"

#define CAMERA_COUNT 3
#define LOG(x) std::cout<<x<<std::endl;


std::string cmdParse(int argc, char* argv[]);
void printHelp(char* argv[]);

int main(int argc, char* argv[])
{
    VmbErrorType err; //initialize the variable used for error checking
    AVT::VmbAPI::Examples::ProgramConfig Config;
    std::string camID;
    AVT::VmbAPI::CameraPtr mainCam;
    const char * pFileName = NULL;


    camID = cmdParse(argc, argv); //parse the command line arguments, should be a single camera name
    if(camID == "parse_error")
    {
        LOG("Program Ending");
        return 0;
    }
    else
    {
        if ( NULL == pFileName )
        {
            pFileName = "SynchronousGrab.bmp";
        }

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
            }
            else{err = VmbErrorNotFound;}

            if(err == VmbErrorSuccess) //the loop will need to start in this scope
            {   
                AVT::VmbAPI::FramePtr pFrame;
                while(true)
                {
                    std::cin.get();
                    err = control.AcquireSingleImage(camID, pFrame);
                    if(VmbErrorSuccess == err)
                    {
                        err = pFrame->GetReceiveStatus(status);
                        if(VmbErrorSuccess ==  err && 
                            VmbFrameStatusComplete == status)
                        {
                            VmbPixelFormatType ePixelFormat = VmbPixelFormatMono8;
                            err = pFrame->GetPixelFormat(ePixelFormat);
                            if (VmbErrorSuccess == err)
                            {
                                if ((VmbPixelFormatMono8 != ePixelFormat) && 
                                    (VmbPixelFormatRgb8 != ePixelFormat))
                                {
                                    err = VmbErrorInvalidValue;
                                }
                                else
                                {
                                    VmbUint32_t nImageSize = 0;
                                    err = pFrame->GetImageSize(nImageSize);
                                    if(VmbErrorSuccess == err)
                                    {
                                        VmbUint32_t nWidth = 0;
                                        err = pFrame->GetWidth(nWidth);
                                        if(VmbErrorSuccess == err)
                                        {
                                            VmbUint32_t nHeight = 0;
                                            err = pFrame->GetHeight(nHeight);
                                            if(VmbErrorSuccess == err)
                                            {
                                                VmbUchar_t *pImage = NULL;
                                                err = pFrame->GetImage(pImage);
                                                if(VmbErrorSuccess == err)
                                                {
                                                    AVTBitmap bitmap;

                                                    if(VmbPixelFormatRgb8 == ePixelFormat)
                                                    {
                                                        bitmap.colorCode = ColorCodeRGB24;
                                                    }
                                                    else
                                                    {
                                                        bitmap.colorCode = ColorCodeMono8;
                                                    }

                                                    bitmap.bufferSize = nImageSize;
                                                    bitmap.width = nWidth;
                                                    bitmap.height = nHeight;

                                                    //create the bitmap
                                                    if (0 == AVTCreateBitmap(&bitmap, pImage))
                                                    {
                                                        LOG("ERROR: Could not create bitmap");
                                                        err = VmbErrorResources;
                                                    }
                                                    else
                                                    {
                                                        //Save the bitmap
                                                        pFileName = "../../../../Image.bmp";
                                                        if(0 == AVTWriteBitmapToFile(&bitmap, pFileName))
                                                        {
                                                            LOG("ERROR: Could not write bitmap to file");
                                                            err = VmbErrorOther;
                                                        }
                                                        else
                                                        {
                                                            LOG("Bitmap successfully written to file");
                                                            //LOG("Ran"+std::to_string(i));
                                                            //release the bitmap buffer
                                                            if(0 == AVTReleaseBitmap(&bitmap))
                                                            {
                                                                LOG("ERROR: Could not release the bitmap");
                                                                err = VmbErrorInternalFault;
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            } 
                        }

                    }
                }
            }
            else{LOG("ERROR: Input ID did not match a detected camera");LOG("Program Ending");control.ShutDown();return 0;}

        }
        control.ShutDown(); //end the vimba instance

        if ( VmbErrorSuccess != err )
        {
            std::string strError = control.ErrorCodeToMessage( err );
            std::cout << "\nAn error occurred: " << strError.c_str() << "\n";
        }
        if( VmbFrameStatusIncomplete == status)
        {
            std::cout<<"received frame was not complete\n";
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