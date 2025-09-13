#include <iostream>
#include <cstring>

class String {
protected:
	char* data;
	int length;

	void allocateMemory(int len) {
		data = new char[len + 1];
		data[len] = '\0';
		length = len;
}

	void copyFrom(const String& other, int len) {
		allocateMemory(other.length);
		strcpy_s(data, other.data, len);
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
			int len = strlen(str);
			allocateMemory(len);
			strcpy_s(data, str);
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

	int getLength() const {
		return length;
	}

	void clear() {
		delete[] data;
		allocateMemory(0);
	}

	~String() {
		delete[] data;
	}

	String operator+(const String& other) const {
		String result;
		result.allocateMemory(length + other.length);
		strcpy_s(result.data, data, length);
		strcpy_s(result.data + length, other.data, other.length);
		return result;
	}

	String operator+=(const String& other) {
		char* newData = new char[length + other.length + 1];
		strcpy_s(newData, data, length);
		strcpy_s(newData + length, other.data, other.length);
		newData[length + other.length] = '\0';

		delete[] data;
		data = newData;
		length += other.length;
		return *this;
	}

	bool operator==(const String& other) const {
		if (length != other.length) {
			return false;
		}
		return strcmp_s(data, other.data, other.length) == 0;
	}
	bool operator!=(const String& other) const {
		return !(*this == other);
	}

	friend std::ostream& operator<<(std::ostream& os, const String& str) {
		os << str.data;
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
		if (length != other.length) {
			return BitString();
		}

		int maxLen = length;
		BitString temp1 = *this;
		BitString temp2 = other;

		temp1.complement();
		temp2.complement();

		BitString result = temp1 + temp2;
		result.allocateMemory(maxLen + 1);

		char carry = '0';
		for (int i = maxLen - 1; i >= 0; --i) {
			int sum = (temp1.data[i] - '0') + (temp2.data[i] - '0') + (carry - '0');
			if (sum == 0) {
				result.data[i + 1] = '0';
				carry = '0';
			}
			else if (sum == 1) {
				result.data[i + 1] = '1';
				carry = '0';
			}
			else if (sum == 2) {
				result.data[i + 1] = '0';
				carry = '1';
			}
			else {
				result.data[i + 1] = '1';
				carry = '1';
			}
		}
		result.data[0] = carry;
		result.length = maxLen + 1;

		if (result.data[0] == '0') {
			char* newData = new char[maxLen + 1];
			strcpy_s(newData, result.data + 1);
			delete[] result.data;
			result.data = newData;
			result.length = maxLen;
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