#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>
#include <cstdlib>

class Character {
public:
    std::string name;
    int health;
    int attack;
    int defense;

    Character(std::string n, int h, int a, int d)
        : name(n), health(h), attack(a), defense(d) {
    }

    void displayInfo() {
        std::cout << name << " - Health: " << health
            << ", Attack: " << attack
            << ", Defense: " << defense << std::endl;
    }

    bool isAlive() const {
        return health > 0;
    }
};

class Monster {
public:
    std::string name;
    int health;
    int attack;
    int defense;

    Monster(std::string n, int h, int a, int d)
        : name(n), health(h), attack(a), defense(d) {
    }

    void displayInfo() {
        std::cout << name << " - Health: " << health
            << ", Attack: " << attack
            << ", Defense: " << defense << std::endl;
    }

    bool isAlive() const {
        return health > 0;
    }
};

std::vector<Monster> monsters;
std::mutex monstersMutex;
std::mutex fightMutex;

void generateMonsters() {
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(3));
        std::lock_guard<std::mutex> lock(monstersMutex);
        monsters.push_back(Monster("Goblin", 50, 15, 5));
        std::cout << "New monster generated!\n";
    }
}

void fightMonster(Character& hero, Monster& monster) {
    while (hero.isAlive() && monster.isAlive()) {
        std::this_thread::sleep_for(std::chrono::seconds(1));

        std::lock_guard<std::mutex> lock(fightMutex);

        // Hero attacks monster
        if (monster.isAlive()) {
            int damage = hero.attack - monster.defense;
            if (damage > 0) {
                monster.health -= damage;
                if (monster.health < 0) monster.health = 0;
                std::cout << hero.name << " hits " << monster.name
                    << " for " << damage << " damage! ";
                monster.displayInfo();
            }
        }

        // Monster attacks hero if still alive
        if (monster.isAlive() && hero.isAlive()) {
            int damage = monster.attack - hero.defense;
            if (damage > 0) {
                hero.health -= damage;
                if (hero.health < 0) hero.health = 0;
                std::cout << monster.name << " hits " << hero.name
                    << " for " << damage << " damage! ";
                hero.displayInfo();
            }
        }
    }

    std::lock_guard<std::mutex> lock(fightMutex);
    if (!hero.isAlive()) {
        std::cout << hero.name << " has been defeated by " << monster.name << "!\n";
    }
    else {
        std::cout << monster.name << " has been defeated by " << hero.name << "!\n";
    }
}

int main() {
    std::thread monsterGenerator(generateMonsters);
    monsterGenerator.detach();

    Character hero("Hero", 100, 20, 10);

    while (hero.isAlive()) {
        std::this_thread::sleep_for(std::chrono::seconds(1));

        // Check for new monsters to fight
        std::unique_lock<std::mutex> monsterLock(monstersMutex);
        if (!monsters.empty()) {
            Monster monster = monsters.back(); // Copy the monster
            monsters.pop_back();
            monsterLock.unlock(); // Unlock as soon as possible

            std::cout << "\nStarting fight between " << hero.name
                << " and " << monster.name << "!\n";

            std::thread fightThread(fightMonster, std::ref(hero), std::ref(monster));
            fightThread.detach();
        }
        else {
            monsterLock.unlock();
        }

        // Display hero info
        {
            std::lock_guard<std::mutex> lock(fightMutex);
            std::cout << "\nCurrent status: ";
            hero.displayInfo();
        }
    }

    std::cout << "Game over! " << hero.name << " has been defeated.\n";
    return 0;
}