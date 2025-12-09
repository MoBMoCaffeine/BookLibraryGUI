#ifndef LIBARARYSYS_BOOK_H
#define LIBARARYSYS_BOOK_H

#include <string>
#include "include/nlohmann/json.hpp"

class clsBook {
    std::string _name, _authorName, _category;
    double _price{-1};
    int _id{0}; // Changed from double to int
    bool _isBorrowed{false};
public:
    clsBook() = default;
    clsBook(const std::string& name, int id,
        const std::string& authorName,
        const std::string& category, double price)
    : _name(name), _authorName(authorName),
    _category(category),
    _price(price),
    _id(id) {}

    int GetId() const { return _id; }
    const std::string& GetName() const { return _name; }
    const std::string& GetAuthorName() const { return _authorName; }
    const std::string& GetCategory() const { return _category; }
    double GetPrice() const { return _price; }
    bool IsBorrowed() const { return _isBorrowed; }

    void borrow() { _isBorrowed = true; }
    void giveBack() { _isBorrowed = false; }

    // JSON helpers
    nlohmann::json to_json() const;
    static clsBook from_json(const nlohmann::json& j);
};

#endif //LIBARARYSYS_BOOK_H
