#ifndef SAVER_H
#define SAVER_H
void WriteToBitmap(AVT::VmbAPI::FramePtr &pFrame, VmbErrorType &err, VmbFrameStatusType &status, const char* pFileName);
void WriteToBitmap(AVT::VmbAPI::FramePtr pFrame);
#endif