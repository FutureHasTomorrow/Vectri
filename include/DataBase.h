#ifndef DATABASE_H
#define DATABASE_H

#include "structs.h"

#include <QObject>
#include <QList>


class DataBase : public QObject
{
    Q_OBJECT
public:
    DataBase();
    ~DataBase();

    void setDir(const QString& dirPath);
    // void load();
    // void save();

    QList<FileItem> match(const QString& text);
private:
    QList<FileItem> searchFiles(const QString& dirPath);

    QList<FileItem> fileItemList;
};

#endif // DATABASE_H
