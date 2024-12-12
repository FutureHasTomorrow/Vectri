#include "DataBase.h"

#include <QFileIconProvider>
#include <QDirIterator>
#include <QDir>
#include <QFile>

DataBase::DataBase()
{


}

DataBase::~DataBase(){}

void DataBase::setDir(const QString& dirPath) {
    fileItemList = searchFiles(dirPath);
}

QList<FileItem> DataBase::searchFiles(const QString& dirPath) {
    QList<FileItem> fileList;
    QFileIconProvider iconProvider;

    QDirIterator it(dirPath, QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        QString filePath = it.next();
        QFileInfo fileInfo(filePath);

        FileItem info;
        info.fileName = fileInfo.fileName();
        info.filePath = fileInfo.absoluteFilePath();
        info.icon = iconProvider.icon(fileInfo);

        fileList.append(info);
    }
    return fileList;
}

QList<FileItem> DataBase::match(const QString& text)
{
    return fileItemList;
}

