// @copyright_start
// Copyright (c) 2018 Cisco and/or its affiliates. All rights reserved.
// @copyright_end

/// @file efm_interfaces.h

#pragma once

#include <efm_optional.h>
#include <efm_types.h>
#include <efm_variant.h>


namespace cisco
{
namespace efm_sdk
{

/// @private
class ListResponse
{
public:
  StreamStatus status_;
  Variant updates_;
};

/// @private
class InvokeResponse
{
public:
  StreamStatus status_;
  Variant columns_;
  Variant updates_;
  Optional<ActionStreamMetaData> meta_;
};
}
}
