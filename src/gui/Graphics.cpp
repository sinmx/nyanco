/*!
    @file   Graphics.cpp
    @author dasyprocta
 */

#include "Graphics.hpp"

namespace
{
    struct FontVertex
    {
        nyanco::float32                 x, y, z;
        nyanco::float32                 rhw;
        nyanco::uint32                  color;
        nyanco::float32                 tu, tv;

        static DWORD const              Fvf;
    };

    DWORD const FontVertex::Fvf = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1;

    struct GuiVertex
    {
        nyanco::float32                 x, y, z;
        nyanco::float32                 rhw;
        nyanco::uint32                  color;

        static DWORD const              Fvf;
    };

    DWORD const GuiVertex::Fvf = D3DFVF_XYZRHW | D3DFVF_DIFFUSE;

} // no namespace

BEGIN_NAMESPACE_NYANCO_GUI_IMPL

// ------------------------------------------------------------------------
void Graphics::setColor(
    Color                           color)
{
    color_ = color;
}

// ----------------------------------------------------------------------------
void Graphics::setRectColor(
    Color                           color)
{
    rectColor_[0] = rectColor_[1] = rectColor_[2] = rectColor_[3] = color;
}

// ----------------------------------------------------------------------------
void Graphics::setRectColor(
    Color                           leftTop,
    Color                           rightTop,
    Color                           leftBottom,
    Color                           rightBottom)
{
    rectColor_[0] = leftTop;
    rectColor_[1] = rightTop;
    rectColor_[2] = leftBottom;
    rectColor_[3] = rightBottom;
}

// ------------------------------------------------------------------------
void Graphics::drawText(
    Point const&                    point,
    std::string const&              text,
    Color                           color,
    Rect const&                     region)
{
    static float du = ((float)fontInfo_.charaWidth) / ((float)fontInfo_.texWidth);
    static float dv = ((float)fontInfo_.charaHeight) / ((float)fontInfo_.texHeight);

    uint32 vsize = text.size() * 4;
    FontVertex* v = new FontVertex[vsize];
    std::memset(v, 0, sizeof(FontVertex) * vsize);

    uint32 isize = text.size() * 6;
    uint16* indices = new uint16[isize];
    for (int i = 0, j = 0; i < isize; i += 6, j += 4)
    {
        indices[i]   = j;
        indices[i+1] = j+1;
        indices[i+2] = j+2;
        indices[i+3] = j;
        indices[i+4] = j+2;
        indices[i+5] = j+3;
    }

    for (int i = 0, count = 0; i < vsize; i += 4, count++)
    {
        uint8 code = text[count] - 0x21;

        v[i].rhw = v[i+1].rhw = v[i+2].rhw = v[i+3].rhw = 1.f;
        v[i].color = v[i+1].color = v[i+2].color = v[i+3].color = color;
        v[i].x    = (point.x) + (count * fontInfo_.charaWidth);
        v[i].y    = (point.y);
        v[i].tu   = (float)(code % 16 * fontInfo_.charaWidth + 0.5f) / fontInfo_.texWidth;//(code % 16) * du;
        v[i].tv   = (float)(code / 16 * fontInfo_.charaHeight + 0.5f) / fontInfo_.texHeight;//(code / 16) * dv;

        // スペース入れ忘れのためアドホック
        if (text[count] == 0x20)
        {
            v[i].tu = 1.f - du;
            v[i].tv = 1.f - dv;
        }
        v[i+1].x  = v[i].x + fontInfo_.charaWidth;
        v[i+1].y  = v[i].y;
        v[i+1].tu = v[i].tu + du;
        v[i+1].tv = v[i].tv;
        v[i+2].x  = v[i].x + fontInfo_.charaWidth;
        v[i+2].y  = v[i].y + fontInfo_.charaHeight;
        v[i+2].tu = v[i].tu + du;
        v[i+2].tv = v[i].tv + dv;
        v[i+3].x  = v[i].x;
        v[i+3].y  = v[i].y + fontInfo_.charaHeight;
        v[i+3].tu = v[i].tu;
        v[i+3].tv = v[i].tv + dv;
    }

    device_.SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    device_.SetRenderState(D3DRS_ALPHAREF, 0x08);
    device_.SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

    device_.SetFVF(FontVertex::Fvf);
    device_.SetTexture(0, fontTexture_);

    D3DVIEWPORT9 viewport, clipView;
    device_.GetViewport(&viewport);

    clipView.X = region.left < 0? 0: region.left;
    clipView.Y = region.top < 0? 0: region.top;
    clipView.Width = region.getWidth();
    clipView.Height = region.getHeight();
    clipView.MinZ = 0.0f;
    clipView.MaxZ = 1.0f;
    device_.SetViewport(&clipView);
    
    device_.DrawIndexedPrimitiveUP(
        D3DPT_TRIANGLELIST,
        0,
        vsize,
        text.size() * 2,
        indices,
        D3DFMT_INDEX16,
        v,
        sizeof(FontVertex));
    device_.SetTexture(0, 0);

    device_.SetViewport(&viewport);
    device_.SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

    delete[] v;
    delete[] indices;
}

// ------------------------------------------------------------------------
void Graphics::drawRect(
    Rect const&                     rect)
{
    GuiVertex v[4] =
    {
        { rect.left,    rect.top,       0.f,    1.f },
        { rect.right,   rect.top,       0.f,    1.f },
        { rect.right,   rect.bottom,    0.f,    1.f },
        { rect.left,    rect.bottom,    0.f,    1.f }
    };

    device_.SetFVF(GuiVertex::Fvf);
    device_.DrawPrimitiveUP(
        D3DPT_LINESTRIP,
        4,
        v,
        sizeof(GuiVertex));
}

// ------------------------------------------------------------------------
void Graphics::drawFillRect(
    Rect const&                     rect)
{
    GuiVertex v[4] =
    {
        { rect.left,    rect.top,       0.f,    1.f,    rectColor_[0] },
        { rect.right,   rect.top,       0.f,    1.f,    rectColor_[1] },
        { rect.right,   rect.bottom,    0.f,    1.f,    rectColor_[3] },
        { rect.left,    rect.bottom,    0.f,    1.f,    rectColor_[2] }
    };
device_.SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    device_.SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    device_.SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    device_.SetFVF(GuiVertex::Fvf);
    device_.DrawPrimitiveUP(
        D3DPT_TRIANGLEFAN,
        2,
        v,
        sizeof(GuiVertex));
}

// ------------------------------------------------------------------------
void Graphics::drawLine(
    Point const&                    p1,
    Point const&                    p2)
{
    GuiVertex v[2] =
    {
        { p1.x,     p1.y,   0.f,    1.f,    color_ },
        { p2.x,   p2.y,   0.f,    1.f,    color_ },
    };

    device_.SetFVF(GuiVertex::Fvf);
    device_.DrawPrimitiveUP(
        D3DPT_LINELIST,
        1,
        v,
        sizeof(GuiVertex));
}

// ----------------------------------------------------------------------------
void Graphics::drawIbeamCursor(
    Point const&                    p)
{
    
}

// ----------------------------------------------------------------------------
void Graphics::drawTriangle(
    Point const&                    p1,
    Point const&                    p2,
    Point const&                    p3)
{
    GuiVertex v[3] =
    {
        { p1.x, p1.y, 0.f, 1.f, color_ },
        { p2.x, p2.y, 0.f, 1.f, color_ },
        { p3.x, p3.y, 0.f, 1.f, color_ },
    };

    device_.SetFVF(GuiVertex::Fvf);
    device_.DrawPrimitiveUP(
        D3DPT_TRIANGLEFAN,
        1,
        v,
        sizeof(GuiVertex));
}

// ------------------------------------------------------------------------
Graphics::Graphics(
    IDirect3DDevice9*               devicePtr)
    : device_(*devicePtr),
      color_(0xffffffff)
{
    fontTexture_ = createFontTexture(devicePtr, fontInfo_);

    device_.SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    device_.SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    device_.SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

// ------------------------------------------------------------------------
Graphics::~Graphics()
{
    fontTexture_->Release();
}

END_NAMESPACE_NYANCO_GUI_IMPL

BEGIN_NAMESPACE_NYANCO_GUI

// ----------------------------------------------------------------------------
void ComponentGraphics::drawEdit(Rect const& rect)
{
    Rect a    = rect;
    a.right  -= 1;
    a.bottom -= 1;

    m_g.setRectColor(0x44444488);
    m_g.drawFillRect(a);

    m_g.setColor(0xff222222);
    m_g.drawLine(Point(a.left, a.top), Point(a.left, a.bottom-1));
    m_g.drawLine(Point(a.left, a.top), Point(a.right, a.top));

    m_g.setColor(0xff888888);
    m_g.drawLine(Point(a.right, a.top+1), Point(a.right, a.bottom));
    m_g.drawLine(Point(a.left, a.bottom), Point(a.right, a.bottom));
}

// ----------------------------------------------------------------------------
void ComponentGraphics::drawButton(Rect const& rect, bool pushed)
{
    Rect a    = rect;
    a.right  -= 1;
    a.bottom -= 1;
}

// ----------------------------------------------------------------------------
void ComponentGraphics::drawFrame(Rect const& rect, bool rise, bool gradation)
{
    Rect a    = rect;
    a.right  -= 1;
    a.bottom -= 1;

    if (gradation)
        m_g.setRectColor(0xff777777, 0xff777777, 0xff333333, 0xff333333);
    else
        m_g.setRectColor(0xff444444);
    m_g.drawFillRect(a);

    typedef std::pair<Color, Color> ColorPair;
    ColorPair col = rise?
        ColorPair(0xff888888, 0xff222222): ColorPair(0xff222222, 0xff888888);

    m_g.setColor(col.first);
    m_g.drawLine(Point(a.left, a.top), Point(a.left, a.bottom-1));
    m_g.drawLine(Point(a.left, a.top), Point(a.right, a.top));

    m_g.setColor(col.second);
    m_g.drawLine(Point(a.right, a.top+1), Point(a.right, a.bottom));
    m_g.drawLine(Point(a.left, a.bottom), Point(a.right+1, a.bottom));
}

END_NAMESPACE_NYANCO_GUI

