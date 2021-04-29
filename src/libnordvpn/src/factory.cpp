#include "nordvpn/factory.hpp"

#include <stdexcept>

#include "controllers/accountcontroller.hpp"
#include "controllers/connectioncontroller.hpp"
#include "controllers/countrycontroller.hpp"
#include "controllers/envcontroller.hpp"
#include "controllers/preferencescontroller.hpp"
#include "controllers/recentscontroller.hpp"
#include "controllers/servercontroller.hpp"
#include "controllers/statuscontroller.hpp"
#include "controllers/updatecheckcontroller.hpp"
#include "nordvpn/iaccountcontroller.hpp"
#include "nordvpn/iconnectioncontroller.hpp"
#include "nordvpn/icountrycontroller.hpp"
#include "nordvpn/ienvcontroller.hpp"
#include "nordvpn/ipreferencescontroller.hpp"
#include "nordvpn/irecentscontroller.hpp"
#include "nordvpn/iservercontroller.hpp"
#include "nordvpn/istatuscontroller.hpp"
#include "nordvpn/iupdatecheckcontroller.hpp"
#include "repositories/ipreferencesrepository.hpp"
#include "repositories/iserverrepository.hpp"
#include "repositories/iupdatecheckrepository.hpp"
#include "repositories/preferencesrepository.hpp"
#include "repositories/serverrepository.hpp"
#include "repositories/updatecheckrepository.hpp"

namespace libnordvpn {

    /**
     * @brief Helper factory to construct classes that are not publicly
     * accessible like repositories.
     * @details Same implementation style as get<T>().
     */
    template <typename T> auto _get() -> std::shared_ptr<T> {
        constexpr bool validT = std::is_same_v<T, IServerRepository> ||
                                std::is_same_v<T, IPreferencesRepository> ||
                                std::is_same_v<T, IUpdateCheckRepository>;
        static_assert(validT,
                      "invalid template parameter T for libnordvpn::_get<T>()");

        if constexpr (std::is_same_v<T, IServerRepository>) {
            return std::make_shared<ServerRepository>();
        } else if constexpr (std::is_same_v<T, IPreferencesRepository>) {
            return std::make_shared<PreferencesRepository>();
        } else if constexpr (std::is_same_v<T, IUpdateCheckRepository>) {
            return std::make_shared<UpdateCheckRepository>();
        } else {
            // not possible
        }
    }

    /**
     * @brief The actual factory implementaion.
     */
    template <typename T> auto get() -> std::shared_ptr<T> {
        // assert at compile time that the template parameter is one of the
        // following types:
        constexpr bool validT = std::is_same_v<T, IAccountController> ||
                                std::is_same_v<T, IConnectionController> ||
                                std::is_same_v<T, ICountryController> ||
                                std::is_same_v<T, IEnvController> ||
                                std::is_same_v<T, IPreferencesController> ||
                                std::is_same_v<T, IRecentsController> ||
                                std::is_same_v<T, IServerController> ||
                                std::is_same_v<T, IStatusController> ||
                                std::is_same_v<T, IUpdateCheckController>;
        static_assert(validT,
                      "invalid template parameter T for libnordvpn::get<T>()");

        // check the type of T and return an instance of the appropriate type:
        // 1. create the returned object as static local variable -> singletons
        // 2. resolve dependencies via get<> or _get<>
        // 3. return the static shared pointer
        if constexpr (std::is_same_v<T, IAccountController>) {
            static auto accountController =
                std::make_shared<AccountController>(get<IEnvController>());
            return accountController;
        } else if constexpr (std::is_same_v<T, IConnectionController>) {
            static auto connectionController =
                std::make_shared<ConnectionController>(
                    get<ICountryController>(), get<IEnvController>(),
                    get<IRecentsController>(), get<IServerController>(),
                    get<IStatusController>());
            return connectionController;
        } else if constexpr (std::is_same_v<T, ICountryController>) {
            static auto countryController = std::make_shared<CountryController>(
                get<IServerController>(), _get<IServerRepository>());
            return countryController;
        } else if constexpr (std::is_same_v<T, IEnvController>) {
            static auto envController = std::make_shared<EnvController>();
            return envController;
        } else if constexpr (std::is_same_v<T, IPreferencesController>) {
            static auto preferencesController =
                std::make_shared<PreferencesController>();
            return preferencesController;
        } else if constexpr (std::is_same_v<T, IRecentsController>) {
            static auto recentsController = std::make_shared<RecentsController>(
                get<ICountryController>(), _get<IPreferencesRepository>());
            return recentsController;
        } else if constexpr (std::is_same_v<T, IServerController>) {
            static auto serverController = std::make_shared<ServerController>(
                get<IPreferencesController>(), _get<IServerRepository>());
            return serverController;
        } else if constexpr (std::is_same_v<T, IStatusController>) {
            static auto statusController =
                std::make_shared<StatusController>(get<IServerController>());
            return statusController;
        } else if constexpr (std::is_same_v<T, IUpdateCheckController>) {
            static auto updateCheckController =
                std::make_shared<UpdateCheckController>(
                    _get<IUpdateCheckRepository>());
            return updateCheckController;
        } else {
            // not possible if all classes tested in the previous if-else
            // branches are checked with validT and covered in
            // getExplicitTemplateInstantiations()
        }
    }

} // namespace libnordvpn

/**
 * @brief Notes on template implementation
 *
 * @details Separating the definition and implementation of template
 * classes/functions ain't that easy in C++. The problem is, that a template is
 * not a class/function per se, but a pattern that the compiler uses to generate
 * a family of classes/functions. In order to generate the code, the compiler
 * needs to be able to see the implementation. Otherwise the linker will
 * complain about unresolved symbols or undefined references or something the
 * like. Hevertheless, there are different methods to separate definition and
 * implementation:
 *
 * 1. The user of the template could include both the definition (.hpp) and the
 *    implementation (.cpp) file.
 *    Pros: -
 *    Cons: 2 #include statements required; .cpp needs to be in the include path
 *
 * 2. The template header includes the implementation file itself. This is
 *    basically just a variation or method 1.
 *    Pros: only one #include statement
 *    Cons: .cpp needs to be in the include path
 *
 * 3. The template implementation can do some explicit instantiation/calling of
 *    the template class/function at the end of the implementation file.
 *    Pros: only one #include statement; behaves as if implemented in the same
 *          file as the definition
 *    Cons: the template may only be used with the template parameters used
 *          in the explicit instantioation
 *
 * Essentially method 3 is the best option, since with a factory method the
 * instantiable types must be known in advance anyway. Additionally, if someone
 * tries to use the factory method with a template parameter that is not
 * supported a linker error will automatically occur.
 *
 * https://stackoverflow.com/questions/495021
 * https://stackoverflow.com/questions/115703
 * https://isocpp.org/wiki/faq/templates#templates-defn-vs-decl
 * https://www.codeproject.com/Articles/48575/How-to-Define-a-Template-Class-in-a-h-File-and-Imp
 */
void getExplicitTemplateInstantiations() {
    libnordvpn::get<IAccountController>();
    libnordvpn::get<IConnectionController>();
    libnordvpn::get<ICountryController>();
    libnordvpn::get<IEnvController>();
    libnordvpn::get<IPreferencesController>();
    libnordvpn::get<IRecentsController>();
    libnordvpn::get<IServerController>();
    libnordvpn::get<IStatusController>();
    libnordvpn::get<IUpdateCheckController>();

    // note: no need to do the same for libnordvpn::_get<T>(), since this
    // template is both defined and implemented in the this file

    // throw an exception, since this method only exists, because we cannot call
    // libnordvpn::get<T>() without a context.
    throw std::logic_error(
        "getExplicitTemplateInstantiations() is not supposed to be called. It "
        "is just a way to explicitly define possible template parameters for "
        "libnordvpn::get<T>(). This is required to avoid linker errors due to "
        "the fact that definition and implementation of the template are "
        "separated. See https://stackoverflow.com/a/115735/13154737 or "
        "https://stackoverflow.com/a/495056/13154737");
}
