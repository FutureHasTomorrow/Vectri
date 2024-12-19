#include "DataBase.h"

#include <QFileIconProvider>
#include <QDirIterator>
#include <QDir>
#include <QFile>
#include <QProcess>
#include <QDebug>

DataBase::DataBase()
{


}

DataBase::~DataBase(){}

void DataBase::setDir(const QString& dirPath) {
    fileItemList = searchFiles(dirPath);
    vecList.resize(fileItemList.size());
    keyMap.clear();
    QJsonArray jsonArray;
    for (auto& item:fileItemList) {
        jsonArray.append(item.fileName);
    }
    wtv.train(jsonArray);
    for(int i = 0;i<fileItemList.size();i++){
        vecList[i] = wtv.wordToVec(fileItemList[i].fileName);
        keyMap[hnsw.insert(vecList[i])]=i;
    }

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
    if(text.isEmpty())return {};
    auto vec = wtv.wordToVec(text);

    auto res = hnsw.k_nn_search_cookie(vec, qMin(10, (int)fileItemList.size()));
    QList<FileItem> list;
    for(auto& x:res)list.append(fileItemList[keyMap[x]]);
    return list;

    // double mx = -1e9; int ans = -1;
    // for(int i = 0;i<fileItemList.size();i++){
    //     double cur = 0;
    //     for(int j = 0;j<vec.size();j++){
    //         cur+=vec[j]*vecList[i][j];
    //     }
    //     if(cur>mx)mx = cur, ans = i;
    // };
    // if(ans==-1)return {};
    // return {fileItemList[ans]};
}
