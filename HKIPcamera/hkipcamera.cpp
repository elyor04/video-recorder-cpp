#include "./hkipcamera.h"
#include "LinuxPlayM4.h"
#include <cstdio>
#include <cstring>
#include <iostream>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

using namespace std;

void yv12toBGRMat(char *inYv12, int width, int height, UMat &outMat) {
    int yuvHeight = height * 3 / 2;
    Mat yuvImg(yuvHeight, width, CV_8UC1, inYv12);
    cvtColor(yuvImg.getUMat(ACCESS_READ), outMat, COLOR_YUV2BGR_YV12);
}

void CALLBACK DecCBFun(int nPort, char *pBuf, int nSize, FRAME_INFO *pFrameInfo,
                       void *pUser, int nReserved2) {
    HKIPcamera *hkipc = static_cast<HKIPcamera *>(pUser);
    LONG lFrameType = pFrameInfo->nType;

    if (hkipc->fImageDataCallBack_ == NULL)
        return;
    if (lFrameType == T_YV12) {
        UMat bgrUMat;
        yv12toBGRMat(pBuf, pFrameInfo->nWidth, pFrameInfo->nHeight, bgrUMat);
        hkipc->fImageDataCallBack_(bgrUMat, hkipc->pUser_);
    }
}

/// real time stream
void CALLBACK fRealDataCallBack(LONG lRealHandle, DWORD dwDataType,
                                BYTE *pBuffer, DWORD dwBufSize, void *pUser) {
    HKIPcamera *hkipc = static_cast<HKIPcamera *>(pUser);
    switch (dwDataType) {
    case NET_DVR_SYSHEAD:
        if (!PlayM4_GetPort(&hkipc->nPort_))
            break;

        if (dwBufSize > 0) {
            if (!PlayM4_OpenStream(hkipc->nPort_, pBuffer, dwBufSize, 1024 * 1024))
                break;
            if (!PlayM4_SetDecCallBackMend(hkipc->nPort_, DecCBFun, pUser))
                break;
            if (!PlayM4_Play(hkipc->nPort_, NULL))
                break;
        }
        break;

    case NET_DVR_STREAMDATA:
        if ((dwBufSize > 0) && (hkipc->nPort_ != -1)) {
            BOOL inData = PlayM4_InputData(hkipc->nPort_, pBuffer, dwBufSize);
            while (!inData) {
                cout << "PlayM4_InputData failed\n";
                sleep(10);
                inData = PlayM4_InputData(hkipc->nPort_, pBuffer, dwBufSize);
            }
        }
        break;
    }
}

void CALLBACK g_ExceptionCallBack(DWORD dwType, LONG user_id_, LONG lHandle,
                                  void *pUser) {
    switch (dwType) {
    case EXCEPTION_RECONNECT:
        printf("----------reconnect--------%d\n", time(NULL));
        break;
    default:
        cout << "Exception: " << dwType << endl;
        break;
    }
}

HKIPcamera::HKIPcamera() {
    user_id_ = -1;
    lRealPlayHandle_ = -1;
    nPort_ = -1;
    channel_ = 1;
    streamtype_ = 0;
    linkmode_ = 0;
    fImageDataCallBack_ = NULL;
    pUser_ = NULL;
    ocl::setUseOpenCL(true);
}

bool HKIPcamera::login(char *ip, char *usr, char *password, WORD port,
                       LONG channel, DWORD streamtype, DWORD linkmode) {
    NET_DVR_Init();
    NET_DVR_SetConnectTime(2000, 1);
    NET_DVR_SetReconnect(10000, true);
    if (!OpenCamera(ip, usr, password, port)) {
        NET_DVR_Cleanup();
        return false;
    }
    channel_ = channel;
    streamtype_ = streamtype;
    linkmode_ = linkmode;
    if (!ReadCamera()) {
        NET_DVR_Logout(user_id_);
        NET_DVR_Cleanup();
        return false;
    }
    return true;
}

bool HKIPcamera::OpenCamera(char *ip, char *usr, char *password, WORD port) {
    user_id_ = NET_DVR_Login_V30(ip, port, usr, password, &struDeviceInfo_);
    if (user_id_ < 0) {
        printf("Login failed! Error number: %d\n", NET_DVR_GetLastError());
        return false;
    }
    return true;
}

bool HKIPcamera::ReadCamera() {
    NET_DVR_SetExceptionCallBack_V30(0, NULL, g_ExceptionCallBack, NULL);

    NET_DVR_PREVIEWINFO previewInfo = {0};
    previewInfo.hPlayWnd = NULL;
    previewInfo.lChannel = (LONG)(struDeviceInfo_.byStartDChan) + channel_;
    previewInfo.dwStreamType = streamtype_;
    previewInfo.dwLinkMode = linkmode_;

    lRealPlayHandle_ = NET_DVR_RealPlay_V40(user_id_, &previewInfo, fRealDataCallBack, this);
    if (lRealPlayHandle_ < 0) {
        printf("NET_DVR_RealPlay_V30 failed! Error number: %d\n",
               NET_DVR_GetLastError());
        return false;
    }
    return true;
}

void HKIPcamera::setImageDataCallBack(IMAGEDATACALLBACK fImageDataCallBack, void *pUser) {
    fImageDataCallBack_ = fImageDataCallBack;
    pUser_ = pUser;
}

void HKIPcamera::release() {
    NET_DVR_StopRealPlay(lRealPlayHandle_);
    NET_DVR_Logout(user_id_);
    NET_DVR_Cleanup();
}
