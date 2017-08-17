#include <block_size.hpp>

int FileSystemBlockSize::getBlockSize() {
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