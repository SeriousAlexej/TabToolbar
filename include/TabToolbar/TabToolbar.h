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
#ifndef TAB_TOOLBAR_H
#define TAB_TOOLBAR_H
#include <QTabWidget>
#include <QToolBar>
#include <QList>
#include <QTimer>
#include <memory>
#include <TabToolbar/API.h>

class QToolButton;
class QFrame;
class QMenu;

namespace tt
{
class Page;
class StyleParams;

class TT_API TabToolbar : public QToolBar
{
    Q_OBJECT
public:
    explicit TabToolbar(QWidget* parent = nullptr, unsigned _groupMaxHeight = 75, unsigned _groupRowCount = 3);
    virtual ~TabToolbar();

    void     SetSpecialTabEnabled(bool enabled);
    Page*    AddPage(const QString& pageName);
    QAction* HideAction();
    void     AddCornerAction(QAction* action);
    void     SetStyle(const QString& styleName);
    QString  GetStyle() const;
    unsigned RowCount() const;
    unsigned GroupMaxHeight() const;
    int      CurrentTab() const;
    void     SetCurrentTab(int index);

signals:
    void     Minimized();
    void     Maximized();
    void     SpecialTabClicked();
    void     StyleChanged();

private slots:
    void     FocusChanged(QWidget* old, QWidget* now);
    void     TabClicked(int index);
    void     CurrentTabChanged(int index);
    void     HideAt(int index = -1);
    void     HideTab(int index);
    void     ShowTab(int index);

protected:
    bool     event(QEvent* event) override;

private:
    void     AdjustVerticalSize(unsigned vSize);

    const unsigned groupRowCount;
    const unsigned groupMaxHeight;
    bool           hasSpecialTab = false;
    int            currentIndex = 0;
    unsigned       maxHeight = QWIDGETSIZE_MAX;
    QFrame*        cornerActions = nullptr;
    QAction*       hideAction = nullptr;
    QToolButton*   hideButton = nullptr;
    QAction*       tabBarHandle = nullptr;
    QTabWidget*    tabBar = nullptr;
    bool           ignoreStyleEvent = false;
    bool           isMinimized = false;
    bool           isShown = true;
    QTimer         tempShowTimer;
    std::unique_ptr<StyleParams> style;

    friend class Page;
};

TabToolbar* _FindTabToolbarParent(QWidget& startingWidget);

}
#endif
