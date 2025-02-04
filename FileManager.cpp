#include "FileManager.h"

using namespace std;

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

QByteArray FileManager::readFile() {
    if (file.isOpen()) {
        return file.readAll();
    } else {
        return QByteArray();
    }
};

void FileManager::writeFile(const QByteArray& data) {
        file.write(data);
};