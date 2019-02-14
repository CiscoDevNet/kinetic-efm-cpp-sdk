// @copyright_start
// Copyright (c) 2018 Cisco and/or its affiliates. All rights reserved.
// @copyright_end

/// @file examples/simple_responder/error_code.h

#pragma once

#include <system_error>

/// Error codes for the SDK API responder example.
enum class responder_error_code
{
  build_with_version = 1000,
  connected,
  disconnected,
  subscribed_text,
  unsubscribed_text,
  set_text
};


/// Error category for the SDK API responder error code.
/// @return A reference to the error category.
const std::error_category& responder_error_category();

/// Converts an error code to a std::error_code.
/// @param e error code to convert to a std::error_code.
/// @return The converted std::error_code.
inline std::error_code make_error_code(responder_error_code e)
{
  const std::error_category& cat = responder_error_category();
  return {static_cast<int>(e), cat};
}

namespace std
{
/// Specifies that the error_code can be converted into a std::error_code.
template <>
struct is_error_code_enum<responder_error_code> : public true_type
{
};
}
