#include <iostream>
#include <iomanip>
#include <cctype>
#include <string>

using namespace std;

// Custom case-insensitive string comparison function
bool caseInsensitiveCompare(const string& a, const string& b) {
    if (a.length() != b.length()) {
        return false;
    }
    
    for (size_t i = 0; i < a.length(); ++i) {
        if (tolower(a[i]) != tolower(b[i])) {
            return false;
        }
    }
    
    return true;
}

// Custom function to convert string to lowercase
string toLowercase(string str) {
    for (char& c : str) {
        c = tolower(c);
    }
    return str;
}

class StringArray {
private:
    string* data;
    size_t size;
    size_t capacity;
    
public:
    // Constructor
    StringArray() : data(nullptr), size(0), capacity(0) {}
    
    // Copy constructor
    StringArray(const StringArray& other) {
        size = other.size;
        capacity = other.capacity;
        if (capacity > 0) {
            data = new string[capacity];
            for (size_t i = 0; i < size; ++i) {
                data[i] = other.data[i];
            }
        } else {
            data = nullptr;
        }
    }
    
    // Assignment operator
    StringArray& operator=(const StringArray& other) {
        if (this != &other) {
            delete[] data;
            size = other.size;
            capacity = other.capacity;
            if (capacity > 0) {
                data = new string[capacity];
                for (size_t i = 0; i < size; ++i) {
                    data[i] = other.data[i];
                }
            } else {
                data = nullptr;
            }
        }
        return *this;
    }
    
    // Destructor
    ~StringArray() {
        delete[] data;
    }
    
    // Add an element
    void push_back(const string& value) {
        if (size >= capacity) {
            size_t newCapacity = capacity == 0 ? 1 : capacity * 2;
            string* newData = new string[newCapacity];
            for (size_t i = 0; i < size; ++i) {
                newData[i] = data[i];
            }
            delete[] data;
            data = newData;
            capacity = newCapacity;
        }
        data[size++] = value;
    }
    
    // Access element
    string& operator[](size_t index) {
        return data[index];
    }
    
    // Const access element
    const string& operator[](size_t index) const {
        return data[index];
    }
    
    // Get size
    size_t length() const {
        return size;
    }
    
    // Check if empty
    bool empty() const {
        return size == 0;
    }
};

class Book {
private:
    string id;
    string isbn;
    string title;
    StringArray authors;
    string edition;
    string publication;
    string category;

public:
    // Constructors
    Book() {}
    Book(const string& id, const string& isbn, const string& title,
         const StringArray& authors, const string& edition,
         const string& publication, const string& category)
        : id(id), isbn(isbn), title(title), authors(authors), 
          edition(edition), publication(publication), category(category) {}

    // Getter methods
    string getId() const { return id; }
    string getValidIsbn() const { return isbn; }
    string getTitle() const { return title; }
    StringArray getAuthors() const { return authors; }
    string getAuthorsAsString() const {
        string result = "";
        for (size_t i = 0; i < authors.length(); ++i) {
            result += authors[i];
            if (i < authors.length() - 1) {
                result += ", ";
            }
        }
        return result;
    }
    string getEdition() const { return edition; }
    string getPublication() const { return publication; }
    string getCategory() const { return category; }

    // Setter methods (excluding ID)
    void setIsbn(const string& newIsbn) { isbn = newIsbn; }
    void setTitle(const string& newTitle) { title = newTitle; }
    void setAuthors(const StringArray& newAuthors) { authors = newAuthors; }
    void setEdition(const string& newEdition) { edition = newEdition; }
    void setPublication(const string& newPublication) { publication = newPublication; }
    void setCategory(const string& newCategory) { category = newCategory; }
};

class LibraryManagementSystem {
private:
    static const int MAX_BOOKS = 100;
    Book books[MAX_BOOKS];
    int bookCount;
    string categories[2] = {"Fiction", "Non-fiction"};
    static const int MAX_CATEGORIES = 2;

    // Input Validation Methods 
    bool isValidId(const string& id) const {
        if (id.empty()) return false;
        
        for (char c : id) {
            if (!isalnum(c)) {
                return false;
            }
        }
        return true;
    }

    bool isIdUnique(const string& id) const {
        for (int i = 0; i < bookCount; ++i) {
            if (caseInsensitiveCompare(books[i].getId(), id)) {
                return false;
            }
        }
        return true;
    }

    bool isValidCategory(const string& category) const {
        string normalizedCategory = category;
        // Replace "non fiction" with "non-fiction" for comparison
        if (toLowercase(normalizedCategory) == "non fiction") {
            normalizedCategory = "Non-fiction";
        }
        
        for (int i = 0; i < MAX_CATEGORIES; ++i) {
            if (caseInsensitiveCompare(categories[i], normalizedCategory)) {
                return true;
            }
        }
        return false;
    }

    string trimString(const string& str) {
        size_t start = str.find_first_not_of(" \t");
        if (start == string::npos) return "";
        size_t end = str.find_last_not_of(" \t");
        return str.substr(start, end - start + 1);
    }

    string getValidIsbn() {
        string isbn;
        bool validInput = false;
        
        do {
            cout << "Enter ISBN (10 or 13 digits, 'x' allowed): ";
            getline(cin, isbn);
            isbn = trimString(isbn);
            
            if (isbn.empty()) {
                cout << "ISBN cannot be empty. Please try again.\n";
                continue;
            }
            
            // Check if ISBN contains only digits and 'x'
            bool onlyDigitsAndX = true;
            int digitCount = 0;
            int xCount = 0;
            
            for (char c : isbn) {
                if (isdigit(c)) {
                    digitCount++;
                } else if (c == 'x' || c == 'X') {
                    xCount++;
                } else {
                    onlyDigitsAndX = false;
                    break;
                }
            }
            
            if (!onlyDigitsAndX) {
                cout << "Invalid ISBN! ISBN must contain only digits and 'x'.\n";
                continue;
            }
            
            // Check if the ISBN has the correct number of digits (10 or 13)
            if (digitCount + xCount != 10 && digitCount + xCount != 13) {
                cout << "Invalid ISBN! ISBN must contain exactly 10 or 13 characters (digits and 'x').\n";
                continue;
            }
            
            validInput = true;
            
        } while (!validInput);
        
        return isbn;
    }

    string getValidInput(const string& prompt, bool allowEmpty = false) {
        string input;
        bool validInput = false;
        
        do {
            cout << prompt;
            getline(cin, input);
            input = trimString(input);
            
            if (!input.empty() || allowEmpty) {
                validInput = true;
            } else {
                cout << "Input cannot be empty. Please try again.\n";
            }
        } while (!validInput);
        
        return input;
    }
    
    string getValidId() {
        string id;
        bool validInput = false;
        
        do {
            id = getValidInput("Enter Book ID: ");
            
            if (!isValidId(id)) {
                cout << "Invalid ID! ID must be alphanumeric.\n";
                continue;
            }
            
            if (!isIdUnique(id)) {
                cout << "Duplicate ID! Please enter a unique ID.\n";
                continue;
            }
            
            validInput = true;
        } while (!validInput);
        
        return id;
    }
    
    string getValidCategory() {
        string category;
        bool validInput = false;
        
        do {
            category = getValidInput("Enter Book Category (Fiction/Non-fiction): ");
            
            if (isValidCategory(category)) {
                // Normalize category to maintain consistency
                string lowerCategory = toLowercase(category);
                if (lowerCategory == "non fiction" || lowerCategory == "non-fiction" || lowerCategory == "nonfiction") {
                    category = "Non-fiction";
                } else if (lowerCategory == "fiction") {
                    category = "Fiction";
                }
                validInput = true;
            } else {
                cout << "Category not found! Please enter a valid category.\n";
            }
        } while (!validInput);
        
        return category;
    }
    
    string getValidPublication() {
        string publication;
        bool validInput = false;
        
        do {
            publication = getValidInput("Enter Publication Year (4 digits): ");
            
            // Check if the input is a 4-digit year 
            bool isValidYear = true;
            if (publication.length() != 4) {
                isValidYear = false;
            } else {
                for (char c : publication) {
                    if (!isdigit(c)) {
                        isValidYear = false;
                        break;
                    }
                }
            }
            
            if (isValidYear) {
                int year = stoi(publication);
                if (year >= 1000 && year <= 2100) {
                    validInput = true;
                } else {
                    cout << "Invalid year! Year must be between 1000 and 2100.\n";
                }
            } else {
                cout << "Invalid publication year! Publication must be a 4-digit year.\n";
            }
        } while (!validInput);
        
        return publication;
    }
    
    string getValidSearchId() {
        string id;
        bool validInput = false;
        
        do {
            cout << "Enter Book ID to search: ";
            getline(cin, id);
            id = trimString(id);
            
            if (id.empty()) {
                cout << "ID cannot be empty. Please try again.\n";
                continue;
            }
            
            // Check if ID contains only alphanumeric characters
            bool isAlphanumeric = true;
            for (char c : id) {
                if (!isalnum(c)) {
                    isAlphanumeric = false;
                    break;
                }
            }
            
            if (!isAlphanumeric) {
                cout << "Invalid ID! ID must contain only alphanumeric characters (no spaces or special characters).\n";
                continue;
            }
            
            validInput = true;
        } while (!validInput);
        
        return id;
    }
    
    StringArray getMultipleAuthors() {
        StringArray authors;
        int authorCount = 0;
        bool validCount = false;
        
        do {
            string input = getValidInput("Enter the number of authors: ");
            
            bool isNumber = true;
            for (char c : input) {
                if (!isdigit(c)) {
                    isNumber = false;
                    break;
                }
            }
            
            if (!isNumber) {
                cout << "Invalid input! Please enter a valid number.\n";
                continue;
            }
            
            authorCount = stoi(input);
            if (authorCount <= 0) {
                cout << "Number of authors must be positive. Please try again.\n";
                continue;
            }
            
            validCount = true;
        } while (!validCount);
        
        for (int i = 0; i < authorCount; ++i) {
            string author = getValidInput("Enter Author " + to_string(i + 1) + ": ");
            authors.push_back(author);
        }
        
        return authors;
    }
    
    int findBookIndexById(const string& id) const {
        for (int i = 0; i < bookCount; ++i) {
            if (caseInsensitiveCompare(books[i].getId(), id)) {
                return i;
            }
        }
        return -1;
    }
    
    void pressAnyContinue() {
        cout << "Press Enter to Continue...";
        cin.get();
    }

    void displayBookDetails(const Book& book) {
        cout << left 
             << setw(15) << book.getId()
             << setw(15) << book.getValidIsbn()
             << setw(20) << book.getTitle()
             << setw(40) << book.getAuthorsAsString()
             << setw(10) << book.getEdition()
             << setw(15) << book.getPublication()
             << setw(15) << book.getCategory() << endl;
    }

    bool getYesNoInput(const string& prompt) {
        string input;
        bool validInput = false;
        bool result = false;
        
        do {
            input = getValidInput(prompt);
            
            if (caseInsensitiveCompare(input, "yes") || 
                caseInsensitiveCompare(input, "y")) {
                result = true;
                validInput = true;
            } else if (caseInsensitiveCompare(input, "no") || 
                       caseInsensitiveCompare(input, "n")) {
                result = false;
                validInput = true;
            } else {
                cout << "Invalid input. Please enter 'yes' or 'no'.\n";
            }
        } while (!validInput);
        
        return result;
    }

public:
    LibraryManagementSystem() : bookCount(0) {}

    void addBook() {
        bool continuedAdding = true;
        
        while (continuedAdding) {
            if (bookCount >= MAX_BOOKS) {
                cout << "Library is full. Cannot add more books.\n";
                return;
            }
    
            string category = getValidCategory();
            string id = getValidId();
            string isbn = getValidIsbn(); 
            string title = getValidInput("Enter Title: ");
            StringArray authors = getMultipleAuthors();
            string edition = getValidInput("Enter Edition: ");
            string publication = getValidPublication();
    
            books[bookCount++] = Book(id, isbn, title, authors, edition, publication, category);
            cout << "Book added successfully!\n";
    
            continuedAdding = getYesNoInput("Would you like to add another book? (yes/no): ");
        }
    }

    void editBook() {
        bool bookFound = false;
        
        while (!bookFound) {
            string id = getValidSearchId();
            int index = findBookIndexById(id);
            
            if (index != -1) {
                Book& book = books[index];
                
                // Update ISBN with proper validation for digits and 'x' only
                string newIsbn = getValidInput("Enter new ISBN (or press Enter to skip): ", true);
                if (!newIsbn.empty()) {
                    // Check if ISBN contains only digits and 'x'
                    bool onlyDigitsAndX = true;
                    int digitCount = 0;
                    int xCount = 0;
                    
                    for (char c : newIsbn) {
                        if (isdigit(c)) {
                            digitCount++;
                        } else if (c == 'x' || c == 'X') {
                            xCount++;
                        } else {
                            onlyDigitsAndX = false;
                            break;
                        }
                    }
                    
                    if (!onlyDigitsAndX) {
                        cout << "Invalid ISBN! ISBN must contain only digits and 'x'. Skipping ISBN update.\n";
                    } else if (digitCount + xCount != 10 && digitCount + xCount != 13) {
                        cout << "Invalid ISBN! ISBN must contain exactly 10 or 13 characters. Skipping ISBN update.\n";
                    } else {
                        book.setIsbn(newIsbn);
                    }
                }
                
                string newTitle = getValidInput("Enter new Title (or press Enter to skip): ", true);
                if (!newTitle.empty()) book.setTitle(newTitle);
                
                // Update authors
                cout << "Update authors? ";
                if (getYesNoInput("(yes/no): ")) {
                    StringArray newAuthors = getMultipleAuthors();
                    book.setAuthors(newAuthors);
                }
                
                // Update edition as simple string
                string newEdition = getValidInput("Enter new Edition (or press Enter to skip): ", true);
                if (!newEdition.empty()) {
                    book.setEdition(newEdition);
                }
                
                string newPublication = getValidInput("Enter new Publication Year (or press Enter to skip): ", true);
                if (!newPublication.empty()) {
                    // Check if the input is a 4-digit year without using 
                    bool isValidYear = true;
                    if (newPublication.length() != 4) {
                        isValidYear = false;
                    } else {
                        for (char c : newPublication) {
                            if (!isdigit(c)) {
                                isValidYear = false;
                                break;
                            }
                        }
                    }
                    
                    if (isValidYear) {
                        int year = stoi(newPublication);
                        if (year >= 1000 && year <= 2100) {
                            book.setPublication(newPublication);
                        } else {
                            cout << "Invalid year! Year must be between 1000 and 2100. Skipping Publication update.\n";
                        }
                    } else {
                        cout << "Invalid publication year! Publication must be a 4-digit year. Skipping Publication update.\n";
                    }
                }
                
                cout << "Update category? ";
                if (getYesNoInput("(yes/no): ")) {
                    book.setCategory(getValidCategory());
                } else {
                    cout << "Book category is not updated." << endl;
                }
                
                cout << "Book edited successfully!\n";
                bookFound = true;
            } else {
                cout << "Book not found!\n";
                if (!getYesNoInput("Do you want to try again? (yes/no): ")) {
                    bookFound = true;
                }
            }
        }
    }
    
    void searchBook() {
        bool bookFound = false;
        
        while (!bookFound) {
            string id = getValidSearchId();
            int index = findBookIndexById(id);
            
            if (index != -1) {
                cout << "\n--- Book Details ---\n";
                cout << left 
                     << setw(15) << "ID"
                     << setw(15) << "ISBN"
                     << setw(20) << "Title"
                     << setw(40) << "Authors"
                     << setw(10) << "Edition"
                     << setw(15) << "Publication"
                     << setw(15) << "Category" << endl;
                displayBookDetails(books[index]);
                bookFound = true;
            } else {
                cout << "Book not found!\n";
                if (!getYesNoInput("Do you want to try again? (yes/no): ")) {
                    bookFound = true;
                }
            }
        }
    }
    
    void deleteBook() {
        bool bookFound = false;
        
        while (!bookFound) {
            string id = getValidSearchId();
            int index = findBookIndexById(id);
            
            if (index != -1) {
                cout << "\n--- Book Details ---\n";
                cout << left 
                     << setw(15) << "ID"
                     << setw(15) << "ISBN"
                     << setw(20) << "Title"
                     << setw(40) << "Authors"
                     << setw(10) << "Edition"
                     << setw(15) << "Publication"
                     << setw(15) << "Category" << endl;
                displayBookDetails(books[index]);
                
                if (getYesNoInput("Do you want to delete this book? (yes/no): ")) {
                    for (int i = index; i < bookCount - 1; ++i) {
                        books[i] = books[i + 1];
                    }
                    bookCount--;
                    cout << "Book deleted successfully!\n";
                    bookFound = true;
                } else {
                    bookFound = true;
                }
            } else {
                cout << "Book not found!\n";
                if (!getYesNoInput("Do you want to try again? (yes/no): ")) {
                    bookFound = true;
                }
            }
        }
    }

    void viewBooksByCategory() {
        string category = getValidCategory();
        bool found = false;

        cout << "\n--- Books in " << category << " Category ---\n";
        cout << left 
             << setw(15) << "ID"
             << setw(15) << "ISBN"
             << setw(20) << "Title"
             << setw(40) << "Authors"
             << setw(10) << "Edition"
             << setw(15) << "Publication"
             << setw(15) << "Category" << endl;

        for (int i = 0; i < bookCount; ++i) {
            if (caseInsensitiveCompare(books[i].getCategory(), category)) {
                displayBookDetails(books[i]);
                found = true;
            }
        }

        if (!found) {
            cout << "No books found in this category.\n";
        }
        pressAnyContinue();
    }

    void viewAllBooks() {
        if (bookCount == 0) {
            cout << "No books in the library.\n";
            pressAnyContinue();
            return;
        }

        cout << "\n--- All Books ---\n";
        cout << left 
             << setw(15) << "ID"
             << setw(15) << "ISBN"
             << setw(20) << "Title"
             << setw(40) << "Authors"
             << setw(10) << "Edition"
             << setw(15) << "Publication"
             << setw(15) << "Category" << endl;

        for (int i = 0; i < bookCount; ++i) {
            displayBookDetails(books[i]);
        }
        pressAnyContinue();
    }

    int getMenuChoice() {
        string input;
        bool validChoice = false;
        int choice = 0;
        
        do {
            cout << "\n--- Library Management System ---\n";
            cout << "1 - Add Book\n";
            cout << "2 - Edit Book\n";
            cout << "3 - Search Book\n";
            cout << "4 - Delete Book\n";
            cout << "5 - View Books by Category\n";
            cout << "6 - View All Books\n";
            cout << "7 - Exit\n";
            cout << "Enter your choice (1-7): ";

            getline(cin, input);
            
            // Manual validation 
            if (input.length() == 1 && input[0] >= '1' && input[0] <= '7') {
                choice = input[0] - '0';
                validChoice = true;
            } else {
                cout << "Invalid choice! Please enter a single digit between 1 and 7.\n";
            }
        } while (!validChoice);
        
        return choice;
    }

    void run() {
        bool running = true;
        
        while (running) {
            int choice = getMenuChoice();

            switch (choice) {
                case 1: addBook(); break;
                case 2: editBook(); break;
                case 3: searchBook(); break;
                case 4: deleteBook(); break;
                case 5: viewBooksByCategory(); break;
                case 6: viewAllBooks(); break;
                case 7: 
                    cout << "Exiting Library Management System...\n";
                    running = false;
                    break;
            }
        }
    }
};

int main() {
    LibraryManagementSystem lms;
    lms.run();
    return 0;
}
