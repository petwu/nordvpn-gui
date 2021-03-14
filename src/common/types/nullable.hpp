#ifndef NULLABLE_HPP
#define NULLABLE_HPP

#include <algorithm>
#include <iostream>
#include <utility>

template <typename T>
/**
 * @brief Nullable is a simple wrapper class for value types that allows objects
 * or native type variables to have "null" value.
 *
 * The class is useful for passing parameters to functions when parameters are
 * optional and no default values should be used or when a non-assigned state is
 * needed, such as in e.g. fetching null values from database.
 *
 * A Nullable can be default constructed. In this case, the Nullable will have a
 * Null value and isNull() will return true. Calling value() (without default
 * value) on a Null object will throw a NullValueException.
 *
 * A Nullable can also be constructed from a value. It is possible to assign a
 * value to a Nullable, and to reset a Nullable to contain a Null value by
 * calling clear().
 *
 * For use with Nullable, the value type should support default construction.
 */
class Nullable {
  public:
    /**
     * @brief Creates an empty Nullable.
     */
    Nullable() : _value() {}

    /**
     * @brief Creates a Nullable with the given value.
     * @param value Initial value of the Nullable.
     */
    Nullable(T value) : _value(std::move(value)), _isNull(false) {}

    /**
     * @brief Creates a Nullable by copying another one.
     * @param other The Nullable to copy.
     */
    Nullable(const Nullable &other)
        : _value(other._value), _isNull(other._isNull) {}

    /**
     * @brief Creates a Nullable by moving another one.
     * @param other The Nullable to move.
     */
    Nullable(Nullable &&other) noexcept
        : _value(std::move(other._value)), _isNull(other._isNull) {
        other._isNull = true;
    }

    /**
     * @brief Destroys the Nullable.
     */
    ~Nullable() = default;

    /**
     * @brief Assigns a value to the Nullable.
     * @param value The value to assign.
     * @return The Nullable itself.
     */
    auto assign(const T &value) -> Nullable & {
        _value = value;
        _isNull = false;
        return *this;
    }

    /**
     * @brief Assigns a value to the Nullable.
     * @param value The value to assign.
     * @return The Nullable itself.
     */
    auto assign(T &&value) -> Nullable & {
        _value = std::move(value);
        _isNull = false;
        return *this;
    }

    /**
     * @brief Assigns another Nullable.
     * @param other The other Nullable to assign.
     * @return The Nullable itself.
     */
    auto assign(const Nullable &other) -> Nullable & {
        Nullable tmp(other);
        swap(tmp);
        return *this;
    }

    /**
     * @brief Assigns a value to the Nullable.
     * @param value The value to assign.
     * @return The Nullable itself.
     */
    auto operator=(const T &value) -> Nullable & {
        // disable clang-tidy check, because "return *this" is implicitly given
        // by assign()
        // NOLINTNEXTLINE(cppcoreguidelines-c-copy-assignment-signature,misc-unconventional-assign-operator)
        return assign(value);
    }

    /**
     * @brief Move-assigns a value to the Nullable.
     * @param value The value to assign.
     * @return The nullable itself.
     */
    auto operator=(T &&value) -> Nullable & {
        // disable clang-tidy check, because "return *this" is implicitly given
        // by assign()
        // NOLINTNEXTLINE(cppcoreguidelines-c-copy-assignment-signature,misc-unconventional-assign-operator)
        return assign(std::move(value));
    }

    /**
     * @brief Assigns another Nullable.
     * @param other The other Nullable to assign.
     * @return The Nullable itself.
     */
    auto operator=(const Nullable &other) -> Nullable & {
        if (&other == this) {
            return *this;
        }
        // disable clang-tidy check, because "return *this" is implicitly given
        // by assign()
        // NOLINTNEXTLINE(cppcoreguidelines-c-copy-assignment-signature,misc-unconventional-assign-operator)
        return assign(other);
    }

    /**
     * @brief Moves another Nullable.
     * @param other THe Nullable to move.
     * @return The Nullable itself.
     */
    auto operator=(Nullable &&other) noexcept -> Nullable & {
        _isNull = other._isNull;
        _value = std::move(other._value);
        other._isNull = true;
        return *this;
    }

    /**
     * @brief Swaps this Nullable with other.
     * @param other The other Nullable to swap with.
     */
    void swap(Nullable &other) {
        std::swap(_value, other._value);
        std::swap(_isNull, other._isNull);
    }

    /**
     * @brief Compares two Nullables for equality
     * @param other The Nullable to compare with.
     * @return true if the have equal values, false otherwise.
     */
    auto operator==(const Nullable<T> &other) const -> bool {
        return (_isNull && other._isNull) ||
               (_isNull == other._isNull && _value == other._value);
    }

    /**
     * @brief Compares Nullable with value for equality
     * @param value The value to compare the Nullables value with.
     * @return true if equal, false otherwise.
     */
    auto operator==(const T &value) const -> bool {
        return (!_isNull && _value == value);
    }

    /**
     * @brief Compares Nullable with value for non equality.
     * @param value The value to compare with.
     * @return true if not equal, false otherwise.
     */
    auto operator!=(const T &value) const -> bool { return !(*this == value); }

    /**
     * @brief Compares two Nullables for non equality
     * @param other The other Nullable to compare with.
     * @return true if not equal, false otherwise.
     */
    auto operator!=(const Nullable<T> &other) const -> bool {
        return !(*this == other);
    }

    /**
     * @brief Compares two Nullable objects.
     * @param other The other Nullable to compare with.
     * @return true if this object's value is smaler than the other object's
     * value. Null value is smaller than a non-null value.
     */
    auto operator<(const Nullable<T> &other) const -> bool {
        if (_isNull && other._isNull) {
            return false;
        }
        if (!_isNull && !other._isNull) {
            return (_value < other._value);
        }
        return (_isNull && !other._isNull);
    }

    /**
     * @brief Compares two Nullable objects.
     * @param other The other Nullable to compare with.
     * @return true if this object's value is greater than the other object's
     * value. A non-null value is greater than a null value.
     */
    auto operator>(const Nullable<T> &other) const -> bool {
        return !(*this == other) && !(*this < other);
    }

    /**
     * @brief value
     * @return The Nullable's value.
     * @throws Exception if the Nullable is empty.
     */
    auto value() -> T & {
        if (!_isNull) {
            return _value;
        }
        throw "null value exception";
    }

    /**
     * @brief value
     * @return The Nullable's value.
     * @throws Exception if the Nullable is empty.
     */
    auto value() const -> const T & {
        if (!_isNull) {
            return _value;
        }
        throw "null value exception";
    }

    /**
     * @brief value
     * @param deflt The default value.
     * @return The Nullable's value, or the given default value if the Nullable
     * is empty.
     */
    auto value(const T &deflt) const -> const T & {
        return _isNull ? deflt : _value;
    }

    /**
     * @brief Get reference to the value.
     * @throws Exception if the Nullable is empty.
     */
    operator T &() { return value(); }

    /**
     * @brief Get const reference to the value
     */
    operator const T &() const { return value(); }

    /**
     * @brief Checks whether the Nullable holds a value.
     * @return true, if the Nullable is empty, false otherwise.
     */
    auto isNull() const -> bool { return _isNull; }

    /**
     * @brief Checks whether the Nullable holds a value.
     * @return true, if the Nullable is non-empty, false otherwise.
     */
    auto isNotNull() const -> bool { return !_isNull; }

    /**
     * @brief Clears the Nullable aka. makes it null.
     */
    void clear() { _isNull = true; }

  private:
    T _value;
    bool _isNull{true};
};

#endif // NULLABLE_HPP
