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
#ifndef TT_TOOL_BUTTON_STYLE
#define TT_TOOL_BUTTON_STYLE
#include <QProxyStyle>

class QStyleOptionToolButton;

namespace tt
{

//this class uses part of Qt's source code to align text and icon on ToolButton
class TTToolButtonStyle : public QProxyStyle
{
public:
    void drawControl(ControlElement element, const QStyleOption* opt, QPainter* p, const QWidget* widget) const override;
    void drawComplexControl(ComplexControl cc, const QStyleOptionComplex* opt, QPainter* p, const QWidget* widget) const override;
};

}
#endif
