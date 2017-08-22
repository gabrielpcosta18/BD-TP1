#include <block.hpp>
#include <iostream>
#include <stdio.h>
#include <cstring>

using namespace std;

int FileSystemBlock::m_blockSize = calcBlockSize();

int FileSystemBlock::calcBlockSize() {
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

int FileSystemBlock::getBlockSize() {
    return m_blockSize;
}

char* FileSystemBlock::toByteArray() {
    return reinterpret_cast<char*>(&m_info);
}

bool FileSystemBlock::tryAdd(Article article) {
    unsigned occupation = m_info.m_header & m_occupationMask;
    unsigned articleSize = article.getSizeOfData();

    if((occupation + 1) * articleSize >= getBlockSize() - sizeof(m_info.m_header)) {
        return false;
    }
    else {
        char* bytesArray = article.toByteArray();
        unsigned begin = occupation * articleSize; 
        
        for(unsigned i = begin; i < begin + articleSize ; ++i) {
            m_info.m_content[i] = bytesArray[i - begin];
        }

        occupation++;
        m_info.m_header = m_info.m_header | occupation; 
        return true;
    }
}

Article FileSystemBlock::getArticles() {
    if(m_info.m_header & m_occupationMask > 0)
        return Article(m_info.m_content);
    else return Article(-1);
}

FileSystemBlock::FileSystemBlock() {
    m_info.m_header = 0;
    m_info.m_content = new char[getBlockSize() - sizeof(m_info.m_header)];
}

FileSystemBlock::FileSystemBlock(const FileSystemBlock& other){
    int size = getBlockSize() - sizeof(m_info.m_header);
    m_info.m_header = 0;
    m_info.m_content = new char[size];
    memcpy(m_info.m_content, other.m_info.m_content, size);
}

FileSystemBlock::FileSystemBlock(char* bytes) {
    m_info.m_header = 0;
    m_info.m_content = new char[getBlockSize() - sizeof(m_info.m_header)];
    m_info = *(reinterpret_cast<FileSystemBlock::BlockInfo*>(bytes));
}

FileSystemBlock::~FileSystemBlock() {
    if(m_info.m_content != nullptr) {
        //delete[] m_info.m_content;
        // m_info.m_content = nullptr;
    }
}

FileSystemBlock& FileSystemBlock::operator= (const FileSystemBlock& other){
    if (this != &other) {
        int size = getBlockSize() - sizeof(m_info.m_header);
        memcpy(m_info.m_content, other.m_info.m_content, size);
    }
    return *this;
}