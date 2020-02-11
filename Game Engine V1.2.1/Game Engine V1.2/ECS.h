#pragma once

//ECS = entity component system

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>

using namespace std;

class Component;
class Entity;

// Replaces the word ComponentID into size_t
// size_t means that it is an unsigned integer type
// It works fine if ComponentID is replaced with int
using ComponentID = size_t;


inline ComponentID getNewComponentTypeID() {
	// Static means that the variable will be allocated to the memory once. It can be updated,
	// but not replaced. 
	static ComponentID lastID = 0u;
	return lastID++;
}

// The noexcept is not needed, as it just checks to ensure that the compiler will not throw an exception
template <typename T> inline ComponentID getComponentTypeID() noexcept {
	static ComponentID typeID = getNewComponentTypeID();
	return typeID;
}

// The first getComponentTypeID() can be deleted and the second one can replace it by modifying 
// static ComponentID typeID = getComponentTypeID() ...into ...
// static ComponentID typeID = 0;
// The inline is also not needed. Used in the old days, but compiler now is good enough to not need it


constexpr size_t maxComponents = 32;

// Creates arrays and bitsets that would take in components up to max of 32 and would replace
// the words on the left to the words in the right
using ComponentBitSet = bitset<maxComponents>;
using ComponentArray = array<Component*, maxComponents>;

// A component is the tag for the entity. 
class Component {
public:

	// Also remember that "virtual" is an important keyword so the program knows that this function is to be overridden
	// Wihtout virutal, Component* c, ComponentChild d, c = &d, c->init() will run the Component init()
	// With virtual, Component*c, ComponentChild d, c = &d, c->init() will run the ComponentChild init()
	Entity* entity;
	virtual void init() {}
	virtual void update() {}
	virtual void draw() {}
	virtual ~Component() {}
};

// An entity is the actual object of the game
class Entity {
private:
	// The Entity is defaulted as active
	bool active = true;

	// There is a list of components that the entity has, and will make a vecotr, array and bitset
	vector<unique_ptr<Component>> components;
	ComponentArray componentArray;
	ComponentBitSet componentBitSet;

public:

	// For every component of the entity, it will update based on that
	void update() {
		for (auto& c : components) c->update();
	}

	void draw() {
		for (auto& c : components) c->draw();
	}

	// Return the active function
	bool isActive() const { 
		return active; 
	}

	// Change the active into false
	void destroy() { 
		active = false;  
	}

	//Not needed
	// Also, the const meanst that the compiler will make sure that the function will not alter the Entity
	template <typename T> bool hasComponent() const {
		return componentBitSet[getComponentTypeID<T>()];
	}

	// The T and ... TArgs indicate that the function addComponent takes in 1 or more arguments
	// Also, the && will refer to the VALUE of teh variable, not the memory address.
	template <typename T, typename... TArgs> T& addComponent(TArgs&&... mArgs) {
		T* c(new T(forward<TArgs>(mArgs)...));
		c->entity = this;
		unique_ptr<Component> uPtr{ c };
		components.emplace_back(move(uPtr));

		componentArray[getComponentTypeID<T>()] = c;
		componentBitSet[getComponentTypeID<T>()] = true;

		c->init();
		return *c;
	}

	template<typename T> T& getComponent() const{
		auto ptr(componentArray[getComponentTypeID<T>()]);
		return *static_cast<T*>(ptr);
	}
};

// A Manager is a vector of entities
class Manager {
private:
	vector<unique_ptr<Entity>> entities;

public:
	void update() {
		for (auto& e : entities) e->update();
	}
	void draw() {
		for (auto& e : entities) e->draw();
	}
	void refresh() {
		entities.erase(remove_if(begin(entities), end(entities),
			[](const unique_ptr<Entity> &mEntity) {
			return !mEntity->isActive();
		}),
			end(entities));
	}

	Entity& addEntity() {
		Entity* e = new Entity;
		unique_ptr<Entity> uPtr{ e };
		entities.emplace_back(move(uPtr));
		return *e;
	}
};
