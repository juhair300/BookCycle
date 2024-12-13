#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

// Structure for storing history details (File handling for persistence)
typedef struct History {
    char sellerUsername[32];  // Username of the seller (linked to User)
    char buyerUsername[32];   // Username of the buyer (linked to User)
    char title[100];          // Title of the book
    float amount;             // Transaction amount
} History;

// Structure for managing the cart (Linked list for dynamic cart items)
typedef struct Cart {
    char username[50];  // Username for login
    char title[100];    // Title of the book
    struct Cart* next;  // Pointer to the next cart item (for linked list)
} Cart;

// Structure for storing user information (for file storage)
typedef struct User {
    int isActive;         // Unique ID for each user
    char username[50];    // Username for login
    char password[50];    // Password for login
    char fullName[100];   // Full name of the user
    char email[100];      // Email address
    char address[200];    // Physical address
    float walletBalance;  // Current balance in wallet
    int booksBought;      // Total books bought by the user
    int booksSold;        // Total books sold by the user
} User;

// Structure for storing new book information
typedef struct NewBook {
    char title[100];          // Title of the book
    char author[100];         // Author of the book
    float price;              // Price of the book
    char sellerUsername[32];  // Username of the seller (linked to User)
    char buyerUsername[32];   // Username of the buyer (linked to User)
    int isSold;               // 0 = Not sold, 1 = Sold
    struct NewBook* next;     // Pointer to the next book in the linked list
} NewBook;

// Structure for storing old book information
typedef struct OldBook {
    char title[100];          // Title of the book
    char author[100];         // Author of the book
    float price;              // Price of the book
    char sellerUsername[32];  // Username of the seller (linked to User)
    char buyerUsername[32];   // Username of the buyer (linked to User)
    int isSold;               // 0 = Not sold, 1 = Sold
    char buyDate[11];         // Purchase date (YYYY-MM-DD)
    int hasMarks;             // Whether the book has marks (1 = Yes, 0 = No)
    int isTorn;               // Whether the book is torn (1 = Yes, 0 = No)
    struct OldBook* next;     // Pointer to the next book in the linked list
} OldBook;

// Structure for the admin profile
typedef struct Admin {
    int adminId;          // Unique ID for each admin
    char username[32];    // Admin username for login
    char password[32];    // Admin password
    char fullName[50];    // Full name of the admin
    char email[50];       // Admin's email address
    char role[30];        // Role of the admin
    float walletBalance;  // Admin's wallet balance for transaction tracking
} Admin;

// Global Admin Data Array
Admin admins[5] = {
    {101, "juhair300", "241-15-300", "Juhair Akhter", "juhair241-15-300@diu.edu.bd", "Project Administrator", 0.0},
    {102, "sakib048", "241-15-048", "Sakibul Hasan Sakib", "sakib241-15-048@diu.edu.bd", "Web Designer and Editor", 0.0},
    {103, "kushol042", "241-15-042", "Kushol Ranjan Sarker", "kushol241-15-042@diu.edu.bd", "Software Designer", 0.0},
    {104, "huzaifa727", "241-15-727", "Abdullah Mohammad Huzaifa", "huzaifa241-15-727@diu.edu.bd", "Database Administrator", 0.0},
    {105, "kamrunnahar321", "241-15-321", "MST. Kamrunnahar Mukty", "kamrunnahar241-15-321@diu.edu.bd", "Quality Assurance Lead", 0.0}};

// Declare file pointers for persistent storage
FILE* newBooksFile;  // File pointer for storing new books
FILE* oldBooksFile;  // File pointer for storing old books
FILE* userFile;      // File pointer for storing user data
FILE* historyFile;   // File pointer for storing user history (purchase/sell logs)

// Declare global variables for dynamic data management
NewBook* newBooksInventory = NULL;  // Pointer to dynamically allocated new book inventory list
OldBook* oldBooksInventory = NULL;  // Pointer to dynamically allocated old book inventory list
User* users = NULL;                 // Dynamically allocated array of users (if needed)
History* history = NULL;            // Pointer to track all transactions in memory
Cart* userCart = NULL;

// Function to manage the whole BookCycle platform
void bookCycle();

// Admin Panel Functions
char* adminLogin();        // Handle admin login
void adminDashboard();     // Display admin dashboard
void viewAdminProfile();   // View admin profile
void manageInventory();    // Admin can add/remove books
int getBookId();           // auto book id generator
void addNewBook();         // Add a new book to inventory
void addOldBook();         // Add an old book to inventory
void viewStore();          // view all books;
void removeNewBook();      // Remove a new book
void removeOldBook();      // Remove an old book
void viewHistory();        // View history of user transactions
void manageUsers();        // Manage user accounts (view/delete)
void viewUserHistory();    // View a user's history
void deleteUserAccount();  // Delete a user's account
void logoutAdmin();        // Logout from the admin panel

// User Panel Functions
void userPanel();
int isUsernameTaken();
char* userLogin();             // Handle user login
void userRegistration();       // Register a new user
void userHome();               // Display user home
void userProfile();            // View user profile
void viewCart();               // View cart of user
void addToCart();              // Add books to the cart
void removeFromCart();         // Remove books from cart
void bookstore();              // Manage user's store
void mystore();                // my store
void otherstore();             // other store
void findBookInBookCycle();    // Find New book
void findBookInOtherStores();  // Find Old book in other stores
void purchaseBook();           // Purchase a book
void accountSettings();
// About Us Function
void aboutUs();  // Display brief info about the platform

// Exit Function
void exitSystem();  // Exit the program

// Cursor move function
void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// loading screen function
void load() {
    int r, q;
    gotoxy(11, 4);
    printf("loading...");
    gotoxy(5, 5);
    for (r = 1; r <= 20; r++) {
        for (q = 0; q <= 100000000; q++);  // to display the character slowly
        printf("%c", 177);
    }
    getch();
}

// Starting point for the BookCycle system
int main() {
    system("cls");
    getchar();  // Clear the newline character from previous input
    getchar();  // Wait for user to press any key
    userCart = (Cart*)malloc(sizeof(Cart));
    if (userCart == NULL) {
        printf("Memory allocation failed for userCart.\n");
        return 1;
    }
    userCart->next = NULL;  // Initialize the linked list

    // Rest of your main function logic

    bookCycle();  // Call the main function for BookCycle system
    free(userCart);
    return 0;
}

// Helper function to pause and wait for user input
void pauseForUser() {
    printf("Press any key to continue...");
    getchar();  // Clear the newline character from previous input
    getchar();  // Wait for user to press any key
}

// Function to manage the whole BookCycle platform
void bookCycle() {
    load();
    int choice;

    while (1) {
        system("cls");
        printf("\n========= Welcome to BookCycle =========\n");
        printf("1. Admin Panel\n");
        printf("2. User Panel\n");
        printf("3. About Us\n");
        printf("4. Exit\n");
        printf("========================================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                adminDashboard();
                break;
            case 2:
                userPanel();
                break;
            case 3:
                aboutUs();
                break;
            case 4:
                exitSystem();
                return;  // Exit the loop and program
            default:
                printf("Invalid choice! Please try again.\n");
                pauseForUser();  // Wait for user input before redisplaying menu
                break;
        }
    }
}

// Admin Panel Functions

// Handles the admin login
char* adminLogin() {
    char enteredUsername[32];
    char enteredPassword[32];

    system("cls");
    printf("Admin Login\n");
    printf("Enter Username: ");
    scanf("%s", enteredUsername);  // Get username input
    printf("Enter Password: ");
    scanf("%s", enteredPassword);  // Get password input

    // Check entered credentials against the global admin array
    for (int i = 0; i < 5; i++) {
        if (strcmp(admins[i].username, enteredUsername) == 0 &&
            strcmp(admins[i].password, enteredPassword) == 0) {
            printf("Login successful. Welcome, %s!\n", admins[i].fullName);
            return admins[i].username;  // Return the admin username if login is successful
        }
    }

    return NULL;  // Return NULL if login fails
}

// Displays the admin dashboard
void adminDashboard() {
    char* loggedInAdminUsername = NULL;

    // Admin login loop
    while (1) {
        loggedInAdminUsername = adminLogin();  // Assuming adminLogin() returns the admin username on success
        if (loggedInAdminUsername != NULL) {
            pauseForUser();
            break;  // Login successful, exit loop
        } else {
            printf("Invalid admin credentials! ");
            char choice;
            printf("Would you like to try again? (y/n): ");
            scanf(" %c", &choice);

            if (choice == 'y' || choice == 'Y') {
            } else {
                return;
            }
        }
    }

    // Admin dashboard menu
    while (1) {
        int choice;

        system("cls");
        printf("\n--- Admin Dashboard ---\n");
        printf("1. View Admin Profile\n");
        printf("2. Manage Inventory\n");
        printf("3. View History\n");
        printf("4. Manage Users\n");
        printf("5. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                viewAdminProfile(loggedInAdminUsername);  // Pass logged-in admin username
                break;

            case 2:
                manageInventory(loggedInAdminUsername);  // Pass logged-in admin username
                break;

            case 3:
                viewHistory();
                break;

            case 4:
                manageUsers();
                break;

            case 5:
                printf("Logging out...\n");
                return;  // Exit the dashboard function to log out

            default:
                printf("Invalid choice! Please select a valid option.\n");
                break;
        }
    }
}

// Function to view the admin profile
void viewAdminProfile(char* adminUsername) {
    system("cls");

    // Search for the admin based on username
    int adminIndex = -1;
    for (int i = 0; i < 5; i++) {
        if (strcmp(admins[i].username, adminUsername) == 0) {
            adminIndex = i;
            break;
        }
    }

    if (adminIndex == -1) {
        printf("Admin not found. Cannot display profile.\n");
        pauseForUser();
        return;
    }

    // Display admin profile
    printf("\n--- Admin Profile ---\n");
    printf("Username: %s\n", admins[adminIndex].username);
    printf("Full Name: %s\n", admins[adminIndex].fullName);
    printf("Email: %s\n", admins[adminIndex].email);
    printf("Role: %s\n", admins[adminIndex].role);
    printf("Wallet Balance: $%.2f\n", admins[adminIndex].walletBalance);
    printf("------------------------\n");

    pauseForUser();
}

// Manage the inventory (adding/removing books)
void manageInventory(char* adminUsername) {
    int choice;

    while (1) {
        system("cls");
        printf("\n========= Manage Inventory =========\n");
        printf("1. Add New Book\n");
        printf("2. Add Old Book\n");
        printf("3. Remove New Book\n");
        printf("4. Remove Old Book\n");
        printf("5. View My Store\n");
        printf("6. Back to Admin Dashboard\n");
        printf("====================================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addNewBook(adminUsername);  // Pass admin username
                break;
            case 2:
                addOldBook(adminUsername);  // Pass admin username
                break;
            case 3:
                removeNewBook(adminUsername);  // Pass admin username
                break;
            case 4:
                removeOldBook(adminUsername);  // Pass admin username
                break;
            case 5:
                viewStore(adminUsername);  // Assuming this function is implemented to display all books
                break;
            case 6:
                return;  // Exit to the admin dashboard
            default:
                printf("Invalid choice! Please try again.\n");
                pauseForUser();
                break;
        }
    }
}

// Function to add a new book (updated for sellerUsername)
void addNewBook(char* sellerUsername) {
    NewBook newBook;

    // Get book details from the admin
    system("cls");
    printf("=== Add New Book ===\n");
    printf("Enter Book Title: ");
    getchar();  // To clear any leftover newline character from previous input
    fgets(newBook.title, sizeof(newBook.title), stdin);
    newBook.title[strcspn(newBook.title, "\n")] = '\0';  // Remove newline character at the end

    printf("Enter Author: ");
    fgets(newBook.author, sizeof(newBook.author), stdin);
    newBook.author[strcspn(newBook.author, "\n")] = '\0';  // Remove newline character

    printf("Enter Price: $");
    scanf("%f", &newBook.price);

    // Set seller username
    strncpy(newBook.sellerUsername, sellerUsername, sizeof(newBook.sellerUsername) - 1);
    newBook.sellerUsername[sizeof(newBook.sellerUsername) - 1] = '\0';  // Ensure null termination

    // Set Buyser username
    strncpy(newBook.buyerUsername, sellerUsername, sizeof(newBook.buyerUsername) - 1);
    newBook.buyerUsername[sizeof(newBook.buyerUsername) - 1] = '\0';  // Ensure null termination

    // Set initial status of the book (not sold yet)
    newBook.isSold = 0;

    // Add the new book to the dynamic list of new books
    NewBook* newBookNode = (NewBook*)malloc(sizeof(NewBook));
    *newBookNode = newBook;
    newBookNode->next = newBooksInventory;
    newBooksInventory = newBookNode;

    // Save the new book to the file (without sellerId)
    newBooksFile = fopen("newBooks.txt", "a");  // Open file in append mode
    if (newBooksFile == NULL) {
        printf("Error opening file to save new book.\n");
        pauseForUser();
        return;
    }

    // Write the book details to the file in the required format (without labels)
    fprintf(newBooksFile, "%s\n", newBook.title);
    fprintf(newBooksFile, "%s\n", newBook.author);
    fprintf(newBooksFile, "%.2f\n", newBook.price);
    fprintf(newBooksFile, "%s\n", newBook.sellerUsername);
    fprintf(newBooksFile, "%s\n", newBook.buyerUsername);
    fprintf(newBooksFile, "%d\n\n", newBook.isSold);  // Indicate whether the book is sold or not

    fclose(newBooksFile);

    printf("New book added successfully!\n");
    pauseForUser();
}
// Function to add an old book (updated for sellerUsername)
void addOldBook(char* sellerUsername) {
    OldBook newOldBook;

    // Get book details from the admin
    system("cls");
    printf("=== Add Old Book ===\n");
    printf("Enter Book Title: ");
    getchar();  // To clear any leftover newline character from previous input
    fgets(newOldBook.title, sizeof(newOldBook.title), stdin);
    newOldBook.title[strcspn(newOldBook.title, "\n")] = '\0';  // Remove newline character

    printf("Enter Author: ");
    fgets(newOldBook.author, sizeof(newOldBook.author), stdin);
    newOldBook.author[strcspn(newOldBook.author, "\n")] = '\0';  // Remove newline character

    printf("Enter Price: $");
    scanf("%f", &newOldBook.price);

    // Get purchase date (in "YYYY-MM-DD" format)
    printf("Enter Purchase Date (YYYY-MM-DD): ");
    scanf("%s", newOldBook.buyDate);

    // Ask about the condition of the book
    printf("Does the book have marks? (1 = Yes, 0 = No): ");
    scanf("%d", &newOldBook.hasMarks);

    printf("Is the book torn? (1 = Yes, 0 = No): ");
    scanf("%d", &newOldBook.isTorn);

    // Set seller username (this is the admin/seller adding the book)
    strncpy(newOldBook.sellerUsername, sellerUsername, sizeof(newOldBook.sellerUsername) - 1);
    newOldBook.sellerUsername[sizeof(newOldBook.sellerUsername) - 1] = '\0';  // Ensure null termination

    strncpy(newOldBook.buyerUsername, sellerUsername, sizeof(newOldBook.buyerUsername) - 1);
    newOldBook.buyerUsername[sizeof(newOldBook.buyerUsername) - 1] = '\0';  // Ensure null termination

    // Set initial status of the book (not sold yet)
    newOldBook.isSold = 0;

    // Add the new book to the dynamic list of old books
    OldBook* newOldBookNode = (OldBook*)malloc(sizeof(OldBook));
    *newOldBookNode = newOldBook;
    newOldBookNode->next = oldBooksInventory;
    oldBooksInventory = newOldBookNode;

    // Save the new book to the file (without sellerId)
    oldBooksFile = fopen("oldBooks.txt", "a");  // Open the file in append mode
    if (oldBooksFile == NULL) {
        printf("Error opening file to save old book.\n");
        pauseForUser();
        return;
    }

    // Write the book details to the file in the required format (without labels)
    fprintf(oldBooksFile, "%s\n", newOldBook.title);
    fprintf(oldBooksFile, "%s\n", newOldBook.author);
    fprintf(oldBooksFile, "%.2f\n", newOldBook.price);
    fprintf(oldBooksFile, "%s\n", newOldBook.buyDate);
    fprintf(oldBooksFile, "%d\n", newOldBook.hasMarks);
    fprintf(oldBooksFile, "%d\n", newOldBook.isTorn);
    fprintf(oldBooksFile, "%s\n", newOldBook.sellerUsername);
    fprintf(oldBooksFile, "%s\n", newOldBook.buyerUsername);
    fprintf(oldBooksFile, "%d\n\n", newOldBook.isSold);  // Indicate whether the book is sold or not

    fclose(oldBooksFile);

    printf("Old book added successfully!\n");
    pauseForUser();
}

void removeNewBook(char* sellerUsername) {
    system("cls");

    char bookTitle[100];
    int found = 0;  // Flag to check if the book was found and removed
    printf("Enter the Book Title to remove: ");
    getchar();  // To clear any leftover newline character
    fgets(bookTitle, sizeof(bookTitle), stdin);
    bookTitle[strcspn(bookTitle, "\n")] = '\0';  // Remove the newline character at the end

    // If the user enters "back", exit the function
    if (strcmp(bookTitle, "back") == 0) {
        return;  // Exit the function and go back to the previous menu
    }

    // Open the newBooks.txt file to read and load the data into the linked list
    FILE* newBooksFile = fopen("newBooks.txt", "r");
    if (newBooksFile == NULL) {
        printf("Error opening file to read new books.\n");
        pauseForUser();
        return;
    }

    // Read the books from the file and populate the linked list
    NewBook* newBooksInventory = NULL;
    NewBook* last = NULL;
    NewBook tempBook;

    // Read books from the file and populate the linked list
    while (fgets(tempBook.title, sizeof(tempBook.title), newBooksFile) != NULL) {
        tempBook.title[strcspn(tempBook.title, "\n")] = '\0';  // Remove newline

        // Read the author name
        fgets(tempBook.author, sizeof(tempBook.author), newBooksFile);
        tempBook.author[strcspn(tempBook.author, "\n")] = '\0';  // Remove newline

        // Read the price (directly as a float)
        if (fscanf(newBooksFile, "%f\n", &tempBook.price) != 1) {
            printf("Error reading price for book \"%s\".\n", tempBook.title);
            continue;  // Skip this book if reading the price fails
        }

        // Read the seller's username
        fgets(tempBook.sellerUsername, sizeof(tempBook.sellerUsername), newBooksFile);
        tempBook.sellerUsername[strcspn(tempBook.sellerUsername, "\n")] = '\0';  // Remove newline

        // Read the buyer's username
        fgets(tempBook.buyerUsername, sizeof(tempBook.buyerUsername), newBooksFile);
        tempBook.buyerUsername[strcspn(tempBook.buyerUsername, "\n")] = '\0';  // Remove newline

        // Read the sold status (0 or 1)
        if (fscanf(newBooksFile, "%d\n\n", &tempBook.isSold) != 1) {
            printf("Error reading sold status for book \"%s\".\n", tempBook.title);
            continue;  // Skip this book if reading the sold status fails
        }

        // Create a new node and add it to the list
        NewBook* newNode = (NewBook*)malloc(sizeof(NewBook));
        *newNode = tempBook;
        newNode->next = NULL;

        if (newBooksInventory == NULL) {
            newBooksInventory = newNode;  // First node
        } else {
            last->next = newNode;  // Add to the end of the list
        }
        last = newNode;  // Update the last pointer
    }

    fclose(newBooksFile);

    // Search for the book by title and seller username
    NewBook* current = newBooksInventory;
    NewBook* previous = NULL;

    while (current != NULL) {
        // If the book title matches and the seller's username matches
        if (strcmp(current->title, bookTitle) == 0 && strcmp(current->sellerUsername, sellerUsername) == 0) {
            // Book found, now remove it from the linked list
            if (previous == NULL) {
                // If the book is the first book in the list, update the head
                newBooksInventory = current->next;
            } else {
                // Otherwise, unlink the book from the list
                previous->next = current->next;
            }

            // Free the memory allocated for the book
            free(current);

            // Write the updated list back to the file
            FILE* tempFile = fopen("newBooks.txt", "w");  // Use "w" to overwrite the file
            if (tempFile == NULL) {
                printf("Error opening file to update new books.\n");
                pauseForUser();
                return;
            }

            // Write the updated list to the file
            NewBook* temp = newBooksInventory;
            while (temp != NULL) {
                fprintf(tempFile, "%s\n", temp->title);
                fprintf(tempFile, "%s\n", temp->author);
                fprintf(tempFile, "%.2f\n", temp->price);
                fprintf(tempFile, "%s\n", temp->sellerUsername);
                fprintf(tempFile, "%s\n", temp->buyerUsername);
                fprintf(tempFile, "%d\n\n", temp->isSold);

                temp = temp->next;
            }
            fclose(tempFile);

            printf("Book removed successfully!\n");
            pauseForUser();
            found = 1;
            break;  // Exit the loop since the book was found and removed
        }

        previous = current;
        current = current->next;
    }

    // If no book was found, prompt the user with a simple retry option
    if (!found) {
        printf("Book with title \"%s\" not found or you do not have permission to remove it.\n", bookTitle);
        char choice;
        printf("Would you like to try again? (y/n): ");
        scanf(" %c", &choice);

        if (choice == 'y' || choice == 'Y') {
            removeNewBook(sellerUsername);  // Exit the loop if the user chooses not to try again
        } else {
            manageInventory(sellerUsername);
        }
    }

    // Free the memory allocated for the linked list before exiting
    NewBook* temp;
    while (newBooksInventory != NULL) {
        temp = newBooksInventory;
        newBooksInventory = newBooksInventory->next;
        free(temp);
    }
}

void removeOldBook(char* sellerUsername) {
    system("cls");

    char bookTitle[100];
    int found = 0;  // Flag to check if the book was found and removed
    printf("Enter the Book Title to remove: ");
    getchar();  // To clear any leftover newline character
    fgets(bookTitle, sizeof(bookTitle), stdin);
    bookTitle[strcspn(bookTitle, "\n")] = '\0';  // Remove the newline character at the end

    // If the user enters "back", exit the function
    if (strcmp(bookTitle, "back") == 0) {
        return;  // Exit the function and go back to the previous menu
    }

    // Open the oldBooks.txt file to read and load the data into the linked list
    FILE* oldBooksFile = fopen("oldBooks.txt", "r");
    if (oldBooksFile == NULL) {
        perror("Error opening file to read old books");
        pauseForUser();
        return;
    }

    // Read the books from the file and populate the linked list
    OldBook* oldBooksInventory = NULL;
    OldBook* last = NULL;

    while (1) {
        OldBook tempBook;

        // Read book title
        if (fgets(tempBook.title, sizeof(tempBook.title), oldBooksFile) == NULL)
            break;  // End of file
        tempBook.title[strcspn(tempBook.title, "\n")] = '\0';

        // Read author
        fgets(tempBook.author, sizeof(tempBook.author), oldBooksFile);
        tempBook.author[strcspn(tempBook.author, "\n")] = '\0';

        // Read price
        if (fscanf(oldBooksFile, "%f\n", &tempBook.price) != 1)
            break;

        // Read the publication date (skip, but do not store it)
        char publicationDate[20];
        fgets(publicationDate, sizeof(publicationDate), oldBooksFile);

        // Read additional fields for condition (hasMarks and isTorn)
        if (fscanf(oldBooksFile, "%d\n%d\n", &tempBook.hasMarks, &tempBook.isTorn) != 2)
            break;

        // Read seller's username
        fgets(tempBook.sellerUsername, sizeof(tempBook.sellerUsername), oldBooksFile);
        tempBook.sellerUsername[strcspn(tempBook.sellerUsername, "\n")] = '\0';

        // Read buyer's username
        fgets(tempBook.buyerUsername, sizeof(tempBook.buyerUsername), oldBooksFile);
        tempBook.buyerUsername[strcspn(tempBook.buyerUsername, "\n")] = '\0';

        // Read sold status
        if (fscanf(oldBooksFile, "%d\n", &tempBook.isSold) != 1)
            break;

        // Add book to the linked list
        OldBook* newNode = (OldBook*)malloc(sizeof(OldBook));
        *newNode = tempBook;
        newNode->next = NULL;

        if (oldBooksInventory == NULL) {
            oldBooksInventory = newNode;
        } else {
            last->next = newNode;
        }
        last = newNode;
    }

    fclose(oldBooksFile);

    // Search for the book by title and seller username
    OldBook* current = oldBooksInventory;
    OldBook* previous = NULL;

    while (current != NULL) {
        if (strcmp(current->title, bookTitle) == 0 && strcmp(current->sellerUsername, sellerUsername) == 0) {
            // Book found, remove it from the list
            if (previous == NULL) {
                oldBooksInventory = current->next;
            } else {
                previous->next = current->next;
            }

            free(current);

            // Write the updated list back to the file
            FILE* tempFile = fopen("oldBooks.txt", "w");
            if (tempFile == NULL) {
                perror("Error opening file to update old books");
                pauseForUser();
                return;
            }

            OldBook* temp = oldBooksInventory;
            while (temp != NULL) {
                fprintf(tempFile, "%s\n", temp->title);
                fprintf(tempFile, "%s\n", temp->author);
                fprintf(tempFile, "%.2f\n", temp->price);
                fprintf(tempFile, "%s\n", temp->buyDate);
                fprintf(tempFile, "%d\n", temp->hasMarks);
                fprintf(tempFile, "%d\n", temp->isTorn);
                fprintf(tempFile, "%s\n", temp->sellerUsername);
                fprintf(tempFile, "%s\n", temp->buyerUsername);
                fprintf(tempFile, "%d\n", temp->isSold);

                temp = temp->next;
            }
            fclose(tempFile);

            printf("Book removed successfully!\n");
            pauseForUser();
            found = 1;
            break;
        }

        previous = current;
        current = current->next;
    }

    if (!found) {
        printf("Book with title \"%s\" not found or you do not have permission to remove it.\n", bookTitle);
        char choice;
        printf("Would you like to try again? (y/n): ");
        scanf(" %c", &choice);

        if (choice == 'y' || choice == 'Y') {
            removeOldBook(sellerUsername);
        } else {
            return;
        }
    }

    // Free the memory allocated for the linked list
    OldBook* temp;
    while (oldBooksInventory != NULL) {
        temp = oldBooksInventory;
        oldBooksInventory = oldBooksInventory->next;
        free(temp);
    }
}

void viewStore(char* sellerUsername) {
    system("cls");
    printf("Viewing All Books for Seller: %s\n", sellerUsername);
    printf("---------------------------------------------------\n");

    int found = 0;

    // Open the new books file and read it
    FILE* newBooksFile = fopen("newBooks.txt", "r");
    if (newBooksFile == NULL) {
        printf("Error opening new books file.\n");
        pauseForUser();
        return;
    }

    FILE* oldBooksFile = fopen("oldBooks.txt", "r");
    if (oldBooksFile == NULL) {
        printf("Error opening new books file.\n");
        pauseForUser();
        return;
    }

    // Read and display new books
    NewBook newBook;
    printf("New Books:\n");

    while (fgets(newBook.title, sizeof(newBook.title), newBooksFile)) {
        newBook.title[strcspn(newBook.title, "\n")] = '\0';  // Remove newline character

        if (!fgets(newBook.author, sizeof(newBook.author), newBooksFile)) break;
        newBook.author[strcspn(newBook.author, "\n")] = '\0';  // Remove newline

        if (fscanf(newBooksFile, "%f\n", &newBook.price) != 1) break;  // Read price

        if (!fgets(newBook.sellerUsername, sizeof(newBook.sellerUsername), newBooksFile)) break;
        newBook.sellerUsername[strcspn(newBook.sellerUsername, "\n")] = '\0';  // Remove newline

        if (!fgets(newBook.buyerUsername, sizeof(newBook.buyerUsername), newBooksFile)) break;
        newBook.buyerUsername[strcspn(newBook.buyerUsername, "\n")] = '\0';  // Remove newline

        if (fscanf(newBooksFile, "%d\n\n", &newBook.isSold) != 1) break;  // Read sold status

        // Check if the seller matches
        if (strcmp(newBook.sellerUsername, sellerUsername) == 0) {
            printf("Title: %s, Author: %s, Price: %.2f, Status: %s\n",
                   newBook.title,
                   newBook.author,
                   newBook.price,
                   newBook.isSold ? "Sold to buyer: " : "Available");
            if (newBook.isSold) {
                printf("%s\n", newBook.buyerUsername);
            }
            found = 1;
        }
    }

    fclose(newBooksFile);

    // Open the old books file and read it
    // Read and display old books
    OldBook oldBook;
    printf("\nOld Books:\n");

    while (fgets(oldBook.title, sizeof(oldBook.title), oldBooksFile)) {
        oldBook.title[strcspn(oldBook.title, "\n")] = '\0';  // Remove newline

        if (!fgets(oldBook.author, sizeof(oldBook.author), oldBooksFile)) break;
        oldBook.author[strcspn(oldBook.author, "\n")] = '\0';  // Remove newline

        if (fscanf(oldBooksFile, "%f\n", &oldBook.price) != 1) break;  // Read price

        // Skip the publication date (not used in this case)
        char publicationDate[20];
        fgets(publicationDate, sizeof(publicationDate), oldBooksFile);

        // Read condition attributes
        if (fscanf(oldBooksFile, "%d\n%d\n", &oldBook.hasMarks, &oldBook.isTorn) != 2)
            break;

        if (!fgets(oldBook.sellerUsername, sizeof(oldBook.sellerUsername), oldBooksFile)) break;
        oldBook.sellerUsername[strcspn(oldBook.sellerUsername, "\n")] = '\0';  // Remove newline

        if (!fgets(oldBook.buyerUsername, sizeof(oldBook.buyerUsername), oldBooksFile)) break;
        oldBook.buyerUsername[strcspn(oldBook.buyerUsername, "\n")] = '\0';  // Remove newline

        if (fscanf(oldBooksFile, "%d\n\n", &oldBook.isSold) != 1) break;  // Read sold status

        // Check if the seller matches
        if (strcmp(oldBook.sellerUsername, sellerUsername) == 0) {
            printf("Title: %s, Author: %s, Price: %.2f, Status: %s\n",
                   oldBook.title,
                   oldBook.author,
                   oldBook.price,
                   oldBook.isSold ? "Sold to buyer: " : "Available");
            if (oldBook.isSold) {
                printf("%s\n", oldBook.buyerUsername);
            }
            found = 1;
        }
    }

    fclose(oldBooksFile);

    // If no books were found for this seller
    if (!found) {
        printf("No books available for Seller: %s.\n", sellerUsername);
    }

    pauseForUser();
}

// Views the transaction history of all users with search functionality
void viewHistory() {
    system("cls");
    printf("Viewing Transaction History:\n");
    printf("---------------------------------------------\n");

    // Ask user what they want to search by
    int choice;
    printf("Search by:\n");
    printf("1. Seller\n");
    printf("2. Buyer\n");
    printf("3. Book Title\n");
    printf("Enter your choice (1-3): ");
    scanf("%d", &choice);
    getchar();  // To consume the newline character left by scanf

    char searchTerm[100];
    switch (choice) {
        case 1:
            printf("Enter buyer username: ");
            fgets(searchTerm, sizeof(searchTerm), stdin);
            searchTerm[strcspn(searchTerm, "\n")] = '\0';  // Remove newline
            break;
        case 2:
            printf("Enter seller username: ");
            fgets(searchTerm, sizeof(searchTerm), stdin);
            searchTerm[strcspn(searchTerm, "\n")] = '\0';  // Remove newline
            break;
        case 3:
            printf("Enter book title: ");
            fgets(searchTerm, sizeof(searchTerm), stdin);
            searchTerm[strcspn(searchTerm, "\n")] = '\0';  // Remove newline
            break;
        default:
            printf("Invalid choice. Returning to menu.\n");
            pauseForUser();
            return;
    }

    printf("\n%-20s %-20s %-20s %-10s\n", "Seller", "Buyer", "Book Title", "Amount");
    printf("---------------------------------------------\n");

    // Open the history file
    FILE* historyFile = fopen("history.txt", "r");
    if (historyFile == NULL) {
        printf("No transaction history available.\n");
        pauseForUser();
        return;
    }

    // Read and display all matching transaction records
    History tempHistory;
    int found = 0;
    while (fscanf(historyFile, "%s %s %s %f\n",
                  tempHistory.sellerUsername,
                  tempHistory.buyerUsername,
                  tempHistory.title,
                  &tempHistory.amount) == 4) {
        // Search for matching transactions
        int match = 0;
        switch (choice) {
            case 1:  // Search by seller
                if (strcmp(tempHistory.sellerUsername, searchTerm) == 0) {
                    match = 1;
                }
                break;
            case 2:  // Search by buyer
                if (strcmp(tempHistory.buyerUsername, searchTerm) == 0) {
                    match = 1;
                }
                break;
            case 3:  // Search by book title
                if (strcmp(tempHistory.title, searchTerm) == 0) {
                    match = 1;
                }
                break;
        }

        if (match) {
            printf("%-20s %-20s %-20s $%-10.2f\n",
                   tempHistory.sellerUsername,
                   tempHistory.buyerUsername,
                   tempHistory.title,
                   tempHistory.amount);
            found = 1;
        }
    }

    if (!found) {
        printf("No matching transactions found.\n");
    }

    fclose(historyFile);
    pauseForUser();
}

// Function to manage users (view, deactivate/reactivate user accounts)
void manageUsers() {
    system("cls");
    printf("Managing Users:\n");
    printf("---------------------------------------------\n");

    // Open the user file for reading
    FILE* userFile = fopen("users.txt", "r");
    if (userFile == NULL) {
        printf("Error opening user file.\n");
        pauseForUser();
        return;
    }

    User tempUser;
    int userFound = 0;

    // Display the list of users
    printf("%-20s %-20s %-30s %-30s %-10s $%-10s\n", "Username", "Fullname", "Email", "Address", "Status", "Balance");
    printf("---------------------------------------------------------------\n");

    // Read and display all users from the file
    while (fscanf(userFile, "%s\n%s\n%s\n%s\n%s\n%f\n%d\n%d\n%d\n",
                  tempUser.username, tempUser.password, tempUser.fullName,
                  tempUser.email, tempUser.address, &tempUser.walletBalance,
                  &tempUser.booksBought, &tempUser.booksSold, &tempUser.isActive) == 9) {
        printf("%-20s %-20s %-30s %-30s %-10s $%-10.2f\n",
               tempUser.username,
               tempUser.fullName,
               tempUser.email,
               tempUser.address,
               (tempUser.isActive ? "Active" : "Inactive"),
               tempUser.walletBalance);
        userFound = 1;
    }

    if (!userFound) {
        printf("No users found.\n");
        fclose(userFile);
        pauseForUser();
        return;
    }

    // Ask the admin for an action (deactivate/reactivate account)
    char usernameToChange[50];
    int newStatus;
    printf("\nEnter the username of the user you want to deactivate/reactivate: ");
    getchar();  // Clear any previous newline character
    fgets(usernameToChange, sizeof(usernameToChange), stdin);
    usernameToChange[strcspn(usernameToChange, "\n")] = '\0';  // Remove newline

    // Ask the admin to choose to deactivate (0) or reactivate (1)
    printf("Enter 0 to deactivate or 1 to reactivate the account: ");
    scanf("%d", &newStatus);
    getchar();  // To clear the newline left in the input buffer

    // Create a temporary file to store updated user data
    FILE* tempFile = fopen("tempUser.txt", "w");
    if (tempFile == NULL) {
        printf("Error opening temporary file.\n");
        fclose(userFile);
        pauseForUser();
        return;
    }

    // Rewind the userFile to start reading again from the beginning
    rewind(userFile);
    int updated = 0;

    // Go through the file and update the status of the user
    while (fscanf(userFile, "%s\n%s\n%s\n%s\n%s\n%f\n%d\n%d\n%d\n",
                  tempUser.username, tempUser.password, tempUser.fullName,
                  tempUser.email, tempUser.address, &tempUser.walletBalance,
                  &tempUser.booksBought, &tempUser.booksSold, &tempUser.isActive) == 9) {
        if (strcmp(tempUser.username, usernameToChange) == 0) {
            tempUser.isActive = newStatus;  // Change the user status (deactivate or reactivate)
            updated = 1;
        }

        // Write the updated user info to the temp file
        fprintf(tempFile, "%s\n%s\n%s\n%s\n%s\n%.2f\n%d\n%d\n%d\n",
                tempUser.username, tempUser.password, tempUser.fullName,
                tempUser.email, tempUser.address, tempUser.walletBalance,
                tempUser.booksBought, tempUser.booksSold, tempUser.isActive);
    }

    if (!updated) {
        printf("User not found or no change needed.\n");
    } else {
        printf("User account updated successfully.\n");
    }

    fclose(userFile);
    fclose(tempFile);

    // Replace the original file with the temporary file
    remove("users.txt");
    rename("tempUser.txt", "users.txt");

    pauseForUser();
}

// Logs out the admin and returns to the main menu
void logoutAdmin() {
    printf("Logging out Admin\n");
    pauseForUser();
}

// User Panel Functions
void userPanel() {
    int choice;

    while (1) {
        system("cls");
        printf("\n========= User Panel =========\n");
        printf("1. User Registration\n");
        printf("2. User Login\n");
        printf("3. Exit\n");
        printf("=============================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                userRegistration();  // Call function for user registration
                break;

            case 2:
                userHome();  // Call function for user login
                break;

            case 3:
                printf("Exiting User Panel...\n");
                return;  // Exit the User Panel
                break;

            default:
                printf("Invalid choice! Please try again.\n");
                pauseForUser();  // Wait for user to press any key to continue
                break;
        }
    }
}

int isUsernameTaken(const char* username) {
    // Open the file in read mode
    userFile = fopen("users.txt", "r");
    if (!userFile) {
        // If the file doesn't exist, assume no users exist
        return 0;
    }

    User tempUser;
    while (fscanf(userFile, "%s", tempUser.username) != EOF) {
        fscanf(userFile, "%s", tempUser.password);
        fscanf(userFile, "%s", tempUser.fullName);
        fscanf(userFile, "%s", tempUser.email);
        fscanf(userFile, "%s", tempUser.address);
        fscanf(userFile, "%f", &tempUser.walletBalance);
        fscanf(userFile, "%d", &tempUser.booksBought);
        fscanf(userFile, "%d", &tempUser.booksSold);
        fscanf(userFile, "%d", &tempUser.isActive);

        if (strcmp(tempUser.username, username) == 0) {
            fclose(userFile);
            return 1;  // Username is already taken
        }
    }
    fclose(userFile);
    return 0;  // Username is not taken
}

void userRegistration() {
    system("cls");
    User newUser;

    // Input user details
    printf("Enter Username: ");
    getchar();
    fgets(newUser.username, sizeof(newUser.username), stdin);
    newUser.username[strcspn(newUser.username, "\n")] = 0;  // Remove newline character from fgets

    // Check if the username already exists
    if (isUsernameTaken(newUser.username)) {
        printf("Username is already taken. Please choose another one.\n");
        pauseForUser();  // Pause before returning to allow user to read the message
        return;
    }

    // Input other details
    printf("Enter Password: ");
    fgets(newUser.password, sizeof(newUser.password), stdin);
    newUser.password[strcspn(newUser.password, "\n")] = 0;  // Remove newline character

    printf("Enter Full Name: ");
    fgets(newUser.fullName, sizeof(newUser.fullName), stdin);
    newUser.fullName[strcspn(newUser.fullName, "\n")] = 0;  // Remove newline character

    printf("Enter Email: ");
    fgets(newUser.email, sizeof(newUser.email), stdin);
    newUser.email[strcspn(newUser.email, "\n")] = 0;  // Remove newline character

    printf("Enter Address: ");
    fgets(newUser.address, sizeof(newUser.address), stdin);
    newUser.address[strcspn(newUser.address, "\n")] = 0;  // Remove newline character

    newUser.walletBalance = 100000.0f;
    newUser.booksBought = 0;
    newUser.booksSold = 0;
    newUser.isActive = 1;  // Set user as active by default

    // Open the file to update user data
    userFile = fopen("users.txt", "a");
    if (!userFile) {
        printf("Error opening file for user registration.\n");
        pauseForUser();  // Pause before returning
        return;
    }

    // Write the new user data to the file
    fprintf(userFile, "%s\n", newUser.username);
    fprintf(userFile, "%s\n", newUser.password);
    fprintf(userFile, "%s\n", newUser.fullName);
    fprintf(userFile, "%s\n", newUser.email);
    fprintf(userFile, "%s\n", newUser.address);
    fprintf(userFile, "%.2f\n", newUser.walletBalance);
    fprintf(userFile, "%d\n", newUser.booksBought);
    fprintf(userFile, "%d\n", newUser.booksSold);
    fprintf(userFile, "%d\n", newUser.isActive);

    // Close the file after writing
    fclose(userFile);

    printf("User registered successfully!\n");
    pauseForUser();  // Pause before returning to main
}

char* userLogin() {
    static char username[50];  // Using static to ensure the username persists after the function
    char password[50];
    User tempUser;

    // Get login credentials from the user
    printf("Enter Username: ");
    getchar();
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = 0;  // Remove newline character from fgets

    printf("Enter Password: ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = 0;  // Remove newline character from fgets

    userFile = fopen("users.txt", "r");
    if (!userFile) {
        // If the file doesn't exist, assume no users exist
        return 0;
    }

    while (fscanf(userFile, "%s", tempUser.username) != EOF) {
        fscanf(userFile, "%s", tempUser.password);
        fscanf(userFile, "%s", tempUser.fullName);
        fscanf(userFile, "%s", tempUser.email);
        fscanf(userFile, "%s", tempUser.address);
        fscanf(userFile, "%f", &tempUser.walletBalance);
        fscanf(userFile, "%d", &tempUser.booksBought);
        fscanf(userFile, "%d", &tempUser.booksSold);
        fscanf(userFile, "%d", &tempUser.isActive);

        if (strcmp(tempUser.username, username) == 0 && strcmp(tempUser.password, password) == 0) {
            fclose(userFile);
            printf("Login successful!\n");
            pauseForUser();
            if (tempUser.isActive == 0) {
                system("cls");
                printf("Your Account is Deactivate by the Admin\n");
                pauseForUser();
                userPanel();
            }
            return username;  // Return the username if login is successful
        }
    }
    fclose(userFile);

    // If we reach here, login failed
    printf("Invalid username or password.\n");
    return NULL;  // Return NULL if login is invalid
}

/// Displays the user home page
void userHome() {
    userFile = fopen("users.txt", "r");
    if (!userFile) {
        // If the file doesn't exist, assume no users exist
        return 0;
    }
    char* username;  // User logs in
    while (1) {
        system("cls");
        username = userLogin();
        if (username == NULL) {  // If login fails, return to the User Panel
            char choice;
            printf("Would you like to try again? (y/n): ");
            scanf(" %c", &choice);
            if (choice == 'y' || choice == 'Y') {
            } else {
                return;
            }
        } else {
            break;
        }
    }
    while (1) {
        system("cls");  // Clear the screen (Windows)
        printf("========= User Home =========\n");
        printf("Welcome, %s!\n", username);  // Display logged-in username
        printf("1. View Profile\n");
        printf("2. My Cart\n");
        printf("3. Store\n");
        printf("4. Logout\n");
        printf("Enter your choice: ");

        int choice;
        scanf("%d", &choice);
        getchar();  // To consume the newline character left by scanf

        switch (choice) {
            case 1:
                userProfile(username);
                break;
            case 2:
                cartMenu(username);
                break;
            case 3:
                bookstore(username);
                break;
            case 4:
                printf("Logging out...\n");
                return;  // Return to User Panel
            default:
                printf("Invalid choice! Please try again.\n");
        }
        pauseForUser();
    }
}

// Function to display user profile
void userProfile(const char* username) {
    system("cls");
    User tempUser;
    int found = 0;

    // Open the user file for reading
    userFile = fopen("users.txt", "r");
    if (!userFile) {
        printf("Error opening file to view profile.\n");
        return;
    }

    // Loop through the user file and find the matching username
    while (fscanf(userFile, "%s", tempUser.username) != EOF) {
        fscanf(userFile, "%s", tempUser.password);
        fscanf(userFile, "%s", tempUser.fullName);
        fscanf(userFile, "%s", tempUser.email);
        fscanf(userFile, "%s", tempUser.address);
        fscanf(userFile, "%f", &tempUser.walletBalance);
        fscanf(userFile, "%d", &tempUser.booksBought);
        fscanf(userFile, "%d", &tempUser.booksSold);
        fscanf(userFile, "%d", &tempUser.isActive);

        // Check if the current user matches the given username
        if (strcmp(tempUser.username, username) == 0) {
            found = 1;
            break;
        }
    }
    fclose(userFile);

    if (!found) {
        printf("User not found.\n");
        return;
    }

    // Display user profile
    printf("========= User Profile =========\n");
    printf("Username: %s\n", tempUser.username);
    printf("Full Name: %s\n", tempUser.fullName);
    printf("Email: %s\n", tempUser.email);
    printf("Address: %s\n", tempUser.address);

    // Option to go to account settings or back
    int choice;
    printf("\n1. Delete Your Account\n");
    printf("2. Back\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    if (choice == 1) {
        accountSettings(username);
    } else if (choice == 2) {
        return;  // Return to the previous menu
    } else {
        printf("Invalid choice! Returning to previous menu.\n");
    }
}

// Function to handle account settings (only delete in this case)
void accountSettings(const char* username) {
    system("cls");
    User tempUser;
    int found = 0;

    // Open the user file for reading and writing
    userFile = fopen("users.txt", "r+");
    if (!userFile) {
        printf("Error opening file for account settings.\n");
        return;
    }

    // Open a temporary file to write the updated user data
    FILE* tempFile = fopen("tempUsers.txt", "w");
    if (!tempFile) {
        printf("Error opening temporary file for user deletion.\n");
        fclose(userFile);
        return;
    }

    // Loop through the user file and find the matching username to delete
    while (fscanf(userFile, "%s", tempUser.username) != EOF) {
        fscanf(userFile, "%s", tempUser.password);
        fscanf(userFile, "%s", tempUser.fullName);
        fscanf(userFile, "%s", tempUser.email);
        fscanf(userFile, "%s", tempUser.address);
        fscanf(userFile, "%f", &tempUser.walletBalance);
        fscanf(userFile, "%d", &tempUser.booksBought);
        fscanf(userFile, "%d", &tempUser.booksSold);
        fscanf(userFile, "%d", &tempUser.isActive);

        // If this is the user to be deleted, skip writing their data
        if (strcmp(tempUser.username, username) != 0) {
            fprintf(tempFile, "%s\n", tempUser.username);
            fprintf(tempFile, "%s\n", tempUser.password);
            fprintf(tempFile, "%s\n", tempUser.fullName);
            fprintf(tempFile, "%s\n", tempUser.email);
            fprintf(tempFile, "%s\n", tempUser.address);
            fprintf(tempFile, "%.2f\n", tempUser.walletBalance);
            fprintf(tempFile, "%d\n", tempUser.booksBought);
            fprintf(tempFile, "%d\n", tempUser.booksSold);
            fprintf(tempFile, "%d\n", tempUser.isActive);
        } else {
            found = 1;  // User found and skipped (i.e., deleted)
        }
    }

    // If user not found, return and inform
    if (!found) {
        printf("User not found.\n");
        fclose(userFile);
        fclose(tempFile);
        return;
    }

    // Close both files
    fclose(userFile);
    fclose(tempFile);

    // Remove the old user file and rename the temporary file
    remove("users.txt");
    rename("tempUsers.txt", "users.txt");

    printf("Your account has been successfully deleted.\n");
    pauseForUser();
    // Call the userPanel function to return to the User Panel
    userPanel();
}

void cartMenu(const char* username) {
    int choice;
    char title[100];

    while (1) {
        system("cls");  // Clear the screen
        printf("======== Cart Menu ========\n");
        printf("1. View Cart\n");
        printf("2. Add to Cart\n");
        printf("3. Remove from Cart\n");
        printf("4. Clear Cart\n");
        printf("5. Back to User Home\n");
        printf("===========================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();  // Clear newline from buffer

        switch (choice) {
            case 1:
                viewCart(username);
                pauseForUser();
                break;
            case 2:
                printf("Enter book title to add: ");
                fgets(title, sizeof(title), stdin);
                title[strcspn(title, "\n")] = 0;  // Remove newline character
                addToCart(username, title);
                pauseForUser();
                break;
            case 3:
                printf("Enter book title to remove: ");
                fgets(title, sizeof(title), stdin);
                title[strcspn(title, "\n")] = 0;  // Remove newline character
                removeFromCart(username, title);
                pauseForUser();
                break;
            case 4:
                clearCart(username);
                pauseForUser();
                break;
            case 5:
                return;  // Exit to user home
            default:
                printf("Invalid choice! Please try again.\n");
                pauseForUser();
        }
    }
}

void viewCart(const char* username) {
    // Check if the cart is empty
    if (userCart == NULL || userCart->next == NULL) {
        printf("Your cart is empty!\n");
        return;
    }

    printf("======= Cart for %s =======\n", username);
    Cart* current = userCart->next;
    int found = 0;

    // Traverse the cart and display items for the given user
    while (current != NULL) {
        if (strcmp(current->username, username) == 0) {
            printf("- %s\n", current->title);
            found = 1;
        }
        current = current->next;
    }

    if (!found) {
        printf("No items in the cart for user %s.\n", username);
    }

    printf("===========================\n");
}

void addToCart(const char* username, const char* title) {
    // Allocate memory for a new cart item
    Cart* newItem = (Cart*)malloc(sizeof(Cart));
    if (newItem == NULL) {
        printf("Memory allocation failed for new cart item.\n");
        return;
    }

    // Set details for the new cart item
    strcpy(newItem->username, username);
    strcpy(newItem->title, title);
    newItem->next = NULL;

    // Insert the new item at the end of the linked list
    if (userCart == NULL || userCart->next == NULL) {
        userCart->next = newItem;
    } else {
        Cart* current = userCart->next;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newItem;
    }

    printf("Book '%s' has been added to %s's cart.\n", title, username);
}

void removeFromCart(const char* username, const char* title) {
    if (userCart == NULL || userCart->next == NULL) {
        printf("Your cart is empty!\n");
        return;
    }

    Cart* current = userCart->next;
    Cart* previous = userCart;

    // Search for the item to remove
    while (current != NULL) {
        if (strcmp(current->username, username) == 0 && strcmp(current->title, title) == 0) {
            // Remove the item from the linked list
            previous->next = current->next;
            free(current);
            printf("Book '%s' has been removed from %s's cart.\n", title, username);
            return;
        }
        previous = current;
        current = current->next;
    }

    printf("Book '%s' not found in %s's cart.\n", title, username);
}

void clearCart(const char* username) {
    if (userCart == NULL || userCart->next == NULL) {
        printf("Your cart is already empty!\n");
        return;
    }

    Cart* current = userCart->next;
    Cart* previous = userCart;

    // Traverse the cart and remove all items for the given user
    while (current != NULL) {
        if (strcmp(current->username, username) == 0) {
            previous->next = current->next;
            free(current);
            current = previous->next;
        } else {
            previous = current;
            current = current->next;
        }
    }

    printf("All items have been cleared from %s's cart.\n", username);
}

// Manages the user's store (selling books)
void bookstore(const char* username) {
    int choice;

    while (1) {
        system("cls");
        printf("========= Bookstore =========\n");
        printf("Welcome to the Bookstore, %s!\n\n", username);
        printf("1. My Store\n");
        printf("2. Other Stores\n");
        printf("3. Back to Home\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                mystore(username);  // Call the function for managing the user's own store
                break;
            case 2:
                otherstore(username);  // Call the function to browse other stores
                break;
            case 3:
                return;  // Return to the User Home menu
            default:
                printf("Invalid choice! Please try again.\n");
                pauseForUser();
        }
    }
}

void mystore(const char* username) {
    int choice;

    while (1) {
        system("cls");
        printf("============== My Store ==============\n");
        printf("1. Add Book for Sale\n");
        printf("2. Remove Book from Store\n");
        printf("3. View My Store\n");
        printf("4. Back to Home\n");
        printf("======================================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addOldBook(username);
                break;
            case 2:
                removeOldBook(username);
                break;
            case 3:
                viewStore(username);
                break;
            case 4:
                return;  // Back to Home
            default:
                printf("Invalid choice. Please try again.\n");
                pauseForUser();
                break;
        }
    }
}

void otherstore(const char* username) {
    system("cls");
    int choice;

    printf("Search for a book in:\n");
    printf("1. BookCycle(For new book)\n");
    printf("2. Other Stores(For old book)\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            // Call the function to search for a book in BookCycle
            findBookInBookCycle(username);
            break;
        case 2:
            // Call the function to search for a book in other stores
            findBookInOtherStores(username);
            break;
        default:
            printf("Invalid choice. Please try again.\n");
            pauseForUser();
            otherstore(username);  // Recursively call the function for another try
            break;
    }
}

void findBookInBookCycle(const char* username) {
    // Open the new books file and read it
    FILE* newBooksFile = fopen("newBooks.txt", "r");
    if (newBooksFile == NULL) {
        printf("Error opening new books file.\n");
        pauseForUser();
        return;
    }
    int found = 0;
    char title[100];
    printf("Enter the Book Name: ");
    scanf("%s", title);
    // Read and display new books
    NewBook newBook;
    printf("New Books:\n");

    while (fgets(newBook.title, sizeof(newBook.title), newBooksFile)) {
        newBook.title[strcspn(newBook.title, "\n")] = '\0';  // Remove newline character

        if (!fgets(newBook.author, sizeof(newBook.author), newBooksFile)) break;
        newBook.author[strcspn(newBook.author, "\n")] = '\0';  // Remove newline

        if (fscanf(newBooksFile, "%f\n", &newBook.price) != 1) break;  // Read price

        if (!fgets(newBook.sellerUsername, sizeof(newBook.sellerUsername), newBooksFile)) break;
        newBook.sellerUsername[strcspn(newBook.sellerUsername, "\n")] = '\0';  // Remove newline

        if (!fgets(newBook.buyerUsername, sizeof(newBook.buyerUsername), newBooksFile)) break;
        newBook.buyerUsername[strcspn(newBook.buyerUsername, "\n")] = '\0';  // Remove newline

        if (fscanf(newBooksFile, "%d\n\n", &newBook.isSold) != 1) break;  // Read sold status

        // Check if the seller matches
        if (strcmp(newBook.title, title) == 0 && !newBook.isSold) {
            printf("Title: %s, Author: %s, Price: %.2f, Status: %s\n",
                   newBook.title,
                   newBook.author,
                   newBook.price,
                   newBook.isSold ? "Sold to buyer: " : "Available");
            if (newBook.isSold) {
                printf("%s\n", newBook.buyerUsername);
            }
            found = 1;
            pauseForUser();
            while (1) {
                printf("\n========= Options =========\n");
                printf("1. Purchase the Book\n");
                printf("2. Add to Cart\n");
                printf("3. Back\n");
                printf("Enter your choice: ");
                int choice;
                scanf("%d", &choice);

                switch (choice) {
                    case 1:
                        purchaseBook(username, title);  // Call to purchase the book
                        return;                         // Exit after purchase
                    case 2:
                        addToCart(username, title);  // Call to add the book to the cart
                        printf("Book '%s' added to your cart successfully!\n", title);
                        return;  // Exit after adding to cart
                    case 3:
                        printf("Returning to the previous menu...\n");
                        return;  // Exit to the previous menu
                    default:
                        printf("Invalid choice. Please try again.\n");
                }
            }
            break;
        }
    }
    fclose(newBooksFile);
    if (!found) {
        printf("No books available.\n");
    }

    pauseForUser();
}

void findBookInOtherStores(const char* username) {
    int found = 0;
    // Read and display old books
    OldBook oldBook;
    printf("\nOld Books:\n");
    printf("Enter the Book Name: ");
    char title[100];
    scanf("%s", title);

    while (fgets(oldBook.title, sizeof(oldBook.title), oldBooksFile)) {
        oldBook.title[strcspn(oldBook.title, "\n")] = '\0';  // Remove newline

        if (!fgets(oldBook.author, sizeof(oldBook.author), oldBooksFile)) break;
        oldBook.author[strcspn(oldBook.author, "\n")] = '\0';  // Remove newline

        if (fscanf(oldBooksFile, "%f\n", &oldBook.price) != 1) break;  // Read price

        // Skip the publication date (not used in this case)
        char publicationDate[20];
        fgets(publicationDate, sizeof(publicationDate), oldBooksFile);

        // Read condition attributes
        if (fscanf(oldBooksFile, "%d\n%d\n", &oldBook.hasMarks, &oldBook.isTorn) != 2)
            break;

        if (!fgets(oldBook.sellerUsername, sizeof(oldBook.sellerUsername), oldBooksFile)) break;
        oldBook.sellerUsername[strcspn(oldBook.sellerUsername, "\n")] = '\0';  // Remove newline

        if (!fgets(oldBook.buyerUsername, sizeof(oldBook.buyerUsername), oldBooksFile)) break;
        oldBook.buyerUsername[strcspn(oldBook.buyerUsername, "\n")] = '\0';  // Remove newline

        if (fscanf(oldBooksFile, "%d\n\n", &oldBook.isSold) != 1) break;  // Read sold status

        // Check if the seller matches
        if (strcmp(oldBook.title, title) == 0 && !oldBook.isSold) {
            printf("Title: %s, Author: %s, Price: %.2f, Status: %s, Has Marks: %s, Is Torn: %s\n",
                   oldBook.title,
                   oldBook.author,
                   oldBook.price,
                   oldBook.hasMarks ? "Yes" : "NO",
                   oldBook.isTorn ? "Yes: " : "NO",
                   oldBook.isSold ? "Sold to buyer: " : "Available");
            if (oldBook.isSold) {
                printf("%s\n", oldBook.buyerUsername);
            }
            while (1) {
                printf("\n========= Options =========\n");
                printf("1. Purchase the Book\n");
                printf("2. Add to Cart\n");
                printf("3. Back\n");
                printf("Enter your choice: ");
                int choice;
                scanf("%d", &choice);

                switch (choice) {
                    case 1:
                        purchaseBook(username, title);  // Call to purchase the book
                        return;                         // Exit after purchase
                    case 2:
                        addToCart(username, title);  // Call to add the book to the cart
                        printf("Book '%s' added to your cart successfully!\n", title);
                        return;  // Exit after adding to cart
                    case 3:
                        printf("Returning to the previous menu...\n");
                        return;  // Exit to the previous menu
                    default:
                        printf("Invalid choice. Please try again.\n");
                }
            }
            found = 1;
        }
    }

    fclose(oldBooksFile);

    // If no books were found for this seller
    if (!found) {
        printf("No books available\n");
    }

    pauseForUser();
}

void purchaseBook(const char* username, const char* title) {
    FILE* historyFile = fopen("history.txt", "a+");  // Open for both reading and appending
    if (historyFile == NULL) {
        printf("Error opening history file.\n");
        pauseForUser();
        return;
    }
    History history;
    FILE* userFile = fopen("users.txt", "r+");
    int found = 0;

    // Open the new books file and read it
    FILE* newBooksFile = fopen("newBooks.txt", "r+");
    if (newBooksFile == NULL) {
        printf("Error opening new books file.\n");
        pauseForUser();
        return;
    }

    // Read and display new books
    NewBook newBook;

    while (fgets(newBook.title, sizeof(newBook.title), newBooksFile)) {
        newBook.title[strcspn(newBook.title, "\n")] = '\0';  // Remove newline character

        if (!fgets(newBook.author, sizeof(newBook.author), newBooksFile)) break;
        newBook.author[strcspn(newBook.author, "\n")] = '\0';  // Remove newline

        if (fscanf(newBooksFile, "%f\n", &newBook.price) != 1) break;  // Read price

        if (!fgets(newBook.sellerUsername, sizeof(newBook.sellerUsername), newBooksFile)) break;
        newBook.sellerUsername[strcspn(newBook.sellerUsername, "\n")] = '\0';  // Remove newline

        if (!fgets(newBook.buyerUsername, sizeof(newBook.buyerUsername), newBooksFile)) break;
        newBook.buyerUsername[strcspn(newBook.buyerUsername, "\n")] = '\0';  // Remove newline

        if (fscanf(newBooksFile, "%d\n\n", &newBook.isSold) != 1) break;  // Read sold status

        // Check if the seller matches
        if (strcmp(newBook.title, title) == 0 && !newBook.isSold) {
            printf("The Book \"%s\" is Bought Successfully\n", newBook.title);
            newBook.isSold = 1;
            strcpy(newBook.buyerUsername, username);
            removeFromCart(username, newBook.title);

            // Write to history file
            strcpy(history.buyerUsername, username);
            strcpy(history.sellerUsername, newBook.sellerUsername);
            strcpy(history.title, newBook.title);
            history.amount = newBook.price;

            // Write the history record to the file
            fprintf(historyFile, "%s %s %s %.2f\n", history.buyerUsername, history.sellerUsername, history.title, history.amount);

            found = 1;
            pauseForUser();
        }
    }

    fclose(newBooksFile);

    if (found) {
        fclose(historyFile);
        return;
    }

    // Open the old books file and read it
    FILE* oldBooksFile = fopen("oldBooks.txt", "r+");
    if (oldBooksFile == NULL) {
        printf("Error opening old books file.\n");
        pauseForUser();
        return;
    }

    // Read and display old books
    OldBook oldBook;

    while (fgets(oldBook.title, sizeof(oldBook.title), oldBooksFile)) {
        oldBook.title[strcspn(oldBook.title, "\n")] = '\0';  // Remove newline

        if (!fgets(oldBook.author, sizeof(oldBook.author), oldBooksFile)) break;
        oldBook.author[strcspn(oldBook.author, "\n")] = '\0';  // Remove newline

        if (fscanf(oldBooksFile, "%f\n", &oldBook.price) != 1) break;  // Read price

        // Skip the publication date (not used in this case)
        char publicationDate[20];
        fgets(publicationDate, sizeof(publicationDate), oldBooksFile);

        // Read condition attributes
        if (fscanf(oldBooksFile, "%d\n%d\n", &oldBook.hasMarks, &oldBook.isTorn) != 2)
            break;

        if (!fgets(oldBook.sellerUsername, sizeof(oldBook.sellerUsername), oldBooksFile)) break;
        oldBook.sellerUsername[strcspn(oldBook.sellerUsername, "\n")] = '\0';  // Remove newline

        if (!fgets(oldBook.buyerUsername, sizeof(oldBook.buyerUsername), oldBooksFile)) break;
        oldBook.buyerUsername[strcspn(oldBook.buyerUsername, "\n")] = '\0';  // Remove newline

        if (fscanf(oldBooksFile, "%d\n\n", &oldBook.isSold) != 1) break;  // Read sold status

        // Check if the seller matches
        if (strcmp(oldBook.title, title) == 0 && !oldBook.isSold) {
            printf("The Book \"%s\" is Bought Successfully\n", oldBook.title);
            oldBook.isSold = 1;
            strcpy(oldBook.buyerUsername, username);
            removeFromCart(username, oldBook.title);

            // Write to history file
            strcpy(history.buyerUsername, username);
            strcpy(history.sellerUsername, oldBook.sellerUsername);
            strcpy(history.title, oldBook.title);
            history.amount = oldBook.price;

            // Write the history record to the file
            fprintf(historyFile, "%s %s %s %.2f\n", history.buyerUsername, history.sellerUsername, history.title, history.amount);

            found = 1;
            pauseForUser();
        }
    }

    fclose(oldBooksFile);
    fclose(historyFile);

    // If no books were found for this seller
    if (!found) {
        printf("No books available\n");
    }

    pauseForUser();
}

// Display information about the BookCycle platform and administrators
void aboutUs() {
    system("cls");
    printf("About BookCycle\n");
    printf("-------------------------------------------------------------\n");
    printf("BookCycle: E-commerce Management System\n\n");

    printf("BookCycle is an online platform designed for buying and \n");
    printf("selling pre-owned books at reasonable prices.\n");
    printf("Users can sign up, log in, and either browse books by \n");
    printf("category or search by name to find their next read.\n");
    printf("Sellers can list books by providing details like name, \n");
    printf("category, condition, and price.\n");
    printf("Buyers can add books to their cart for easy purchasing. \n");
    printf("The platform is overseen by an admin who ensures a safe \n");
    printf("and fair marketplace by monitoring activity and removing \n");
    printf("fraudulent users. BookCycle promotes sustainability by \n");
    printf("giving books a second life while connecting readers.\n\n");

    printf("Administrators:\n");
    printf("-------------------------------------------------------------\n");
    printf("1. Juhair Akhter, Project Administrator.\n");
    printf("2. Sakibul Hasan Sakib, Web Designer and Editor.\n");
    printf("3. Kushol Ranjan Sarker, Software Designer.\n");
    printf("4. Abdullah Mohammad Huzaifa, Database Administrator.\n");
    printf("5. MST. Kamrunnahar Mukty, Quality Assurance Lead.\n");

    printf("-------------------------------------------------------------\n");

    pauseForUser();  // Pause for the user to acknowledge the information
}

// Exits the program
void exitSystem() {
    printf("Exiting System\n");
}
