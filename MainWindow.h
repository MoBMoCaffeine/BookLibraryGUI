#ifndef LIBRARYSYS_MAINWINDOW_H
#define LIBRARYSYS_MAINWINDOW_H

#include <QWidget>
#include <QListWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "Library.h"

class MainWindow : public QWidget {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void addBook();
    void deleteBook();
    void borrowBook();
    void returnBook();
    void searchBook();
    void saveAndQuit();
    void loadLibrary();
    void updateBookList();

private:
    clsLibrary library;
    QListWidget *bookListWidget;
    QPushButton *addButton;
    QPushButton *deleteButton;
    QPushButton *borrowButton;
    QPushButton *returnButton;
    QPushButton *searchButton;
    QPushButton *saveButton;
    QLineEdit *searchLineEdit;
    QLabel *statusLabel;

    void setupUi();
    void connectSignalsSlots();
    void displayBookInfo(const clsBook& book);
    clsBook* findBookInList(int id);
};

#endif // LIBRARYSYS_MAINWINDOW_H
