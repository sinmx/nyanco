/*!
    @file   TitleBar.cpp
    @author dasyprocta
 */

#include "TitleBar.h"
#include "Graphics.hpp"
#include "WindowManager.hpp"
#include "Frame.h"

BEGIN_NAMESPACE_NYANCO_GUI

// ----------------------------------------------------------------------------
TitleBar::Ptr TitleBar::Create(
    ComponentId                     id,
    std::string const&              caption,
    Frame<>::Ptr                    owner)
{
    TitleBar* titleBar = new TitleBar;

    titleBar->m_id      = id;
    titleBar->caption_  = caption;
    titleBar->owner_    = owner;
    titleBar->onMoving_ = false;
    titleBar->onClosing_ = false;

    titleBar->setLocation(Rect(0, 0, 0, 14));

    return TitleBar::Ptr(titleBar);
}

// ----------------------------------------------------------------------------
void TitleBar::setCaption(std::string const& caption)
{
    caption_ = caption;
}

// ----------------------------------------------------------------------------
std::string const& TitleBar::getCaption() const
{
    return caption_;
}

// ----------------------------------------------------------------------------
void TitleBar::draw(Graphics& graphics)
{
    Rect caption = location_;
    caption.bottom = caption.top + 14;
    graphics.setColor(0xff6666ff);

    WindowManager& wm = WindowManager::GetInterface();
    if (wm.getActiveWindow() != owner_)
        graphics.setRectColor(0xff4444aa, 0xff444466, 0xff4444aa, 0xff444466);
    else
        graphics.setRectColor(0xff0000aa, 0xff000066, 0xff0000aa, 0xff000066);
    graphics.drawFillRect(caption);
    graphics.drawText(Point(caption.left+1, caption.top+1), caption_, 0xffeeeeee);

    // close button
    Rect closeRegion = location_;
    closeRegion.right  -= 2;
    closeRegion.left    = closeRegion.right - 10;
    closeRegion.top    += 2;
    closeRegion.bottom -= 2;

    graphics.setRectColor(0xff444444);
    graphics.drawFillRect(closeRegion);

    graphics.setColor(0xff888888);
    graphics.drawLine(Point(closeRegion.left, closeRegion.top), Point(closeRegion.right, closeRegion.top));
    graphics.drawLine(Point(closeRegion.left, closeRegion.top), Point(closeRegion.left, closeRegion.bottom-1));

    graphics.setColor(0xff222222);
    graphics.drawLine(Point(closeRegion.right, closeRegion.top+1), Point(closeRegion.right, closeRegion.bottom));
    graphics.drawLine(Point(closeRegion.left, closeRegion.bottom), Point(closeRegion.right, closeRegion.bottom));

    graphics.setColor(0xffeeeeee);
    graphics.drawLine(Point(closeRegion.left+2, closeRegion.top+2), Point(closeRegion.right-1, closeRegion.bottom-1));
    graphics.drawLine(Point(closeRegion.left+2, closeRegion.bottom-2), Point(closeRegion.right-1, closeRegion.top+1));
}

// ----------------------------------------------------------------------------
int TitleBar::getHeight() const
{
    return 14;
}

// ----------------------------------------------------------------------------
bool TitleBar::onMouseProcess(MouseCommand const& command)
{
    if (command.onPushLeft)
    {
        // in button region
        Rect closeRegion = location_;
        closeRegion.right  -= 2;
        closeRegion.left    = closeRegion.right - 10;
        closeRegion.top    += 2;
        closeRegion.bottom -= 2;

        if (closeRegion.isInnerPoint(command.posX, command.posY))
        {
            onClosing_ = true;
        }
        else onMoving_ = true;
        return true;
    }
    else if (command.onUpLeft)
    {
        if (onClosing_)
        {
            // in button region
            Rect closeRegion = location_;
            closeRegion.right  -= 2;
            closeRegion.left    = closeRegion.right - 10;
            closeRegion.top    += 2;
            closeRegion.bottom -= 2;

            if (closeRegion.isInnerPoint(command.posX, command.posY))
            {
                WindowManager& wm = WindowManager::GetInterface();
                wm.detach(owner_);
            }
            onClosing_ = false;
        }
        else if (onMoving_)
        {
            onMoving_ = false;
        }
        return false;
    }
    else if (command.onDownLeft)
    {
        if (onMoving_)
        {
            owner_->move(command.moveX, command.moveY);
        }
        return true;
    }
    return false;
}

END_NAMESPACE_NYANCO_GUI
