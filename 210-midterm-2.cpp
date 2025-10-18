#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>
using namespace std;

const int MIN_NR = 10, MAX_NR = 99, MIN_LS = 5, MAX_LS = 20;

class DoublyLinkedList {
private:
    struct Node {
        int data;
        Node* prev;
        Node* next;
        Node(int val, Node* p = nullptr, Node* n = nullptr) {
            data = val; 
            prev = p;
            next = n;
        }
    };

    Node* head;
    Node* tail;

public:
    DoublyLinkedList() { head = nullptr; tail = nullptr; }

    void insert_after(int value, int position) {
        if (position < 0) {
            cout << "Position must be >= 0." << endl;
            return;
        }

        Node* newNode = new Node(value);
        if (!head) {
            head = tail = newNode;
            return;
        }

        Node* temp = head;
        for (int i = 0; i < position && temp; ++i)
            temp = temp->next;

        if (!temp) {
            cout << "Position exceeds list size. Node not inserted.\n";
            delete newNode;
            return;
        }

        newNode->next = temp->next;
        newNode->prev = temp;
        if (temp->next)
            temp->next->prev = newNode;
        else
            tail = newNode;
        temp->next = newNode;
    }

    void delete_val(int value) {
        if (!head) return;

        Node* temp = head;
        
        while (temp && temp->data != value)
            temp = temp->next;

        if (!temp) return; 

        if (temp->prev)
            temp->prev->next = temp->next;
        else
            head = temp->next; 

        if (temp->next)
            temp->next->prev = temp->prev;
        else
            tail = temp->prev; 

        delete temp;
    }

    void delete_pos(int pos) {
        if (!head) {
            cout << "List is empty." << endl;
            return;
        }
    
        if (pos == 1) {
            pop_front();
            return;
        }
    
        Node* temp = head;
    
        for (int i = 1; i < pos; i++){
            if (!temp) {
                cout << "Position doesn't exist." << endl;
                return;
            }
            else
                temp = temp->next;
        }
        if (!temp) {
            cout << "Position doesn't exist." << endl;
            return;
        }
    
        if (!temp->next) {
            pop_back();
            return;
        }
    
        Node* tempPrev = temp->prev;
        tempPrev->next = temp->next;
        temp->next->prev = tempPrev;
        delete temp;
    }

    void push_back(int v) {
        Node* newNode = new Node(v);
        if (!tail)
            head = tail = newNode;
        else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
    }
    
    void push_front(int v) {
        Node* newNode = new Node(v);
        if (!head)
            head = tail = newNode;
        else {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
    }
    
    void pop_front() {

        if (!head) {
            cout << "List is empty." << endl;
            return;
        }

        Node * temp = head;

        if (head->next) {
            head = head->next;
            head->prev = nullptr;
        }
        else
            head = tail = nullptr;
        delete temp;
    }

    void pop_back() {
        if (!tail) {
            cout << "List is empty." << endl;
            return;
        }
        Node * temp = tail;

        if (tail->prev) {
            tail = tail->prev;
            tail->next = nullptr;
        }
        else
            head = tail = nullptr;
        delete temp;
    }

    ~DoublyLinkedList() {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }
    void print() {
        Node* current = head;
        if (!current) {
            cout << "List is empty." << endl;
            return;
        }
        while (current) {
            cout << current->data << " ";
            current = current->next;
        }
        cout << endl;
    }

    void print_reverse() {
        Node* current = tail;
        if (!current) { 
            cout << "List is empty." << endl;
            return;
        }
        while (current) {
            cout << current->data << " ";
            current = current->prev;
        }
        cout << endl;
    }
};

int main() {
    srand(time(0));

    vector<string> names;
    ifstream file("names.txt");
    if (!file) {
        cout << "Could not open names.txt, please try again.\n";
        return 1;
    }

    string name;
    while (file >> name)
        names.push_back(name);
    file.close();

    DoublyLinkedList line;
    vector<string> lineNames;

    cout << "Store is open:\n";

    for (int minute = 1; minute <= 20; ++minute) {
        cout << "\nTime step #" << minute << ":\n";

        if (!lineNames.empty() && (rand() % 100 + 1) <= 40) {
            cout << "   " << lineNames.front() << " is served\n";
            line.pop_front();
            lineNames.erase(lineNames.begin());
        }

        if ((rand() % 100 + 1) <= 10) {
            string vip = names[rand() % names.size()];
            cout << "   " << vip << " (VIP) joins the front of the line\n";
            line.push_front(rand() % 90 + 10);
            lineNames.insert(lineNames.begin(), vip);
        }

        if ((rand() % 100 + 1) <= 60) {
            string newcomer = names[rand() % names.size()];
            cout << "   " << newcomer << " joins the line\n";
            line.push_back(rand() % 90 + 10);
            lineNames.push_back(newcomer);
        }

        if (!lineNames.empty() && (rand() % 100 + 1) <= 20) {
            cout << "   " << lineNames.back() << " (at the rear) left the line\n";
            line.pop_back();
            lineNames.pop_back();
        }

        if (lineNames.size() > 2 && (rand() % 100 + 1) <= 10) {
            int pos = rand() % lineNames.size();
            cout << "   " << lineNames[pos] << " left the line\n";
            line.delete_pos(pos + 1);
            lineNames.erase(lineNames.begin() + pos);
        }

        cout << " Resulting line:\n";
        if (lineNames.empty())
            cout << "   (empty)\n";
        else
            for (string n : lineNames)
                cout << "   " << n << "\n";

    }
// COULD NOT FIGURE OUT HOW TO DO IT ALL IN 1 MIN INTERVALS. FEEDBACK IS APPRECIATED
    cout << "\nSimulation complete.\n";
    return 0;
}
