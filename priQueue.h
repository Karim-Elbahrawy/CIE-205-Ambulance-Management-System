#pragma once
#include <iostream>
using namespace std;


template < typename T>
class priNode
{
private:
    T item;		// A data item
    int pri;	//priority of the item
    priNode<T>* next; // Pointer to next node
public:
    priNode(const T& r_Item, int PRI)
    {
        setItem(r_Item, PRI);
        next = nullptr;
    }
    void setItem(const T& r_Item, int PRI)
    {
        item = r_Item;
        pri = PRI;
    }
    void setNext(priNode<T>* nextNodePtr)
    {
        next = nextNodePtr;
    }

    T getItem(int& PRI) const
    {
        PRI = pri;
        return item;
    }

    priNode<T>* getNext() const
    {
        return next;
    }

    int getPri() const
    {
        return pri;
    }
}; // end Node

//This class impelements the priority queue as a sorted list (Linked List)
//The item with highest priority is at the front of the queue
template <typename T>
class priQueue
{
    priNode<T>* head;
    int count;
public:
    priQueue() : head(nullptr), count(0) {}
    priQueue(const priQueue<T>& other) : head(nullptr), count(0) {
        priNode<T>* current = other.head;
        while (current) {
            int priority = current->getPri();
            T data = current->getItem(priority);
            enqueue(data, priority); // uses enqueue method that creates new nodes
            current = current->getNext();
        }
    }

    ~priQueue() {
        T tmp;
        int p;
        while (dequeue(tmp, p));
    }
    void print() const;  // <------------------------------------------------- Print Function


    //insert the new node in its correct position according to its priority
    void enqueue(const T& data, int priority) {
        priNode<T>* newNode = new priNode<T>(data, priority);

        if (head == nullptr || priority > head->getPri()) {

            newNode->setNext(head);
            head = newNode;
            count++;
            return;
        }

        priNode<T>* current = head;
        while (current->getNext() && priority <= current->getNext()->getPri()) {
            current = current->getNext();
        }
        newNode->setNext(current->getNext());
        current->setNext(newNode);
        count++;
    }

    bool dequeue(T& topEntry, int& pri) {
        if (isEmpty())
            return false;

        topEntry = head->getItem(pri);
        priNode<T>* temp = head;
        head = head->getNext();
        delete temp;
        count--;
        return true;
    }

    bool peek(T& topEntry, int& pri) {
        if (isEmpty())
            return false;

        topEntry = head->getItem(pri);
        pri = head->getPri();
        return true;
    }

    int size() {
        return count;
    }
    bool isEmpty() const {
        return head == nullptr;
    }
};

///////////////////////////////////////////////////////////////////// Print Function ///////////////////////////////////////////////////

template <typename T>
void priQueue<T>::print() const
{
    if (isEmpty()) {
        cout << "Empty Priority Queue" << endl;
        return;
    }

    priNode<T>* current = head;
    cout << "Priority Queue Contents: ";
    while (current != nullptr) {
        int pri;
        T item = current->getItem(pri);
        cout << "(" << item << ", Priority:" << pri << ") ";
        current = current->getNext();
    }
    cout << endl;
}
