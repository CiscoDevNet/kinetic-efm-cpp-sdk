// @copyright_start
// Copyright (c) 2018 Cisco and/or its affiliates. All rights reserved.
// @copyright_end

/// @file efm_exception.h

#pragma once

#include <string>
#include <system_error>

namespace cisco
{
namespace efm_sdk
{
/// Provide a SDK specific exception.
class exception : public std::system_error
{
public:
  /// Default copy constructor.
  exception(const exception&) = default;

  /// Default move constructor.
  exception(exception&&) = default;

  /// Default assignment operator.
  /// @return A reference to this object.
  exception& operator=(const exception&) = default;

  /// Default move assignment operator.
  /// @return A reference to this object.
  exception& operator=(exception&&) = default;

  /// Default destructor.
  ~exception() override = default;

  /// Constructor.
  /// @param ec Error code to set.
  exception(const std::error_code& ec)
    : std::system_error(ec)
    , what_{ec.message()}
  {
  }

  /// Constructor.
  /// @param ec Error code to set.
  /// @param what_arg Provides additional information to the error.
  exception(const std::error_code& ec, const std::string& what_arg)
    : system_error(ec, what_arg)
  {
    build_what(ec, what_arg);
  }

  /// Constructor.
  /// @param ec Error code to set.
  /// @param what_arg Provides additional information to the error.
  exception(const std::error_code& ec, const char* what_arg)
    : system_error(ec, what_arg)
  {
    build_what(ec, what_arg);
  }

  /// Returns the formatted message.
  /// @return The formatted message.
  const char* what() const noexcept override
  {
    return what_.c_str();
  }

  /// Returns the additional information for the error - might be empty.
  /// @return The additional information for the error.
  const std::string& message() const
  {
    return message_;
  }

private:
  void build_what(const std::error_code& ec, const std::string& what_arg)
  {
    what_ = ec.message() + ": " + what_arg;
    message_ = what_arg;
  }

  std::string what_;
  std::string message_;
};
}
}
