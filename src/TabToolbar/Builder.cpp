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
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QFile>
#include <QMap>
#include <QByteArray>
#include <functional>
#include <stdexcept>
#include <TabToolbar/Builder.h>
#include <TabToolbar/TabToolbar.h>
#include <TabToolbar/Page.h>
#include <TabToolbar/Group.h>
#include <TabToolbar/SubGroup.h>

using namespace tt;

Builder::Builder(QWidget* parent) : QObject(parent)
{
}

QWidget* Builder::operator[](const QString& widgetName) const
{
    return guiWidgets.value(widgetName, nullptr);
}

void Builder::SetCustomWidgetCreator(const QString& name, const std::function<QWidget*()>& creator)
{
    customWidgetCreators[name] = [creator](const QJsonObject&) { return creator(); };
}

void Builder::SetCustomWidgetCreator(const QString& name, const std::function<QWidget*(const QJsonObject&)>& creator)
{
    customWidgetCreators[name] = creator;
}

TabToolbar* Builder::CreateTabToolbar(const QString& configPath)
{
    const QList<QAction*>& actions = parent()->findChildren<QAction*>();
    const QList<QMenu*>& menus = parent()->findChildren<QMenu*>();
    QMap<QString, QAction*> actionsMap;
    QMap<QString, QMenu*> menusMap;
    for(QAction* action : actions)
        actionsMap.insert(action->objectName(), action);
    for(QMenu* menu : menus)
        menusMap.insert(menu->objectName(), menu);

    QFile configFile(configPath);
    configFile.open(QIODevice::ReadOnly);
    const QJsonDocument config(QJsonDocument::fromJson(configFile.readAll()));
    const QJsonObject root = config.object();
    const int groupHeight = root["groupHeight"].toInt();
    const int groupRowCount = root["groupRowCount"].toInt();
    const bool hasSpecialTab = root["specialTab"].toBool();
    TabToolbar* tt = new TabToolbar((QWidget*)parent(), groupHeight, groupRowCount);

    auto CreateCustomWidget = [this, tt](const QString& name, const QJsonObject& item)
    {
        if(!customWidgetCreators.contains(name))
            throw std::logic_error(std::string("Unknown item type: ") + name.toStdString());
        QWidget* w = customWidgetCreators[name](item);
        if(item.contains("name"))
        {
            w->setObjectName(item["name"].toString());
            guiWidgets[w->objectName()] = w;
        }
        w->setParent(tt);
        return w;
    };

    auto CreateActionParams = [this, &actionsMap, &menusMap](const QJsonObject& obj)
    {
        SubGroup::ActionParams params;
        const QString type = obj["type"].toString();
        if(type == "delayedPopup")
            params.type = QToolButton::DelayedPopup;
        else if(type == "instantPopup")
            params.type = QToolButton::InstantPopup;
        else if(type == "menuButtonPopup")
            params.type = QToolButton::MenuButtonPopup;
        else
            throw std::logic_error(std::string("Unknown toolbutton popup mode: ") + type.toStdString());

        params.action = actionsMap[obj["name"].toString()];
        params.menu = nullptr;
        if(obj.contains("menu") && !obj["menu"].isNull())
            params.menu = menusMap[obj["menu"].toString()];
        return params;
    };

    const QJsonArray cornerActions = root["cornerActions"].toArray();
    for(int i=0; i<cornerActions.size(); i++)
        tt->AddCornerAction(actionsMap[cornerActions.at(i).toString()]);
    
    const QJsonArray menusList = root["menus"].toArray();
    for(int i=0; i<menusList.size(); i++)
    {
        const QJsonObject menuObject = menusList.at(i).toObject();
        QMenu* menu = new QMenu((QWidget*)parent());
        menu->setObjectName(menuObject["name"].toString());
        menusMap[menu->objectName()] = menu;
        guiWidgets[menu->objectName()] = menu;
        const QJsonArray menuActions = menuObject["actions"].toArray();
        for(int j=0; j<menuActions.size(); j++)
        {
            const QString actionName = menuActions.at(j).toString();
            if(actionName == "separator")
                menu->addSeparator();
            else
                menu->addActions({actionsMap[actionName]});
        }
    }

    const QJsonArray tabs = root["tabs"].toArray();
    for(int i=0; i<tabs.size(); i++)
    {
        const QJsonObject tab = tabs.at(i).toObject();
        const QString pageDisplayName = tab["displayName"].toString();
        const QString pageName = tab["name"].toString();
        Page* page = tt->AddPage(pageDisplayName);
        guiWidgets[pageName] = page;

        const QJsonArray groups = tab["groups"].toArray();
        for(int j=0; j<groups.size(); j++)
        {
            const QJsonObject groupObject = groups.at(j).toObject();
            const QString groupDisplayName = groupObject["displayName"].toString();
            const QString groupName = groupObject["name"].toString();
            Group* group = page->AddGroup(groupDisplayName);
            guiWidgets[groupName] = group;

            const QJsonArray content = groupObject["content"].toArray();
            for(int k=0; k<content.size(); k++)
            {
                const QJsonObject item = content.at(k).toObject();

                static const QList<QString> defaultTypes = {"action", "subgroup", "separator"};
                const QString itemType = item["itemType"].toString();
                int type = defaultTypes.indexOf(itemType);
                switch(type)
                {
                    case 0: //action
                    {
                        SubGroup::ActionParams params = CreateActionParams(item);
                        group->AddAction(params.type, params.action, params.menu);
                        break;
                    }
                    case 1: //subgroup
                    {
                        SubGroup::Align align = SubGroup::Align::Yes;
                        if(!item["aligned"].toBool())
                            align = SubGroup::Align::No;
                        SubGroup* subGroup = group->AddSubGroup(align);
                        const QString subGroupName = item["name"].toString();
                        subGroup->setObjectName(subGroupName);
                        guiWidgets[subGroupName] = subGroup;

                        const QJsonArray subGroupContent = item["content"].toArray();
                        for(int w=0; w<subGroupContent.size(); w++)
                        {
                            const QJsonObject sgItem = subGroupContent.at(w).toObject();

                            static const QList<QString> defaultSgTypes = {"action", "horizontalActions"};
                            const QString sgItemType = sgItem["itemType"].toString();
                            int sgType = defaultSgTypes.indexOf(sgItemType);
                            switch(sgType)
                            {
                                case 0: //action
                                {
                                    SubGroup::ActionParams params = CreateActionParams(sgItem);
                                    subGroup->AddAction(params.type, params.action, params.menu);
                                    break;
                                }
                                case 1: //horizontalActions
                                {
                                    std::vector<SubGroup::ActionParams> horizActions;
                                    const QJsonArray horizActionsArray = sgItem["actions"].toArray();
                                    for(int x=0; x<horizActionsArray.size(); x++)
                                    {
                                        const QJsonObject horizAction = horizActionsArray.at(x).toObject();
                                        horizActions.push_back(CreateActionParams(horizAction));
                                    }
                                    subGroup->AddHorizontalButtons(horizActions);
                                    break;
                                }
                                default:
                                {
                                    QWidget* w = CreateCustomWidget(sgItemType, sgItem);
                                    subGroup->AddWidget(w);
                                    break;
                                }
                            }
                        }
                        break;
                    }
                    case 2: //separator
                        group->AddSeparator();
                        break;
                    default: //custom widget
                    {
                        QWidget* w = CreateCustomWidget(itemType, item);
                        group->AddWidget(w);
                        break;
                    }
                }
            }
        }
    }

    tt->SetSpecialTabEnabled(hasSpecialTab);
    return tt;
}
