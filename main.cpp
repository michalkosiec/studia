#include "FileManager.h"
#include "HuffmanCodec.h"
#include <QApplication>
#include <QMainWindow>
#include <QTextEdit>
#include <QMenuBar>
#include <QAction>
#include <QFileDialog>
#include <QLabel>
#include <QMessageBox>
#include <QString>
using namespace std;

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QMainWindow window;
    window.setWindowTitle("Kodowanie Huffmana");

    // Tworzenie instrukcji
    QLabel* instructionLabel = new QLabel(&window);
    instructionLabel->setAlignment(Qt::AlignCenter);
    instructionLabel->setText("<h1>Instrukcja obslugi</h1>"
                              "<ul>"
                              "<li>Kliknij \"Plik\" a nastepnie \"Otworz\", aby wybrac plik</li>"
                              "<li>Aby zakodowac plik kliknij \"Koduj\" i wybierz/utworz plik docelowy .bin</li>"
                              "<li>Aby odkodowac plik .bin kliknij \"Dekoduj\" i wybierz gdzie zapisac plik</li>"
                              "</ul>");
    instructionLabel->setStyleSheet("QLabel {"
                                    "color: white;"
                                    "background-color: #5c5c5c;"
                                    "font-size: 15px;}");
    // Tworzenie pozostalych elementow GUI
    window.setCentralWidget(instructionLabel);
    QMenu *fileMenu = window.menuBar()->addMenu("Plik");
    QAction *openAction = new QAction("Otworz", &window);
    QAction *encodeAction = new QAction("Koduj", &window);
    QAction *decodeAction = new QAction("Dekoduj", &window);

    // Deklaracja i inicjalizacja zmiennych, ktore sluza do pracy z plikami oraz do kodowania Huffmana
    FileManager* sourceFileManager = nullptr;
    FileManager* destinationFileManager = nullptr;
    HuffmanCodec codec = HuffmanCodec();

    // Obsluga zdarzen w aplikacji Qt
    fileMenu->addAction(openAction);
    fileMenu->addAction(encodeAction);
    fileMenu->addAction(decodeAction);

    /* Funkcja lambda po nacisnieciu przycisku otworz otwiera eksplorator plikow i tworzy nowa instancje
     * klasy FileManager. Jesli nie uda sie otworzyc pliku, to wyswietla sie komunikat o niepowodzeniu*/
    QObject::connect(openAction, &QAction::triggered, [&]() {
        QString fileName = QFileDialog::getOpenFileName(&window, "Otworz plik");
        if (!fileName.isEmpty()) {
            sourceFileManager = new FileManager(fileName);
            if (!sourceFileManager->openFile()) {
                QMessageBox::critical(nullptr, "Blad", "Nie udalo się otworzyc pliku");
            }
        }
    });

    /* Funkcja lambda po nacisnieciu przycisku koduj pokazuje komunikat, aby najpierw otworzyc plik
     * zrodlowy (jesli nie wybrano wczesniej). Jesli uzytkownik wybral juz plik zrodlowy, to funkcja wyswietla
     * eksplorator plikow, ktory prosi o wybranie pliku docelowego (albo jego utworzenie). Jesli nie uda sie
     * otworzyc pliku docelowego, to wyswietlany jest komunikat o niepowodzeniu. Jesli uda sie otworzyc plik
     * docelowy, to funkcja lambda wzywa metode encodeFile() obiektu codec, ktora odpowiada za zakodowanie danych metoda huffmana.
     * Nastepnie zakodowane dane sa zapisywane do pliku docelowego, ktory zostaje potem zamkniety.
     */
    QObject::connect(encodeAction, &QAction::triggered, [&]() {
        if (!sourceFileManager) {
            QMessageBox::critical(nullptr, "Blad", "Najpierw otwórz plik.");
            return;
        }
        QString outputPath = QFileDialog::getSaveFileName(&window, "Zapisz zakodowany plik", "", "Pliki binarne (*.bin)");
        destinationFileManager = new FileManager(outputPath);
        if (destinationFileManager->openFile()) {
            const QByteArray encodedData = codec.encodeFile(sourceFileManager->readFile());
            destinationFileManager->writeFile(encodedData);
            destinationFileManager->closeFile();
        } else {
            QMessageBox::critical(nullptr, "Blad", "Nie udalo sie otworzyc pliku docelowego");
        }
    });

    /*Funkcja lambda po nacisnieciu przyciku dekoduj pokazuje komunikat, aby najpierw otworzyc plik
     * zrodlowy (jesli nie wybrano wczesniej). Jesli uzytkownik wybral juz plik zrodlowy, to funkcja wyswietla
     * eksplorator plikow, ktory prosi o wybranie pliku docelowego (albo jego utworzenie). Jesli nie uda sie
     * otworzyc pliku docelowego, to wyswietlany jest komunikat o niepowodzeniu. Jesli uda sie otworzyc plik docelowy,
     * to funkcja lambda wzywa metode decodeFile() obiektu codec, ktora odpowiada za odkodowanie danych zakodowanych
     * metoda Huffmana.
     */
    QObject::connect(decodeAction, &QAction::triggered, [&]() {
        if (!sourceFileManager) {
            QMessageBox::critical(nullptr, "Blad", "Najpierw otwórz plik.");
            return;
        }
        QString outputPath = QFileDialog::getSaveFileName(&window, "Zapisz zdekodowany plik", "", "Wszystkie pliki (*.*)");
        destinationFileManager = new FileManager(outputPath);
        if (destinationFileManager->openFile()) {
            const QByteArray decodedData = codec.decodeFile(sourceFileManager->readFile());
            destinationFileManager->writeFile(decodedData);
            destinationFileManager->closeFile();
        } else {
            QMessageBox::critical(nullptr, "Blad", "Nie udalo sie otworzyc pliku docelowego");
        }
    });

    QObject::connect(&window, &QMainWindow::destroyed, [&]() {
        delete sourceFileManager;
        sourceFileManager = nullptr;
        delete destinationFileManager;
        destinationFileManager = nullptr;
    });

    window.resize(800, 600);
    window.show();
    return QApplication::exec();
}