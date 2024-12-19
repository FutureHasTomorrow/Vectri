#ifndef DATABASE_H
#define DATABASE_H

#include "structs.h"
#include "WordToVec.h"
#include "HNSW.h"

#include <QObject>
#include <QList>


class DataBase : public QObject
{
    Q_OBJECT
public:
    DataBase();
    ~DataBase();

    void setDir(const QString& dirPath);

    QList<FileItem> match(const QString& text);
private:
    QList<FileItem> searchFiles(const QString& dirPath);


    WordToVec wtv;
    Hnsw::hnsw<double> hnsw;

    QList<FileItem> fileItemList;
    QList<std::vector<double>> vecList;
    std::unordered_map<unsigned long long, int> keyMap;


};

#endif // DATABASE_H
