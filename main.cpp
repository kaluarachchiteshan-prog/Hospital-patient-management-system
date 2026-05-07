#include <iostream>
#include <string>
#include <ctime>
#include <cctype>
using namespace std;



//  PATIENT STRUCTURE
struct Patient {
    int    id;
    string name;
    int    age;
    string disease;
    int    priority;  // 1=Emergency, 2=Urgent, 3=Normal

    Patient() : id(0), age(0), priority(3) {}
    Patient(int i, string n, int a, string d, int p)
        : id(i), name(n), age(a), disease(d), priority(p) {}

    string priorityLabel() const {
        if (priority == 1) return "EMERGENCY";
        if (priority == 2) return "URGENT";
        return "NORMAL";
    }
};



//  1. LINKED LIST — Patient Registry
struct LLNode {
    Patient data;
    LLNode* next;
    LLNode(Patient p) : data(p), next(nullptr) {}
};

class PatientLinkedList {
    LLNode* head;
    int count;
public:
    PatientLinkedList() : head(nullptr), count(0) {}

    void insert(Patient p) {
        LLNode* node = new LLNode(p);
        if (!head) { head = node; count++; return; }
        LLNode* cur = head;
        while (cur->next) cur = cur->next;
        cur->next = node;
        count++;
    }

    bool remove(int id) {
        if (!head) return false;
        if (head->data.id == id) {
            LLNode* tmp = head;
            head = head->next;
            delete tmp;
            count--;
            return true;
        }
        LLNode* cur = head;
        while (cur->next && cur->next->data.id != id)
            cur = cur->next;
        if (!cur->next) return false;
        LLNode* tmp = cur->next;
        cur->next = tmp->next;
        delete tmp;
        count--;
        return true;
    }

    Patient* search(int id) {
        LLNode* cur = head;
        while (cur) {
            if (cur->data.id == id) return &cur->data;
            cur = cur->next;
        }
        return nullptr;
    }

    void display() const {
        if (!head) { cout << "  No patients registered.\n"; return; }
        cout << "  ID    Name                Age  Disease           Priority\n";
        cout << "  " << string(58, '-') << "\n";
        LLNode* cur = head;
        while (cur) {
            Patient& p = cur->data;
            cout << "  " << p.id << "\t" << p.name << "\t\t"
                 << p.age << "\t" << p.disease << "\t" << p.priorityLabel() << "\n";
            cur = cur->next;
        }
    }

    int size() const { return count; }

    ~PatientLinkedList() {
        while (head) { LLNode* t = head; head = head->next; delete t; }
    }
};



//  2. PRIORITY QUEUE — Waiting Line
struct QNode {
    Patient data;
    QNode* next;
    QNode(Patient p) : data(p), next(nullptr) {}
};

class PriorityQueue {
    QNode* front;
    int count;
public:
    PriorityQueue() : front(nullptr), count(0) {}

    void enqueue(Patient p) {
        QNode* node = new QNode(p);
        if (!front || p.priority < front->data.priority) {
            node->next = front;
            front = node;
        } else {
            QNode* cur = front;
            while (cur->next && cur->next->data.priority <= p.priority)
                cur = cur->next;
            node->next = cur->next;
            cur->next = node;
        }
        count++;
    }

    Patient dequeue() {
        if (!front) throw runtime_error("Queue is empty!");
        QNode* tmp = front;
        Patient p = tmp->data;
        front = front->next;
        delete tmp;
        count--;
        return p;
    }

    bool isEmpty() const { return front == nullptr; }
    int  size()    const { return count; }

    void display() const {
        if (!front) { cout << "  Queue is empty.\n"; return; }
        cout << "  Pos  ID    Name                Priority\n";
        cout << "  " << string(42, '-') << "\n";
        QNode* cur = front;
        int pos = 1;
        while (cur) {
            cout << "  " << pos++ << "    " << cur->data.id
                 << "   " << cur->data.name
                 << "\t\t" << cur->data.priorityLabel() << "\n";
            cur = cur->next;
        }
    }

    ~PriorityQueue() {
        while (front) { QNode* t = front; front = front->next; delete t; }
    }
};



//  3. STACK — Treatment History
struct SNode {
    Patient data;
    SNode* next;
    SNode(Patient p) : data(p), next(nullptr) {}
};

class TreatmentStack {
    SNode* top;
    int count;
public:
    TreatmentStack() : top(nullptr), count(0) {}

    void push(Patient p) {
        SNode* node = new SNode(p);
        node->next = top;
        top = node;
        count++;
    }

    Patient pop() {
        if (!top) throw runtime_error("Stack is empty!");
        SNode* tmp = top;
        Patient p = tmp->data;
        top = top->next;
        delete tmp;
        count--;
        return p;
    }

    bool isEmpty() const { return top == nullptr; }
    int  size()    const { return count; }

    void display() const {
        if (!top) { cout << "  No treatment history.\n"; return; }
        cout << "  ID    Name                Disease           Priority\n";
        cout << "  " << string(54, '-') << "\n";
        SNode* cur = top;
        while (cur) {
            cout << "  " << cur->data.id << "\t" << cur->data.name
                 << "\t\t" << cur->data.disease
                 << "\t" << cur->data.priorityLabel() << "\n";
            cur = cur->next;
        }
    }

    ~TreatmentStack() {
        while (top) { SNode* t = top; top = top->next; delete t; }
    }
};




//  4. BST — Fast Patient Search by ID
struct BSTNode {
    Patient data;
    BSTNode* left;
    BSTNode* right;
    BSTNode(Patient p) : data(p), left(nullptr), right(nullptr) {}
};

class PatientBST {
    BSTNode* root;

    BSTNode* insert(BSTNode* node, Patient p) {
        if (!node) return new BSTNode(p);
        if (p.id < node->data.id) node->left  = insert(node->left, p);
        else if (p.id > node->data.id) node->right = insert(node->right, p);
        return node;
    }

    BSTNode* search(BSTNode* node, int id) {
        if (!node || node->data.id == id) return node;
        if (id < node->data.id) return search(node->left, id);
        return search(node->right, id);
    }

    BSTNode* findMin(BSTNode* node) {
        while (node->left) node = node->left;
        return node;
    }

    BSTNode* remove(BSTNode* node, int id) {
        if (!node) return nullptr;
        if (id < node->data.id)       node->left  = remove(node->left, id);
        else if (id > node->data.id)  node->right = remove(node->right, id);
        else {
            if (!node->left)  { BSTNode* t = node->right; delete node; return t; }
            if (!node->right) { BSTNode* t = node->left;  delete node; return t; }
            BSTNode* mn = findMin(node->right);
            node->data = mn->data;
            node->right = remove(node->right, mn->data.id);
        }
        return node;
    }

    void destroy(BSTNode* node) {
        if (!node) return;
        destroy(node->left);
        destroy(node->right);
        delete node;
    }

public:
    PatientBST() : root(nullptr) {}
    void     insert(Patient p) { root = insert(root, p); }
    void     remove(int id)    { root = remove(root, id); }
    Patient* search(int id)    { BSTNode* n = search(root, id); return n ? &n->data : nullptr; }
    ~PatientBST() { destroy(root); }
};


//  MENU
void printMenu() {
    cout << "\n====== HOSPITAL PATIENT MANAGEMENT SYSTEM ======\n";
    cout << " [1]  Register New Patient(Linked List)\n";
    cout << " [2]  Add Patient to Waiting List(Queue)\n";
    cout << " [3]  Serve Next Patient (Dequeue)\n";
    cout << " [4]  View Patient (Queue)\n";
    cout << " [5]  View All Registered Patients (Linked List)\n";
    cout << " [6]  Search Patient by ID (BST)\n";
    cout << " [7]  View Treatment History (Stack)\n";
    cout << " [8]  Undo Last Treatment (Stack Pop)\n";
    cout << " [9]  Remove Patient from Registry\n";
    cout << " [0]  Exit\n";
    cout << "=================================================\n";
    cout << " Enter choice: ";
}

//  MAIN

int main() {
    PatientLinkedList registry;
    PriorityQueue     waitingQueue;
    TreatmentStack    treatmentHistory;
    PatientBST        bst;

    int nextId = 1001;

    // Pre-load sample patients
    Patient samples[] = {
        Patient(nextId++, "Amal Perera",    45, "Chest Pain", 1),
        Patient(nextId++, "Nimal Silva",    30, "Flu",        3),
        Patient(nextId++, "Kamala Bandara", 62, "Diabetes",   2),
        Patient(nextId++, "Sunil Fernando", 25, "Fracture",   2),
        Patient(nextId++, "Priya Mendis",   19, "Headache",   3)
    };
    for (auto& s : samples) { registry.insert(s); bst.insert(s); }

    int choice;
    do {
        printMenu();
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            string name, ageStr, disease;
            int age, priority;
            cout << "\n Name    : "; getline(cin, name);
            while (true) {
                cout << " Age     : "; getline(cin, ageStr);
                bool validAge = !ageStr.empty();
                for (char c : ageStr) {
                    if (!isdigit(static_cast<unsigned char>(c))) {
                        validAge = false;
                        break;
                    }
                }
                if (validAge) {
                    age = stoi(ageStr);
                    break;
                }
                cout << " Invalid age. Please enter only numbers.\n";
            }
            cout << " Disease : "; getline(cin, disease);
            string priorityStr;
            while (true) {
                cout << " Priority (1=Emergency, 2=Urgent, 3=Normal): ";
                getline(cin, priorityStr);
                bool validPriority = !priorityStr.empty();
                for (char c : priorityStr) {
                    if (!isdigit(static_cast<unsigned char>(c))) {
                        validPriority = false;
                        break;
                    }
                }
                if (validPriority) {
                    priority = stoi(priorityStr);
                    if (priority >= 1 && priority <= 3) break;
                }
                cout << " Invalid priority. Please enter 1, 2, or 3.\n";
            }

            Patient p(nextId++, name, age, disease, priority);
            registry.insert(p);
            bst.insert(p);
            cout << "\n Registered! ID: " << p.id << " | Priority: " << p.priorityLabel() << "\n";

        } else if (choice == 2) {
            int id;
            cout << "\n Enter Patient ID: "; cin >> id; cin.ignore();
            Patient* p = registry.search(id);
            if (!p) cout << " Patient ID " << id << " not found.\n";
            else {
                waitingQueue.enqueue(*p);
                cout << " " << p->name << " added to queue (" << p->priorityLabel() << ").\n";
            }

        } else if (choice == 3) {
            if (waitingQueue.isEmpty()) {
                cout << "\n Queue is empty.\n";
            } else {
                Patient served = waitingQueue.dequeue();
                treatmentHistory.push(served);
                cout << "\n Now serving: " << served.name
                     << " (ID: " << served.id << ", " << served.disease << ")\n";
            }

        } else if (choice == 4) {
            cout << "\n--- Waiting Queue (" << waitingQueue.size() << " patients) ---\n";
            waitingQueue.display();

        } else if (choice == 5) {
            cout << "\n--- All Registered Patients (" << registry.size() << ") ---\n";
            registry.display();

        } else if (choice == 6) {
            int id;
            cout << "\n Enter Patient ID: "; cin >> id; cin.ignore();
            Patient* p = bst.search(id);
            if (!p) cout << " Patient ID " << id << " not found.\n";
            else {
                cout << "\n Found:\n";
                cout << "  ID      : " << p->id       << "\n";
                cout << "  Name    : " << p->name     << "\n";
                cout << "  Age     : " << p->age      << "\n";
                cout << "  Disease : " << p->disease  << "\n";
                cout << "  Priority: " << p->priorityLabel() << "\n";
            }

        } else if (choice == 7) {
            cout << "\n--- Treatment History (" << treatmentHistory.size() << " patients) ---\n";
            treatmentHistory.display();

        } else if (choice == 8) {
            if (treatmentHistory.isEmpty()) {
                cout << "\n No treatment history to undo.\n";
            } else {
                Patient undone = treatmentHistory.pop();
                waitingQueue.enqueue(undone);
                cout << "\n Treatment of " << undone.name << " undone. Re-added to queue.\n";
            }

        } else if (choice == 9) {
            int id;
            cout << "\n Enter Patient ID to remove: "; cin >> id; cin.ignore();
            if (registry.remove(id)) {
                bst.remove(id);
                cout << " Patient ID " << id << " removed.\n";
            } else {
                cout << " Patient ID " << id << " not found.\n";
            }

        } else if (choice != 0) {
            cout << " Invalid option.\n";
        }

    } while (choice != 0);

    cout << "\n Goodbye!\n";
    return 0;
}