//
// Created by peccator on 1/4/25.
//

#ifndef HUFFMANCODEC_H
#define HUFFMANCODEC_H

#include <QString>
#include <memory>
#include <unordered_map>
#include <QByteArray>


class HuffmanCodec {
public:
    HuffmanCodec();
    QByteArray encodeFile(QByteArray data);
    QByteArray decodeFile(QByteArray data);
private:
    struct HuffmanNode {
        char symbol;
        int freq;
        std::shared_ptr<HuffmanNode> left;
        std::shared_ptr<HuffmanNode> right;
        HuffmanNode(char symbol, int freq)
            : symbol(symbol), freq(freq), left(nullptr), right(nullptr) {}
    };

    struct Compare {
        bool operator()(const std::shared_ptr<HuffmanNode>&, const std::shared_ptr<HuffmanNode>&);
    };

    std::unordered_map<char, int> calculateFrequencies(const QByteArray& data);
    std::shared_ptr<HuffmanNode> buildHuffmanTree(const std::unordered_map<char, int>& frequencies);
    void generateHuffmanCodes(const std::shared_ptr<HuffmanNode>& node, const std::string& code, std::unordered_map<char, std::string>& codes);
    std::unordered_map<char, std::string> getHuffmanCodes(const std::shared_ptr<HuffmanNode>& root);
    QByteArray encodeData(const QByteArray& data, const std::unordered_map<char, std::string>& codes);
    QByteArray decodeData(const QByteArray& data);
};



#endif //HUFFMANCODEC_H
