#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "DataBase.h"

#include <QWidget>
#include <QLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QListWidget>
#include <QSystemTrayIcon>

#include <QString>
#include <QMenu>
#include <QIcon>


class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    QHBoxLayout* hblMain;
    QLabel *lIcon;
    QFrame* fSep;
    QLineEdit* leSearch;
    QPushButton* pbClose;
    QListWidget* lwDropdown;
    QSystemTrayIcon* stiTray;
private:
    void lIconInit();
    void fSepInit();
    void leSearchInit();
    void pbCloseInit();
    void hblMainInit();

    void lwDropdownInit();
    void stiTrayInit();
private:
    Theme theme;
    DataBase dataBase;


    bool isDragging = false;
    QPoint dragStartPos;
    QPoint dragEndPos;

    QList<FileItem> fileItemList;
    QMenu *mTray;
    QAction *aShowSettings;
    QAction *aExitApp;

private:
    void updateDropdownListPosition();
    void updateListItem();

    void openFile(int index);

protected:
    void paintEvent(QPaintEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    void moveEvent(QMoveEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

};
#endif // MAINWINDOW_H
