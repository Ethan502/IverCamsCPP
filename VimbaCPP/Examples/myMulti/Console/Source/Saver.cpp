#include <iostream>
#include <string>


#include "VimbaCPP/Include/VimbaCPP.h"
#include "Common/StreamSystemInfo.h"
#include "Common/ErrorCodeToMessage.h"
#include "Bitmap.h"

#ifndef LOG
#define LOG(x) std::cout<<x<<std::endl;
#endif

void WriteToBitmap(AVT::VmbAPI::FramePtr &pFrame, VmbErrorType &err, VmbFrameStatusType &status, const char* pFileName)
{
    err = pFrame->GetReceiveStatus(status);
    if(VmbErrorSuccess ==  err && VmbFrameStatusComplete == status)
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

void WriteToBitmap(AVT::VmbAPI::FramePtr pFrame)
{
    
    VmbFrameStatusType status = VmbFrameStatusIncomplete;
    VmbErrorType err = pFrame->GetReceiveStatus(status);
    if(VmbErrorSuccess ==  err && VmbFrameStatusComplete == status)
    {
        VmbPixelFormatType ePixelFormat = VmbPixelFormatMono8;
        err = pFrame->GetPixelFormat(ePixelFormat);
        if (VmbErrorSuccess == err)
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
                                const char* pFileName = "../../../../Image.bmp";
                                if(0 == AVTWriteBitmapToFile(&bitmap, pFileName))
                                {
                                    LOG("ERROR: Could not write bitmap to file");
                                    err = VmbErrorOther;
                                }
                                else
                                {
                                    std::cout<<"logged"<<std::endl;
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


