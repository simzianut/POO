#include "GameException.h"

#include <utility>

GameException::GameException(std::string message) : message(std::move(message)) {}

const char* GameException::what() const noexcept
{
    return message.c_str();
}

InvalidCommandException::InvalidCommandException(const std::string& command) :
    GameException("Invalid command: " + command + ". Type exit to quit.") {}

NotEnoughCoinsException::NotEnoughCoinsException(const int price, const int coins) :
    GameException(
        "Not enough coins. Price: " + std::to_string(price) +
        " coins, your coins: " + std::to_string(coins) + "."
    ) {}

BerryAlreadyInUseException::BerryAlreadyInUseException() :
    GameException("A berry effect is already active. Wait until it ends before feeding another berry.") {}
