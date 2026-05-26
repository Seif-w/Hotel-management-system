#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <string>

using namespace std;

const int EMERGENCY = 3;
const int VIP = 2;
const int REGULAR = 1;

// ==================== RESERVATION SYSTEM ====================

class Reservation {
public:
    int guestID;
    string guestName;
    Reservation(int gID, string name, string date, int rID) : guestID(gID), guestName(name) {}
    void display() const { cout << "Guest ID: " << guestID << ", Name: " << guestName << endl; }
};

void swapReservation(Reservation& a, Reservation& b) { Reservation temp = a; a = b; b = temp; }

int partition(vector<Reservation>& arr, int low, int high) {
    int pivot = arr[high].guestID;
    int i = (low - 1);
    for (int j = low; j < high; j++) {
        if (arr[j].guestID < pivot) {
            i++;
            swapReservation(arr[i], arr[j]);
        }
    }
    swapReservation(arr[i + 1], arr[high]);
    return (i + 1);
}

void quickSort(vector<Reservation>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

void sortReservations(vector<Reservation>& reservationsArray) {
    if (reservationsArray.empty()) return;
    quickSort(reservationsArray, 0, reservationsArray.size() - 1);
}

int binarySearch(const vector<Reservation>& arr, int targetGuestID) {
    int low = 0, high = arr.size() - 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (arr[mid].guestID == targetGuestID) return mid;
        if (arr[mid].guestID < targetGuestID) low = mid + 1;
        else high = mid - 1;
    }
    return -1;
}

void findReservation(vector<Reservation>& reservations, int guestID) {
    if (reservations.empty()) { cout << "No reservations available!" << endl; return; }
    sortReservations(reservations);
    int index = binarySearch(reservations, guestID);
    if (index != -1) { cout << "Reservation FOUND: "; reservations[index].display(); }
    else { cout << "Reservation with Guest ID " << guestID << " NOT FOUND!" << endl; }
}

void displayAllReservations(const vector<Reservation>& reservations) {
    if (reservations.empty()) { cout << "\nNo reservations in the system!" << endl; return; }
    cout << "\n=== ALL RESERVATIONS (" << reservations.size() << " total) ===" << endl;
    for (size_t i = 0; i < reservations.size(); i++) { cout << "[" << i << "] "; reservations[i].display(); }
}

void addReservation(vector<Reservation>& reservations, int& nextReservationID, int guestID, string name, string date) {
    reservations.push_back(Reservation(guestID, name, date, nextReservationID));
    nextReservationID++;
}

// ==================== ROOM INVENTORY ====================

struct Room {
    int roomID;
    string type;
    double pricePerNight;
    bool isAvailable;
    void display() const { cout << "Room ID: " << roomID << " | Type: " << type << " | Price: $" << pricePerNight << " | Status: " << (isAvailable ? "Available" : "Occupied") << endl; }
};

void addRoom(vector<Room>& rooms, Room newRoom) {
    rooms.push_back(newRoom);
    int i = rooms.size() - 2;
    while (i >= 0 && rooms[i].roomID > newRoom.roomID) {
        rooms[i + 1] = rooms[i];
        i--;
    }
    rooms[i + 1] = newRoom;
}

int findRoomByID(const vector<Room>& rooms, int targetID) {
    int low = 0, high = rooms.size() - 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (rooms[mid].roomID == targetID) return mid;
        else if (rooms[mid].roomID < targetID) low = mid + 1;
        else high = mid - 1;
    }
    return -1;
}

void sortByPrice(vector<Room>& rooms) {
    for (int i = 1; i < (int)rooms.size(); i++) {
        Room key = rooms[i];
        int j = i - 1;
        while (j >= 0 && rooms[j].pricePerNight > key.pricePerNight) {
            rooms[j + 1] = rooms[j];
            j--;
        }
        rooms[j + 1] = key;
    }
}

int lowerBound(const vector<Room>& rooms, double minPrice) {
    int low = 0, high = rooms.size();
    while (low < high) {
        int mid = low + (high - low) / 2;
        if (rooms[mid].pricePerNight < minPrice) low = mid + 1;
        else high = mid;
    }
    return low;
}

int upperBound(const vector<Room>& rooms, double maxPrice) {
    int low = 0, high = rooms.size();
    while (low < high) {
        int mid = low + (high - low) / 2;
        if (rooms[mid].pricePerNight <= maxPrice) low = mid + 1;
        else high = mid;
    }
    return low;
}

vector<Room> findRoomsByPriceRange(const vector<Room>& rooms, double minPrice, double maxPrice) {
    vector<Room> result;
    if (rooms.empty()) { cout << "No rooms available!" << endl; return result; }
    vector<Room> sortedByPrice = rooms;
    sortByPrice(sortedByPrice);
    int start = lowerBound(sortedByPrice, minPrice);
    int end = upperBound(sortedByPrice, maxPrice);
    for (int i = start; i < end; i++) { result.push_back(sortedByPrice[i]); }
    return result;
}

void displayAllRooms(const vector<Room>& rooms) {
    if (rooms.empty()) { cout << "No rooms in system!" << endl; return; }
    cout << "\n=== ALL ROOMS (" << rooms.size() << ") ===" << endl;
    for (const auto& r : rooms) r.display();
}

void showAvailableRooms(const vector<Room>& rooms) {
    cout << "\n=== AVAILABLE ROOMS ===\n";
    for (const auto& r : rooms) {
        if (r.isAvailable) {
            cout << "Room " << r.roomID << " | " << r.type << " | $" << r.pricePerNight << "/night\n";
        }
    }
}

// ==================== GUEST PRIORITY QUEUE ====================

struct PriorityNode {
    string guestName;
    string requestDesc;
    int priority;
    PriorityNode* next;
    PriorityNode(const string& name, const string& desc, int prio) : guestName(name), requestDesc(desc), priority(prio), next(nullptr) {}
};

string priorityLabel(int p) { if (p == EMERGENCY) return "EMERGENCY"; if (p == VIP) return "VIP"; return "REGULAR"; }

class GuestPriorityQueue {
private:
    PriorityNode* head;
    int size;
public:
    GuestPriorityQueue() : head(nullptr), size(0) {}
    ~GuestPriorityQueue() { while (head) { PriorityNode* temp = head; head = head->next; delete temp; } }
    void enqueueRequest(const string& name, const string& desc, int priority) {
        PriorityNode* newNode = new PriorityNode(name, desc, priority);
        if (!head || priority > head->priority) { newNode->next = head; head = newNode; }
        else {
            PriorityNode* current = head;
            while (current->next && current->next->priority >= priority) { current = current->next; }
            newNode->next = current->next;
            current->next = newNode;
        }
        size++;
        cout << "Request added: " << priorityLabel(priority) << " | Guest: " << name << " | " << desc << endl;
    }
    bool dequeueRequest() {
        if (!head) { cout << "No requests pending.\n"; return false; }
        PriorityNode* toRemove = head;
        head = head->next;
        size--;
        cout << "Processing: " << priorityLabel(toRemove->priority) << " | Guest: " << toRemove->guestName << " | " << toRemove->requestDesc << endl;
        delete toRemove;
        return true;
    }
    void displayQueue() const {
        if (!head) { cout << "No pending requests.\n"; return; }
        cout << "\n=== PENDING REQUESTS (Priority Order) ===\n";
        PriorityNode* current = head;
        int pos = 1;
        while (current) {
            cout << pos++ << ". [" << priorityLabel(current->priority) << "] " << current->guestName << " - " << current->requestDesc << endl;
            current = current->next;
        }
    }
    bool isEmpty() const { return head == nullptr; }
};

// ==================== RESTAURANT BOOKINGS ====================

struct Booking {
    int id;
    string name;
    string time;
};

void merge(Booking arr[], int left, int mid, int right, Booking temp[]) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    for (int i = 0; i < n1; i++) temp[i] = arr[left + i];
    for (int j = 0; j < n2; j++) temp[n1 + j] = arr[mid + 1 + j];
    int i = 0, j = n1, k = left;
    while (i < n1 && j < n1 + n2) {
        if (temp[i].id <= temp[j].id) { arr[k] = temp[i]; i++; }
        else { arr[k] = temp[j]; j++; }
        k++;
    }
    while (i < n1) { arr[k] = temp[i]; i++; k++; }
    while (j < n1 + n2) { arr[k] = temp[j]; j++; k++; }
}

void mergeSort(Booking arr[], int left, int right, Booking temp[]) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid, temp);
        mergeSort(arr, mid + 1, right, temp);
        merge(arr, left, mid, right, temp);
    }
}

void sortBookings(Booking arr[], int size) { Booking* temp = new Booking[size]; mergeSort(arr, 0, size - 1, temp); delete[] temp; }

int binarySearchBooking(Booking arr[], int size, int target) {
    int left = 0, right = size - 1;
    while (left <= right) { int mid = (left + right) / 2; if (arr[mid].id == target) return mid; else if (arr[mid].id < target) left = mid + 1; else right = mid - 1; }
    return -1;
}

int findBooking(Booking arr[], int size, int bookingID) { return binarySearchBooking(arr, size, bookingID); }

void addBooking(Booking*& arr, int& size, Booking newBooking) {
    Booking* newArr = new Booking[size + 1];
    for (int i = 0; i < size; i++) newArr[i] = arr[i];
    newArr[size] = newBooking;
    delete[] arr;
    arr = newArr;
    size++;
    sortBookings(arr, size);
}

void printBookings(Booking arr[], int size) {
    if (size == 0) { cout << "No restaurant bookings.\n"; return; }
    cout << "\n=== RESTAURANT BOOKINGS (Sorted by ID) ===\n";
    for (int i = 0; i < size; i++) { cout << "ID: " << arr[i].id << " | " << arr[i].name << " | " << arr[i].time << endl; }
}

// ==================== UNDO/REDO ====================

stack<string> undoStack;
stack<string> redoStack;
string currentState = "No guest selected";
int currentGuestID = -1;
string currentGuestName = "";
int currentGuestPriority = REGULAR;

void applyChange(string newState) {
    undoStack.push(currentState);
    currentState = newState;
    while (!redoStack.empty()) redoStack.pop();
    cout << "Change Applied: " << currentState << endl;
}

void undoChange() {
    if (undoStack.empty()) { cout << "Nothing to undo." << endl; return; }
    redoStack.push(currentState);
    currentState = undoStack.top();
    undoStack.pop();
    cout << "Undo Performed. Current State: " << currentState << endl;
}

void redoChange() {
    if (redoStack.empty()) { cout << "Nothing to redo." << endl; return; }
    undoStack.push(currentState);
    currentState = redoStack.top();
    redoStack.pop();
    cout << "Redo Performed. Current State: " << currentState << endl;
}

void showCurrentState() {
    if (currentGuestID == -1) {
        cout << "No guest selected.\n";
    }
    else {
        cout << "Current guest: " << currentGuestName << " (ID: " << currentGuestID << ") | Priority: " << priorityLabel(currentGuestPriority) << endl;
    }
}

void setCurrentGuest(int id, string name, int priority) {
    currentGuestID = id;
    currentGuestName = name;
    currentGuestPriority = priority;
    currentState = name + " (ID: " + to_string(id) + ") | Priority: " + priorityLabel(priority);
}

// ==================== RECEPTION QUEUE ====================

int guestCounter = 0;

struct QueueGuest {
    int id;
    string name;
};

queue<QueueGuest> receptionQueue;

void enqueueGuest(string name) {
    guestCounter++;
    QueueGuest g;
    g.id = guestCounter;
    g.name = name;
    receptionQueue.push(g);
    cout << "Guest " << name << " added to reception queue. Position: " << receptionQueue.size() << endl;
}

void dequeueGuest() {
    if (!receptionQueue.empty()) {
        QueueGuest g = receptionQueue.front();
        cout << "\nNow serving: " << g.name << " (ID: " << g.id << ")" << endl;
        receptionQueue.pop();
        cout << "Remaining in queue: " << receptionQueue.size() << endl;
    }
    else { cout << "Queue is empty!\n"; }
}

void displayReceptionQueue() {
    if (receptionQueue.empty()) {
        cout << "Reception queue is empty.\n";
        return;
    }
    queue<QueueGuest> temp = receptionQueue;
    cout << "\n=== RECEPTION QUEUE (FIFO Order) ===\n";
    int pos = 1;
    while (!temp.empty()) { QueueGuest g = temp.front(); cout << pos++ << ". " << g.name << " (ID: " << g.id << ")" << endl; temp.pop(); }
}

// ==================== REPORTS ====================

struct Report {
    int day;
    double occ, sat, rev;
    Report* next;
};

class ReportList {
private:
    Report* head;
public:
    ReportList() : head(nullptr) {
        addReportEntry(5, 85.5, 4.5, 12000);
        addReportEntry(3, 78.0, 4.2, 9800);
        addReportEntry(7, 92.0, 4.8, 15000);
        addReportEntry(1, 65.0, 3.8, 7500);
        addReportEntry(4, 82.0, 4.4, 11000);
        insertionSort();
    }
    ~ReportList() { while (head) { Report* temp = head; head = head->next; delete temp; } }
    void addReportEntry(int day, double occ, double sat, double rev) {
        Report* r = new Report{ day, occ, sat, rev, nullptr };
        if (!head) head = r;
        else { Report* t = head; while (t->next) t = t->next; t->next = r; }
    }
    void insertionSort() {
        Report* sorted = nullptr;
        Report* curr = head;
        while (curr) {
            Report* next = curr->next;
            if (!sorted || curr->day < sorted->day) { curr->next = sorted; sorted = curr; }
            else { Report* t = sorted; while (t->next && t->next->day < curr->day) t = t->next; curr->next = t->next; t->next = curr; }
            curr = next;
        }
        head = sorted;
    }
    void generateKPIs() {
        if (!head) return;
        double occ = 0, sat = 0, rev = 0;
        int c = 0;
        for (Report* t = head; t; t = t->next) { occ += t->occ; sat += t->sat; rev += t->rev; c++; }
        cout << "\n===== KPI REPORT =====" << endl;
        cout << "Avg Occupancy: " << occ / c << "%" << endl;
        cout << "Avg Satisfaction: " << sat / c << "/10" << endl;
        cout << "Avg Revenue: $" << rev / c << endl;
    }
};

// ==================== GUEST STRUCTURE WITH PRIORITY ====================

struct GuestInfo {
    int id;
    string name;
    int priority;
    int roomID;
    int nights;
    double restaurantCost;
    double roomCost;

    GuestInfo(int i, string n, int p) : id(i), name(n), priority(p), roomID(0), nights(0), restaurantCost(0), roomCost(0) {}
};

vector<GuestInfo> allGuests;
int nextGuestID = 1;

// ==================== RESTAURANT PRIORITY QUEUE ====================

struct ComparePriority {
    bool operator()(const GuestInfo& a, const GuestInfo& b) {
        return a.priority < b.priority;
    }
};

priority_queue<GuestInfo, vector<GuestInfo>, ComparePriority> restaurantPriorityQueue;

void addToRestaurantQueue(int guestID, double cost, string time) {
    for (auto& g : allGuests) {
        if (g.id == guestID) {
            g.restaurantCost = cost;
            restaurantPriorityQueue.push(g);
            string pName = priorityLabel(g.priority);
            cout << g.name << " added to restaurant queue with " << pName << " priority at " << time << endl;
            return;
        }
    }
}

void showRestaurantPriorityQueue() {
    if (restaurantPriorityQueue.empty()) {
        cout << "Restaurant queue is empty.\n";
        return;
    }
    priority_queue<GuestInfo, vector<GuestInfo>, ComparePriority> temp = restaurantPriorityQueue;
    cout << "\n=== RESTAURANT QUEUE (Priority Order: Emergency > VIP > Regular) ===\n";
    int pos = 1;
    while (!temp.empty()) {
        GuestInfo g = temp.top();
        string pName = priorityLabel(g.priority);
        cout << pos++ << ". " << g.name << " (ID: " << g.id << ") - " << pName << endl;
        temp.pop();
    }
}

void serveNextRestaurantGuest() {
    if (restaurantPriorityQueue.empty()) {
        cout << "No guests in restaurant queue.\n";
        return;
    }
    GuestInfo g = restaurantPriorityQueue.top();
    string pName = priorityLabel(g.priority);
    cout << "\nNow serving at restaurant: " << g.name << " (ID: " << g.id << ") - " << pName << endl;
    restaurantPriorityQueue.pop();
    cout << "Remaining in queue: " << restaurantPriorityQueue.size() << endl;
}

// ==================== BILLING ====================

void generateBill() {
    if (allGuests.empty()) {
        cout << "No guests in the system.\n";
        return;
    }

    int searchID;
    cout << "\n=========================================\n";
    cout << "              SEARCH GUEST               \n";
    cout << "=========================================\n";
    cout << "Enter Guest ID to generate bill: ";
    cin >> searchID;

    for (auto& g : allGuests) {
        if (g.id == searchID) {
            double total = g.roomCost + g.restaurantCost;
            string pName = priorityLabel(g.priority);

            cout << "\n=========================================\n";
            cout << "                GUEST BILL                \n";
            cout << "=========================================\n";
            cout << "Guest ID: " << g.id << endl;
            cout << "Guest Name: " << g.name << endl;
            cout << "Priority: " << pName << endl;
            cout << "Room ID: " << g.roomID << endl;
            cout << "Nights: " << g.nights << endl;
            cout << "Room Charges: $" << g.roomCost << endl;
            cout << "Restaurant Charges: $" << g.restaurantCost << endl;
            cout << "-----------------------------------------\n";
            cout << "GRAND TOTAL: $" << total << endl;
            cout << "=========================================\n";
            return;
        }
    }
    cout << "Guest ID " << searchID << " not found.\n";
}

// ==================== MAIN ====================

int main() {

    cout << "\n=========================================\n";
    cout << "     GRAND HOTEL AM NSYB RECEPTION \n";
    cout << "=========================================\n";

    vector<Reservation> reservations;
    int nextReservationID = 1000;

    vector<Room> rooms;
    addRoom(rooms, { 101, "Single", 200.0, true });
    addRoom(rooms, { 102, "Single", 350.0, true });
    addRoom(rooms, { 103, "Double", 500.0, true });
    addRoom(rooms, { 104, "Double", 600.0, true });
    addRoom(rooms, { 105, "Suite", 800.0, true });

    GuestPriorityQueue priorityRequests;

    int bookingSize = 0;
    Booking* bookings = new Booking[0];

    ReportList reports;

    int choice;

    do {
        cout << "\n\n";
        cout << "            MAIN MENU                  \n";
        cout << "\n";
        cout << " 1. New Guest   \n";
        cout << " 2. Show All Rooms                     \n";
        cout << " 3. Book Room      \n";
        cout << " 4. Make Restaurant Booking            \n";
        cout << " 5. Show My Bill     \n";
        cout << " 6. Change Current Guest   \n";
        cout << " 7. Show Hotel KPI Reports             \n";
        //cout << "                                       \n";
        cout << " 8. Guest Requests     \n";
        cout << " 9. Restaurant Bookings    \n";
        cout << " 10. Reception Queue      \n";
        cout << " 11. Room Management  \n";
        cout << " 12. Exit System                       \n";
        cout << "\n";
        cout << "Choice: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input! Please enter a number.\n";
            continue;
        }

        switch (choice) {
        case 1: {
            string name;
            int tempPriority;
            cout << "Enter guest name: ";
            cin.ignore();
            getline(cin, name);

            cout << "Select Priority:\n";
            cout << "1. REGULAR\n";
            cout << "2. VIP\n";
            cout << "3. EMERGENCY\n";
            cout << "Choice: ";
            cin >> tempPriority;

            int mappedPriority = REGULAR;
            if (tempPriority == 2) mappedPriority = VIP;
            else if (tempPriority == 3) mappedPriority = EMERGENCY;

            GuestInfo newGuest(nextGuestID, name, mappedPriority);
            allGuests.push_back(newGuest);
            setCurrentGuest(nextGuestID, name, mappedPriority);

            addReservation(reservations, nextReservationID, nextGuestID, name, "2024-01-15");

            nextGuestID++;

            string pName = priorityLabel(mappedPriority);
            cout << "\nGuest added! ID: " << currentGuestID << " | Priority: " << pName << endl;
            cout << "Current guest: " << currentGuestName << " (ID: " << currentGuestID << ")\n";
            break;
        }

        case 2: {
            displayAllRooms(rooms);
            break;
        }

        case 3: {
            if (currentGuestID == -1) {
                cout << "No guest selected. Please add a new guest first (Option 1).\n";
                break;
            }
            showAvailableRooms(rooms);
            cout << "\nEnter Room ID to book: ";
            int tempRoomID;
            cin >> tempRoomID;
            cout << "Enter number of nights: ";
            int tempNights;
            cin >> tempNights;

            bool found = false;
            for (auto& r : rooms) {
                if (r.roomID == tempRoomID && r.isAvailable) {
                    r.isAvailable = false;
                    for (auto& g : allGuests) {
                        if (g.id == currentGuestID) {
                            g.roomID = tempRoomID;
                            g.nights = tempNights;
                            g.roomCost = r.pricePerNight * tempNights;
                            found = true;
                            break;
                        }
                    }
                    cout << "\nRoom " << tempRoomID << " booked for " << tempNights << " nights!\n";
                    cout << "Room status changed from Available to Occupied.\n";
                    break;
                }
            }
            if (!found) {
                cout << "Room not available or invalid ID.\n";
            }
            break;
        }

        case 4: {
            if (currentGuestID == -1) {
                cout << "No guest selected. Please add a new guest first (Option 1).\n";
                break;
            }
            string time;
            double cost;
            cout << "Enter booking time (e.g., 7PM): ";
            cin.ignore();
            getline(cin, time);
            cout << "Enter restaurant charges: $";
            cin >> cost;

            Booking newBooking;
            newBooking.id = currentGuestID;
            newBooking.name = currentGuestName;
            newBooking.time = time;
            addBooking(bookings, bookingSize, newBooking);

            addToRestaurantQueue(currentGuestID, cost, time);

            cout << "Restaurant booking added!\n";
            break;
        }

        case 5: {
            generateBill();
            break;
        }

        case 6: {
            int subChoice;
            cout << "\n";
            cout << "         UNDO/REDO MENU                \n";
            cout << "\n";
            cout << " 1. Show Current Guest                 \n";
            cout << " 2. Change Current Guest               \n";
            cout << " 3. Undo Last Change                   \n";
            cout << " 4. Redo Last Change                   \n";
            cout << " 5. Back to Main Menu                  \n";
            cout << "n";
            cout << "Choice: ";
            cin >> subChoice;

            switch (subChoice) {
            case 1: {
                showCurrentState();
                break;
            }
            case 2: {
                if (allGuests.empty()) {
                    cout << "No guests in system.\n";
                    break;
                }
                cout << "\n=== SELECT GUEST ===\n";
                for (auto& g : allGuests) {
                    string pName = priorityLabel(g.priority);
                    cout << "ID: " << g.id << " | Name: " << g.name << " | Priority: " << pName << endl;
                }
                int id;
                cout << "Enter Guest ID: ";
                cin >> id;
                bool found = false;
                for (auto& g : allGuests) {
                    if (g.id == id) {
                        string guestState = currentGuestName + "|" + to_string(currentGuestID) + "|" + to_string(currentGuestPriority);
                        undoStack.push(guestState);

                        while (!redoStack.empty()) redoStack.pop();

                        currentGuestID = g.id;
                        currentGuestName = g.name;
                        currentGuestPriority = g.priority;
                        currentState = g.name + " (ID: " + to_string(g.id) + ") | Priority: " + priorityLabel(g.priority);

                        cout << "Current guest changed to: " << currentGuestName << " (ID: " << currentGuestID << ")\n";
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    cout << "Guest ID not found.\n";
                }
                break;
            }
            case 3: {
                if (undoStack.empty()) {
                    cout << "Nothing to undo.\n";
                    break;
                }
                string currentGuestState = currentGuestName + "|" + to_string(currentGuestID) + "|" + to_string(currentGuestPriority);
                redoStack.push(currentGuestState);

                string previousState = undoStack.top();
                undoStack.pop();

                size_t pos1 = previousState.find('|');
                size_t pos2 = previousState.find('|', pos1 + 1);

                string prevName = previousState.substr(0, pos1);
                int prevID = stoi(previousState.substr(pos1 + 1, pos2 - pos1 - 1));
                int prevPriority = stoi(previousState.substr(pos2 + 1));

                currentGuestName = prevName;
                currentGuestID = prevID;
                currentGuestPriority = prevPriority;
                currentState = prevName + " (ID: " + to_string(prevID) + ") | Priority: " + priorityLabel(prevPriority);

                cout << "Undo performed. Current guest: " << currentGuestName << " (ID: " << currentGuestID << ")\n";
                break;
            }
            case 4: {
                if (redoStack.empty()) {
                    cout << "Nothing to redo.\n";
                    break;
                }
                string currentGuestState = currentGuestName + "|" + to_string(currentGuestID) + "|" + to_string(currentGuestPriority);
                undoStack.push(currentGuestState);

                string nextState = redoStack.top();
                redoStack.pop();

                size_t pos1 = nextState.find('|');
                size_t pos2 = nextState.find('|', pos1 + 1);

                string nextName = nextState.substr(0, pos1);
                int nextID = stoi(nextState.substr(pos1 + 1, pos2 - pos1 - 1));
                int nextPriority = stoi(nextState.substr(pos2 + 1));

                currentGuestName = nextName;
                currentGuestID = nextID;
                currentGuestPriority = nextPriority;
                currentState = nextName + " (ID: " + to_string(nextID) + ") | Priority: " + priorityLabel(nextPriority);

                cout << "Redo performed. Current guest: " << currentGuestName << " (ID: " << currentGuestID << ")\n";
                break;
            }
            case 5: {
                cout << "Returning to Main Menu...\n";
                break;
            }
            default: {
                cout << "Invalid choice.\n";
            }
            }
            break;
        }

        case 7: {
            reports.generateKPIs();
            break;
        }

        case 8: {
            int reqChoice;
            cout << "\n";
            cout << "    GUEST REQUESTS MENU | \n";
            cout << "\n";
            cout << " 1. Add New Request                    \n";
            cout << " 2. View Pending Requests              \n";
            cout << " 3. Process Next Request               \n";
            cout << " 4. Back to Main Menu                  \n";
            cout << "\n";
            cout << "Choice: ";
            cin >> reqChoice;

            switch (reqChoice) {
            case 1: {
                if (currentGuestID == -1) {
                    cout << "No guest selected.\n";
                    break;
                }
                string desc;
                cout << "Enter request description: ";
                cin.ignore();
                getline(cin, desc);
                priorityRequests.enqueueRequest(currentGuestName, desc, currentGuestPriority);
                break;
            }
            case 2: {
                priorityRequests.displayQueue();
                break;
            }
            case 3: {
                priorityRequests.dequeueRequest();
                break;
            }
            case 4: {
                cout << "Returning to Main Menu...\n";
                break;
            }
            default: {
                cout << "Invalid choice.\n";
            }
            }
            break;
        }

        case 9: {
            int bookChoice;
            cout << "\n\n";
            cout << "     RESTAURANT BOOKINGS MENU          \n";
            cout << "\n";
            cout << " 1. View All Restaurant Bookings       \n";
            cout << " 2. Search Booking by ID               \n";
            cout << " 3. View Restaurant Queue (Priority)   \n";
            cout << " 4. Serve Next Restaurant Guest        \n";
            cout << " 5. Back to Main Menu                  \n";
            cout << "\n";
            cout << "Choice: ";
            cin >> bookChoice;

            switch (bookChoice) {
            case 1: {
                printBookings(bookings, bookingSize);
                break;
            }
            case 2: {
                int searchID;
                cout << "Enter Booking ID: ";
                cin >> searchID;
                int index = findBooking(bookings, bookingSize, searchID);
                if (index != -1) {
                    cout << "Booking Found: " << bookings[index].name << " at " << bookings[index].time << endl;
                }
                else {
                    cout << "Booking not found.\n";
                }
                break;
            }
            case 3: {
                showRestaurantPriorityQueue();
                break;
            }
            case 4: {
                serveNextRestaurantGuest();
                break;
            }
            case 5: {
                cout << "Returning to Main Menu...\n";
                break;
            }
            default: {
                cout << "Invalid choice.\n";
            }
            }
            break;
        }

        case 10: {
            int queueChoice;
            cout << "\n\n";
            cout << "       RECEPTION QUEUE MENU            \n";
            cout << "\n";
            cout << " 1. Add Guest to Reception Queue       \n";
            cout << " 2. Serve Next Guest (Check-in)        \n";
            cout << " 3. View Reception Queue               \n";
            cout << " 4. Back to Main Menu                  \n";
            cout << "\n";
            cout << "Choice: ";
            cin >> queueChoice;

            switch (queueChoice) {
            case 1: {
                string name;
                cout << "Enter guest name: ";
                cin.ignore();
                getline(cin, name);
                enqueueGuest(name);
                break;
            }
            case 2: {
                dequeueGuest();
                break;
            }
            case 3: {
                displayReceptionQueue();
                break;
            }
            case 4: {
                cout << "Returning to Main Menu...\n";
                break;
            }
            default: {
                cout << "Invalid choice.\n";
            }
            }
            break;
        }

        case 11: {
            int roomChoice;
            cout << "\n\n";
            cout << "        ROOM MANAGEMENT MENU           \n";
            cout << "\n";
            cout << " 1. Search Room by ID (Binary Search)  \n";
            cout << " 2. Find Rooms by Price Range          \n";
            cout << " 3. Show Available Rooms               \n";
            cout << " 4. Back to Main Menu                  \n";
            cout << "\n";
            cout << "Choice: ";
            cin >> roomChoice;

            switch (roomChoice) {
            case 1: {
                int searchID;
                cout << "Enter Room ID: ";
                cin >> searchID;
                int index = findRoomByID(rooms, searchID);
                if (index != -1) {
                    rooms[index].display();
                }
                else {
                    cout << "Room not found.\n";
                }
                break;
            }
            case 2: {
                double minP, maxP;
                cout << "Enter minimum price: $";
                cin >> minP;
                cout << "Enter maximum price: $";
                cin >> maxP;
                vector<Room> result = findRoomsByPriceRange(rooms, minP, maxP);
                if (result.empty()) {
                    cout << "No rooms in this price range.\n";
                }
                else {
                    cout << "\n=== ROOMS BETWEEN $" << minP << " - $" << maxP << " ===\n";
                    for (auto& r : result) {
                        r.display();
                    }
                }
                break;
            }
            case 3: {
                showAvailableRooms(rooms);
                break;
            }
            case 4: {
                cout << "Returning to Main Menu...\n";
                break;
            }
            default: {
                cout << "Invalid choice.\n";
            }
            }
            break;
        }

        case 12: {
            cout << "\nThank you for using Grand Hotel System!\n";
            break;
        }

        default: {
            cout << "Invalid choice. Please enter 1-12.\n";
        }
        }
    } while (choice != 12);

    delete[] bookings;
    return 0;
}