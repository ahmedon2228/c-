#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>

class Entity {
protected:
    std::string name;
    int health;
    int attackPower;
    int defense;

public:
    Entity(const std::string& n, int h, int a, int d)
        : name(n), health(h), attackPower(a), defense(d) {
    }

    const std::string& getName() const { return name; }
    int getHealth() const { return health; }
    int getAttackPower() const { return attackPower; }
    int getDefense() const { return defense; }

    void takeDamage(int damage) {
        health -= damage;
        if (health < 0) health = 0;
    }

    virtual void performAttack(Entity& target) {
        int damage = attackPower - target.getDefense();
        if (damage > 0) {
            target.takeDamage(damage);
            std::cout << name << " attacks " << target.getName() << " for " << damage << " damage!\n";
        }
        else {
            std::cout << name << " attacks " << target.getName() << ", but it has no effect!\n";
        }
    }

    virtual void displayInfo() const {
        std::cout << "Name: " << name << ", HP: " << health
            << ", Attack: " << attackPower << ", Defense: " << defense << std::endl;
    }

    virtual void heal(int amount) {
        health += amount;
        if (health > 100) health = 100;
        std::cout << name << " healed for " << amount << " HP. Current HP: " << health << std::endl;
    }

    virtual ~Entity() {}
};

class Character : public Entity {
public:
    Character(const std::string& n, int h, int a, int d)
        : Entity(n, h, a, d) {
    }

    void performAttack(Entity& target) override {
        int damage = getAttackPower() - target.getDefense();
        if (damage > 0) {
            if (rand() % 100 < 20) {
                damage *= 2;
                std::cout << "Critical hit! ";
            }
            target.takeDamage(damage);
            std::cout << getName() << " attacks " << target.getName() << " for " << damage << " damage!\n";
        }
        else {
            std::cout << getName() << " attacks " << target.getName() << ", but it has no effect!\n";
        }
    }

    void displayInfo() const override {
        std::cout << "Character: " << getName() << ", HP: " << getHealth()
            << ", Attack: " << getAttackPower() << ", Defense: " << getDefense() << std::endl;
    }

    void heal(int amount) override {
        Entity::heal(amount);
        std::cout << getName() << " used a healing potion!" << std::endl;
    }
};

class Monster : public Entity {
public:
    Monster(const std::string& n, int h, int a, int d)
        : Entity(n, h, a, d) {
    }

    void performAttack(Entity& target) override {
        int damage = getAttackPower() - target.getDefense();
        if (damage > 0) {
            if (rand() % 100 < 30) {
                damage += 5;
                std::cout << "Poisonous attack! ";
            }
            target.takeDamage(damage);
            std::cout << getName() << " attacks " << target.getName() << " for " << damage << " damage!\n";
        }
        else {
            std::cout << getName() << " attacks " << target.getName() << ", but it has no effect!\n";
        }
    }

    void displayInfo() const override {
        std::cout << "Monster: " << getName() << ", HP: " << getHealth()
            << ", Attack: " << getAttackPower() << ", Defense: " << getDefense() << std::endl;
    }
};

class Boss : public Monster {
private:
    std::string specialAbility;

public:
    Boss(const std::string& n, int h, int a, int d, const std::string& ability)
        : Monster(n, h, a, d), specialAbility(ability) {
    }

    void performAttack(Entity& target) override {
        if (rand() % 100 < 40) {
            int fireDamage = 15;
            target.takeDamage(fireDamage);
            std::cout << getName() << " uses " << specialAbility << " for " << fireDamage << " fire damage!\n";
        }
        else {
            Monster::performAttack(target);
        }
    }

        void displayInfo() const override {
        Monster::displayInfo();
        std::cout << "Special Ability: " << specialAbility << std::endl;
    }
};

int main() {
    srand(static_cast<unsigned>(time(0)));

    Character hero("Hero", 100, 20, 10);
    Monster goblin("Goblin", 50, 15, 5);
    Boss dragon("Dragon", 150, 25, 20, "Fire Breath");

    Entity* entities[] = { &hero, &goblin, &dragon };

    for (auto& entity : entities) {
        entity->displayInfo();
    }

    hero.performAttack(goblin);
    goblin.performAttack(hero);
    dragon.performAttack(hero);

    hero.heal(20);
    hero.displayInfo();

    return 0;
}