#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

class Entity {
protected:
    std::string name;
    int health;
    int maxHealth;
    int attack;
    int defense;

public:
    Entity(const std::string& n, int h, int a, int d)
        : name(n), health(h), maxHealth(h), attack(a), defense(d) {
    }

    // Виртуальный метод для атаки
    virtual void attack(Entity& target) {
        int damage = attack - target.defense;
        if (damage > 0) {
            target.health -= damage;
            std::cout << name << " attacks " << target.name << " for " << damage << " damage!\n";
        }
        else {
            std::cout << name << " attacks " << target.name << ", but it has no effect!\n";
        }
    }

    // Новый метод для лечения
    virtual void heal(int amount) {
        health += amount;
        if (health > maxHealth) {
            health = maxHealth;
        }
        std::cout << name << " heals " << amount << " HP. Current HP: " << health << "/" << maxHealth << "\n";
    }

    // Виртуальный метод для вывода информации
    virtual void displayInfo() const {
        std::cout << "Name: " << name << ", HP: " << health << "/" << maxHealth
            << ", Attack: " << attack << ", Defense: " << defense << std::endl;
    }

    bool isAlive() const {
        return health > 0;
    }

    // Виртуальный деструктор
    virtual ~Entity() {}
};

class Character : public Entity {
public:
    Character(const std::string& n, int h, int a, int d)
        : Entity(n, h, a, d) {
    }

    // Переопределение метода attack
    void attack(Entity& target) override {
        int damage = attack - target.defense;
        if (damage > 0) {
            // Шанс на критический удар (20%)
            if (rand() % 100 < 20) {
                damage *= 2;
                std::cout << "Critical hit! ";
            }
            target.health -= damage;
            std::cout << name << " attacks " << target.name << " for " << damage << " damage!\n";
        }
        else {
            std::cout << name << " attacks " << target.name << ", but it has no effect!\n";
        }
    }

    // Переопределение метода heal для персонажа
    void heal(int amount) override {
        if (health < maxHealth * 0.5) { // Может лечиться только при HP < 50%
            Entity::heal(amount);
            std::cout << name << " uses a healing potion!\n";
        }
        else {
            std::cout << name << " doesn't need healing right now.\n";
        }
    }

    void displayInfo() const override {
        std::cout << "Character: " << name << ", HP: " << health << "/" << maxHealth
            << ", Attack: " << attack << ", Defense: " << defense << std::endl;
    }
};

class Monster : public Entity {
public:
    Monster(const std::string& n, int h, int a, int d)
        : Entity(n, h, a, d) {
    }

    void attack(Entity& target) override {
        int damage = attack - target.defense;
        if (damage > 0) {
            // Шанс на ядовитую атаку (30%)
            if (rand() % 100 < 30) {
                damage += 5;
                std::cout << "Poisonous attack! ";
            }
            target.health -= damage;
            std::cout << name << " attacks " << target.name << " for " << damage << " damage!\n";
        }
        else {
            std::cout << name << " attacks " << target.name << ", but it has no effect!\n";
        }
    }

    void displayInfo() const override {
        std::cout << "Monster: " << name << ", HP: " << health << "/" << maxHealth
            << ", Attack: " << attack << ", Defense: " << defense << std::endl;
    }
};

class Boss : public Monster {
private:
    bool canUseFireAttack = true;

public:
    Boss(const std::string& n, int h, int a, int d)
        : Monster(n, h, a, d) {
    }

    void attack(Entity& target) override {
        if (canUseFireAttack && rand() % 3 == 0) { // 33% шанс использовать огненную атаку
            fireAttack(target);
            canUseFireAttack = false;
        }
        else {
            Monster::attack(target);
            canUseFireAttack = true;
        }
    }

    void fireAttack(Entity& target) {
        int damage = (attack * 2) - target.defense; // Двойной урон
        if (damage > 0) {
            target.health -= damage;
            std::cout << name << " uses FIRE ATTACK on " << target.name
                << " for " << damage << " massive damage!\n";
        }
        else {
            std::cout << name << "'s fire attack has no effect on " << target.name << "!\n";
        }
    }

    void displayInfo() const override {
        std::cout << "BOSS: " << name << ", HP: " << health << "/" << maxHealth
            << ", Attack: " << attack << ", Defense: " << defense << std::endl;
    }
};

int main() {
    srand(static_cast<unsigned>(time(0)));

    Character hero("Hero", 100, 20, 10);
    Monster goblin("Goblin", 50, 15, 5);
    Boss dragon("Dragon King", 200, 30, 20);

    Entity* entities[] = { &hero, &goblin, &dragon };

    std::cout << "=== Initial Status ===\n";
    for (auto& entity : entities) {
        entity->displayInfo();
    }

    std::cout << "\n=== Battle Starts ===\n";

    // Первый раунд боя
    hero.attack(goblin);
    if (goblin.isAlive()) goblin.attack(hero);
    dragon.attack(hero);

    std::cout << "\n=== After First Round ===\n";
    for (auto& entity : entities) {
        entity->displayInfo();
    }

    // Герой решает полечиться
    std::cout << "\nHero decides to heal:\n";
    hero.heal(30);

    // Второй раунд боя
    std::cout << "\n=== Second Round ===\n";
    hero.attack(dragon);
    if (goblin.isAlive()) goblin.attack(hero);
    dragon.attack(hero);

    std::cout << "\n=== Final Status ===\n";
    for (auto& entity : entities) {
        entity->displayInfo();
    }

    return 0;
}