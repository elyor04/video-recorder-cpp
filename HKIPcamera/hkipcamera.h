#ifndef HKIPCAMERA_H
#define HKIPCAMERA_H

#include "HCNetSDK.h"
#include <opencv2/opencv.hpp>
#include <opencv2/core/ocl.hpp>

using namespace cv;

typedef void (CALLBACK *IMAGEDATACALLBACK) (UMat bgrUMat, void *pUser);

class HKIPcamera {
public:
    HKIPcamera();
    ~HKIPcamera() { release(); }
    bool login(char *ip, char *usr, char *password, WORD port = 8000,
               LONG channel = 1, DWORD streamtype = 0, DWORD linkmode = 0);
    void setImageDataCallBack(IMAGEDATACALLBACK fImageDataCallBack, void *pUser = NULL);
    void release();

public:
    LONG user_id_;
    LONG lRealPlayHandle_;
    int nPort_;
    LONG channel_;
    DWORD streamtype_;
    DWORD linkmode_;
    NET_DVR_DEVICEINFO_V30 struDeviceInfo_;
    IMAGEDATACALLBACK fImageDataCallBack_;
    void* pUser_;
    bool useOcl_;

private:
    bool OpenCamera(char *ip, char *usr, char *password, WORD port);
    bool ReadCamera();
};
#endif // HKIPCAMERA_H
