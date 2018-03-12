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
#include <QPainter>
#include <QStyleOptionToolButton>
#include "ToolButtonStyle.h"

using namespace tt;

//redefine text alignment
void TTToolButtonStyle::drawControl(ControlElement element, const QStyleOption* opt, QPainter* p, const QWidget* widget) const
{
    if(element == CE_ToolButtonLabel)
    {
        if (const QStyleOptionToolButton* toolbutton = static_cast<const QStyleOptionToolButton*>(opt))
        {
            QRect rect = toolbutton->rect;
            int shiftX = 0;
            int shiftY = 0;
            if (toolbutton->state & (State_Sunken | State_On)) {
                shiftX = proxy()->pixelMetric(PM_ButtonShiftHorizontal, toolbutton, widget);
                shiftY = proxy()->pixelMetric(PM_ButtonShiftVertical, toolbutton, widget);
            }
            // Arrow type always overrules and is always shown
            bool hasArrow = toolbutton->features & QStyleOptionToolButton::Arrow;
            if (((!hasArrow && toolbutton->icon.isNull()) && !toolbutton->text.isEmpty())
                || toolbutton->toolButtonStyle == Qt::ToolButtonTextOnly) {
                int alignment = Qt::AlignTop | Qt::AlignHCenter | Qt::TextShowMnemonic;
                if (!proxy()->styleHint(SH_UnderlineShortcut, opt, widget))
                    alignment |= Qt::TextHideMnemonic;
                rect.translate(shiftX, shiftY);
                p->setFont(toolbutton->font);
                proxy()->drawItemText(p, rect, alignment, toolbutton->palette,
                                      opt->state & State_Enabled, toolbutton->text,
                                      QPalette::ButtonText);
            } else {
                QPixmap pm;
                QSize pmSize = toolbutton->iconSize;
                if (!toolbutton->icon.isNull()) {
                    QIcon::State state = toolbutton->state & State_On ? QIcon::On : QIcon::Off;
                    QIcon::Mode mode;
                    if (!(toolbutton->state & State_Enabled))
                        mode = QIcon::Disabled;
                    else if ((opt->state & State_MouseOver) && (opt->state & State_AutoRaise))
                        mode = QIcon::Active;
                    else
                        mode = QIcon::Normal;
                    pm = toolbutton->icon.pixmap(toolbutton->rect.size().boundedTo(toolbutton->iconSize),
                                                 mode, state);
                    pmSize = pm.size();
                }

                if (toolbutton->toolButtonStyle != Qt::ToolButtonIconOnly) {
                    p->setFont(toolbutton->font);
                    QRect pr = rect,
                    tr = rect;
                    int alignment = Qt::TextShowMnemonic;
                    if (!proxy()->styleHint(SH_UnderlineShortcut, opt, widget))
                        alignment |= Qt::TextHideMnemonic;

                    if (toolbutton->toolButtonStyle == Qt::ToolButtonTextUnderIcon) {
                        pr.setHeight(pmSize.height() + 6);
                        tr.adjust(0, pr.height() - 1, 0, -2);
                        pr.translate(shiftX, shiftY);
                        if (!hasArrow)
                            proxy()->drawItemPixmap(p, pr, Qt::AlignTop | Qt::AlignHCenter, pm);
                        alignment |= Qt::AlignTop | Qt::AlignHCenter;
                    } else {
                        pr.setWidth(pmSize.width() + 8);
                        tr.adjust(pr.width(), 0, 0, 0);
                        pr.translate(shiftX, shiftY);
                        if (!hasArrow)
                            proxy()->drawItemPixmap(p, QStyle::visualRect(opt->direction, rect, pr), Qt::AlignTop | Qt::AlignHCenter, pm);
                        alignment |= Qt::AlignTop | Qt::AlignHCenter;
                    }
                    tr.translate(shiftX, shiftY);
                    proxy()->drawItemText(p, QStyle::visualRect(opt->direction, rect, tr), alignment, toolbutton->palette,
                                          toolbutton->state & State_Enabled, toolbutton->text,
                                          QPalette::ButtonText);
                } else {
                    rect.translate(shiftX, shiftY);
                    if (!hasArrow)
                        proxy()->drawItemPixmap(p, rect, Qt::AlignTop | Qt::AlignHCenter, pm);
                }
            }
        }
        return;
    }
    return QProxyStyle::drawControl(element, opt, p, widget);
}

//redefine arrow alignment
void TTToolButtonStyle::drawComplexControl(ComplexControl cc, const QStyleOptionComplex* opt, QPainter* p, const QWidget* widget) const
{
    if(cc == CC_ToolButton)
    {
        if (const QStyleOptionToolButton *toolbutton
            = qstyleoption_cast<const QStyleOptionToolButton *>(opt)) {
            QRect button, menuarea;
            button = proxy()->subControlRect(cc, toolbutton, SC_ToolButton, widget);
            menuarea = proxy()->subControlRect(cc, toolbutton, SC_ToolButtonMenu, widget);

            State bflags = toolbutton->state & ~State_Sunken;

            if (bflags & State_AutoRaise) {
                if (!(bflags & State_MouseOver) || !(bflags & State_Enabled)) {
                    bflags &= ~State_Raised;
                }
            }
            State mflags = bflags;
            if (toolbutton->state & State_Sunken) {
                if (toolbutton->activeSubControls & SC_ToolButton)
                    bflags |= State_Sunken;
                mflags |= State_Sunken;
            }

            QStyleOption tool(0);
            tool.palette = toolbutton->palette;
            if (toolbutton->subControls & SC_ToolButton) {
                if (bflags & (State_Sunken | State_On | State_Raised)) {
                    tool.rect = button;
                    tool.state = bflags;
                    proxy()->drawPrimitive(PE_PanelButtonTool, &tool, p, widget);
                }
            }

            if (toolbutton->state & State_HasFocus) {
                QStyleOptionFocusRect fr;
                fr.QStyleOption::operator=(*toolbutton);
                fr.rect.adjust(3, 3, -3, -3);
                if (toolbutton->features & QStyleOptionToolButton::MenuButtonPopup)
                    fr.rect.adjust(0, 0, -proxy()->pixelMetric(QStyle::PM_MenuButtonIndicator,
                                                      toolbutton, widget), 0);
                proxy()->drawPrimitive(PE_FrameFocusRect, &fr, p, widget);
            }
            QStyleOptionToolButton label = *toolbutton;
            label.state = bflags;
            int fw = proxy()->pixelMetric(PM_DefaultFrameWidth, opt, widget);
            label.rect = button.adjusted(fw, fw, -fw, -fw);
            proxy()->drawControl(CE_ToolButtonLabel, &label, p, widget);

            if (toolbutton->subControls & SC_ToolButtonMenu) {
                tool.rect = menuarea;
                tool.state = mflags;
                if (mflags & (State_Sunken | State_On | State_Raised))
                    proxy()->drawPrimitive(PE_IndicatorButtonDropDown, &tool, p, widget);
                proxy()->drawPrimitive(PE_IndicatorArrowDown, &tool, p, widget);
            } else if (toolbutton->features & QStyleOptionToolButton::HasMenu) {
                int mbi = proxy()->pixelMetric(PM_MenuButtonIndicator, toolbutton, widget);
                QRect ir = toolbutton->rect;
                QStyleOptionToolButton newBtn = *toolbutton;
                newBtn.rect = QRect(ir.center().x() + 1 - (mbi - 6)/2, ir.y() + ir.height() - mbi + 4, mbi - 6, mbi - 6);
                proxy()->drawPrimitive(PE_IndicatorArrowDown, &newBtn, p, widget);
            }
        }
        return;
    }
    return QProxyStyle::drawComplexControl(cc, opt, p, widget);
}
