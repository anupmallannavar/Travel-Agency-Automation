#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <queue>
#include <climits>

using namespace std;

// ==== Global Data Structures ====
const int MAX_LOCATIONS = 6;
int graph[MAX_LOCATIONS][MAX_LOCATIONS] = {
    {0, 4, 0, 0, 0, 10},
    {4, 0, 8, 0, 0, 0},
    {0, 8, 0, 7, 0, 4},
    {0, 0, 7, 0, 9, 14},
    {0, 0, 0, 9, 0, 10},
    {10, 0, 4, 14, 10, 0}
};

string locations[MAX_LOCATIONS] = {
    "Bus Stand", "Railway Station", "Mall", "Airport", "Hotel", "Temple"
};

// Driver Structure
struct Driver {
    string name;
    string vehicle;
};

// ==== Utility Functions ====

bool userExists(const string &username) {
    ifstream file("users.txt");
    string u, p;
    while (file >> u >> p) {
        if (u == username) return true;
    }
    return false;
}

void signUp() {
    string username, password;
    cout << "\n=== Sign Up ===\n";
    cout << "Enter a new username: ";
    cin >> username;

    if (userExists(username)) {
        cout << "Username already exists. Try a different one.\n";
        return;
    }

    cout << "Enter password: ";
    cin >> password;

    ofstream file("users.txt", ios::app);
    file << username << " " << password << "\n";
    cout << "Signup successful! Please log in now.\n";
}

bool login() {
    string username, password;
    cout << "\n=== Login ===\n";
    cout << "Username: ";
    cin >> username;
    cout << "Password: ";
    cin >> password;

    ifstream file("users.txt");
    string u, p;
    while (file >> u >> p) {
        if (u == username && p == password) {
            cout << "Login successful! Welcome, " << username << "!\n";
            return true;
        }
    }

    cout << "Login failed. Incorrect username or password.\n";
    return false;
}

queue<Driver> generateDrivers() {
    queue<Driver> q;
    q.push({"Arjun", "KA01AB1234"});
    q.push({"Vikram", "KA01BC2345"});
    q.push({"Ravi", "KA01CD3456"});
    q.push({"Sunil", "KA01DE4567"});
    q.push({"Manoj", "KA01EF5678"});
    return q;
}

int dijkstra(int start, int end) {
    vector<int> dist(MAX_LOCATIONS, INT_MAX);
    dist[start] = 0;
    vector<bool> visited(MAX_LOCATIONS, false);

    for (int i = 0; i < MAX_LOCATIONS; i++) {
        int u = -1;
        for (int j = 0; j < MAX_LOCATIONS; j++) {
            if (!visited[j] && (u == -1 || dist[j] < dist[u])) {
                u = j;
            }
        }

        if (dist[u] == INT_MAX) break;

        visited[u] = true;
        for (int v = 0; v < MAX_LOCATIONS; v++) {
            if (graph[u][v] && dist[u] + graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph[u][v];
            }
        }
    }

    return dist[end];
}

void tripFlow() {
    cout << "\nAvailable Locations in the City:\n";
    for (int i = 0; i < MAX_LOCATIONS; i++) {
        cout << i << ". " << locations[i] << endl;
    }

    int from, to;
    cout << "\nEnter FROM location number: ";
    cin >> from;
    cout << "Enter TO location number: ";
    cin >> to;

    if (from < 0 || from >= MAX_LOCATIONS || to < 0 || to >= MAX_LOCATIONS) {
        cout << "Invalid location selection!\n";
        return;
    }

    int distance = dijkstra(from, to);
    cout << "\nShortest Distance from " << locations[from] << " to " << locations[to] << " is: " << distance << " km\n";

    int fare = distance * 15;
    cout << "Your fare will be ₹" << fare << endl;

    queue<Driver> drivers = generateDrivers();
    Driver assigned = drivers.front();
    drivers.pop();

    cout << "\nDriver Assigned:\n";
    cout << "Name: " << assigned.name << "\nVehicle No: " << assigned.vehicle << endl;
    cout << "Estimated arrival time: 15 mins\n";

    int rating;
    cout << "\nPlease rate your trip (1 to 5): ";
    cin >> rating;
    if (rating < 1 || rating > 5) {
        cout << "Invalid rating.\n";
    } else {
        cout << "Thank you for rating us " << rating << " stars!\n";
    }
}

void displayStatesAndCities(map<string, vector<string>> &states) {
    int stateChoice;
    cout << "\nAvailable States:\n";
    int idx = 1;
    for (auto &pair : states) {
        cout << idx++ << ". " << pair.first << endl;
    }

    cout << "\nEnter the number corresponding to your chosen state: ";
    cin >> stateChoice;

    if (stateChoice < 1 || stateChoice > states.size()) {
        cout << "Invalid state selection!\n";
        return;
    }

    auto it = states.begin();
    advance(it, stateChoice - 1);
    string chosenState = it->first;

    cout << "\nHighlighted Cities in " << chosenState << ":\n";
    vector<string> cities = it->second;

    for (int i = 0; i < cities.size(); i++) {
        cout << i + 1 << ". " << cities[i] << endl;
    }

    int cityChoice;
    cout << "\nEnter the number of the city you want to travel in: ";
    cin >> cityChoice;

    if (cityChoice < 1 || cityChoice > cities.size()) {
        cout << "Invalid city selection!\n";
        return;
    }

    cout << "\nYou have selected: " << cities[cityChoice - 1] << " in " << chosenState << endl;

    tripFlow(); // Proceed to trip details
}

void mainMenu() {
    map<string, vector<string>> states = {
        {"Karnataka", {"Bangalore", "Mysore", "Mangalore", "Hubli", "Belgaum", "Shimoga", "Davangere", "Udupi", "Bijapur", "Bellary"}},
        {"Maharashtra", {"Mumbai", "Pune", "Nagpur", "Nashik", "Aurangabad", "Solapur", "Thane", "Amravati", "Kolhapur", "Sangli"}},
        {"Tamil Nadu", {"Chennai", "Coimbatore", "Madurai", "Tiruchirappalli", "Salem", "Erode", "Tirunelveli", "Vellore", "Thoothukudi", "Dindigul"}},
        {"Kerala", {"Thiruvananthapuram", "Kochi", "Kozhikode", "Thrissur", "Kollam", "Alappuzha", "Palakkad", "Kottayam", "Kannur", "Malappuram"}},
        {"Delhi", {"New Delhi", "Dwarka", "Rohini", "Saket", "Lajpat Nagar", "Karol Bagh", "Connaught Place", "Rajouri Garden", "Mayur Vihar", "Vasant Kunj"}},
        {"Uttar Pradesh", {"Lucknow", "Kanpur", "Varanasi", "Agra", "Meerut", "Prayagraj", "Ghaziabad", "Noida", "Aligarh", "Jhansi"}},
        {"Gujarat", {"Ahmedabad", "Surat", "Vadodara", "Rajkot", "Bhavnagar", "Jamnagar", "Gandhinagar", "Anand", "Navsari", "Mehsana"}},
        {"Rajasthan", {"Jaipur", "Jodhpur", "Udaipur", "Ajmer", "Bikaner", "Kota", "Alwar", "Bharatpur", "Sikar", "Pali"}},
        {"West Bengal", {"Kolkata", "Howrah", "Durgapur", "Asansol", "Siliguri", "Malda", "Bardhaman", "Haldia", "Kharagpur", "Krishnanagar"}},
        {"Punjab", {"Ludhiana", "Amritsar", "Jalandhar", "Patiala", "Bathinda", "Mohali", "Hoshiarpur", "Pathankot", "Moga", "Barnala"}}
    };

    int choice;
    while (true) {
        cout << "\n====== Travel Agency Automation ======\n";
        cout << "1. Login\n";
        cout << "2. Sign Up\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) {
            if (login()) {
                displayStatesAndCities(states);
                break;
            }
        } else if (choice == 2) {
            signUp();
        } else if (choice == 3) {
            cout << "Exiting the program. Goodbye!\n";
            break;
        } else {
            cout << "Invalid choice. Try again.\n";
        }
    }
}

int main() {
    mainMenu();
    return 0;
}

