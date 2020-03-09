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
#ifndef TT_STYLES_H
#define TT_STYLES_H
#include <QObject>
#include <QColor>
#include <QString>
#include <vector>
#include <functional>
#include <TabToolbar/API.h>

namespace tt
{
	
struct TT_API Color
{
    bool operator==(const Color& o) const { return o.coefficient == coefficient && o.value == value; }
    float  coefficient;
    QColor value;
};
class TT_API Colors
{
private:
    using TVectorType = std::vector<Color>;

public:
    Colors() = default;
    Colors(const Qt::GlobalColor& c)
        : m_colors(TVectorType({{1.0f, c}})) {}
    Colors(const QColor& c)
        : m_colors(TVectorType({{1.0f, c}})) {}
    Colors(const std::initializer_list<Color>& c)
        : m_colors(TVectorType(c)) {}

    operator QColor () const
    {
        return m_colors.at(0).value;
    }
    bool operator != (const Colors& c) const
    {
        return m_colors != c.m_colors;
    }
    Color& operator [] (std::size_t i)
    {
        return m_colors[i];
    }
    const Color& operator [] (std::size_t i) const
    {
        return m_colors[i];
    }
    TVectorType::size_type size() const
    {
        return m_colors.size();
    }
    TVectorType::iterator begin()
    {
        return m_colors.begin();
    }
    TVectorType::iterator end()
    {
        return m_colors.end();
    }
    TVectorType::const_iterator begin() const
    {
        return m_colors.cbegin();
    }
    TVectorType::const_iterator end() const
    {
        return m_colors.cend();
    }

private:
    TVectorType m_colors;
};

#define TT_PROPERTY(type, name)\
    Q_PROPERTY(type name MEMBER name)\
    type name
class TT_API StyleParams : public QObject
{
    Q_OBJECT
public:
    virtual ~StyleParams() = default;

    TT_PROPERTY(tt::Colors,   SeparatorColor);
    TT_PROPERTY(tt::Colors,   TabSelectedColor);
    TT_PROPERTY(tt::Colors,   TabUnselectedColor);
    TT_PROPERTY(tt::Colors,   PaneColor);
    TT_PROPERTY(tt::Colors,   TabHoverBorderColorSide);
    TT_PROPERTY(tt::Colors,   TabUnselectedHoverBorderColorSide);
    TT_PROPERTY(tt::Colors,   TabSpecialBorderColorSide);
    TT_PROPERTY(tt::Colors,   TabSpecialHoverBorderColorSide);
    TT_PROPERTY(tt::Colors,   TabSpecialHoverColor);
    TT_PROPERTY(tt::Colors,   TabSpecialColor);
    TT_PROPERTY(tt::Colors,   HorizontalFrameBackgroundColor);
    TT_PROPERTY(tt::Colors,   HorizontalFrameBorderColor);
    TT_PROPERTY(int,          HorizontalFrameBorderSize);
    TT_PROPERTY(tt::Colors,   HideArrowColor);
    TT_PROPERTY(tt::Colors,   TabUnselectedHoverBorderColorTop);
    TT_PROPERTY(tt::Colors,   TabHoverBorderColorTop);
    TT_PROPERTY(tt::Colors,   TabSpecialBorderColor);
    TT_PROPERTY(tt::Colors,   TabSpecialHoverBorderColor);
    TT_PROPERTY(tt::Colors,   TabSpecialFontColor);
    TT_PROPERTY(tt::Colors,   TabFontColor);
    TT_PROPERTY(tt::Colors,   ToolbarBackgroundColor);
    TT_PROPERTY(tt::Colors,   BorderColor);
    TT_PROPERTY(tt::Colors,   GroupNameColor);
    TT_PROPERTY(int,          TabBorderRadius);
    TT_PROPERTY(int,          TabSpacing);

    bool                      UseTemplateSheet;
    QString                   AdditionalStyleSheet;
};
#undef TT_PROPERTY

extern TT_API const QString g_styleKool;
extern TT_API const QString g_styleVienna;
extern TT_API const QString g_styleThreshold;

}
Q_DECLARE_METATYPE(tt::Colors)
#endif
