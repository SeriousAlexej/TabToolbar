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

	tt::TabToolbar* tabToolbar = new tt::TabToolbar(this, 75, 3);
    addToolBar(Qt::TopToolBarArea, tabToolbar);

    QMenu* dummyMenu = new QMenu(this);
    dummyMenu->setObjectName("dummyMenu");
    dummyMenu->addActions({ui->actionDummy});
	dummyMenu->addSeparator();
	dummyMenu->addAction(ui->actionHelp);

    tabToolbar->AddPage("File");
    static bool kek = true;
    tt::Page* editPage = tabToolbar->AddPage("Edit");

	tt::Group * editGroup1 = editPage->AddGroup("Styles");
	tt::SubGroup * editGroup1sub1 = editGroup1->AddSubGroup(tt::SubGroup::Align::Yes);
	QPushButton * nativeStyleButton = new QPushButton();
	editGroup1sub1->AddWidget(nativeStyleButton);
	QPushButton * defaultStyleButton = new QPushButton();
	editGroup1sub1->AddWidget(defaultStyleButton);

	editGroup1->AddSeparator();
	QStringList styles = tt::GetRegisteredStyles();
	for (int i = 0; i < styles.size(); i++)
	{
		const QString styleName = styles.at(i);
		QPushButton* styleButton = new QPushButton(styleName, this);
		QObject::connect(styleButton, &QPushButton::clicked,
			[styleName, tabToolbar]() { tabToolbar->SetStyle(styleName); });
		editGroup1->AddWidget(styleButton);
	}

	tt::RegisterStyle("NoStyle", []()
	{
		tt::StyleParams* params = new tt::StyleParams();
		params->UseTemplateSheet = false;
		params->AdditionalStyleSheet = "";
		return params;
	});
	nativeStyleButton->setText("No Style");
	QObject::connect(nativeStyleButton, &QPushButton::clicked,
		[tabToolbar]() { tabToolbar->SetStyle("NoStyle"); });
	defaultStyleButton->setText("Default");
	QObject::connect(defaultStyleButton, &QPushButton::clicked,
		[tabToolbar]() { tabToolbar->SetStyle(tt::GetDefaultStyle()); });

    tt::Page* viewPage = tabToolbar->AddPage("View");
    tt::Group* viewGroup1 = viewPage->AddGroup("Group 1");
    tt::Group* viewGroup2 = viewPage->AddGroup("Group 2");
    tt::Group* viewGroup3 = viewPage->AddGroup("Group 3");
    viewGroup1->AddAction(QToolButton::DelayedPopup, ui->actionOpen);
    viewGroup1->AddSeparator();
    viewGroup1->AddAction(QToolButton::DelayedPopup, ui->actionSave);
    viewGroup1->AddAction(QToolButton::DelayedPopup, ui->actionSaveAs);
    viewGroup2->AddAction(QToolButton::InstantPopup, ui->actionPolypaint, dummyMenu);
    viewGroup2->AddAction(QToolButton::InstantPopup, ui->actionScale, dummyMenu);
    viewGroup2->AddSeparator();
    QTextEdit* textEdit = new QTextEdit();
    viewGroup2->AddWidget(textEdit);
    textEdit->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Ignored);
    textEdit->setMaximumWidth(100);
    tt::SubGroup* viewGroup2sub1 = viewGroup2->AddSubGroup(tt::SubGroup::Align::Yes);
    viewGroup2sub1->AddAction(QToolButton::DelayedPopup, ui->actionUndo);
    viewGroup2sub1->AddAction(QToolButton::DelayedPopup, ui->actionRedo);
    viewGroup2sub1->AddAction(QToolButton::InstantPopup, ui->actionClose, dummyMenu);
    viewGroup3->AddAction(QToolButton::MenuButtonPopup, ui->actionSettings, dummyMenu);
    tt::SubGroup* viewGroup3sub1 = viewGroup3->AddSubGroup(tt::SubGroup::Align::Yes);
    viewGroup3sub1->AddHorizontalButtons({{QToolButton::DelayedPopup, ui->actionSave},
                               {QToolButton::InstantPopup, ui->actionPolypaint, dummyMenu},
                               {QToolButton::MenuButtonPopup, ui->actionSettings, dummyMenu}});

    viewGroup3sub1->AddHorizontalButtons({{QToolButton::DelayedPopup, ui->actionUndo},
                               {QToolButton::DelayedPopup, ui->actionRedo},
                               {QToolButton::InstantPopup, ui->actionClose, dummyMenu}});
    QCheckBox* checkBox = new QCheckBox("Check 1");
    viewGroup3sub1->AddWidget(checkBox);

    viewGroup3->AddSeparator();
    tt::SubGroup* viewGroup3sub2 = viewGroup3->AddSubGroup(tt::SubGroup::Align::No);
    QPushButton* pushButton = new QPushButton("Edit");
    viewGroup3sub2->AddWidget(pushButton);
    viewGroup3sub2->AddAction(QToolButton::DelayedPopup, ui->actionSaveAs);
    QObject::connect(pushButton, &QPushButton::clicked, [editPage]()
    {
        if(kek)
            editPage->hide();
        else
            editPage->show();
        kek = !kek;
    });

	tt::Page* helpPage = tabToolbar->AddPage("Help");
	tt::Group * helpGroup1 = helpPage->AddGroup("");
	helpGroup1->AddAction(QToolButton::DelayedPopup, ui->actionHelp);

    tabToolbar->SetSpecialTabEnabled(true);
    tabToolbar->AddCornerAction(ui->actionHelp);
    QObject::connect(tabToolbar, &tt::TabToolbar::SpecialTabClicked, this, [this]()
    {
        QMessageBox::information(this, "Kek", "Cheburek");
    });
    
    setWindowTitle("TabToolbar TestNoJson");
}

CMainWindow::~CMainWindow()
{
    delete ui;
}
