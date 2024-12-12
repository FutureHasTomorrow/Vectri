#ifndef STRUCTS_H
#define STRUCTS_H

#include <QString>
#include <QIcon>
#include <QSize>
#include <QColor>
#include <QFont>

struct FileItem
{
    QString fileName;
    QString filePath;
    QIcon icon;
};

struct Theme
{
    QSize leSearchSize = QSize(500, 50);
    QSize pbCloseSize = QSize(100, 50);
    QSize lIconSize = QSize(40,40);
    QSize listItemIconSize = QSize(50, 50);


    QColor mainWindowColor = QColor(30,30,30,128);
    QColor highlightTextColor = QColor(200, 255, 255);
    QColor dropdownListColor = QColor(30,30,30,224);

    QFont searchFont = QFont("Consolas", 14, QFont::Bold);
    QFont closeButtonFont = QFont("Consolas", 14, QFont::Normal);
    QFont dropdownListFont = QFont("Consolas", 16, QFont::Normal);
    QFont fileNameFont = QFont("Consolas", 14, QFont::Bold);
    QFont filePathFont = QFont("Consolas", 10, QFont::Light);

    int listItemHeight = 60;
    double borderRadius = 20;
};

#endif // STRUCTS_H
