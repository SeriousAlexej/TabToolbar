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
#include <QStylePainter>
#include <QStyleOptionToolButton>
#include <QResizeEvent>
#include <TabToolbar/TabToolbar.h>
#include <TabToolbar/StyleTools.h>
#include "CompactToolButton.h"
#include "ToolButtonStyle.h"

namespace
{
class TTOverlayToolButton : public QToolButton
{
public:
   TTOverlayToolButton(QWidget* parent) : QToolButton(parent)
    {
       setAttribute(Qt::WA_NoSystemBackground);
       setAttribute(Qt::WA_TranslucentBackground);
       setAttribute(Qt::WA_TransparentForMouseEvents);
       parent->installEventFilter(this);
       lower();
   }

   bool paint = false;

protected:
   bool eventFilter(QObject* obj, QEvent* ev) override
   {
      if (obj == parent())
      {
         if (ev->type() == QEvent::Resize)
            resize(static_cast<QResizeEvent*>(ev)->size());
         else if (ev->type() == QEvent::ChildAdded)
            lower();
      }
      return QWidget::eventFilter(obj, ev);
   }

   void paintEvent(QPaintEvent*) override
   {
       if(!paint)
           return;

       QStylePainter sp(this);
       QStyleOptionToolButton opt;
       initStyleOption(&opt);
       opt.state |= QStyle::State_MouseOver | QStyle::State_AutoRaise | QStyle::State_Raised;
       opt.activeSubControls |= QStyle::SC_ToolButton;
       sp.drawComplexControl(QStyle::CC_ToolButton, opt);
   }
};

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
            toolButton->SetHover(false);
        }
        else if(event->type() == QEvent::HoverEnter)
        {
            if(watched == upButton || watched == downButton)
                toolButton->SetHover(upButton->isEnabled());
        }
        if(watched == upButton)
        {
            if(event->type() == QEvent::Hide)
                downButton->hide();
            else if(event->type() == QEvent::Show)
                downButton->show();
            else if(event->type() == QEvent::EnabledChange)
            {
                downButton->setEnabled(upButton->isEnabled());
                toolButton->SetHover(upButton->isEnabled() && upButton->underMouse());
            }
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
    QFrame(parent)
{
    overlay = new TTOverlayToolButton(this);

    const int iconSize = QApplication::style()->pixelMetric(QStyle::PM_LargeIconSize);
    upButton = new QToolButton(this);
    upButton->setProperty("TTInternal", QVariant(true));
    upButton->setAutoRaise(true);
    upButton->setDefaultAction(action);
    upButton->setIconSize(QSize(iconSize, iconSize));
    upButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    upButton->setStyle(new TTToolButtonStyle());
    upButton->setMaximumHeight(iconSize + 5);

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
    downButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
    downButton->setText(action->text());
    downButton->setToolTip(action->toolTip());
    downButton->setStyle(new TTToolButtonStyle());

    if(menu)
    {
        downButton->setMenu(menu);
        QObject::connect(menu, &QMenu::aboutToHide, this, [this]{ SetHover(false); });
    }
    l->addWidget(downButton);
    setLayout(l);

    TTHover* hover = new TTHover(this, upButton, downButton);
    upButton->installEventFilter(hover);
    downButton->installEventFilter(hover);
}

void CompactToolButton::SetHover(bool hover)
{
    static_cast<TTOverlayToolButton*>(overlay)->paint = hover;
    update();
}

}
