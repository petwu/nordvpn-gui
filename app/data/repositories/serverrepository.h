#ifndef SERVERREPOSITORY_H
#define SERVERREPOSITORY_H

#include "baserepository.h"

class ServerRepository : public BaseRepository {
  public:
    static json::array_t getCountriesJSON();
};

#endif // SERVERREPOSITORY_H
