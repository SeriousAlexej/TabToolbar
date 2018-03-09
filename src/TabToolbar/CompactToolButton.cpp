/*
    TabToolbar - a small utility library for Qt, providing tabbed toolbars
    Copyright (C) 2018 Oleksii Sierov

    TabToolbar is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    TabToolbar is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with TabToolbar.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <QAction>
#include <QMenu>
#include <QToolButton>
#include <QPainter>
#include <QVBoxLayout>
#include <QStyle>
#include <QApplication>
#include <QLinearGradient>
#include <TabToolbar/TabToolbar.h>
#include <TabToolbar/StyleTools.h>
#include "CompactToolButton.h"

namespace
{
class TTHover : public QObject
{
public:
    TTHover(tt::CompactToolButton* parent, QToolButton* up, QToolButton* down) :
        QObject(parent),
        toolButton(parent),
        upButton(up),
        downButton(down)
    {
    }

protected:
    bool eventFilter(QObject* watched, QEvent* event) override
    {
        if(event->type() == QEvent::HoverLeave)
        {
            toolButton->SetHoverType(tt::CompactToolButton::Hover::None);
        }
        else if(event->type() == QEvent::HoverEnter)
        {
            if(watched == upButton)
                toolButton->SetHoverType(tt::CompactToolButton::Hover::Top);
            else if(watched == downButton)
                toolButton->SetHoverType(tt::CompactToolButton::Hover::Bottom);
        }
        if(watched == upButton)
        {
            if(event->type() == QEvent::Hide)
                downButton->hide();
            else if(event->type() == QEvent::Show)
                downButton->show();
            else if(event->type() == QEvent::EnabledChange)
                downButton->setEnabled(upButton->isEnabled());
        }
        return QObject::eventFilter(watched, event);
    }

private:
    tt::CompactToolButton* toolButton;
    QToolButton*           upButton;
    QToolButton*           downButton;
};
}

namespace tt
{

CompactToolButton::CompactToolButton(QAction* action, QMenu* menu, QWidget* parent) :
    QFrame(parent),
    borderColor(Qt::transparent)
{
    const int iconSize = QApplication::style()->pixelMetric(QStyle::PM_LargeIconSize);
    upButton = new QToolButton(this);
    upButton->setProperty("TTInternal", QVariant(true));
    upButton->setAutoRaise(true);
    upButton->setDefaultAction(action);
    upButton->setIconSize(QSize(iconSize, iconSize));
    upButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);

    QVBoxLayout* l = new QVBoxLayout(this);
    l->setMargin(0);
    l->setContentsMargins(0, 0, 0, 0);
    l->setSpacing(0);
    l->setDirection(QBoxLayout::TopToBottom);

    upButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
    upButton->setPopupMode(QToolButton::DelayedPopup);
    l->addWidget(upButton);

    downButton = new QToolButton(this);
    downButton->setProperty("TTInternal", QVariant(true));
    downButton->setAutoRaise(true);
    downButton->setToolButtonStyle(Qt::ToolButtonTextOnly);
    downButton->setPopupMode(QToolButton::InstantPopup);
    downButton->setMinimumHeight(25);
    downButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    downButton->setText(action->text());
    if(menu)
    {
        downButton->setMenu(menu);
        QObject::connect(menu, &QMenu::aboutToHide, this, [this]{ SetHoverType(Hover::None); });
    }
    l->addWidget(downButton);
    setLayout(l);

    TTHover* hover = new TTHover(this, upButton, downButton);
    upButton->installEventFilter(hover);
    downButton->installEventFilter(hover);

    QTimer::singleShot(500, [this] { GetBorderColor(); });
}

void CompactToolButton::SetHoverType(Hover type)
{
    hoverType = type;
    update();
}

bool CompactToolButton::event(QEvent* e)
{
    if(e->type() == QEvent::StyleChange)
        QTimer::singleShot(500, [this] { GetBorderColor(); update(); });
    return QFrame::event(e);
}

void CompactToolButton::paintEvent(QPaintEvent*)
{
    if(hoverType == Hover::None || !upButton->isEnabled())
        return;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, false);
    painter.setBrush(Qt::NoBrush);

    QRect rect(0, 1, width()-1, height()-3);

    QLinearGradient gradient;
    gradient.setColorAt(0.0f, Qt::transparent);
    gradient.setColorAt(downButton->height()*1.0f/rect.height(), borderColor);
    gradient.setColorAt(1.0f, borderColor);

    if(hoverType == Hover::Bottom)
    {
        gradient.setStart(0.0f, rect.height());
        gradient.setFinalStop(0.0f, 0.0f);
    }
    else
    {
        gradient.setStart(0.0f, 0.0f);
        gradient.setFinalStop(0.0f, rect.height());
    }
    QPen pen(QBrush(gradient), 1.0f);
    painter.setPen(pen);

    painter.drawRect(rect);
}

void CompactToolButton::GetBorderColor()
{
    QObject* par = this;
    do
    {
        par = par->parent();
        const TabToolbar* toolbar = dynamic_cast<TabToolbar*>(par);
        if(toolbar)
        {
            borderColor = Qt::transparent;
            const QString style = toolbar->GetStyle();
            if(!style.isEmpty())
            {
                auto styleParams = tt::CreateStyle(style);
                if(!styleParams->BorderColor.empty())
                    borderColor = tt::CreateStyle(style)->BorderColor;
            }
            return;
        }
    } while(par);
}

}
