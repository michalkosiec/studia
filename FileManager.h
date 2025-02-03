//
// Created by peccator on 1/2/25.
//

#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QApplication>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>


class FileManager {
public:
    QString fileName;
    FileManager(const QString &fileName);
    bool openFile();
    void closeFile();
    QString readFile();
    bool writeFile(QString &content);
private:
    QFile file;
};



#endif //FILEMANAGER_H
