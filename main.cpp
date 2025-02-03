#include "FileManager.h"
#include "HuffmanCodec.h"
#include <QApplication>
#include <QMainWindow>
#include <QTextEdit>
#include <QMenuBar>
#include <QAction>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QString>

// do usuniecia
#include <QDebug>
#include <iostream>
using namespace std;

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QMainWindow window;
    window.setWindowTitle("Kodowanie Huffmana");
    QTextEdit* textEdit = new QTextEdit(&window);
    window.setCentralWidget(textEdit);
    QMenu *fileMenu = window.menuBar()->addMenu("Plik");
    QAction *openAction = new QAction("Otworz", &window);
    QAction *encodeAction = new QAction("Koduj", &window);
    QAction *decodeAction = new QAction("Dekoduj", &window);
    fileMenu->addAction(openAction);
    fileMenu->addAction(encodeAction);
    fileMenu->addAction(decodeAction);

    FileManager* fileManager = nullptr;
    HuffmanCodec codec = HuffmanCodec();

    QObject::connect(openAction, &QAction::triggered, [&]() {
        QString fileName = QFileDialog::getOpenFileName(&window, "Otworz plik");
        if (!fileName.isEmpty()) {
            fileManager = new FileManager(fileName);
            if (!fileManager->openFile()) {
                QMessageBox::critical(nullptr, "Blad", "Nie udalo się otworzyc pliku");
            }
        }
    });

    QObject::connect(encodeAction, &QAction::triggered, [&]() {
        if (!fileManager) {
            QMessageBox::critical(nullptr, "Blad", "Najpierw otwórz plik.");
            return;
        }
        QString outputPath = QFileDialog::getSaveFileName(&window, "Zapisz zakodowany plik", "", "Pliki binarne (*.bin)");
        codec.encodeFile(fileManager->fileName, outputPath);
    });

    QObject::connect(decodeAction, &QAction::triggered, [&]() {
        QString outputPath = QFileDialog::getSaveFileName(&window, "Zapisz zdekodowany plik", "", "Wszystkie pliki (*.*)");
        codec.decodeFile(fileManager->fileName, outputPath);
    });

    QObject::connect(&window, &QMainWindow::destroyed, [&]() {
        delete fileManager;
        fileManager = nullptr;
    });

    window.resize(800, 600);
    window.show();
    return QApplication::exec();
}