#include <block.hpp>

int FileSystemBlock::getBlockSize() {
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
        return dwSectPerClust * dwBytesPerSect;
    }
    #endif
    
    return 4096;
}

char* FileSystemBlock::toByteArray() {
    return reinterpret_cast<char*>(&m_info);
}

FileSystemBlock::FileSystemBlock() {
    m_info.m_content = new char[getBlockSize() - sizeof(m_info.m_headerInfo)];
}

FileSystemBlock::FileSystemBlock(char* bytes) {
    m_info = *(reinterpret_cast<FileSystemBlock::BlockInfo*>(bytes));
}