/*!
    @file   Rollup.cpp
    @author dasyprocta
 */

#include "Rollup.h"
#include "Graphics.hpp"
#include <boost/foreach.hpp>
#include <boost/bind.hpp>

#define foreach BOOST_FOREACH

BEGIN_NAMESPACE_NYANCO_GUI

// ----------------------------------------------------------------------------
Rollup::Ptr Rollup::Create(
    ComponentId                         id,
    std::string const&                  label)
{
    Ptr p(new Rollup);
    p->setMargin(Rect(4, 18, 4, 4));
    p->m_label = label;
    return p;
}

// ----------------------------------------------------------------------------
void Rollup::draw(Graphics& graphics)
{
    Rect box = location_;
    box.left    += 6;
    box.right   -= 6;
    box.top     += 2;
    box.bottom   = box.top + 14;

    //graphics.setColor(0xff444444);
    graphics.setRectColor(0xff777777, 0xff777777, 0xff333333, 0xff333333);
    graphics.drawFillRect(box);

    graphics.setColor(0xff888888);
    graphics.drawLine(Point(box.left, box.top), Point(box.right, box.top));
    graphics.drawLine(Point(box.left, box.top), Point(box.left, box.bottom-1));

    graphics.setColor(0xff222222);
    graphics.drawLine(Point(box.right, box.top+1), Point(box.right, box.bottom));
    graphics.drawLine(Point(box.left, box.bottom), Point(box.right, box.bottom));

    Rect clip = location_;
    clip.left += 2; clip.right -= 2;
    size_t textWidth = m_label.size() * 6;
    size_t left = (box.getWidth() - textWidth) / 2;
    graphics.drawText(Point(box.left + left, box.top + 2), m_label, 0xffeeeeee, clip);

    impl::FontInfo const& font = dynamic_cast<impl::Graphics&>(graphics).getFontInfo();
    sint32 const labelWidth  = font.charaWidth * m_label.size();
    sint32 const labelHeight = font.charaHeight;
    sint32 const labelHalfHeight = labelHeight / 2;

    // top
    graphics.setColor(0xff888888);
    graphics.drawLine(Point(box.left-4, box.top+2+labelHalfHeight), Point(box.left, box.top+2+labelHalfHeight));
    graphics.drawLine(Point(box.right, box.top+2+labelHalfHeight), Point(box.right+4, box.top+2+labelHalfHeight));
    graphics.setColor(0xff222222);
    graphics.drawLine(Point(box.left-6, box.top+labelHalfHeight), Point(box.left, box.top+labelHalfHeight));
    graphics.drawLine(Point(box.right, box.top+labelHalfHeight), Point(box.right+6, box.top+labelHalfHeight));

    Rect border = location_;
    border.top+=(2+labelHalfHeight); border.bottom-=2;
    if (!m_collapsed)
    {
        // �q�̕`��
        using boost::bind;
        using boost::ref;
        std::for_each(componentList_.begin(), componentList_.end(), bind(&Component::draw, _1, ref(graphics)));
    }
    graphics.setColor(0xff888888);
    // left
    graphics.drawLine(Point(border.left+2, border.top+2), Point(border.left+2, location_.bottom-1-2));
    // right
    graphics.drawLine(Point(border.right, border.top+1), Point(border.right, border.bottom));
    // bottom
    graphics.drawLine(Point(border.left, border.bottom), Point(border.right, border.bottom));

    graphics.setColor(0xff222222);
    // left
    graphics.drawLine(Point(border.left, border.top), Point(border.left, location_.bottom-1));
    // right
    graphics.drawLine(Point(border.right-2, border.top+1+2), Point(border.right-2, border.bottom-2));
    // bottom
    graphics.drawLine(Point(border.left+2, border.bottom-2), Point(border.right, border.bottom-2));
}

// ----------------------------------------------------------------------------
int Rollup::getHeight() const
{
    if (!m_collapsed)
    {
        sint32 height = 0;
        foreach (Component::Ptr p, componentList_)
        {
            height += p->getHeight();
        }
        return height + margin_.top + margin_.bottom;
    }
    return margin_.top + margin_.bottom;
}

// ----------------------------------------------------------------------------
bool Rollup::onMouseProcess(MouseCommand const& command)
{
    // TODO: �{�^�����E�`�F�b�N
    if (command.onPushLeft)
    {
        m_collapsed = !m_collapsed;
        location_.bottom = location_.top + getHeight();
    }
    return false;
}

// TODO: Container::checkHit �̃I�[�o�[���C�h
// �܂��ݎ��Ɏq�R���|�[�l���g�̃q�b�g�`�F�b�N�����Ȃ��悤�ɂ���

END_NAMESPACE_NYANCO_GUI