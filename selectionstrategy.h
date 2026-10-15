#ifndef SELECTIONSTRATEGY_H
#define SELECTIONSTRATEGY_H
#include "guest.h"
#include "room.h"
#include <vector>

// Паттерн STRATEGY
class SelectionStrategy
{
public:
    virtual ~SelectionStrategy() = default;
    virtual std::vector<Room*> select(Guest* guest,
                                       const std::vector<Room*>& rooms) const = 0;
};

// Стратегия 1: по пожеланиям
class ByPreferencesStrategy : public SelectionStrategy
{
public:
    std::vector<Room*> select(Guest* guest,
                               const std::vector<Room*>& rooms) const override;
};

// Стратегия 2: по цене
class ByPriceStrategy : public SelectionStrategy
{
public:
    std::vector<Room*> select(Guest* guest,
                               const std::vector<Room*>& rooms) const override;
};

#endif // SELECTIONSTRATEGY_H
