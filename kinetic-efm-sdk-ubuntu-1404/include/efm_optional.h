// @copyright_start
// Copyright (c) 2018 Cisco and/or its affiliates. All rights reserved.
// @copyright_end

/// @file efm_optional.h

#pragma once

#include <efm_error_code.h>
#include <efm_exception.h>

namespace cisco
{
namespace efm_sdk
{

/// @brief Manages an optionally contained value. The value may be present or not.

/// An optional does either contain a valid value or not at any point in time. The type of the wrapped value has to be
/// default constructible. The optional will contain a valid value upon assignment of a value of the optionals type or
/// of another valid optional.
/// When an optional is converted to bool, it will yield true if it contains a valid value and false if not.
template <typename T>
class Optional
{
public:
  /// Constructs a default optional. The optional will not contain a valid value.
  Optional()
    : optional_{true}
  {
  }

  /// Constructs an optional with a value. It will contain a valid value.
  /// @param value The value.
  Optional(T&& value)
    : value_(std::move(value))
    , optional_{false}
  {
  }

  /// Constructs an optional from another optional. It will contain a valid value if the other optional contained a
  /// valid value. The other optionals value will be used to construct the optional.
  /// @param other The other optional to use for the construction.
  Optional(const Optional& other)
    : value_(other.value_)
    , optional_(other.optional_)
  {
  }

  /// Constructs an optional from another optional. It will contain a valid value if the other optional contained a
  /// valid value. The other optionals value will be moved to construct the optional.
  /// @param other The other optional to use for the construction.
  Optional(Optional&& other) noexcept
    : value_(std::move(other.value_))
    , optional_(other.optional_)
  {
    other.optional_ = true;
  }

  /// Replaces the content of this optional with the others content. It will contain a valid value if the other optional
  /// contained a valid value.
  /// @param other The other optional to use for the replacement.
  /// @return The optional.
  Optional& operator=(const Optional& other)
  {
    optional_ = other.optional_;
    value_ = other.value_;
    return *this;
  }

  /// Replaces the content of this optional with the others content. It will contain a valid value if the other optional
  /// contained a valid value.
  /// @param other The other optional to use for the replacement.
  /// @return The optional.
  Optional& operator=(Optional&& other) noexcept
  {
    optional_ = other.optional_;
    value_ = std::move(other.value_);
    other.optional_ = true;
    return *this;
  }

  /// Replaces the content of this optional with the given value. It will contain a valid value after the assignment.
  /// @param rhs The value to use for the replacement.
  /// @return The value.
  Optional& operator=(const T& rhs)
  {
    value_ = rhs;
    optional_ = false;
    return *this;
  }

  /// Replaces the content of this optional with the given value. It will contain a valid value after the assignment.
  /// @param rhs The value to use for the replacement.
  /// @return The value.
  Optional& operator=(T&& rhs)
  {
    value_ = rhs;
    optional_ = false;
    return *this;
  }

  /// Checks if the optional contains a valid value.
  /// @return true if the optional contains a valid value, otherwise false.
  explicit operator bool() const
  {
    return !optional_;
  }

  /// Checks if the optional contains a valid value.
  /// @return true if the optional contains a valid value, otherwise false.
  bool has_value() const
  {
    return !optional_;
  }

  /// Returns the contained value of the optional. Checks if the optional contains a valid value.
  /// @exception If the optional does not contain a valid value.
  /// @return A reference to the contained value.
  const T& value() const
  {
    if (optional_) {
      throw exception(error_code::bad_optional_value);
    }
    return value_;
  }

  /// Returns the contained value of the optional. Checks if the optional contains a valid value.
  /// @exception If the optional does not contain a valid value.
  /// @return A reference to the contained value.
  T& value()
  {
    if (optional_) {
      throw exception(error_code::bad_optional_value);
    }
    return value_;
  }

  /// Returns the contained value of the optional. Does not check if the optional contains a valid value. The behaviour
  /// is undefined if the optional does not contain a valid value.
  /// @return A pointer to the contained value.
  const T* operator->() const
  {
    return &value_;
  }

  /// Returns the contained value of the optional. Does not check if the optional contains a valid value. The behaviour
  /// is undefined if the optional does not contain a valid value.
  /// @return A pointer to the contained value.
  T* operator->()
  {
    return &value_;
  }

  /// Returns the contained value of the optional. Does not check if the optional contains a valid value. The behaviour
  /// is undefined if the optional does not contain a valid value.
  /// @return A reference to the contained value.
  const T& operator*() const
  {
    return value_;
  }

  /// Returns the contained value of the optional. Does not check if the optional contains a valid value. The behaviour
  /// is undefined if the optional does not contain a valid value.
  /// @return A reference to the contained value.
  T& operator*()
  {
    return value_;
  }

private:
  T value_;
  bool optional_{true};
};
}
}
