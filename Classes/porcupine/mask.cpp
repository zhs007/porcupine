#include "mask.hpp"

Mask::Mask()
    : m_width(0)
    , m_height(0)
    , m_pData(NULL)
{
    
}

Mask::~Mask()
{
    release();
}


void Mask::load(const char* filename)
{
    release();
    
    rapidjson::Document readdoc;
    loadJsonFile(filename, readdoc);
    
    m_width = readdoc["w"].GetInt();
    m_height = readdoc["h"].GetInt();
    
    m_pData = (int*)malloc(m_width * m_height * sizeof(int));
    
    rapidjson::Value& arr = readdoc["data"];
    if (arr.IsArray() && arr.Size() == m_width * m_height) {
        for (int i = 0; i < arr.Size(); ++i) {
            m_pData[i] = arr[i].GetInt();
        }
    }
    else {
        release();
    }
}

void Mask::release()
{
    if (m_pData != NULL) {
        free(m_pData);
        
        m_width = 0;
        m_height = 0;
        
        m_pData = NULL;
    }
}

int Mask::getData(int x, int y) const
{
    if (m_pData == NULL) {
        return 0;
    }
    
    if (x < 0 || x > m_width) {
        return 0;
    }
    
    if (y < 0 || y > m_height) {
        return 0;
    }
    
    return m_pData[y * m_width + x];
}