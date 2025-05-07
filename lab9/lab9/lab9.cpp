#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <memory>
#include <ctime>
#include <cstdlib>
#include <conio.h>
#include <sstream>

#define _CRT_SECURE_NO_WARNINGS

using namespace std;

// Шаблонный класс Logger
template<typename T>
class Logger {
private:
    ofstream logFile;
public:
    Logger(const string& filename) {
        logFile.open(filename, ios::app);
        if (!logFile.is_open()) {
            throw runtime_error("Unable to open log file");
        }
    }

    ~Logger() {
        if (logFile.is_open()) {
            logFile.close();
        }
    }

    void log(const T& message) {
        time_t now = time(0);
        char dt[26];
        ctime_s(dt, sizeof(dt), &now);
        dt[24] = '\0';
        logFile << "[" << dt << "] " << message << endl;
        cout << "[" << dt << "] " << message << endl; // Также выводим в консоль
    }
};

// Базовый класс предмета
class Item {
public:
    virtual ~Item() = default;
    virtual void use(class Character& character, Logger<string>& logger) = 0;
    virtual string getName() const = 0;
    virtual string getDescription() const = 0;
    virtual string serialize() const = 0;
    static unique_ptr<Item> deserialize(const string& data);
};

// Зелье здоровья
class HealthPotion : public Item {
private:
    string name;
    string description;
    int healAmount;
public:
    HealthPotion(const string& n, const string& desc, int ha)
        : name(n), description(desc), healAmount(ha) {
    }

    HealthPotion(const string& data) {
        istringstream iss(data);
        getline(iss, name, ':');
        getline(iss, description, ':');
        iss >> healAmount;
    }

    void use(Character& character, Logger<string>& logger) override;
    string getName() const override { return name; }
    string getDescription() const override { return description; }
    int getHealAmount() const { return healAmount; }

    string serialize() const override {
        return "HealthPotion:" + name + ":" + description + ":" + to_string(healAmount);
    }
};

// Оружие
class Weapon : public Item {
private:
    string name;
    string description;
    int attackBonus;
public:
    Weapon(const string& n, const string& desc, int ab)
        : name(n), description(desc), attackBonus(ab) {
    }

    Weapon(const string& data) {
        istringstream iss(data);
        getline(iss, name, ':');
        getline(iss, description, ':');
        iss >> attackBonus;
    }

    void use(Character& character, Logger<string>& logger) override;
    string getName() const override { return name; }
    string getDescription() const override { return description; }
    int getAttackBonus() const { return attackBonus; }

    string serialize() const override {
        return "Weapon:" + name + ":" + description + ":" + to_string(attackBonus);
    }
};

unique_ptr<Item> Item::deserialize(const string& data) {
    if (data.find("HealthPotion") == 0) {
        return make_unique<HealthPotion>(data);
    }
    else if (data.find("Weapon") == 0) {
        return make_unique<Weapon>(data);
    }
    throw runtime_error("Unknown item type in save file");
}

class Character; // Предварительное объявление

// Класс инвентаря
class Inventory {
private:
    vector<unique_ptr<Item>> items;
public:
    void addItem(unique_ptr<Item> item) {
        items.push_back(move(item));
    }

    void useItem(int index, Character& character, Logger<string>& logger);

    void display() const {
        if (items.empty()) {
            cout << "Inventory is empty.\n";
            return;
        }

        cout << "=== Inventory ===\n";
        for (size_t i = 0; i < items.size(); ++i) {
            cout << i + 1 << ". " << items[i]->getName()
                << " - " << items[i]->getDescription() << "\n";
        }
        cout << "=================\n";
    }

    size_t size() const { return items.size(); }
    const vector<unique_ptr<Item>>& getItems() const { return items; }
};

class Character {
protected:
    string name;
    int health;
    int maxHealth;
    int attack;
    int defense;
    int level;
    int experience;
    Inventory inventory;
    int baseAttack;

public:
    Character(const string& n, int h, int a, int d)
        : name(n), health(h), maxHealth(h), attack(a), defense(d),
        level(1), experience(0), baseAttack(a) {
    }

    virtual ~Character() = default;

    virtual void attackEnemy(Character& enemy, Logger<string>& logger) {
        int damage = attack - enemy.defense;
        if (damage > 0) {
            enemy.health -= damage;
            string msg = name + " attacks " + enemy.name + " for " + to_string(damage) + " damage!";
            cout << msg << "\n";
            logger.log(msg);
        }
        else {
            string msg = name + " attacks " + enemy.name + ", but it has no effect!";
            cout << msg << "\n";
            logger.log(msg);
        }
    }

    void takeDamage(int damage) {
        health -= damage;
        if (health < 0) health = 0;
    }

    void heal(int amount, Logger<string>& logger) {
        int oldHealth = health;
        health += amount;
        if (health > maxHealth) health = maxHealth;
        string msg = name + " heals for " + to_string(health - oldHealth) + " HP!";
        cout << msg << "\n";
        logger.log(msg);
    }

    void gainExperience(int exp, Logger<string>& logger) {
        experience += exp;
        if (experience >= 100) {
            level++;
            experience -= 100;
            maxHealth += 10;
            health = maxHealth;
            baseAttack += 2;
            attack = baseAttack;
            defense += 1;
            string msg = name + " leveled up to level " + to_string(level) + "!";
            cout << msg << "\n";
            logger.log(msg);
        }
    }

    void addToInventory(unique_ptr<Item> item) {
        inventory.addItem(move(item));
    }

    void useItem(int index, Logger<string>& logger) {
        try {
            inventory.useItem(index - 1, *this, logger);
        }
        catch (const out_of_range& e) {
            string msg = "Error using item: " + string(e.what());
            cout << msg << "\n";
            logger.log(msg);
        }
    }

    void displayInventory() const {
        inventory.display();
    }

    void displayStats() const {
        cout << "=== " << name << " ===\n"
            << "HP: " << health << "/" << maxHealth << "\n"
            << "Attack: " << attack << "\n"
            << "Defense: " << defense << "\n"
            << "Level: " << level << "\n"
            << "Exp: " << experience << "/100\n";
    }

    bool isAlive() const { return health > 0; }

    string getName() const { return name; }
    int getHealth() const { return health; }
    int getMaxHealth() const { return maxHealth; }
    int getAttack() const { return attack; }
    int getDefense() const { return defense; }
    int getLevel() const { return level; }
    int getExperience() const { return experience; }
    Inventory& getInventory() { return inventory; }

    void setAttack(int a) { attack = a; }
    void setHealth(int h) { health = h; }

    void saveToFile(const string& filename) const {
        ofstream out(filename);
        if (!out) {
            throw runtime_error("Failed to open save file");
        }

        out << name << "\n"
            << health << "\n"
            << maxHealth << "\n"
            << attack << "\n"
            << defense << "\n"
            << level << "\n"
            << experience << "\n"
            << baseAttack << "\n";

        // Сохраняем инвентарь
        out << inventory.size() << "\n";
        for (const auto& item : inventory.getItems()) {
            out << item->serialize() << "\n";
        }

        if (!out) {
            throw runtime_error("Failed to write save data");
        }
    }

    void loadFromFile(const string& filename) {
        ifstream in(filename);
        if (!in) {
            throw runtime_error("Failed to open save file");
        }

        in >> name
            >> health
            >> maxHealth
            >> attack
            >> defense
            >> level
            >> experience
            >> baseAttack;

        // Загружаем инвентарь
        size_t inventorySize;
        in >> inventorySize;
        in.ignore(); // Пропускаем оставшийся символ новой строки

        for (size_t i = 0; i < inventorySize; ++i) {
            string itemData;
            getline(in, itemData);
            addToInventory(Item::deserialize(itemData));
        }

        if (!in) {
            throw runtime_error("Failed to read save data");
        }
    }
};

void Inventory::useItem(int index, Character& character, Logger<string>& logger) {
    if (index < 0 || index >= items.size()) {
        throw out_of_range("Invalid item index");
    }
    items[index]->use(character, logger);
    string msg = "Used item: " + items[index]->getName();
    logger.log(msg);
    items.erase(items.begin() + index);
}

void HealthPotion::use(Character& character, Logger<string>& logger) {
    character.heal(healAmount, logger);
    cout << character.getName() << " used " << name
        << " and recovered " << healAmount << " HP!\n";
}

void Weapon::use(Character& character, Logger<string>& logger) {
    character.setAttack(character.getAttack() + attackBonus);
    string msg = character.getName() + " equipped " + name
        + " and gained +" + to_string(attackBonus) + " attack!";
    cout << msg << "\n";
    logger.log(msg);
}

class Monster : public Character {
private:
    bool hasPotion;
public:
    Monster(const string& n, int h, int a, int d, bool hp = false)
        : Character(n, h, a, d), hasPotion(hp) {
    }

    bool dropPotion() const { return hasPotion; }
};

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pause() {
    cout << "\nPress any key to continue...";
    _getch();
    cout << "\n";
}

void postBattleMenu(Character& hero, Logger<string>& logger) {
    while (true) {
        clearScreen();
        hero.displayStats();
        hero.displayInventory();

        cout << "\n=== Post Battle Menu ===\n"
            << "1. Use item\n"
            << "2. Continue fighting\n"
            << "3. Save and quit\n"
            << "Choose option: ";

        int choice;
        cin >> choice;
        cin.ignore(); // Очищаем буфер ввода

        switch (choice) {
        case 1: {
            if (hero.getInventory().size() == 0) {
                cout << "No items in inventory!\n";
                pause();
                break;
            }

            clearScreen();
            hero.displayInventory();
            cout << "Enter item number to use (0 to cancel): ";
            int itemChoice;
            cin >> itemChoice;
            cin.ignore();

            if (itemChoice > 0 && itemChoice <= hero.getInventory().size()) {
                hero.useItem(itemChoice, logger);
                pause();
            }
            break;
        }
        case 2:
            return;
        case 3:
            hero.saveToFile("hero_save.txt");
            cout << "Game saved. Goodbye!\n";
            exit(0);
        default:
            cout << "Invalid choice!\n";
            pause();
        }
    }
}

void battle(Character& hero, Monster& monster, Logger<string>& logger) {
    clearScreen();
    cout << "=== BATTLE ===\n";
    hero.displayStats();
    monster.displayStats();
    cout << "================\n\n";

    while (hero.isAlive() && monster.isAlive()) {
        cout << "1. Attack\n"
            << "2. Use item\n"
            << "Choose action: ";

        int choice;
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            hero.attackEnemy(monster, logger);
        }
        else if (choice == 2) {
            if (hero.getInventory().size() == 0) {
                cout << "No items in inventory!\n";
                pause();
                continue;
            }

            hero.displayInventory();
            cout << "Enter item number to use (0 to cancel): ";
            int itemChoice;
            cin >> itemChoice;
            cin.ignore();

            if (itemChoice > 0 && itemChoice <= hero.getInventory().size()) {
                hero.useItem(itemChoice, logger);
            }
            continue;
        }
        else {
            cout << "Invalid choice!\n";
            pause();
            continue;
        }

        if (!monster.isAlive()) {
            string msg = monster.getName() + " has been defeated!";
            cout << msg << "\n";
            logger.log(msg);

            int expGain = 30;
            hero.gainExperience(expGain, logger);

            if (monster.dropPotion()) {
                hero.addToInventory(make_unique<HealthPotion>(
                    "Health Potion", "Restores 30 HP", 30));
                cout << "Got Health Potion from defeated enemy!\n";
            }

            postBattleMenu(hero, logger);
            return;
        }

        monster.attackEnemy(hero, logger);

        if (!hero.isAlive()) {
            string msg = hero.getName() + " has been defeated!";
            cout << msg << "\n";
            logger.log(msg);
            cout << "Game Over!\n";
            exit(0);
        }

        pause();
        clearScreen();
        hero.displayStats();
        monster.displayStats();
    }
}

int main() {
    try {
        Logger<string> gameLogger("game_log.txt");
        gameLogger.log("Game started");

        Character hero("Hero", 100, 15, 10);

        // Проверяем наличие сохранения
        ifstream testFile("hero_save.txt");
        if (testFile.good()) {
            testFile.close();
            cout << "Found saved game. Load it? (y/n): ";
            char choice;
            cin >> choice;
            cin.ignore();
            if (choice == 'y' || choice == 'Y') {
                hero.loadFromFile("hero_save.txt");
                cout << "Game loaded successfully!\n";
                pause();
            }
        }

        // Стартовые предметы
        hero.addToInventory(make_unique<HealthPotion>(
            "Small Health Potion", "Restores 20 HP", 20));
        hero.addToInventory(make_unique<Weapon>(
            "Iron Sword", "Basic sword (+3 attack)", 3));

        while (true) {
            bool hasPotion = rand() % 2 == 0;
            Monster monster("Goblin", 50, 10, 5, hasPotion);

            battle(hero, monster, gameLogger);
        }

    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}