#include <iostream>
#include <vector>
#include <string>

template <typename T>
class Queue {
private:
    std::vector<T> elements;

public:
    // Добавление элемента в очередь
    void push(const T& item) {
        elements.push_back(item);
    }

    // Удаление элемента из очереди
    void pop() {
        if (!elements.empty()) {
            elements.erase(elements.begin());
        }
    }

    // Вывод очереди на экран
    void print() const {
        std::cout << "Очередь: [";
        for (size_t i = 0; i < elements.size(); ++i) {
            std::cout << elements[i];
            if (i != elements.size() - 1) {
                std::cout << ", ";
            }
        }
        std::cout << "]" << std::endl;
    }
};

int main() {
    // Тестирование очереди для целых чисел
    Queue<int> intQueue;

    std::cout << "Очередь целых чисел:" << std::endl;
    intQueue.push(10);
    intQueue.push(20);
    intQueue.push(30);
    intQueue.print();  // Очередь: [10, 20, 30]

    intQueue.pop();
    intQueue.print();  // Очередь: [20, 30]

    // Тестирование очереди для строк
    Queue<std::string> stringQueue;

    std::cout << "\nОчередь строк:" << std::endl;
    stringQueue.push("Hello");
    stringQueue.push("World");
    stringQueue.push("!");
    stringQueue.print();  // Очередь: [Hello, World, !]

    stringQueue.pop();
    stringQueue.print();  // Очередь: [World, !]

    return 0;
}