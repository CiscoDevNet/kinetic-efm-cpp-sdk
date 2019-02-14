// @copyright_start
// Copyright (c) 2018 Cisco and/or its affiliates. All rights reserved.
// @copyright_end

/// @file efm_string_utils.h

#pragma once

#include <string>

namespace cisco
{
namespace efm_sdk
{
/// Escapes all non digit or alpha characters to its base16 encoded form. Can be used to encode node names with illegal
/// characters. Mind, that all usages of the name also have to be encoded, i.e. NodePath etc.
/// @param s The string to encode.
/// @return The encoded string.
std::string string_escape(const std::string& s);
}
}
