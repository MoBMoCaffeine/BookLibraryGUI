#include "Book.h"

nlohmann::json clsBook::to_json() const {
    return nlohmann::json{
        {"name", _name},
        {"id", _id},
        {"author", _authorName},
        {"category", _category},
        {"price", _price},
        {"borrowed", _isBorrowed}
    };
}

clsBook clsBook::from_json(const nlohmann::json& j) {
    clsBook b;
    b._name = j.value("name", "");
    b._id = j.value("id", 0);
    b._authorName = j.value("author", "");
    b._category = j.value("category", "");
    b._price = j.value("price", -1.0);
    b._isBorrowed = j.value("borrowed", false);
    return b;
}
