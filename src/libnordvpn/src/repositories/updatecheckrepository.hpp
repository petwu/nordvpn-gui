#ifndef UPDATECHECKREPOSITORY_HPP
#define UPDATECHECKREPOSITORY_HPP

#include <filesystem>
#include <vector>

#include "baserepository.hpp"
#include "iupdatecheckrepository.hpp"
#include "nordvpn/models/version.hpp"

class UpdateCheckRepository : public IUpdateCheckRepository,
                              public BaseRepository {
  public:
    UpdateCheckRepository();

    auto getCurrentVersion() -> Version override;
    auto getAllReleaseVersions() -> std::vector<Version> override;
    void setIsUpdateHidden(Version v, bool isHidden) override;
    auto isUpdateHidden(Version v) -> bool override;

  private:
    std::filesystem::path _hiddenUpdatesJsonPath;
};

#endif // UPDATECHECKREPOSITORY_HPP
