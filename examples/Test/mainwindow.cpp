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
#include <QMenu>
#include <QCheckBox>
#include <QComboBox>
#include <QSpinBox>
#include <QLineEdit>
#include <QTextEdit>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <TabToolbar/TabToolbar.h>
#include <TabToolbar/Page.h>
#include <TabToolbar/Group.h>
#include <TabToolbar/SubGroup.h>
#include <TabToolbar/StyleTools.h>
#include <TabToolbar/Builder.h>

CMainWindow::CMainWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->statusBar->addPermanentWidget(new QLabel("Some fancy status."));

    tt::Builder ttb(this);
    ttb.SetCustomWidgetCreator("textEdit", []() { return new QTextEdit(); });
    ttb.SetCustomWidgetCreator("checkBox", []() { return new QCheckBox(); });
    ttb.SetCustomWidgetCreator("pushButton", []() { return new QPushButton(); });
    tt::TabToolbar* tabToolbar = ttb.CreateTabToolbar(":/tt/tabtoolbar.json");
    addToolBar(Qt::TopToolBarArea, tabToolbar);

    ttb["customTextEdit"]->setMaximumWidth(100);

    ((QCheckBox*)ttb["customCheckBox"])->setText("Check 1");
    QPushButton* btn = (QPushButton*)ttb["customEditButton"];
    btn->setText("Edit");
    static bool kek = true;
    tt::Page* editPage = (tt::Page*)ttb["Edit"];
    QObject::connect(btn, &QPushButton::clicked, [editPage]()
    {
        if(kek)
            editPage->hide();
        else
            editPage->show();
        kek = !kek;
    });

    QObject::connect(tabToolbar, &tt::TabToolbar::SpecialTabClicked, this, [this]()
    {
        QMessageBox::information(this, "Kek", "Cheburek");
    });

    //create buttons for each style
    tt::Group* stylesGroup = (tt::Group*)ttb["Styles"];
    stylesGroup->AddSeparator();
    QStringList styles = tt::GetRegisteredStyles();
    for(int i=0; i<styles.size(); i++)
    {
        const QString styleName = styles.at(i);
        QPushButton* btn = new QPushButton(styleName, this);
        QObject::connect(btn, &QPushButton::clicked, [styleName, tabToolbar]() { tabToolbar->SetStyle(styleName); });
        stylesGroup->AddWidget(btn);
    }

    tt::RegisterStyle("NoStyle", []()
    {
        tt::StyleParams* params = new tt::StyleParams();
        params->UseTemplateSheet = false;
        params->AdditionalStyleSheet = "";
        return params;
    });
    btn = (QPushButton*)ttb["nativeStyleButton"];
    btn->setText("No Style");
    QObject::connect(btn, &QPushButton::clicked, [tabToolbar]() { tabToolbar->SetStyle("NoStyle"); });
    btn = (QPushButton*)ttb["defaultStyleButton"];
    btn->setText("Default");
    QObject::connect(btn, &QPushButton::clicked, [tabToolbar]() { tabToolbar->SetStyle(tt::GetDefaultStyle()); });
    /*
    TabToolbar* tt = new TabToolbar(this, 75, 3);
    addToolBar(Qt::TopToolBarArea, tt);

    QMenu* menu = new QMenu(this);
    menu->setObjectName("dummyMenu");
    menu->addActions({ui->actionDummy});

    tt->AddPage("File");
    static bool kek = true;
    TTPage* editPage = tt->AddPage("Edit");

    TTPage* viewPage = tt->AddPage("View");
    TTGroup* g1 = viewPage->AddGroup("Group 1");
    TTGroup* g2 = viewPage->AddGroup("Group 2");
    TTGroup* g3 = viewPage->AddGroup("Group 3");
    g1->AddAction(QToolButton::DelayedPopup, ui->actionOpen);
    g1->AddSeparator();
    g1->AddAction(QToolButton::DelayedPopup, ui->actionSave);
    g1->AddAction(QToolButton::DelayedPopup, ui->actionSaveAs);
    g2->AddAction(QToolButton::InstantPopup, ui->actionPolypaint, menu);
    g2->AddAction(QToolButton::InstantPopup, ui->actionScale, menu);
    g2->AddSeparator();
    QTextEdit* te = new QTextEdit();
    g2->AddWidget(te);
    te->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Ignored);
    te->setMaximumWidth(100);
    TTSubGroup* g2s = g2->AddSubGroup(TTSubGroup::Align::Yes);
    g2s->AddAction(QToolButton::DelayedPopup, ui->actionUndo);
    g2s->AddAction(QToolButton::DelayedPopup, ui->actionRedo);
    g2s->AddAction(QToolButton::InstantPopup, ui->actionClose, menu);
    g3->AddAction(QToolButton::MenuButtonPopup, ui->actionSettings, menu);
    TTSubGroup* g3s = g3->AddSubGroup(TTSubGroup::Align::Yes);
    g3s->AddHorizontalButtons({{QToolButton::DelayedPopup, ui->actionSave},
                               {QToolButton::InstantPopup, ui->actionPolypaint, menu},
                               {QToolButton::MenuButtonPopup, ui->actionSettings, menu}});

    g3s->AddHorizontalButtons({{QToolButton::DelayedPopup, ui->actionUndo},
                               {QToolButton::DelayedPopup, ui->actionRedo},
                               {QToolButton::InstantPopup, ui->actionClose, menu}});
    QCheckBox* ch = new QCheckBox("Check 1");
    g3s->AddWidget(ch);

    g3->AddSeparator();
    TTSubGroup* g3ss = g3->AddSubGroup(TTSubGroup::Align::No);
    QPushButton* btn = new QPushButton("Edit");
    g3ss->AddWidget(btn);
    g3ss->AddAction(QToolButton::DelayedPopup, ui->actionSaveAs);
    QObject::connect(btn, &QPushButton::clicked, [editPage]()
    {
        if(kek)
            editPage->hide();
        else
            editPage->show();
        kek = !kek;
    });

    tt->AddPage("Help");

    tt->SetSpecialTabEnabled(true);
    tt->AddCornerAction(ui->actionHelp);
    QObject::connect(tt, &TabToolbar::SpecialTabClicked, this, [this]()
    {
        QMessageBox::information(this, "Kek", "Cheburek");
    });
    */
}

CMainWindow::~CMainWindow()
{
    delete ui;
}
