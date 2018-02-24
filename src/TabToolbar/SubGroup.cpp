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
#include <QMenu>
#include <QAction>
#include <QSize>
#include <QSpacerItem>
#include <TabToolbar/SubGroup.h>
#include <TabToolbar/TabToolbar.h>

using namespace tt;

SubGroup::SubGroup(Align align, QWidget* parent) : QFrame(parent)
{
    setFrameShape(NoFrame);
    setLineWidth(0);
    setContentsMargins(0, 0, 0, 0);
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    innerLayout = new QVBoxLayout(this);
    innerLayout->setMargin(0);
    innerLayout->setContentsMargins(0, 0, 0, 0);
    innerLayout->setSpacing(1);
    innerLayout->setDirection(QBoxLayout::TopToBottom);
    QSpacerItem* spacer = new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, (align == Align::Yes ? QSizePolicy::Expanding : QSizePolicy::Ignored));
    innerLayout->addItem(spacer);
    setLayout(innerLayout);
}

void SubGroup::AddAction(QToolButton::ToolButtonPopupMode type, QAction* action, QMenu* menu)
{
    const int iconSize = QApplication::style()->pixelMetric(QStyle::PM_SmallIconSize);
    QFrame* frame = ConstructInnerFrame(0);

    QToolButton* btn = new QToolButton(this);
    btn->setProperty("TTInternal", QVariant(true));
    btn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    btn->setAutoRaise(true);
    btn->setDefaultAction(action);
    btn->setPopupMode(type);
    btn->setIconSize(QSize(iconSize, iconSize));
    if(menu)
        btn->setMenu(menu);
    btn->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);

    frame->layout()->addWidget(btn);
    innerLayout->insertWidget(innerLayout->count()-1, frame);
}

void SubGroup::AddWidget(QWidget* widget)
{
    QFrame* frame = ConstructInnerFrame(4);
    widget->setParent(frame);
    widget->setProperty("TTInternal", QVariant(true));
    frame->layout()->addWidget(widget);
    innerLayout->insertWidget(innerLayout->count()-1, frame);
}

void SubGroup::AddHorizontalButtons(const std::vector<ActionParams>& params)
{
    const int iconSize = QApplication::style()->pixelMetric(QStyle::PM_SmallIconSize);
    QFrame* frame = ConstructInnerFrame(0);
    frame->setProperty("TTHorizontalFrame", QVariant(true));

    for(auto& param : params)
    {
        QToolButton* btn = new QToolButton(this);
        btn->setProperty("TTInternal", QVariant(true));
        btn->setToolButtonStyle(Qt::ToolButtonIconOnly);
        btn->setAutoRaise(true);
        btn->setDefaultAction(param.action);
        btn->setPopupMode(param.type);
        btn->setIconSize(QSize(iconSize, iconSize));
        if(param.menu)
            btn->setMenu(param.menu);
        btn->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        frame->layout()->addWidget(btn);
    }

    innerLayout->insertWidget(innerLayout->count()-1, frame);
}

QFrame* SubGroup::ConstructInnerFrame(int spacing)
{
    unsigned groupMaxHeight;
    unsigned rowCount;
    bool found = false;
    QObject* par = this;
    do
    {
        par = par->parent();
        const TabToolbar* tt = dynamic_cast<TabToolbar*>(par);
        if(tt)
        {
            groupMaxHeight = tt->GroupMaxHeight();
            rowCount = tt->RowCount();
            found = true;
        }
    } while(par && !found);
    if(!found)
        throw std::runtime_error("SubGroup should be constructed inside TabToolbar!");

    QFrame* frame = new QFrame(this);
    frame->setFrameShape(QFrame::NoFrame);
    frame->setLineWidth(0);
    frame->setContentsMargins(0, 0, 0, 0);
    QSizePolicy policy(QSizePolicy::Maximum, QSizePolicy::Preferred);
    policy.setHorizontalStretch(0);
    policy.setVerticalStretch(1);
    frame->setSizePolicy(policy);
    frame->setMaximumHeight(groupMaxHeight / rowCount);
    QHBoxLayout* l = new QHBoxLayout(frame);
    l->setMargin(0);
    l->setContentsMargins(0, 0, 0, 0);
    l->setSpacing(spacing);
    l->setDirection(QBoxLayout::LeftToRight);
    frame->setLayout(l);
    QSpacerItem* spacer = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
    l->addItem(spacer);
    return frame;
}
