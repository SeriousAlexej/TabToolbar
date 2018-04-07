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
#ifndef TT_BUILDER_H
#define TT_BUILDER_H
#include <QToolButton>
#include <QString>
#include <QAction>
#include <QMenu>
#include <QMap>
#include <QWidget>
#include <functional>
#include <TabToolbar/API.h>

class QJsonObject;

namespace tt
{

class TabToolbar;

class TT_API Builder : public QObject
{
    Q_OBJECT
public:
    explicit Builder(QWidget* parent);

    TabToolbar* CreateTabToolbar(const QString& configPath);
    void        SetCustomWidgetCreator(const QString& name, const std::function<QWidget*()>& creator);
    void        SetCustomWidgetCreator(const QString& name, const std::function<QWidget*(const QJsonObject&)>& creator);

    QWidget*    operator[](const QString& widgetName) const;

private:
    QMap<QString, QWidget*> guiWidgets;
    QMap<QString, std::function<QWidget*(const QJsonObject&)>> customWidgetCreators;
};

}
#endif
