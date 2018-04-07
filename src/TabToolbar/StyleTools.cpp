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
#include <QMetaProperty>
#include <QFile>
#include <QSysInfo>
#include <QMap>
#include <QLocale>
#include <stdexcept>
#include <cstddef>
#include <TabToolbar/StyleTools.h>

namespace tt
{

static std::unique_ptr<QMap<QString, TStyleCreator>> styleMap;

static QString GetStyleTemplate()
{
    static QString styleTemplate;
    if(!styleTemplate.isEmpty())
        return styleTemplate;

    QFile f(":/tt/StyleTemplate.qss");
    f.open(QFile::ReadOnly);
    styleTemplate = f.readAll();
    return styleTemplate;
}

static QString FormatColor(const QColor& col)
{
    return QString("rgba(%1, %2, %3, %4)")
                      .arg(col.red())
                      .arg(col.green())
                      .arg(col.blue())
                      .arg(col.alpha());
}

static QString FormatColor(const std::vector<Color>& colors)
{
    const std::size_t sz = colors.size();
    if(sz == 1)
        return FormatColor(colors[0].value);

    QString result = "qlineargradient(x1:0, y1:1, x2:0, y2:0";
    for(const Color& col : colors)
        result += QString(", stop:") + QString::number(col.coefficient) + " " + FormatColor(col.value);
    result += ")";

    return result;
}

static void FillStyle(QString& style, const StyleParams& params)
{
    style.clear();
    if(params.UseTemplateSheet)
        style = GetStyleTemplate();
    style += params.AdditionalStyleSheet;

    int numProps = params.metaObject()->propertyCount();
    for(int i=0; i<numProps; i++)
    {
        const QMetaProperty prop = params.metaObject()->property(i);
        if(QString(prop.name()) == "objectName")
            continue;
        if(prop.type() == QVariant::Bool)
            continue;
        const QString propStr = QString("%") + prop.name() + "%";
        if(!style.contains(propStr))
            continue;
        style.replace(propStr, "%1");

        const QVariant property = params.property(prop.name());
        switch(prop.type())
        {
            case QVariant::String:
                style = style.arg(property.toString() + "px");
                break;
            case QVariant::Int:
                style = style.arg(property.toInt());
                break;
            case QVariant::UserType:
            {
                const Colors& colors = property.value<Colors>();
                if(colors.size() == 0)
                    throw std::runtime_error("Some property has no colors!");
                style = style.arg(FormatColor(colors));
                break;
            }
            default:
                throw std::runtime_error("Unknown property type in style!");
        }
    }
}

bool IsStyleRegistered(const QString& styleName)
{
    return styleMap->contains(styleName);
}

std::unique_ptr<StyleParams> CreateStyle(const QString& styleName)
{
    auto creator = styleMap->find(styleName);
    if(creator == styleMap->end())
        throw std::runtime_error(styleName.toStdString() + " - style is not registered!");

    StyleParams* style = (*creator)();
    style->setObjectName(styleName);
    return std::unique_ptr<StyleParams>(style);
}

QString GetSheetForStyle(const StyleParams& style)
{
    QString styleString;
    FillStyle(styleString, style);
    return styleString;
}

void RegisterStyle(const QString& styleName, const TStyleCreator& creator)
{
    if(!styleMap)
        styleMap.reset(new QMap<QString, TStyleCreator>());
    if(styleMap->contains(styleName))
        throw std::runtime_error(styleName.toStdString() + " - style already registered!");

    styleMap->insert(styleName, creator);
}

void UnregisterStyle(const QString& styleName)
{
    styleMap->remove(styleName);
}

QStringList GetRegisteredStyles()
{
    return styleMap->keys();
}

QString GetDefaultStyle()
{
#if defined(Q_OS_UNIX)
    return g_styleKool;
#elif defined(Q_OS_WIN)
    QString versionStr = QSysInfo::kernelVersion();
    const int dotIndex = versionStr.indexOf('.');
    if(dotIndex != -1)
        versionStr = versionStr.left(dotIndex + 2);
    versionStr.replace('.', '0');
    bool ok = false;
    const double versionDouble = QLocale().toDouble(versionStr, &ok);
    if(!ok)
        return g_styleKool;
    if(versionDouble >= 602)
        return g_styleThreshold;
    return g_styleVienna;
#endif
}

}//namespace tt
