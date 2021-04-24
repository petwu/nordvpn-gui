#ifndef FACTORY_HPP
#define FACTORY_HPP

#include <memory>

namespace libnordvpn {

    /**
     * @brief Returns a pointer to an I*Controller instance. If the instance
     * does not exist yet, it will be instantiated.
     *
     * @details Controllers are required as shared pointers rather than unique
     * pointers, since they are not supposed to be instantiated more than once
     * per application. The calling e.g. `libnordvpn::get<IStatusController>()`
     * twice will return a pointer to the same instance. Therefore controllers
     * are exposed as de-facto singletons by the library. There are several good
     * reasons for this:
     *
     * - Since a lot of them provide background tasks where it does not make
     * sense to run them twice.
     * - Also some controllers hold e.g. lists of countries or servers that
     * would just increase memory usage unnecessarily if they were duplicated.
     * - But what about private attributes of controller classes? Well, if a
     * controller has private attributes they always hold information about some
     * some state that reflect the environment (like the current connection
     * status) or the attributes are just references/pointers to dependencies
     * injected via the constructor. There is no point in duplicating either of
     * them.
     *
     * @note Using non-supported types as `T` will result in linker errors like
     * ```
     * error: undefined reference to `std::shared_ptr<Foo>
     * libnordvpn::create<Foo>()'
     * ```
     */
    template <typename T> auto get() -> std::shared_ptr<T>;

} // namespace libnordvpn

#endif // FACTORY_HPP
