#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>

class Entity {
public:
    virtual ~Entity() = default;
    virtual void display() const = 0;
    virtual std::string serialize() const = 0;
    virtual void deserialize(const std::string& data) = 0;
};

class Player : public Entity {
    std::string name;
    int health;
    int level;

public:
    Player(const std::string& name = "", int health = 0, int level = 0)
        : name(name), health(health), level(level) {
    }

    void display() const override {
        std::cout << "Player: " << name << ", Health: " << health
            << ", Level: " << level << std::endl;
    }

    std::string serialize() const override {
        return "Player " + name + " " + std::to_string(health) + " " + std::to_string(level);
    }

    void deserialize(const std::string& data) override {
        size_t pos = data.find(' ');
        if (pos == std::string::npos || data.substr(0, pos) != "Player") {
            throw std::runtime_error("Invalid player data format");
        }

        size_t next_pos = data.find(' ', pos + 1);
        name = data.substr(pos + 1, next_pos - pos - 1);

        pos = next_pos;
        next_pos = data.find(' ', pos + 1);
        health = std::stoi(data.substr(pos + 1, next_pos - pos - 1));

        level = std::stoi(data.substr(next_pos + 1));
    }
};

template<typename T>
class GameManager {
    std::vector<std::unique_ptr<T>> entities;

public:
    void addEntity(std::unique_ptr<T> entity) {
        entities.push_back(std::move(entity));
    }

    void displayAll() const {
        for (const auto& entity : entities) {
            entity->display();
        }
    }

    void saveToFile(const std::string& filename) const {
        std::ofstream file(filename);
        if (!file) {
            throw std::runtime_error("Failed to open file for writing.");
        }

        for (const auto& entity : entities) {
            file << entity->serialize() << "\n";
        }
    }

    void loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file) {
            throw std::runtime_error("Failed to open file for reading.");
        }

        entities.clear();
        std::string line;
        while (std::getline(file, line)) {
            if (line.empty()) continue;

            size_t type_pos = line.find(' ');
            if (type_pos == std::string::npos) {
                throw std::runtime_error("Invalid data format in file");
            }

            std::string type = line.substr(0, type_pos);

            if (type == "Player") {
                auto player = std::make_unique<Player>();
                player->deserialize(line);
                addEntity(std::move(player));
            }
            // Добавьте другие типы сущностей здесь
        }
    }
};

int main() {
    // Тестирование сохранения и загрузки
    GameManager<Entity> manager;
    manager.addEntity(std::make_unique<Player>("Hero", 85, 1));
    manager.addEntity(std::make_unique<Player>("Mage", 70, 3));
    manager.addEntity(std::make_unique<Player>("Warrior", 110, 2));

    std::cout << "Original entities:" << std::endl;
    manager.displayAll();

    try {
        manager.saveToFile("game_save.txt");

        GameManager<Entity> loadedManager;
        loadedManager.loadFromFile("game_save.txt");

        std::cout << "\nLoaded entities:" << std::endl;
        loadedManager.displayAll();
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}