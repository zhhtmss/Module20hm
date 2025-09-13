#include <iostream>
#include <string>

class Pet {
protected:
	std::string name_;
	int age_;
	double weight_;
public:
	Pet(std::string n, int a, double w) : name_(n), age_(a), weight_(w) {}
	virtual void makeSound() const {
		std::cout << "Some generic pet sound\n";
	}
	
	friend std::ostream& operator<<(std::ostream& os, const Pet& pet) {
		os << "Name: " << pet.name_ << ", Age: " << pet.age_ << ", Weight: " << pet.weight_ << "kg";
		return os;
	}

};

class Dog : public Pet {
private:
	std::string breed_;
	int activityLevel_;
public:
	Dog(std::string n, int a, double w, std::string b, int al)
		: Pet(n, a, w), breed_(b), activityLevel_(al) {
	}
	void makeSound() const override {
		std::cout << "Woof! Woof!\n";
	}
	
	friend std::ostream& operator<<(std::ostream& os, const Dog& dog) {
		os << (Pet)dog << ", Breed: " << dog.breed_ << ", Activity Level: " << dog.activityLevel_;
		return os;
	}
};

class Cat : public Pet {
private:
	std::string color_;
	int lives_;
public:
	Cat(std::string n, int a, double w,std::string c, int l)
		: Pet(n, a, w), color_(c), lives_(l) {
	}
	void makeSound() const override {
		std::cout << "Meow! Meow!\n";
	}
	
	friend std::ostream& operator<<(std::ostream& os, const Cat& cat) {
		os << (Pet)cat << ", Color: " << cat.color_ << ", Lives: " << cat.lives_;
		return os;
	}
};

class Parrot : public Pet {
private:
	std::string species_;
	int wingSpan_;
	bool canTalk_;
public:
	Parrot(std::string n, int a, double w,std::string s, int ws, bool ct)
		: Pet(n, a, w), species_(s), wingSpan_(ws), canTalk_(ct) {
	}
	void makeSound() const override {
		if (canTalk_) {
			std::cout << "Hello! I can talk!\n";
		}
		else {
			std::cout << "Squawk! Squawk!\n";
		}
	}
	
	friend std::ostream& operator<<(std::ostream& os, const Parrot& parrot) {
		os << (Pet)parrot << ", Species: " << parrot.species_ << ", Wing Span: " << parrot.wingSpan_ << "cm, Can Talk: " << (parrot.canTalk_ ? "Yes" : "No");
		return os;
	}
};

int main()
{
	Dog myDog("Buddy", 3, 20.5,"Golden Retriever", 8);
	Cat myCat("Stalker", 2, 5.0,"Tabby", 9);
	Parrot myParrot("Vova", 1, 0.5, "African Grey", 25, true);


	std::cout << myDog << std::endl;
	myDog.makeSound();

	std::cout << myCat << std::endl;
	myCat.makeSound();

	std::cout << myParrot << std::endl;
	myParrot.makeSound();
}
