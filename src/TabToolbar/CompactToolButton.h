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
#ifndef TT_COMPACT_TOOL_BUTTON_H
#define TT_COMPACT_TOOL_BUTTON_H
#include <QFrame>
#include <QColor>

class QAction;
class QMenu;
class QToolButton;

namespace tt
{

class CompactToolButton: public QFrame
{
    Q_OBJECT
public:
    enum class Hover
    {
        Top,
        Bottom,
        None
    };

    CompactToolButton(QAction* action, QMenu* menu, QWidget* parent = nullptr);
    virtual ~CompactToolButton() = default;

    void SetHoverType(Hover type);

protected:
    bool event(QEvent* e) override;
    void paintEvent(QPaintEvent*) override;
    void GetBorderColor();

private:
    Hover        hoverType = Hover::None;
    QToolButton* upButton;
    QToolButton* downButton;
    QColor       borderColor;
};

}

#endif
