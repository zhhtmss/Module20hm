#include <iostream>
#include <cstring>

class String {
protected:
	char* data;
	size_t length;

	void allocateMemory(size_t size) {
		data = new char[size + 1];
		data[size] = '\0';
		length = size;
	}

	void copyFrom(const char* str, size_t len) {
		allocateMemory(len);
		for (size_t i = 0; i < len; i++) {
			data[i] = str[i];
		}
	}

public:
	String() : data(nullptr), length(0) {
		allocateMemory(0);
	}

	String(const char* str) {
		if (str == nullptr) {
			allocateMemory(0);
		}
		else {
			size_t len = 0;
			while (str[len] != '\0') len++;
			copyFrom(str, len);
		}
	}

	String(const String& other) {
		copyFrom(other.data, other.length);
	}

	String& operator=(const String& other) {
		if (this != &other) {
			delete[] data;
			copyFrom(other.data, other.length);
		}
		return *this;
	}

	size_t getLength() const {
		return length;
	}

	void clear() {
		delete[] data;
		allocateMemory(0);
	}

	virtual ~String() {
		delete[] data;
	}

	String operator+(const String& other) const {
		String result;
		result.allocateMemory(length + other.length);

		for (size_t i = 0; i < length; i++) {
			result.data[i] = data[i];
		}

		for (size_t i = 0; i < other.length; i++) {
			result.data[length + i] = other.data[i];
		}

		return result;
	}

	String& operator+=(const String& other) {
		char* temp = new char[length + other.length + 1];

		for (size_t i = 0; i < length; i++) {
			temp[i] = data[i];
		}

		for (size_t i = 0; i < other.length; i++) {
			temp[length + i] = other.data[i];
		}

		temp[length + other.length] = '\0';

		delete[] data;
		data = temp;
		length += other.length;

		return *this;
	}

	bool operator==(const String& other) const {
		if (length != other.length) return false;

		for (size_t i = 0; i < length; i++) {
			if (data[i] != other.data[i]) {
				return false;
			}
		}
		return true;
	}

	bool operator!=(const String& other) const {
		return !(*this == other);
	}

	friend std::ostream& operator<<(std::ostream& os, const String& str) {
		for (size_t i = 0; i < str.length; i++) {
			os << str.data[i];
		}
		return os;
	}
};

class BitString : public String {
private:
	bool isValidBitString(const char* str) const {
		for (int i = 0; str[i] != '\0'; ++i) {
			if (str[i] != '0' && str[i] != '1') {
				return false;
			}
		}
		return true;
	}

	void complement() {
		for (int i = 0; i < length; ++i) {
			data[i] = (data[i] == '0') ? '1' : '0';
		}
		for (int i = length - 1; i >= 0; i--) {
			if (data[i] == '0') {
				data[i] = '1';
				break;
			}
			else {
				data[i] = '0';
			}
		}
	}

public:
	BitString() : String() {}

	BitString(const char* str) {
		if (str != nullptr && isValidBitString(str)) {
			int len = strlen(str);
			copyFrom(str, len);
		}
		else {
			allocateMemory(0);
		}
	}

	BitString(const BitString& other) : String(other) {}

	BitString& operator=(const BitString& other) {
		if (this != &other) {
			String::operator=(other);
		}
		return *this;
	}

	void changeSign() {
		complement();
	}

	BitString operator+(const BitString& other) const {
		if (!isValidBitString(data) || !isValidBitString(other.data)) {
			return BitString();
		}
		int maxLen = (length > other.length) ? length : other.length;

		BitString left = *this;
		BitString right = other;

		char leftPad = (left.length > 0) ? left.data[0] : '0';
		char rightPad = (right.length > 0) ? right.data[0] : '0';
		if (left.length < maxLen) {
			char* temp = new char[maxLen + 1];
			int padCount = maxLen - left.length;
			for (int i = 0; i < padCount; i++) {
				temp[i] = leftPad;
			}
			std::memcpy(temp + padCount, left.data, left.length);
			temp[maxLen] = '\0';
			delete[] left.data;
			left.data = temp;
			left.length = maxLen;
		}

		if (right.length < maxLen) {
			char* temp = new char[maxLen + 1];
			int padCount = maxLen - right.length;
			for (int i = 0; i < padCount; i++) {
				temp[i] = rightPad;
			}
			std::memcpy(temp + padCount, right.data, right.length);
			temp[maxLen] = '\0';
			delete[] right.data;
			right.data = temp;
			right.length = maxLen;
		}
		BitString result;
		result.allocateMemory(maxLen);

		int carry = 0;
		for (int i = maxLen - 1; i >= 0; i--) {
			int bit1 = left.data[i] - '0';
			int bit2 = right.data[i] - '0';
			int sum = bit1 + bit2 + carry;

			result.data[i] = (sum % 2) + '0';
			carry = sum / 2;
		}
		if (carry > 0) {
			char* newData = new char[maxLen + 2];
			newData[0] = carry + '0';
			std::memcpy(newData + 1, result.data, maxLen);
			newData[maxLen + 1] = '\0';
			delete[] result.data;
			result.data = newData;
			result.length = maxLen + 1;
		}

		return result;
	}

	BitString operator+=(const BitString& other) {
		*this = *this + other;
		return *this;
	}

	bool operator==(const BitString& other) const {
		return String::operator==(other);
	}

	bool operator!=(const BitString& other) const {
		return !(*this == other);
	}
};

int main() 
{
	String str1("Hello");
	String str2(" World");
	String str3 = str1 + str2;

	std::cout << "String 1: " << str1 << std::endl;
	std::cout << "String 2: " << str2 << std::endl;
	std::cout << "Concatenated String: " << str3 << std::endl;
	std::cout << "Length of Concatenated String: " << str3.getLength() << std::endl;
	std::cout << "Strings are equal: " << (str1 == str2 ? "Yes" : "No") << std::endl;

	BitString bitStr1("1101");
	BitString bitStr2("1011");
	BitString bitStr3("1012");

	BitString bitStr4 = bitStr1 + bitStr2;
	std::cout << "BitString 4: " << bitStr4 << std::endl;

	bitStr1.changeSign();
	std::cout << "BitString 1 after changeSign: " << bitStr1 << std::endl;
}