#ifndef DATA_BUFFER_H
#define DATA_BUFFER_H
#include <vector>
#include <algorithm>

class buffer
{
public:
    buffer( size_t w, size_t h)
        : m_Width( w )
        , m_Height( h )
    {
        init();
    }
    size_t dataSize() const
    {
        return m_Width * m_Height;
    }

    size_t width() const
    {
        return m_Width;
    }

    size_t height() const
    {
        return m_Height;
    }

    std::vector<uint8_t*> rows()
    {
        std::vector<uint8_t*> result(height());
        for (size_t pos = 0; pos < height(); ++pos)
        {
            result[pos] = m_Data.data() + pos * width();
        }
        return result;
    }
private:
    void init()
    {
        m_Data.resize(dataSize());
        std::generate(m_Data.begin(), m_Data.end(), []() {return static_cast<uint8_t>(rand() / 0xff); });
    }
private:
    const size_t m_Width;
    const size_t m_Height;
    std::vector<uint8_t> m_Data;
};

#endif // DATA_BUFFER_H
