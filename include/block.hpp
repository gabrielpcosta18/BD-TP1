#ifndef _BLOCK_SIZE_
#define _BLOCK_SIZE_

#ifdef _WIN32 || WIN32 || _WIN64
#include <stdio.h>
#include <windows.h>

typedef BOOL (WINAPI *P_GDFSE)(LPCTSTR, PULARGE_INTEGER, 
    PULARGE_INTEGER, PULARGE_INTEGER);
#endif

class FileSystemBlock {
    public:
        struct BlockInfo {
            int m_headerInfo;
            char* m_content;
        };

        char* toByteArray();
        static int getBlockSize();
        FileSystemBlock();
        FileSystemBlock(char* bytes);
    
    private:
        BlockInfo m_info;
};

#endif