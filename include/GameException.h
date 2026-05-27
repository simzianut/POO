#ifndef GAMEEXCEPTION_H
#define GAMEEXCEPTION_H

#include <exception>
#include <string>

class GameException : public std::exception {
private:
    std::string message;

public:
    explicit GameException(std::string message);

    [[nodiscard]] const char* what() const noexcept override;
};

class InvalidCommandException : public GameException {
public:
    explicit InvalidCommandException(const std::string& command);
};

class NotEnoughCoinsException : public GameException {
public:
    NotEnoughCoinsException(int price, int coins);
};

class BerryAlreadyInUseException : public GameException {
public:
    BerryAlreadyInUseException();
};

#endif
