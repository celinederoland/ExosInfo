#include "GrayLevelImage2D.hpp"

/**
 * Section 3
 */

GrayLevelImage2D::GrayLevelImage2D()
{
    GrayLevelImage2D(0,0,0);
}

GrayLevelImage2D::GrayLevelImage2D( int w, int h, GrayLevel g)
{
    m_width = w;
    m_height = h;
    m_data.assign(w*h,g);
    fill(g);
}

void GrayLevelImage2D::fill(GrayLevel g)
{
    m_data.assign(m_width*m_height,g);
}

int GrayLevelImage2D::index(int i, int j) const 
{
    return j*m_height + i;
}

GrayLevelImage2D::GrayLevel GrayLevelImage2D::at( int i, int j ) const
{
    return m_data.at(index(i,j));
}

GrayLevelImage2D::GrayLevel& GrayLevelImage2D::at( int i, int j )
{
    return m_data.at(index(i,j));
}

int GrayLevelImage2D::w() const
{
    return m_width;
}

int GrayLevelImage2D::h() const
{
    return m_height;
}

/**
 * Section 4
 */

GrayLevelImage2D::Container GrayLevelImage2D::getData()
{
    return m_data;
}

void GrayLevelImage2D::setData(Container d)
{
    m_data = d;
}

GrayLevelImage2D::Iterator::Iterator(GrayLevelImage2D& Image, int x, int y) : 
    std::vector<GrayLevelImage2D::GrayLevel>::iterator(
        Image.getData().begin() + Image.index(x,y)
    )
{
}

std::vector<GrayLevelImage2D::GrayLevel>::iterator GrayLevelImage2D::sbegin()
{
    return m_data.begin();
}

std::vector<GrayLevelImage2D::GrayLevel>::iterator GrayLevelImage2D::send()
{
    return m_data.end();
}

GrayLevelImage2D::Iterator GrayLevelImage2D::begin()
{
    return start(0,0);
}

GrayLevelImage2D::Iterator GrayLevelImage2D::end()
{
    return start(m_width,m_height - 1);
}

GrayLevelImage2D::Iterator GrayLevelImage2D::start( int x, int y )
{
    GrayLevelImage2D img(m_width,m_height,0);
    img.setData(m_data);
    GrayLevelImage2D::Iterator itC(img,x,y);
    return itC;
}