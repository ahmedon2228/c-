#include <iostream>
#include <stdexcept>
#include <vector>

template <typename T>
class Queue {
private:
    std::vector<T> elements;

public:
    void push(const T& element) {
        elements.push_back(element);
    }

    T pop() {
        if (elements.empty()) {
            throw std::out_of_range("Cannot pop from an empty queue");
        }
        T front = elements.front();
        elements.erase(elements.begin());
        return front;
    }

    bool isEmpty() const {
        return elements.empty();
    }
};

int main() {
    try {
        Queue<int> intQueue;

        // Попытка извлечь элемент из пустой очереди - вызовет исключение
        intQueue.pop();
    }
    catch (const std::out_of_range& e) {
        std::cerr << "Queue error: " << e.what() << std::endl;
    }

    try {
        Queue<std::string> stringQueue;

        stringQueue.push("Hello");
        stringQueue.push("World");

        std::cout << stringQueue.pop() << std::endl; // Hello
        std::cout << stringQueue.pop() << std::endl; // World

        // Еще одна попытка извлечь элемент из пустой очереди
        stringQueue.pop();
    }
    catch (const std::out_of_range& e) {
        std::cerr << "Queue error: " << e.what() << std::endl;
    }

    return 0;
}