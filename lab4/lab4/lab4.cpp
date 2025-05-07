#include <iostream>
#include <memory>
#include <string>
#include <utility>

class Inventory {
private:
    std::unique_ptr<std::string[]> items;
    size_t capacity;
    size_t size;

    void resize() {
        capacity *= 2;
        auto newItems = std::make_unique<std::string[]>(capacity);
        for (size_t i = 0; i < size; ++i) {
            newItems[i] = items[i];
        }
        items = std::move(newItems);
    }

public:
    Inventory() : capacity(10), size(0) {
        items = std::make_unique<std::string[]>(capacity);
    }

    void addItem(const std::string& item) {
        if (size == capacity) {
            resize();
        }
        items[size++] = item;
    }

    void displayInventory() const {
        std::cout << "Inventory contents:" << std::endl;
        for (size_t i = 0; i < size; ++i) {
            std::cout << "- " << items[i] << std::endl;
        }
    }
};

int main() {
    Inventory inventory;

    // Добавляем предметы в инвентарь
    inventory.addItem("Sword");
    inventory.addItem("Health Potion");
    inventory.addItem("Mana Potion");
    inventory.addItem("Shield");
    inventory.addItem("Bow");

    // Отображаем содержимое инвентаря
    inventory.displayInventory();

    return 0;
}