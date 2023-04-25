#include <iostream>
#include <ctime>
#include <exception>

class FishException: public std::exception {
    [[nodiscard]] const char* what() const noexcept override
    {
            return "Successful casting of a fishing rod";
    }
};

class BootException: public std::exception {
    [[nodiscard]] const char* what() const noexcept override
    {
            return "The boot is caught";
    }
};

class SubField {
private:
    std::string obj = "unknown";
public:
    explicit SubField(std::string&& inObj): obj(inObj) {}

    [[nodiscard]] std::string getObj() const
    {
        return obj;
    }
};

class Field {
private:
    SubField** obj;
    char field[13][19];
public:
    Field () {
        obj = new SubField*[9];
        int count = 1;
        for (int i = 0; i < 9; ++i) {
            obj[i] = nullptr;
        }
        for (int i = 0; i < 13; ++i) {
            for (int j = 0;j < 19;++j) {
                if (i == 0 || (i % 4) == 0) {
                    field[i][j] = '-';
                } else if (j == 0 || (j % 6) == 0) {
                    field[i][j] = '|';
                } else if (i == 2 && (j == 3 || j == 9 || j == 15) ||
                           i == 6 && (j == 3 || j == 9 || j == 15) ||
                           i == 10 && (j == 3 || j == 9 || j == 15)) {
                    field[i][j] = count + '0';
                    ++count;
                } else {
                    field[i][j] = ' ';
                }
            }
        }
    }

    void objInit () {
        std::srand(std::time(nullptr));
        int fishCount = 0;
        int bootCount = 0;
        for (int i = 0; i < 4; ++i) {
            int n = std::rand() % 9;
            if (obj[n] == nullptr) {
                if (fishCount != 1) {
                    obj[n] = new SubField ("fish");
                    ++fishCount;
                } else if (bootCount != 3) {
                    obj[n] = new SubField ("boot");
                    ++bootCount;
                }
            } else {
                --i;
            }
        }
        for (int i = 0; i < 9; ++i) {
            if (obj[i] == nullptr) {
                obj[i] = new SubField ("empty");
            }
        }
    }

    void print () const {
        for (int i = 0; i < 13; ++i) {
            for (int j = 0; j < 19; ++j) {
                std::cout << field[i][j];
            }
            std::cout << std::endl;
        }
    }

    int fishing (int num) {
        if (obj[num]->getObj() == "fish") {
            return 0;
        } else if (obj[num]->getObj() == "boot") {
            return 1;
        } else {
            return 2;
        }

    }

    ~Field() {
        for (int i = 0;i < 9;++i) {
            delete obj[i];
        }
        delete[] obj;
    }
};

int main() {
    Field f;
    f.objInit();
    int count = 0;
    while (true) {
        f.print();
        int num;
        std::cout << "Enter the sector number: ";
        std::cin >> num;
        try {
            if (f.fishing(num - 1) == 0) {
                ++count;
                throw FishException();
            } else if (f.fishing(num - 1) == 1) {
                count = -1;
                throw BootException();
            } else {
                ++count;
                std::cout << "empty"<< std::endl;
            }
        }
        catch (const std::exception& x) {
            std::cerr << x.what() << std::endl;
            std::cerr << "Your result: " << count << std::endl;
            break;
        }
    }
    return 0;
}