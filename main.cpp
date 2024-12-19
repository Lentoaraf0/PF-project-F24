#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <iomanip> // For formatting output

using namespace std;

// Structure for a Question
struct Question {
    string questionText;
    vector<string> options;
    char correctOption; // Correct answer (e.g., 'A', 'B', 'C', 'D')
};

// Structure for a User
struct User {
    string username;
    string password;
    string role; // "admin" or "student"
};

// Global variables
vector<User> users = {
    {"admin", "admin123", "admin"},
    {"student1", "student123", "student"},
    {"student2", "password456", "student"}
};
map<string, vector<Question>> exams;
map<string, map<string, int>> results; // results[exam][student] = score

// Function prototypes
void showMenu();
void adminPanel();
void studentPanel();
void createExam();
void takeExam(const string& username);
void viewResults(const string& username);
void login();

// Utility function for user authentication
User* authenticate(const string& username, const string& password) {
    for (auto& user : users) {
        if (user.username == username && user.password == password) {
            return &user;
        }
    }
    return nullptr;
}

// Main function
int main() {
    showMenu();
    return 0;
}

void showMenu() {
    int choice;
    while (true) {
        cout << "\n=== Online Examination System ===\n";
        cout << "1. Login\n";
        cout << "2. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            login();
            break;
        case 2:
            cout << "Exiting the system. Goodbye!\n";
            return;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    }
}

void login() {
    string username, password;
    cout << "\nEnter Username: ";
    cin >> username;
    cout << "Enter Password: ";
    cin >> password;

    User* user = authenticate(username, password);
    if (user) {
        if (user->role == "admin") {
            adminPanel();
        }
        else if (user->role == "student") {
            studentPanel();
        }
    }
    else {
        cout << "Invalid credentials. Please try again.\n";
    }
}

void adminPanel() {
    int choice;
    while (true) {
        cout << "\n=== Admin Panel ===\n";
        cout << "1. Create Exam\n";
        cout << "2. View Results\n";
        cout << "3. Logout\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            createExam();
            break;
        case 2:
            cout << "\n=== Results ===\n";
            for (const auto& exam : results) {
                cout << "Exam: " << exam.first << "\n";
                for (const auto& student : exam.second) {
                    cout << "  Student: " << student.first << " | Score: " << student.second << "\n";
                }
            }
            break;
        case 3:
            return;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    }
}

void createExam() {
    string examName;
    int numQuestions;
    cout << "\nEnter Exam Name: ";
    cin >> examName;

    if (exams.count(examName)) {
        cout << "Exam already exists.\n";
        return;
    }

    cout << "Enter number of questions: ";
    cin >> numQuestions;

    vector<Question> questions;
    for (int i = 0; i < numQuestions; i++) {
        Question q;
        cout << "\nEnter question " << i + 1 << ": ";
        cin.ignore();
        getline(cin, q.questionText);

        cout << "Enter 4 options (A, B, C, D):\n";
        for (int j = 0; j < 4; j++) {
            string option;
            cout << "  Option " << char('A' + j) << ": ";
            getline(cin, option);
            q.options.push_back(option);
        }

        cout << "Enter correct option (A, B, C, D): ";
        cin >> q.correctOption;
        questions.push_back(q);
    }

    exams[examName] = questions;
    cout << "Exam created successfully!\n";
}

void studentPanel() {
    string username;
    cout << "\nEnter your username: ";
    cin >> username;

    int choice;
    while (true) {
        cout << "\n=== Student Panel ===\n";
        cout << "1. Take Exam\n";
        cout << "2. View Results\n";
        cout << "3. Logout\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            takeExam(username);
            break;
        case 2:
            viewResults(username);
            break;
        case 3:
            return;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    }
}

void takeExam(const string& username) {
    string examName;
    cout << "\nAvailable Exams:\n";
    for (const auto& exam : exams) {
        cout << "  - " << exam.first << "\n";
    }

    cout << "Enter exam name to take: ";
    cin >> examName;

    if (!exams.count(examName)) {
        cout << "Exam does not exist.\n";
        return;
    }

    const vector<Question>& questions = exams[examName];
    int score = 0;

    for (const auto& q : questions) {
        cout << "\n" << q.questionText << "\n";
        for (size_t i = 0; i < q.options.size(); i++) {
            cout << "  " << char('A' + i) << ". " << q.options[i] << "\n";
        }
        char answer;
        cout << "Enter your answer (A, B, C, D): ";
        cin >> answer;

        if (toupper(answer) == toupper(q.correctOption)) {
            score++;
        }
    }

    results[examName][username] = score;
    cout << "\nExam completed! Your score: " << score << " / " << questions.size() << "\n";
}

void viewResults(const string& username) {
    cout << "\n=== Your Results ===\n";
    for (const auto& exam : results) {
        if (exam.second.count(username)) {
            cout << "Exam: " << exam.first << " | Score: " << exam.second.at(username) << "\n";
        }
    }
}
