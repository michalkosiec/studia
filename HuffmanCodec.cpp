#include "HuffmanCodec.h"
#include "FileManager.h"
#include <queue>
#include <QBitArray>
#include <QList>
using namespace std;

HuffmanCodec::HuffmanCodec() {}

/*Funkcja encodeFile odpowiada za wezwanie wszystkich metod związanych z kodowaniem danych metodą Huffmana
 *zwraca ona zakodowane dane
 */
QByteArray HuffmanCodec::encodeFile(QByteArray data) {
    if (data.isEmpty()) {
        return QByteArray();
    }

    const auto frequencies = calculateFrequencies(data);

    const auto huffmanTree = buildHuffmanTree(frequencies);
    const auto codes = getHuffmanCodes(huffmanTree);

    // Tworzenie nagłówka
    QByteArray header;
    for (const auto& [symbol, code] : codes) {
        header.append(symbol);
        header.append('\x1F');
        header.append(code.c_str());
        header.append('\x1D');
    }
    header.chop(1);
    header.append('\x1E');

    QByteArray encodedData = encodeData(data, codes);
    encodedData.prepend(header);

    return encodedData;
}

/*Funkcja decodeFile odpowiada za wezwanie funkcji decodeData*/
QByteArray HuffmanCodec::decodeFile(QByteArray data) {
    if (data.isEmpty()) {
        return QByteArray();
    }

    const QByteArray decodedData = decodeData(data);

   return decodedData;
};

/*Funkcja calculateFrequencies odpowiada za policzenie liczby wystąpień określonych symboli w pliku źródłowym*/
unordered_map<char, int> HuffmanCodec::calculateFrequencies(const QByteArray& data) {
    unordered_map<char, int> frequencies;
    for (char byte : data) {
        frequencies[byte]++;
    }
    return frequencies;
}


/*Funkcja buildHuffmanTree odpowiada za zbudowanie drzewa Huffmana, przy wykorzystaniu kolejki priorytetowej, struct i pointerów*/
shared_ptr<HuffmanCodec::HuffmanNode> HuffmanCodec::buildHuffmanTree(const std::unordered_map<char, int>& frequencies) {
    // Jesli true to drugi element b ma wiekszy priorytet niż a
    auto comp = [](const shared_ptr<HuffmanNode>& a, const shared_ptr<HuffmanNode>& b) {
        return a->freq > b->freq;
    };

    priority_queue<shared_ptr<HuffmanNode>, vector<shared_ptr<HuffmanNode>>, decltype(comp)> pq(comp);
    for (auto& pair : frequencies) {
        pq.push(make_shared<HuffmanNode>(pair.first, pair.second));
    }

    if (pq.empty()) return shared_ptr<HuffmanNode>();

    // Łączenie wezłów o najmniejszym freq w pary do czasu otrzymania jednego wezla w kolejce
    while (pq.size() > 1) {
        auto left = pq.top();
        pq.pop();
        auto right = pq.top();
        pq.pop();
        auto newNode = make_shared<HuffmanNode>('\0', left->freq + right->freq);
        newNode->left = left;
        newNode->right = right;
        pq.push(newNode);
    }

    return pq.top();
}

// Przypisanie kodów do danych wezłów wykorzystując rekurencję i wezel o najwiekszym freq
void HuffmanCodec::generateHuffmanCodes(const std::shared_ptr<HuffmanNode>& node, const std::string& code, std::unordered_map<char, std::string>& codes) {
    if (node->left == nullptr && node->right == nullptr) {
        codes[node->symbol] = code;
    } else {
        if (node->left) {
            generateHuffmanCodes(node->left, code + "0", codes);
        }
        if (node->right) {
            generateHuffmanCodes(node->right, code + "1", codes);
        }
    }
}

// Tworzenie mapy kodów symbol, kod
unordered_map<char, string> HuffmanCodec::getHuffmanCodes(const shared_ptr<HuffmanNode>& root) {
    unordered_map<char, string> codes;
    generateHuffmanCodes(root, "", codes);
    return codes;
}

/* Kodowanie danych. Za kazdy symbol wstawia kod. Uzupelnia liczbe bitow do wielokrotnosci 8, zeby mozna bylo ja zapisac w postaci bajtow.
 * Zamienia bity na bajty i dodaje informacje o paddingu (dopelnienie bitami do wielokrotnosci 8)
 */
QByteArray HuffmanCodec::encodeData(const QByteArray& data, const unordered_map<char, string>& codes) {
    string encodedString;
    for (char byte : data) {
        encodedString += codes.at(byte);
    }

    int paddingBits = (8 - (encodedString.size() % 8)) % 8;
    encodedString.append(paddingBits, '0');

    QByteArray encodedData;
    QBitArray bitArray(encodedString.size());
    for (int i = 0; i < encodedString.size(); i++) {
        bitArray.setBit(i, encodedString[i] == '1');
    }
    for (int i = 0; i < bitArray.size(); i += 8) {
        char byte = 0;
        for (int j = 0; j < 8 && i + j < bitArray.size(); j++) {
            byte |= bitArray.testBit(i + j) << (7 - j);
        }
        encodedData.append(byte);
    }

    encodedData.prepend(static_cast<char>(paddingBits));

    return encodedData;
}

/*Funkcja decodeData odpowiada za odkodowanie danych, które następnie zwraca*/
QByteArray HuffmanCodec::decodeData(const QByteArray& data) {

    // Odczytanie naglowka
    QByteArray header;
    int i = 0;
    while (i < data.size() && data[i] != '\x1E') {
        header.append(data[i]);
        i++;
    }

    if (i >= data.size() || data[i] != '\x1E') {
        qWarning("Nie znaleziono delimitera");
    }
    i++;

    QByteArray headerContent = header;

    // Odtworzenie mapy kodow i symboli
    unordered_map<string, char> reverseCodes;

    QList<QByteArray> pairs = headerContent.split('\x1D');
    for (const auto& pair : pairs) {
        QList<QByteArray> symbolCode = pair.split('\x1F');
        char symbol = symbolCode[0][0];
        string code = symbolCode[1].toStdString();
        reverseCodes[code] = symbol;
    }

    QByteArray encodedData = data.mid(i);

    int paddingBits = static_cast<unsigned char>(encodedData[0]);

    // Odczytanie zawartosci zakodowanego pliku z wylaczeniem bitow paddingu
    encodedData = encodedData.mid(1);
    string bitString;
    for (char byte : encodedData) {
        for (int i = 0; i < 8; i++) {
            bitString += (byte >> (7 - i) & 1) ? '1' : '0';
        }
    }

    if (paddingBits > 0) {
        bitString.erase(bitString.size() - paddingBits);
    }

    // Podmienienie kodow na symbole (odkodowywanie)
    QByteArray decodedData;
    string currentCode;
    for (const char bit : bitString) {
        currentCode += bit;
        if (reverseCodes.count(currentCode)) {
            decodedData += reverseCodes.at(currentCode);
            currentCode.clear();
        }
    }

    return decodedData;
}
