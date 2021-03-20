#ifndef UPDATECHECKREPOSITORY_HPP
#define UPDATECHECKREPOSITORY_HPP

#include "common/types/version.hpp"
#include "data/repositories/baserepository.hpp"
#include "iupdatecheckrepository.hpp"

class UpdateCheckRepository : public IUpdateCheckRepository,
                              public BaseRepository {
  public:
    auto getCurrentVersion() -> Version override;
    auto getLatestVersion() -> Version override;
};

#endif // UPDATECHECKREPOSITORY_HPP
