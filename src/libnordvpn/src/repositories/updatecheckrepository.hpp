#ifndef UPDATECHECKREPOSITORY_HPP
#define UPDATECHECKREPOSITORY_HPP

#include "baserepository.hpp"
#include "iupdatecheckrepository.hpp"
#include "nordvpn/models/version.hpp"

class UpdateCheckRepository : public IUpdateCheckRepository,
                              public BaseRepository {
  public:
    auto getCurrentVersion() -> Version override;
    auto getLatestVersion() -> Version override;
};

#endif // UPDATECHECKREPOSITORY_HPP
