#ifndef _BLOCK_SIZE_
#define _BLOCK_SIZE_

#ifdef _WIN32 || WIN32 || _WIN64
#include <stdio.h>
#include <windows.h>

typedef BOOL (WINAPI *P_GDFSE)(LPCTSTR, PULARGE_INTEGER, 
    PULARGE_INTEGER, PULARGE_INTEGER);
#endif

class FileSystemBlockSize {
    public:
    static int getBlockSize() {
        #ifdef  _WIN32 || WIN32 || _WIN64
        BOOL  fResult;
        
        char path[] = "c:";
        char  *pszDrive  = NULL,
                szDrive[4];

        DWORD dwSectPerClust,
            dwBytesPerSect,
            dwFreeClusters,
            dwTotalClusters;

        P_GDFSE pGetDiskFreeSpaceEx = NULL;

        unsigned __int64 i64FreeBytesToCaller,
            i64TotalBytes,
            i64FreeBytes;

        fResult = GetDiskFreeSpace (path, 
            &dwSectPerClust,
            &dwBytesPerSect, 
            &dwFreeClusters,
            &dwTotalClusters);
        if (fResult) {
            /* force 64-bit math */ 
            // i64TotalBytes = (__int64)dwTotalClusters * dwSectPerClust *
            //                 dwBytesPerSect;
            // i64FreeBytes = (__int64)dwFreeClusters * dwSectPerClust *
            //                 dwBytesPerSect;
            
            return dwSectPerClust * dwBytesPerSect;
        }
        #endif
        
        return 4096;
    }

};

#endif