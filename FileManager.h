//
// Created by peccator on 1/2/25.
//

#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QFileDialog>


class FileManager {
public:
    QString fileName;
    FileManager(const QString& fileName);
    bool openFile();
    void closeFile();
    QByteArray readFile();
    void writeFile(const QByteArray& data);
private:
    QFile file;
};



#endif //FILEMANAGER_H
