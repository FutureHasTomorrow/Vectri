#ifndef MAINWINDOW_H
#define MAINWINDOW_H


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
#include <QDir>
#include <QFile>
#include <QFileIconProvider>
#include <QDirIterator>

struct FileItem
{
    QString fileName;
    QString filePath;
    QIcon icon;
};


class MainWindow : public QWidget
{
    Q_OBJECT
public:
    struct Theme
    {
        QSize leSearchSize = QSize(500, 50);
        QSize pbCloseSize = QSize(100, 50);
        QSize lIconSize = QSize(40,40);

        double borderRadius = 20;
        QColor mainWindowColor = QColor(30,30,30,128);
        QColor highlightTextColor = QColor(200, 255, 255);
        QColor dropdownListColor = QColor(30,30,30,192);
        QFont searchFont = QFont("Consolas", 14, QFont::Bold);
        QFont closeButtonFont = QFont("Consolas", 14, QFont::Normal);
        QFont dropdownListFont = QFont("Consolas", 16, QFont::Normal);

    };
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
    Theme theme;


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


protected:
    void paintEvent(QPaintEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    void moveEvent(QMoveEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

public:
    static QList<FileItem> searchFiles(const QString& directoryPath) {
        QList<FileItem> fileList;
        QFileIconProvider iconProvider;

        QDirIterator it(directoryPath, QDir::Files, QDirIterator::Subdirectories);
        while (it.hasNext()) {
            QString filePath = it.next();
            QFileInfo fileInfo(filePath);

            // 获取文件信息
            FileItem info;
            info.fileName = fileInfo.fileName();
            info.filePath = fileInfo.absoluteFilePath();
            info.icon = iconProvider.icon(fileInfo); // 获取文件图标

            // 添加到文件列表
            fileList.append(info);
        }
        return fileList;
    }
};
#endif // MAINWINDOW_H
