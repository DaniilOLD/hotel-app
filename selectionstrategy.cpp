#include "selectionstrategy.h"
#include <algorithm>

std::vector<Room*> ByPreferencesStrategy::select(
    Guest* guest, const std::vector<Room*>& rooms) const
{
    std::vector<Room*> result;
    if (!guest) return result;

    for (Room* room : rooms) {
        if (room->getStatus() != RoomStatus::Free) continue;

        int matches = 0;
        int totalPrefs = guest->getPreferences().size();

        for (Preference* pref : guest->getPreferences()) {
            for (Feature* feat : room->getFeatures()) {
                if (pref->getName() == feat->getName()) {
                    matches++;
                    break;
                }
            }
        }

        // Только если ВСЕ пожелания совпали
        if (matches == totalPrefs) result.push_back(room);
    }

    std::sort(result.begin(), result.end(),
              [guest](Room* a, Room* b) {
                  auto count = [guest](Room* r) {
                      int m = 0;
                      for (auto* p : guest->getPreferences())
                          for (auto* f : r->getFeatures())
                              if (p->getName() == f->getName()) { m++; break; }
                      return m;
                  };
                  return count(a) > count(b);
              });

    return result;
}

std::vector<Room*> ByPriceStrategy::select(
    Guest* guest, const std::vector<Room*>& rooms) const
{
    std::vector<Room*> result;
    for (Room* room : rooms)
        if (room->getStatus() == RoomStatus::Free)
            result.push_back(room);

    std::sort(result.begin(), result.end(),
              [](Room* a, Room* b) { return a->getPrice() < b->getPrice(); });
    return result;
}
