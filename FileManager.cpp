//
// Created by peccator on 1/2/25.
//

#include "FileManager.h"

FileManager::FileManager (const QString &fileName)
    : fileName(fileName), file(fileName) {};

bool FileManager::openFile() {
    if (file.open(QFile::ReadWrite)) {
        return true;
    } else {
        return false;
    }
};

void FileManager::closeFile() {
    if (file.isOpen()) {
        file.close();
    }
};

QString FileManager::readFile() {
    if (file.isOpen()) {
        QTextStream in(&file);
        return in.readAll();
    } else {
        return "";
    }
};

bool FileManager::writeFile(QString &content) {
    if (file.isOpen()) {
        QTextStream out(&file);
        out << content;
        return true;
    } else {
        return false;
    }
};