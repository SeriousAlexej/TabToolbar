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
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QToolButton>
#include <QMenu>
#include <QAction>
#include <QSize>
#include <QApplication>
#include <QStyle>
#include <QPainter>
#include <QProxyStyle>
#include <QScreen>
#include <QStyleOptionToolButton>
#include <stdexcept>
#include <TabToolbar/Group.h>
#include <TabToolbar/Styles.h>
#include <TabToolbar/StyleTools.h>
#include <TabToolbar/SubGroup.h>
#include <TabToolbar/TabToolbar.h>
#include "CompactToolButton.h"
#include "ToolButtonStyle.h"

using namespace tt;

Group::Group(const QString& name, QWidget* parent) : QFrame(parent)
{
    setFrameShape(NoFrame);
    setLineWidth(0);
    setContentsMargins(0, 0, 0, 0);
    setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);

    QHBoxLayout* separatorLayout = new QHBoxLayout(this);
    separatorLayout->setMargin(0);
    separatorLayout->setContentsMargins(0, 0, 0, 0);
    separatorLayout->setSpacing(0);
    separatorLayout->setDirection(QBoxLayout::LeftToRight);
    setLayout(separatorLayout);

    QVBoxLayout* outerLayout = new QVBoxLayout();
    outerLayout->setMargin(0);
    outerLayout->setContentsMargins(0, 0, 0, 0);
    outerLayout->setSpacing(0);
    outerLayout->setDirection(QBoxLayout::TopToBottom);
    separatorLayout->addLayout(outerLayout);
    separatorLayout->addWidget(CreateSeparator());

    QFrame* innerFrame = new QFrame(this);
    innerFrame->setFrameShape(NoFrame);
    innerFrame->setLineWidth(0);
    innerFrame->setContentsMargins(0, 0, 0, 0);
    innerFrame->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);

    innerLayout = new QHBoxLayout(innerFrame);
    innerLayout->setMargin(0);
    innerLayout->setContentsMargins(2, 4, 2, 0);
    innerLayout->setSpacing(4);
    innerLayout->setDirection(QBoxLayout::LeftToRight);
    innerFrame->setLayout(innerLayout);

    outerLayout->addWidget(innerFrame);

    QLabel* groupName = new QLabel(name, this);
    groupName->setProperty("TTGroupName", QVariant(true));
    groupName->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    groupName->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    groupName->adjustSize();

    outerLayout->addWidget(groupName);

    const auto* parentTT = _FindTabToolbarParent(*this);
    if (!parentTT)
        throw std::runtime_error("Group should be constructed inside TabToolbar!");

    unsigned groupMaxHeight = parentTT->GroupMaxHeight();
    unsigned rowCount = parentTT->RowCount();
    const unsigned height = groupMaxHeight + groupName->height() + rowCount - 1;
    setMinimumHeight(height);
    setMaximumHeight(height);
}

SubGroup* Group::AddSubGroup(SubGroup::Align align)
{
    SubGroup* sgrp = new SubGroup(align, this);
    innerLayout->addWidget(sgrp);
    return sgrp;
}

QFrame* Group::CreateSeparator()
{
    QFrame* separator = new QFrame(this);
    separator->setProperty("TTSeparator", QVariant(true));
    separator->setAutoFillBackground(false);
    separator->setFrameShadow(QFrame::Plain);
    separator->setLineWidth(1);
    separator->setMidLineWidth(0);
    separator->setFrameShape(QFrame::VLine);
    return separator;
}

void Group::AddSeparator()
{
    innerLayout->addWidget(CreateSeparator());
}

void Group::AddAction(QToolButton::ToolButtonPopupMode type, QAction* action, QMenu* menu)
{
    if(type == QToolButton::MenuButtonPopup)
    {
        innerLayout->addWidget(new CompactToolButton(action, menu, this));
    }
    else
    {
        const int iconSize = GetPixelMetric(QStyle::PM_LargeIconSize) * GetScaleFactor(*this);
        QToolButton* btn = new QToolButton(this);
        btn->setProperty("TTInternal", QVariant(true));
        btn->setAutoRaise(true);
        btn->setDefaultAction(action);
        btn->setIconSize(QSize(iconSize, iconSize));
        btn->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
        btn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        btn->setPopupMode(type);
        btn->setStyle(new TTToolButtonStyle());
        if(menu)
            btn->setMenu(menu);
        innerLayout->addWidget(btn);
    }
}

void Group::AddWidget(QWidget* widget)
{
    widget->setParent(this);
    widget->setProperty("TTInternal", QVariant(true));
    innerLayout->addWidget(widget);
}
