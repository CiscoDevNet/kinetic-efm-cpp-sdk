// @copyright_start
// Copyright (c) 2018 Cisco and/or its affiliates. All rights reserved.
// @copyright_end

/// @file efm_error_code.h

#pragma once

#include <system_error>

namespace cisco
{
namespace efm_sdk
{
/// Error codes for the SDK API. Can be used to initialize a std::error_code.
enum class error_code
{
  path_not_found = 9000,
  config_value_not_found,
  attribute_value_not_found,
  not_a_value_node,
  invalid_value,
  invalid_config_name,
  invalid_stream,
  unknown_error,
  invalid_editor_type,
  no_node_defined_yet,
  editor_type_not_allowed,
  bad_optional_value,
  invalid_table_replace_modifier_index,
  efm_variant_error,
  invalid_link_type_specified,
  node_is_not_writable,
  empty_name_specified,
  invalid_name_characters_specified,
  internal_error,
  no_dslink_json_found,
  invalid_dslink_json,
  invalid_remove_operation,
  invalid_permission_specified
};


/// Error category for the EFM SDK error code. Used to convert EFM SDK API errors to std::error_code.
/// @return A reference to the EFM SDK API error category.
const std::error_category& efm_error_category();

/// Converts a EFM SDK API error code to a std::error_code.
/// @param e The EFM SDK API error code to convert to a std::error_code.
/// @return The converted std::error_code.
inline std::error_code make_error_code(error_code e)
{
  const std::error_category& cat = efm_error_category();
  return {static_cast<int>(e), cat};
}
}
}

namespace std
{
/// Specifies that the cisco::efm_sdk::error_code can be converted into a std::error_code.
template <>
struct is_error_code_enum<cisco::efm_sdk::error_code> : public true_type
{
};
}
