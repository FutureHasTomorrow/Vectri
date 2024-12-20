#ifndef WORDTOVEC_H
#define WORDTOVEC_H

#include <QObject>
#include <QProcess>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <QByteArray>
#include <QVector>
#include <QJsonArray>
#include <vector>
#include <QEventLoop>
#include <QThread>

class WordToVec : public QObject {
    Q_OBJECT

public:
    explicit WordToVec(QObject *parent = nullptr)
        : QObject(parent), process(new QProcess(this)) {

        process->start("WordToVec.exe");
        process->waitForStarted();

    }

    ~WordToVec() {
        if (process->state() == QProcess::Running) {
            process->close();
        }
    }

    // 同步调用 word_to_vec 方法
    std::vector<double> wordToVec(const QString &word) {
        QJsonObject request;
        request["command"] = "word_to_vec";
        request["word"] = word;

        QJsonDocument doc(request);
        QByteArray jsonData = doc.toJson(QJsonDocument::Compact) + "\n"; // 添加换行符

        process->write(jsonData); // 发送数据到标准输入

        process->waitForBytesWritten();
        process->waitForReadyRead();


        QByteArray line = process->readLine().trimmed();
        while(line.isEmpty()){
            process->waitForReadyRead();
            line = process->readLine().trimmed();
        }

        QJsonDocument responseDoc = QJsonDocument::fromJson(line);

        QJsonObject response = responseDoc.object();
        QJsonArray vectorArray = response["vector"].toArray();
        std::vector<double> vector;
        for (const auto &val : vectorArray) {
            vector.push_back(val.toDouble());
        }

        return vector;
    }

    void train(const QJsonArray& jsonArray)
    {

        QJsonObject request;
        request["command"] = "train";
        request["word_list"] = jsonArray;

        QJsonDocument doc(request);
        QByteArray jsonData = doc.toJson(QJsonDocument::Compact) + "\n"; // 添加换行符

        process->write(jsonData); // 发送数据到标准输入
        process->waitForBytesWritten();
        process->waitForReadyRead();

        QByteArray line = process->readLine().trimmed();
        while(line.isEmpty()){
            process->waitForReadyRead();
            line = process->readLine().trimmed();
        }
        QJsonDocument responseDoc = QJsonDocument::fromJson(line);

        QJsonObject response = responseDoc.object();


    }

private:
    QProcess *process;
};


#endif // WORDTOVEC_H
