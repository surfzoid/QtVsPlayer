#ifndef ERRORMANAGER_H
#define ERRORMANAGER_H
#include <QString>
#include <QObject>

class ErrorManager
{
public:
    ErrorManager();
   static QString  error_codes (QString Source, unsigned int  errcode) {
       QString errtxt;
       switch (errcode) {
       case 0:
           errtxt=QObject::tr("No error");
           break;
       case 1:
           errtxt=QObject::tr("Illegal input parameter");
           break;
       case 2:
           errtxt=QObject::tr("Calling reference error (function are supposed to be called in another order).");
           break;
       case 3:
           errtxt=QObject::tr("Set timer failure");
           break;
       case 4:
           errtxt=QObject::tr("Video decoding failure");
           break;
       case 5:
           errtxt=QObject::tr(" Audio decoding failure");
           break;
       case 6:
           errtxt=QObject::tr("Memory allocation failure");
           break;
       case 7:
           errtxt=QObject::tr("File operation failure");
           break;
       case 8:
           errtxt=QObject::tr("Create thread failure");
           break;
       case 9:
           errtxt=QObject::tr("Create directDraw failure");
           break;
       case 10:
           errtxt=QObject::tr("Create off-screen failure");
           break;
       case 11:
           errtxt=QObject::tr("Buffer overflow, input stream failure");
           break;
       case 12:
           errtxt=QObject::tr("Create sound device failure");
           break;
       case 13:
           errtxt=QObject::tr("Set volume failure");
           break;
       case 14:
           errtxt=QObject::tr("This API can only be called in file decoding mode");
           break;
       case 15:
           errtxt=QObject::tr("This API can only be called in stream decoding mode");
           break;
       case 16:
           //errtxt=QObject::tr("System not support, the SDK can only work with CPU above Pentium 3");
           break;
       case 17:
           errtxt=QObject::tr("Missing file header");
           break;
       case 18:
           errtxt=QObject::tr("Version mismatch between encoder and decoder");
           break;
       case 19:
           errtxt=QObject::tr("Initialize decoder failure");
           break;
       case 20:
           errtxt=QObject::tr("File too short or unrecognizable stream");
           break;
       case 21:
           errtxt=QObject::tr("Initialize timer failure");
           break;
       case 22:
           errtxt=QObject::tr("BLT failure");
           break;
       case 23:
           errtxt=QObject::tr("Update overlay surface failure");
           break;
       case 24:
           errtxt=QObject::tr("Open video & audio stream failure");
           break;
       case 25:
           errtxt=QObject::tr("Open video stream failure");
           break;
       case 26:
           errtxt=QObject::tr("JPEG compression failure");
           break;
       case 27:
           errtxt=QObject::tr("File type not supported");
           break;
       case 28:
           errtxt=QObject::tr("Data error");
           break;
       case 29:
           errtxt=QObject::tr("Secret key error");
           break;
       case 30:
           errtxt=QObject::tr("Key frame decoding failure");
           break;
       case 31:
           errtxt=QObject::tr("NEED_MORE_DATA");
           break;
       case 32:
           errtxt=QObject::tr("INVALID_PORT");
           break;
       case 33:
           errtxt=QObject::tr("NOT_FIND");
           break;
       case 34:
           errtxt=QObject::tr("NEED_LARGER_BUFFER");
           break;
       case 99:
           errtxt=QObject::tr("FAIL_UNKNOWN");
           break;


       }
       if (errcode > 0 && errcode != 16) {
           printf("---Hik Sdk error response :from %s : %s\n\r", Source.toUtf8().data(), errtxt.toUtf8().data());
       }

       if (errcode != 16 && errtxt == "") {
           printf("---Hik Sdk error unknow response :from %s : %u\n\r", Source.toUtf8().data(), errcode);
       }

       return errtxt;
       }
};

#endif // ERRORMANAGER_H
