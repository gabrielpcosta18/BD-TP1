#ifndef _BLOCK_SIZE_
#define _BLOCK_SIZE_

#include <article.hpp>

#ifdef _WIN32 || WIN32 || _WIN64
#include <windows.h>

typedef BOOL (WINAPI *P_GDFSE)(LPCTSTR, PULARGE_INTEGER, 
    PULARGE_INTEGER, PULARGE_INTEGER);
#endif

class FileSystemBlock {
    public:
        struct BlockInfo {
            unsigned m_header;
            char* m_content;
        };

        char* toByteArray();
        static int getBlockSize();
        bool tryAdd(Article article);
        Article getArticles();

        FileSystemBlock();
        FileSystemBlock(const FileSystemBlock& other);
        FileSystemBlock(char* bytes);
        ~FileSystemBlock();

        FileSystemBlock& operator= (const FileSystemBlock& other);
        BlockInfo m_info;
    
    private:   
        unsigned m_occupationMask = 0xFFF;

        static int m_blockSize;
        static int calcBlockSize();
};

#endif