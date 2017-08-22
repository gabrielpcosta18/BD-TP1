#include <block.hpp>

#include <iostream>

using namespace std;

char* FileSystemBlock::toByteArray() {
    return m_content;
}

int FileSystemBlock::verifySpaceAvailable(unsigned size) {
    return (m_content[m_header] & HEADER_MASK) == 0;
}

bool FileSystemBlock::tryAdd(char* bytes, unsigned size) {
    if(verifySpaceAvailable(size)) {
        for(int i = 0; i < size; ++i)
            m_content[i] = bytes[i];

        m_content[m_header] = m_content[m_header] | ((m_content[m_header] & HEADER_MASK) + 1);
        return true;
    }   

    return false;
}

Article FileSystemBlock::getArticle() {
    return Article(m_content);
}

int FileSystemBlock::getBlockSize() {
    return BLOCK_SIZE;
}

FileSystemBlock::FileSystemBlock() {
    m_header = BLOCK_SIZE - HEADER_SIZE;
    m_content[m_header] = 0;
}

FileSystemBlock::FileSystemBlock(char* bytes) {
    m_header = BLOCK_SIZE - HEADER_SIZE;
    memcpy(m_content, bytes, BLOCK_SIZE);
}