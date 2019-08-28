// @copyright_start
// Copyright (c) 2018-2019 Cisco and/or its affiliates. All rights reserved.
// @copyright_end

/// @file efm_string_utils.h

#pragma once

#include <efm_variant.h>

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

/// Encodes a binary buffer into a DSA binary. A DSA binary has a prefix of "\u001Bbytes:" and is Base64 encoded.
/// @throw If the buffer cannot be encoded.
/// @param bytes The buffer to encode.
/// @return The DSA encoded binary.
std::string encode_dsa_binary(const std::vector<unsigned char>& bytes);

/// Decodes a DSA binary. A DSA binary has a prefix of "\u001Bbytes:" and is Base64 encoded.
/// @throw If the buffer cannot be decoded.
/// @param bytes The buffer to decode.
/// @return The decoded binary.
std::vector<unsigned char> decode_dsa_binary(const std::string& bytes);
}
}
