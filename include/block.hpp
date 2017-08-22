#ifndef _FILE_SYSTEM_BLOCK
#define _FILE_SYSTEM_BLOCK

#define BLOCK_SIZE 4096
#define HEADER_SIZE sizeof(char)

#include <article.hpp>
#include <string.h>

class FileSystemBlock {
    public:
        bool tryAdd(char*, unsigned);
        char* toByteArray();
        Article getArticle();

        static int getBlockSize();

        FileSystemBlock();
        FileSystemBlock(char*);

    private:
        char m_content[BLOCK_SIZE];
        unsigned m_header;

        unsigned HEADER_MASK = 0xFFF;

        int verifySpaceAvailable(unsigned);
};

#endif