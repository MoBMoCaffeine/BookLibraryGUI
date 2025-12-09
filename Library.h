#ifndef LIBARARYSYS_LIBRARY_H
#define LIBARARYSYS_LIBRARY_H

#include <vector>
#include <string>
#include <algorithm>
#include "Book.h"

class clsLibrary {
    std::vector<clsBook> _books;
public:
    void AddBook(const clsBook& book);
    bool DeleteBook(int id);
    bool BorrowBook(int id);
    bool ReturnBook(int id);
    clsBook* SearchById(int id);
    std::vector<clsBook> SearchByName(const std::string& name) const;
    const std::vector<clsBook>& getBooks() const { return _books; };

    bool SaveToJson(const std::string& filename) const;
    bool LoadFromJson(const std::string& filename);
};

#endif //LIBARARYSYS_LIBRARY_H
