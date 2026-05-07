#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cctype>

#pragma comment(lib, "Ws2_32.lib")

using namespace std;

struct Patient {
    int id;
    string name;
    int age;
    string disease;
    int priority;

    Patient() : id(0), age(0), priority(3) {}
    Patient(int i, string n, int a, string d, int p)
        : id(i), name(std::move(n)), age(a), disease(std::move(d)), priority(p) {}
};

struct LLNode {
    Patient data;
    LLNode* next;
    LLNode(Patient p) : data(std::move(p)), next(nullptr) {}
};

class PatientLinkedList {
    LLNode* head;
    int count;
public:
    PatientLinkedList() : head(nullptr), count(0) {}

    void insert(const Patient& p) {
        LLNode* node = new LLNode(p);
        if (!head) {
            head = node;
        } else {
            LLNode* cur = head;
            while (cur->next) cur = cur->next;
            cur->next = node;
        }
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

    vector<Patient> toVector() const {
        vector<Patient> result;
        LLNode* cur = head;
        while (cur) {
            result.push_back(cur->data);
            cur = cur->next;
        }
        return result;
    }

    int size() const { return count; }

    ~PatientLinkedList() {
        while (head) { LLNode* t = head; head = head->next; delete t; }
    }
};

struct QNode {
    Patient data;
    QNode* next;
    QNode(Patient p) : data(std::move(p)), next(nullptr) {}
};

class PriorityQueue {
    QNode* front;
    int count;
public:
    PriorityQueue() : front(nullptr), count(0) {}

    void enqueue(const Patient& p) {
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

    bool remove(int id) {
        if (!front) return false;
        if (front->data.id == id) {
            QNode* tmp = front;
            front = front->next;
            delete tmp;
            count--;
            return true;
        }
        QNode* cur = front;
        while (cur->next && cur->next->data.id != id)
            cur = cur->next;
        if (!cur->next) return false;
        QNode* tmp = cur->next;
        cur->next = tmp->next;
        delete tmp;
        count--;
        return true;
    }

    vector<Patient> toVector() const {
        vector<Patient> result;
        QNode* cur = front;
        while (cur) {
            result.push_back(cur->data);
            cur = cur->next;
        }
        return result;
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
    int size() const { return count; }

    ~PriorityQueue() {
        while (front) { QNode* t = front; front = front->next; delete t; }
    }
};

struct SNode {
    Patient data;
    SNode* next;
    SNode(Patient p) : data(std::move(p)), next(nullptr) {}
};

class TreatmentStack {
    SNode* top;
    int count;
public:
    TreatmentStack() : top(nullptr), count(0) {}

    void push(const Patient& p) {
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

    vector<Patient> toVector() const {
        vector<Patient> result;
        SNode* cur = top;
        while (cur) {
            result.push_back(cur->data);
            cur = cur->next;
        }
        return result;
    }

    bool isEmpty() const { return top == nullptr; }
    int size() const { return count; }

    ~TreatmentStack() {
        while (top) { SNode* t = top; top = top->next; delete t; }
    }
};

struct BSTNode {
    Patient data;
    BSTNode* left;
    BSTNode* right;
    BSTNode(Patient p) : data(std::move(p)), left(nullptr), right(nullptr) {}
};

class PatientBST {
    BSTNode* root;

    BSTNode* insert(BSTNode* node, const Patient& p) {
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
        if (id < node->data.id) node->left  = remove(node->left, id);
        else if (id > node->data.id) node->right = remove(node->right, id);
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
    void insert(const Patient& p) { root = insert(root, p); }
    void remove(int id) { root = remove(root, id); }
    Patient* search(int id) { BSTNode* n = search(root, id); return n ? &n->data : nullptr; }
    ~PatientBST() { destroy(root); }
};

string jsonEscape(const string& text) {
    string result;
    for (char c : text) {
        switch (c) {
            case '\\': result += "\\\\"; break;
            case '"': result += "\\\""; break;
            case '\n': result += "\\n"; break;
            case '\r': result += "\\r"; break;
            case '\t': result += "\\t"; break;
            default: result += c; break;
        }
    }
    return result;
}

string patientJson(const Patient& p) {
    ostringstream out;
    out << "{"
        << "\"id\":" << p.id << ","
        << "\"name\":\"" << jsonEscape(p.name) << "\"," 
        << "\"age\":" << p.age << ","
        << "\"disease\":\"" << jsonEscape(p.disease) << "\"," 
        << "\"priority\":" << p.priority
        << "}";
    return out.str();
}

string patientsJson(const vector<Patient>& list) {
    ostringstream out;
    out << "{\"patients\":[";
    for (size_t i = 0; i < list.size(); ++i) {
        if (i) out << ',';
        out << patientJson(list[i]);
    }
    out << "],\"count\":" << list.size() << "}";
    return out.str();
}

string queueJson(const vector<Patient>& list) {
    ostringstream out;
    out << "{\"queue\":[";
    for (size_t i = 0; i < list.size(); ++i) {
        if (i) out << ',';
        out << patientJson(list[i]);
    }
    out << "],\"count\":" << list.size() << "}";
    return out.str();
}

string historyJson(const vector<Patient>& list) {
    ostringstream out;
    out << "{\"history\":[";
    for (size_t i = 0; i < list.size(); ++i) {
        if (i) out << ',';
        out << patientJson(list[i]);
    }
    out << "],\"count\":" << list.size() << "}";
    return out.str();
}

string statsJson(int total, int queueCount, int treatedCount, int emergencyCount) {
    ostringstream out;
    out << "{"
        << "\"total\":" << total << ","
        << "\"queue\":" << queueCount << ","
        << "\"treated\":" << treatedCount << ","
        << "\"emergency\":" << emergencyCount
        << "}";
    return out.str();
}

string trim(const string& s) {
    size_t start = 0;
    while (start < s.size() && isspace((unsigned char)s[start])) start++;
    size_t end = s.size();
    while (end > start && isspace((unsigned char)s[end - 1])) end--;
    return s.substr(start, end - start);
}

bool readFileContents(const string& path, string& out) {
    ifstream file(path, ios::binary);
    if (!file) return false;
    ostringstream ss;
    ss << file.rdbuf();
    out = ss.str();
    return true;
}

bool parseJsonInt(const string& body, const string& key, int& value) {
    string needle = "\"" + key + "\":";
    size_t pos = body.find(needle);
    if (pos == string::npos) return false;
    pos += needle.size();
    while (pos < body.size() && isspace((unsigned char)body[pos])) pos++;
    string num;
    while (pos < body.size() && (isdigit((unsigned char)body[pos]) || body[pos] == '-')) {
        num += body[pos++];
    }
    if (num.empty()) return false;
    value = stoi(num);
    return true;
}

bool parseJsonString(const string& body, const string& key, string& value) {
    string needle = "\"" + key + "\":\"";
    size_t pos = body.find(needle);
    if (pos == string::npos) return false;
    pos += needle.size();
    value.clear();
    while (pos < body.size()) {
        char c = body[pos++];
        if (c == '"') break;
        if (c == '\\' && pos < body.size()) {
            char next = body[pos++];
            if (next == 'n') value += '\n';
            else if (next == 'r') value += '\r';
            else if (next == 't') value += '\t';
            else value += next;
        } else {
            value += c;
        }
    }
    return true;
}

string makeHttpResponse(int status, const string& statusText, const string& body, const string& contentType = "application/json") {
    ostringstream out;
    out << "HTTP/1.1 " << status << " " << statusText << "\r\n"
        << "Content-Type: " << contentType << "; charset=utf-8\r\n"
        << "Content-Length: " << body.size() << "\r\n"
        << "Access-Control-Allow-Origin: *\r\n"
        << "Access-Control-Allow-Methods: GET, POST, DELETE, OPTIONS\r\n"
        << "Access-Control-Allow-Headers: Content-Type\r\n"
        << "Connection: close\r\n"
        << "\r\n"
        << body;
    return out.str();
}

bool parseRequestLine(const string& request, string& method, string& path, string& body) {
    size_t headerEnd = request.find("\r\n\r\n");
    if (headerEnd == string::npos) return false;
    string headerPart = request.substr(0, headerEnd);
    body = request.substr(headerEnd + 4);

    istringstream lines(headerPart);
    string requestLine;
    if (!getline(lines, requestLine)) return false;
    if (!requestLine.empty() && requestLine.back() == '\r') requestLine.pop_back();

    istringstream parts(requestLine);
    parts >> method >> path;
    if (method.empty() || path.empty()) return false;

    string line;
    int contentLength = 0;
    while (getline(lines, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        string lower = line;
        transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
        if (lower.rfind("content-length:", 0) == 0) {
            contentLength = stoi(trim(line.substr(15)));
        }
    }
    while (body.size() < (size_t)contentLength) {
        // Caller must read more if necessary.
        break;
    }
    return true;
}

vector<string> splitPath(const string& path) {
    vector<string> parts;
    size_t start = 0;
    while (start < path.size()) {
        size_t slash = path.find('/', start);
        if (slash == string::npos) slash = path.size();
        string segment = path.substr(start, slash - start);
        if (!segment.empty()) parts.push_back(segment);
        start = slash + 1;
    }
    return parts;
}

string handleApiRequest(const string& method, const string& path, const string& body,
                        PatientLinkedList& registry, PriorityQueue& queue,
                        TreatmentStack& history, PatientBST& bst, int& nextId) {
    if (method == "OPTIONS") {
        return makeHttpResponse(204, "No Content", "", "text/plain");
    }
    if (path == "/api/patients" && method == "GET") {
        return makeHttpResponse(200, "OK", patientsJson(registry.toVector()));
    }
    if (path == "/api/patients" && method == "POST") {
        string name, disease;
        int age = 0, priority = 3;
        if (!parseJsonString(body, "name", name) || !parseJsonString(body, "disease", disease)
            || !parseJsonInt(body, "age", age) || !parseJsonInt(body, "priority", priority)) {
            return makeHttpResponse(400, "Bad Request", "{\"error\":\"Invalid patient payload\"}");
        }
        Patient p(nextId++, name, age, disease, priority);
        registry.insert(p);
        bst.insert(p);
        ostringstream out;
        out << "{\"patient\":" << patientJson(p) << "}";
        return makeHttpResponse(201, "Created", out.str());
    }
    if (path.rfind("/api/patients/", 0) == 0) {
        int id = stoi(path.substr(14));
        if (method == "GET") {
            Patient* p = registry.search(id);
            if (!p) return makeHttpResponse(404, "Not Found", "{\"error\":\"Patient not found\"}");
            ostringstream out;
            out << "{\"patient\":" << patientJson(*p) << "}";
            return makeHttpResponse(200, "OK", out.str());
        }
        if (method == "DELETE") {
            Patient* p = registry.search(id);
            if (!p) return makeHttpResponse(404, "Not Found", "{\"error\":\"Patient not found\"}");
            string removedName = p->name;
            registry.remove(id);
            bst.remove(id);
            queue.remove(id);
            ostringstream out;
            out << "{\"message\":\"Patient " << jsonEscape(removedName) << " removed\"}";
            return makeHttpResponse(200, "OK", out.str());
        }
    }
    if (path == "/api/queue" && method == "GET") {
        return makeHttpResponse(200, "OK", queueJson(queue.toVector()));
    }
    if (path == "/api/queue" && method == "POST") {
        int id = 0;
        if (!parseJsonInt(body, "id", id)) {
            return makeHttpResponse(400, "Bad Request", "{\"error\":\"Missing patient id\"}");
        }
        Patient* p = registry.search(id);
        if (!p) return makeHttpResponse(404, "Not Found", "{\"error\":\"Patient not found\"}");
        vector<Patient> queued = queue.toVector();
        for (auto& item : queued) if (item.id == id)
            return makeHttpResponse(409, "Conflict", "{\"error\":\"Patient already in queue\"}");
        queue.enqueue(*p);
        ostringstream out;
        out << "{\"message\":\"Patient " << jsonEscape(p->name) << " added to queue\",\"patient\":" << patientJson(*p) << "}";
        return makeHttpResponse(201, "Created", out.str());
    }
    if (path == "/api/serve" && method == "POST") {
        if (queue.isEmpty()) {
            return makeHttpResponse(400, "Bad Request", "{\"error\":\"Queue is empty\"}");
        }
        Patient p = queue.dequeue();
        history.push(p);
        ostringstream out;
        out << "{\"message\":\"Now serving " << jsonEscape(p.name) << "\",\"patient\":" << patientJson(p) << "}";
        return makeHttpResponse(200, "OK", out.str());
    }
    if (path == "/api/history" && method == "GET") {
        return makeHttpResponse(200, "OK", historyJson(history.toVector()));
    }
    if (path == "/api/undo" && method == "POST") {
        if (history.isEmpty()) {
            return makeHttpResponse(400, "Bad Request", "{\"error\":\"No treatment history to undo\"}");
        }
        Patient p = history.pop();
        queue.enqueue(p);
        ostringstream out;
        out << "{\"message\":\"Treatment undone for " << jsonEscape(p.name) << "\",\"patient\":" << patientJson(p) << "}";
        return makeHttpResponse(200, "OK", out.str());
    }
    if (path == "/api/stats" && method == "GET") {
        vector<Patient> all = registry.toVector();
        int emergencyCount = 0;
        for (auto& p : all) if (p.priority == 1) emergencyCount++;
        return makeHttpResponse(200, "OK", statsJson(all.size(), queue.size(), history.size(), emergencyCount));
    }
    return makeHttpResponse(404, "Not Found", "{\"error\":\"Route not found\"}");
}

bool readRequest(SOCKET client, string& request, int& contentLength) {
    const int bufferSize = 4096;
    char buffer[bufferSize];
    request.clear();
    contentLength = 0;
    int totalRead = 0;
    while (true) {
        int bytes = recv(client, buffer, bufferSize, 0);
        if (bytes <= 0) return false;
        request.append(buffer, bytes);
        totalRead += bytes;
        size_t headerEnd = request.find("\r\n\r\n");
        if (headerEnd != string::npos) {
            string headers = request.substr(0, headerEnd);
            size_t pos = headers.find("Content-Length:");
            if (pos != string::npos) {
                pos += 15;
                while (pos < headers.size() && isspace((unsigned char)headers[pos])) pos++;
                contentLength = stoi(headers.substr(pos));
            }
            size_t bodyBytes = request.size() - headerEnd - 4;
            if (bodyBytes >= (size_t)contentLength) break;
        }
        if (totalRead >= 65536) break;
    }
    return true;
}

void runServer(int port) {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "WSAStartup failed\n";
        return;
    }

    SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listenSocket == INVALID_SOCKET) {
        cerr << "Socket creation failed\n";
        WSACleanup();
        return;
    }

    sockaddr_in service;
    service.sin_family = AF_INET;
    service.sin_addr.s_addr = inet_addr("127.0.0.1");
    service.sin_port = htons(port);

    int yes = 1;
    setsockopt(listenSocket, SOL_SOCKET, SO_REUSEADDR, (const char*)&yes, sizeof(yes));

    if (bind(listenSocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR) {
        cerr << "Bind failed\n";
        closesocket(listenSocket);
        WSACleanup();
        return;
    }

    if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) {
        cerr << "Listen failed\n";
        closesocket(listenSocket);
        WSACleanup();
        return;
    }

    cout << "Hospital web server running at http://127.0.0.1:" << port << "/\n";

    PatientLinkedList registry;
    PriorityQueue queue;
    TreatmentStack history;
    PatientBST bst;
    int nextId = 1006;

    vector<Patient> samples = {
        Patient(1001, "Amal Perera", 45, "Chest Pain", 1),
        Patient(1002, "Nimal Silva", 30, "Flu", 3),
        Patient(1003, "Kamala Bandara", 62, "Diabetes", 2),
        Patient(1004, "Sunil Fernando", 25, "Fracture", 2),
        Patient(1005, "Priya Mendis", 19, "Headache", 3),
    };
    for (auto& p : samples) {
        registry.insert(p);
        bst.insert(p);
    }

    while (true) {
        SOCKET client = accept(listenSocket, nullptr, nullptr);
        if (client == INVALID_SOCKET) continue;

        string request;
        int contentLength = 0;
        if (!readRequest(client, request, contentLength)) {
            closesocket(client);
            continue;
        }

        string method, path, body;
        if (!parseRequestLine(request, method, path, body)) {
            string response = makeHttpResponse(400, "Bad Request", "{\"error\":\"Malformed request\"}");
            send(client, response.c_str(), (int)response.size(), 0);
            closesocket(client);
            continue;
        }

        if (path == "/") path = "/hospital-system.html";
        if (path == "/hospital-system.html") {
            string html;
            if (readFileContents("hospital-system.html", html)) {
                string response = makeHttpResponse(200, "OK", html, "text/html");
                send(client, response.c_str(), (int)response.size(), 0);
            } else {
                string response = makeHttpResponse(404, "Not Found", "<h1>hospital-system.html not found</h1>", "text/html");
                send(client, response.c_str(), (int)response.size(), 0);
            }
            closesocket(client);
            continue;
        }

        string response = handleApiRequest(method, path, body, registry, queue, history, bst, nextId);
        send(client, response.c_str(), (int)response.size(), 0);
        closesocket(client);
    }

    closesocket(listenSocket);
    WSACleanup();
}

int main(int argc, char* argv[]) {
    int port = 8080;
    if (argc > 1) {
        for (int i = 1; i < argc; ++i) {
            string arg = argv[i];
            if (arg == "--port" && i + 1 < argc) {
                port = stoi(argv[++i]);
            }
        }
    }
    runServer(port);
    return 0;
}
