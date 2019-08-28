// @copyright_start
// Copyright (c) 2018-2019 Cisco and/or its affiliates. All rights reserved.
// @copyright_end

/// @file efm_json_utils.h

#pragma once

#include <efm_variant.h>

#include <string>


namespace cisco
{
namespace efm_sdk
{
namespace json
{
/// Converts a JSON string into a Variant.
/// @throw If the string is not a valid JSON document.
/// @param str A JSON string.
/// @return The converted Variant.
cisco::efm_sdk::Variant from_json_string(const std::string& str);

/// Converts a Variant into a JSON string representation.
/// @param variant The Variant to convert.
/// @return The JSON string of the Variant.
std::string to_json_string(const cisco::efm_sdk::Variant& variant);
}
}
}
