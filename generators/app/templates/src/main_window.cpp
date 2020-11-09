/* Copyright (C) LETA Company, Inc - All Rights Reserved
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
*/

#include "main_window.h"
#include "ui_mainwindow.h"

#define TEST_PARAM "testParam"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    this->setWindowTitle("<%= windowTitle %>");
    settings_ = new QSettings(QSettings::UserScope, "<%= organization %>", "<%= project %>", this);

    statusLabel = new QLabel(this);
    statusLabel->setVisible(false);
    bar = new QProgressBar(this);
    bar->setMaximumSize(80, 14);
    bar->setVisible(false);
    bar->setRange(0, 0);
    bar->setTextVisible(false);
    ui->statusBar->addPermanentWidget(statusLabel);
    ui->statusBar->addPermanentWidget(bar);

    //binding field to persistent setting
    ui->testParamEdit->setValidator(new QIntValidator(0, 100, this));
    ui->testParamEdit->setText("42");
    ui->testParamEdit->setText(settings_->value(TEST_PARAM, 42).toString());
    connect(ui->testParamEdit, &QLineEdit::editingFinished, this, [=]() {
        this->updateSettings(TEST_PARAM, ui->testParamEdit->text());
    });

    //binding button to background task
    connect(ui->testBtn, &QPushButton::clicked, this, &MainWindow::testBtnClick);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::blockUi(const QString &statusText) {
    ui->centralWidget->setEnabled(false);
    bar->setVisible(true);
    statusLabel->setText(statusText);
    statusLabel->setVisible(true);
}

void MainWindow::releaseUi() {
    ui->centralWidget->setEnabled(true);
    bar->setVisible(false);
    statusLabel->setVisible(false);
}

void MainWindow::whenBgFinished() {
    watcher_.disconnect();
    emit bgFinished();
    disconnect(this, SIGNAL(bgFinished()), nullptr, nullptr);
}

void MainWindow::updateSettings(const QString &key, const QString &value) {
    settings_->setValue(key, value);
}

void MainWindow::msg(const QString &text, QMessageBox::Icon icon) {
    QMessageBox msgBox(this);
    msgBox.moveToThread(this->thread());
    msgBox.setIcon(icon);
    msgBox.setText(text);
    msgBox.exec();
}

void MainWindow::testBtnClick() {
    doBg(&MainWindow::testBtn, SLOT(releaseUi()), "testing...");
}

QVariant MainWindow::testBtn() {
    QThread::sleep(3);
    return true;
}

void MainWindow::selectDir(QLineEdit *field) {
    QString newDir = QFileDialog::getExistingDirectory(this, "Select folder", field->text());
    if (!newDir.isEmpty()) {
        field->setText(newDir);
    }
}

void MainWindow::selectFile(QLineEdit *field) {
    QString newFile = QFileDialog::getOpenFileName(this, "Select file", field->text());
    if (!newFile.isEmpty()) {
        field->setText(newFile);
    }
}
