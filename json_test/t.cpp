#include <iostream>
#include <fstream>
#include "json.hpp"
using json = nlohmann::json;
using namespace std;

// Function to load JSON data from a file
json loadJsonData(const string& filename){
    ifstream file(filename);
    json data;

    // If file exists, read its contents
    if(file)
        file >> data;
    file.close();
    return data;
}

// Function to save JSON data to a file
void saveJsonData(const string& filename, const json& data){
    ofstream file(filename);
    file << data.dump(4); // Write JSON data with indentation
    file.close();
}

// Function for sign-up
void signUp(const string& filename) {
    json data = loadJsonData(filename);

    string username, password;
    cout << "Sign Up\n";
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    // Check if the username already exists
    if (data.contains(username)) {
        cout << "Error: Username already exists! Try a different username." << endl;
        return;
    }

    // Add new user to JSON data
    data[username] = {{"password", password}};
    saveJsonData(filename, data);

    cout << "Sign-up successful!" << endl;
}

// Function for login
void login(const string& filename) {
    json data = loadJsonData(filename);

    string username, password;
    cout << "Login\n";
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    // Check if the username exists and the password matches
    if (data.contains(username) && data[username]["password"] == password) {
        cout << "Login successful! Welcome, " << username << "!" << endl;
    } else {
        cout << "Login failed! Invalid username or password." << endl;
    }
}

int main() {
    const string filename = "data.json"; // File to store user data
    int choice;

    while (true) {
        cout << "\n1. Sign Up\n2. Login\n3. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                signUp(filename);
                break;
            case 2:
                login(filename);
                break;
            case 3:
                cout << "Exiting the program. Goodbye!" << endl;
                return 0;
            default:
                cout << "Invalid choice! Please try again." << endl;
        }
    }
    return 0;
}