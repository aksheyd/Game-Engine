#ifndef GAME_OBJECT
#define GAME_OBJECT

#include <vector>
#include <string>
#include "Object.h"
#include "Component.h"
#include "Transform.h"
class Transform;

class GameObject : public Object {
	std::vector<Component*> components;
	bool active = true;

public:
	Transform* transform; // every game object needs a transform

	// make sure that only transform is class transform
	GameObject() : Object("") { 
		this->AddComponent<Transform>();
		transform = this->GetComponent<Transform>();
	}

	GameObject(const std::string& _name) : Object(_name)  { 
		this->AddComponent<Transform>();
		transform = this->GetComponent<Transform>();
	}

	// TODO: move this to behaviour / monobehaviour
	// myGameObject.Update(); 
	// Call individual components's Update() 
	void Update() {
		size_t num_components = components.size();

		for (size_t i = 0; i < num_components; i++)
		{
			components[i]->Update();
		}
	}

	// myGameObject.AddComponent<Transform>();
	// Returns nullptr if errors, component if succeeded
    template<class T>
    Component* AddComponent() {
        // Check if a component of type T already exists in vector
		if (GetComponent<T>() != nullptr) { 
			return nullptr; 
		}

        // If no component of type T exists, create a new one
        Component* new_component = new T;

		// Link gameobject and transforms to this object
		new_component->gameObject = this;
		new_component->transform = transform;

        components.push_back(new_component);
        new_component->Start();
        return new_component;
    }
	
	// Transform* t = myGameObject.GetComponent<Transform>();
	// Returns nullptr if errors, component if succeeded
	template<class T>
	T* GetComponent() {
		size_t num_components = components.size();

		for (auto* component : components) {
			if (dynamic_cast<T*>(component) != nullptr) {
				return static_cast<T*>(component);  // Found a component of type T
			}
		}
		return nullptr;
	}

	void SetActive(bool _newActive) {
		active = _newActive;
	}
};

#endif