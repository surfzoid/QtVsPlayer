#ifndef ERRORMANAGER_H
#define ERRORMANAGER_H
#include <QString>

class ErrorManager
{
public:
    ErrorManager();
   static QString  error_codes (uint32_t errcode) {
       QString errtxt;
       switch (errcode) {
       case 0:
           errtxt="No error";
           break;
       case 1:
           errtxt="Illegal input parameter";
           break;
       case 2:
           errtxt="Calling reference error (function are supposed to be called in another order).";
           break;
       case 3:
           errtxt="Set timer failure";
           break;
       case 4:
           errtxt="Video decoding failure";
           break;
       case 5:
           errtxt=" Audio decoding failure";
           break;
       case 6:
           errtxt="Memory allocation failure";
           break;
       case 7:
           errtxt="File operation failure";
           break;
       case 8:
           errtxt="Create thread failure";
           break;
       case 9:
           errtxt="Create directDraw failure";
           break;
       case 10:
           errtxt="Create off-screen failure";
           break;
       case 11:
           errtxt="Buffer overflow, input stream failure";
           break;
       case 12:
           errtxt="Create sound device failure";
           break;
       case 13:
           errtxt="Set volume failure";
           break;
       case 14:
           errtxt="This API can only be called in file decoding mode";
           break;
       case 15:
           errtxt="This API can only be called in stream decoding mode";
           break;
       case 16:
           errtxt="System not support, the SDK can only work with CPU above Pentium 3";
           break;
       case 17:
           errtxt="Missing file header";
           break;
       case 18:
           errtxt="Version mismatch between encoder and decoder";
           break;
       case 19:
           errtxt="Initialize decoder failure";
           break;
       case 20:
           errtxt="File too short or unrecognizable stream";
           break;
       case 21:
           errtxt="Initialize timer failure";
           break;
       case 22:
           errtxt="BLT failure";
           break;
       case 23:
           errtxt="Update overlay surface failure";
           break;
       case 24:
           errtxt="Open video & audio stream failure";
           break;
       case 25:
           errtxt="Open video stream failure";
           break;
       case 26:
           errtxt="JPEG compression failure";
           break;
       case 27:
           errtxt="File type not supported";
           break;
       case 28:
           errtxt="Data error";
           break;
       case 29:
           errtxt="Secret key error";
           break;
       case 30:
           errtxt="Key frame decoding failure";
           break;

       }
       return errtxt;
       }
};

#endif // ERRORMANAGER_H
