#include "Library.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <QFile>
#include <QTextStream>
#include <QDebug>

// Helper to convert std::string to lowercase for case-insensitive search
std::string toLower(const std::string& str) {
    std::string lower_str = str;
    std::transform(lower_str.begin(), lower_str.end(), lower_str.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    return lower_str;
}

void clsLibrary::AddBook(const clsBook &book) {
    _books.push_back(book);
}

bool clsLibrary::DeleteBook(int id) {
    auto it = std::remove_if(_books.begin(), _books.end(),
                             [id](const clsBook& b){ return b.GetId() == id; });
    if (it != _books.end()) {
        _books.erase(it, _books.end());
        return true;
    }
    return false;
}

bool clsLibrary::BorrowBook(int id) {
    clsBook* book = SearchById(id);
    if (book && !book->IsBorrowed()) {
        book->borrow();
        return true;
    }
    return false;
}

bool clsLibrary::ReturnBook(int id) {
    clsBook* book = SearchById(id);
    if (book && book->IsBorrowed()) {
        book->giveBack();
        return true;
    }
    return false;
}

clsBook* clsLibrary::SearchById(int id) {
    for (auto& book : _books) {
        if (book.GetId() == id) {
            return &book;
        }
    }
    return nullptr;
}

std::vector<clsBook> clsLibrary::SearchByName(const std::string &name) const {
    std::vector<clsBook> results;
    std::string lower_name = toLower(name);
    for (const auto& book : _books) {
        if (toLower(book.GetName()).find(lower_name) != std::string::npos) {
            results.push_back(book);
        }
    }
    return results;
}

bool clsLibrary::SaveToJson(const std::string &filename) const {
    nlohmann::json j = nlohmann::json::array();
    for (const auto& book : _books) {
        j.push_back(book.to_json());
    }

    QFile file(QString::fromStdString(filename));
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Could not open file for writing:" << filename.c_str();
        return false;
    }

    QTextStream out(&file);
    out << QString::fromStdString(j.dump(4));
    file.close();
    return true;
}

bool clsLibrary::LoadFromJson(const std::string &filename) {
    QFile file(QString::fromStdString(filename));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Could not open file for reading:" << filename.c_str();
        return false;
    }

    QTextStream in(&file);
    std::string content = in.readAll().toStdString();
    file.close();

    try {
        nlohmann::json j = nlohmann::json::parse(content);
        if (j.is_array()) {
            _books.clear();
            for (const auto& item : j) {
                _books.push_back(clsBook::from_json(item));
            }
            return true;
        }
    } catch (const nlohmann::json::parse_error& e) {
        qDebug() << "JSON parse error:" << e.what();
    }
    return false;
}
