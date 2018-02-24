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
#ifndef TT_STYLE_TOOLS_H
#define TT_STYLE_TOOLS_H
#include <QStringList>
#include <functional>
#include <memory>
#include <TabToolbar/API.h>
#include <TabToolbar/Styles.h>

namespace tt
{
	
using TStyleCreator = std::function<StyleParams*()>;

TT_API void                         RegisterStyle(const QString& styleName, const TStyleCreator& creator);
TT_API void                         UnregisterStyle(const QString& styleName);
TT_API bool                         IsStyleRegistered(const QString& styleName);
TT_API QString                      GetSheetForStyle(const StyleParams& style);
TT_API QStringList                  GetRegisteredStyles();
TT_API std::unique_ptr<StyleParams> CreateStyle(const QString& styleName);
TT_API QString                      GetDefaultStyle();

}
#endif
