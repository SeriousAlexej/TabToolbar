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
#ifndef TT_SUB_GROUP_H
#define TT_SUB_GROUP_H
#include <QFrame>
#include <QLabel>
#include <QPixmap>
#include <QToolButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QApplication>
#include <QStyle>
#include <vector>
#include <TabToolbar/API.h>

class QAction;
class QMenu;

namespace tt
{

class TT_API SubGroup : public QFrame
{
    Q_OBJECT
public:
    enum class Align
    {
        Yes,
        No
    };

    struct ActionParams
    {
        QToolButton::ToolButtonPopupMode type;
        QAction* action;
        QMenu* menu;
    };

    SubGroup(Align align, QWidget* parent = nullptr);

    void       AddAction(QToolButton::ToolButtonPopupMode type, QAction* action, QMenu* menu = nullptr);
    void       AddHorizontalButtons(const std::vector<ActionParams>& params);
    void       AddWidget(QWidget* widget);

private:
    QFrame*    ConstructInnerFrame(int spacing);

    QVBoxLayout* innerLayout;
};

}
#endif
