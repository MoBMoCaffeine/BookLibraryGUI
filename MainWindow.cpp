#include "MainWindow.h"
#include <QApplication>
#include <QInputDialog>
#include <QMessageBox>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <sstream>

#define LIBRARY_FILE "library.json"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Library Management System (Qt 6.10)");
    setMinimumSize(600, 400);
    setupUi();
    connectSignalsSlots();
    loadLibrary();
    updateBookList();
}

MainWindow::~MainWindow() = default;

void MainWindow::setupUi() {
    // Widgets
    bookListWidget = new QListWidget(this);
    addButton = new QPushButton("Add Book", this);
    deleteButton = new QPushButton("Delete Book", this);
    borrowButton = new QPushButton("Borrow Book", this);
    returnButton = new QPushButton("Return Book", this);
    searchButton = new QPushButton("Search", this);
    saveButton = new QPushButton("Save & Quit", this);
    searchLineEdit = new QLineEdit(this);
    searchLineEdit->setPlaceholderText("Search by Name or ID...");
    statusLabel = new QLabel("Status: Ready", this);

    // Layouts
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QHBoxLayout *searchLayout = new QHBoxLayout();
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    // Search Layout
    searchLayout->addWidget(searchLineEdit);
    searchLayout->addWidget(searchButton);

    // Button Layout
    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(deleteButton);
    buttonLayout->addWidget(borrowButton);
    buttonLayout->addWidget(returnButton);
    buttonLayout->addWidget(saveButton);

    // Main Layout
    mainLayout->addLayout(searchLayout);
    mainLayout->addWidget(bookListWidget);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addWidget(statusLabel);

    setLayout(mainLayout);
}

void MainWindow::connectSignalsSlots() {
    connect(addButton, &QPushButton::clicked, this, &MainWindow::addBook);
    connect(deleteButton, &QPushButton::clicked, this, &MainWindow::deleteBook);
    connect(borrowButton, &QPushButton::clicked, this, &MainWindow::borrowBook);
    connect(returnButton, &QPushButton::clicked, this, &MainWindow::returnBook);
    connect(searchButton, &QPushButton::clicked, this, &MainWindow::searchBook);
    connect(searchLineEdit, &QLineEdit::returnPressed, this, &MainWindow::searchBook);
    connect(saveButton, &QPushButton::clicked, this, &MainWindow::saveAndQuit);
}

void MainWindow::loadLibrary() {
    if (library.LoadFromJson(LIBRARY_FILE)) {
        statusLabel->setText("Status: Library loaded successfully from " LIBRARY_FILE);
    } else {
        statusLabel->setText("Status: New library created. " LIBRARY_FILE " not found or corrupted.");
    }
}

void MainWindow::updateBookList() {
    bookListWidget->clear();
    for (const auto& book : library.getBooks()) {
        QString status = book.IsBorrowed() ? " (BORROWED)" : " (Available)";
        QString itemText = QString("ID: %1 | Title: %2 | Author: %3%4")
                               .arg(book.GetId())
                               .arg(QString::fromStdString(book.GetName()))
                               .arg(QString::fromStdString(book.GetAuthorName()))
                               .arg(status);
        QListWidgetItem *item = new QListWidgetItem(itemText, bookListWidget);
        item->setData(Qt::UserRole, book.GetId()); // Store ID in UserRole
    }
}

void MainWindow::addBook() {
    bool ok;
    QString name = QInputDialog::getText(this, "Add Book", "Title:", QLineEdit::Normal, "", &ok);
    if (!ok || name.isEmpty()) return;

    QString author = QInputDialog::getText(this, "Add Book", "Author:", QLineEdit::Normal, "", &ok);
    if (!ok || author.isEmpty()) return;

    QString category = QInputDialog::getText(this, "Add Book", "Category:", QLineEdit::Normal, "", &ok);
    if (!ok || category.isEmpty()) return;

    int id = QInputDialog::getInt(this, "Add Book", "ID (must be unique):", 0, 1, 99999, 1, &ok);
    if (!ok) return;

    double price = QInputDialog::getDouble(this, "Add Book", "Price:", 0.0, 0.0, 1000.0, 2, &ok);
    if (!ok) return;

    // Check for ID uniqueness
    if (library.SearchById(id) != nullptr) {
        QMessageBox::warning(this, "Error", "Book with this ID already exists.");
        return;
    }

    clsBook newBook(name.toStdString(), id, author.toStdString(), category.toStdString(), price);
    library.AddBook(newBook);
    updateBookList();
    statusLabel->setText(QString("Status: Added book '%1' (ID: %2)").arg(name).arg(id));
}

void MainWindow::deleteBook() {
    bool ok;
    int id = QInputDialog::getInt(this, "Delete Book", "Enter Book ID to delete:", 0, 1, 99999, 1, &ok);
    if (!ok) return;

    if (library.DeleteBook(id)) {
        updateBookList();
        statusLabel->setText(QString("Status: Deleted book with ID: %1").arg(id));
    } else {
        QMessageBox::warning(this, "Error", QString("Book with ID %1 not found.").arg(id));
    }
}

void MainWindow::borrowBook() {
    bool ok;
    int id = QInputDialog::getInt(this, "Borrow Book", "Enter Book ID to borrow:", 0, 1, 99999, 1, &ok);
    if (!ok) return;

    if (library.BorrowBook(id)) {
        updateBookList();
        statusLabel->setText(QString("Status: Borrowed book with ID: %1").arg(id));
    } else {
        QMessageBox::warning(this, "Error", QString("Book with ID %1 not found or already borrowed.").arg(id));
    }
}

void MainWindow::returnBook() {
    bool ok;
    int id = QInputDialog::getInt(this, "Return Book", "Enter Book ID to return:", 0, 1, 99999, 1, &ok);
    if (!ok) return;

    if (library.ReturnBook(id)) {
        updateBookList();
        statusLabel->setText(QString("Status: Returned book with ID: %1").arg(id));
    } else {
        QMessageBox::warning(this, "Error", QString("Book with ID %1 not found or not currently borrowed.").arg(id));
    }
}

void MainWindow::searchBook() {
    QString query = searchLineEdit->text().trimmed();
    if (query.isEmpty()) {
        updateBookList(); // Show all if search is empty
        statusLabel->setText("Status: Showing all books.");
        return;
    }

    bookListWidget->clear();
    bool isIdSearch = false;
    int id = query.toInt(&isIdSearch);

    if (isIdSearch) {
        // Search by ID
        clsBook* book = library.SearchById(id);
        if (book) {
            QString status = book->IsBorrowed() ? " (BORROWED)" : " (Available)";
            QString itemText = QString("ID: %1 | Title: %2 | Author: %3%4")
                                   .arg(book->GetId())
                                   .arg(QString::fromStdString(book->GetName()))
                                   .arg(QString::fromStdString(book->GetAuthorName()))
                                   .arg(status);
            QListWidgetItem *item = new QListWidgetItem(itemText, bookListWidget);
            item->setData(Qt::UserRole, book->GetId());
            statusLabel->setText(QString("Status: Found book with ID: %1").arg(id));
        } else {
            statusLabel->setText(QString("Status: No book found with ID: %1").arg(id));
        }
    } else {
        // Search by Name (case-insensitive substring match)
        std::vector<clsBook> results = library.SearchByName(query.toStdString());
        for (const auto& book : results) {
            QString status = book.IsBorrowed() ? " (BORROWED)" : " (Available)";
            QString itemText = QString("ID: %1 | Title: %2 | Author: %3%4")
                                   .arg(book.GetId())
                                   .arg(QString::fromStdString(book.GetName()))
                                   .arg(QString::fromStdString(book.GetAuthorName()))
                                   .arg(status);
            QListWidgetItem *item = new QListWidgetItem(itemText, bookListWidget);
            item->setData(Qt::UserRole, book.GetId());
        }
        statusLabel->setText(QString("Status: Found %1 book(s) matching '%2'").arg(results.size()).arg(query));
    }
}

void MainWindow::saveAndQuit() {
    if (library.SaveToJson(LIBRARY_FILE)) {
        QMessageBox::information(this, "Save Successful", "Library data saved to " LIBRARY_FILE ". Application will now close.");
        QApplication::quit();
    } else {
        QMessageBox::critical(this, "Save Error", "Failed to save library data to " LIBRARY_FILE ".");
    }
}
