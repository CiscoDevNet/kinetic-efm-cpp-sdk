// @copyright_start
// Copyright (c) 2018 Cisco and/or its affiliates. All rights reserved.
// @copyright_end

/// @file efm_subscription_update.h

#pragma once

#include <efm_variant.h>

#include <limits>
#include <system_error>


namespace cisco
{
namespace efm_sdk
{

/// @brief

/// The SubscriptionUpdate represents a subscription update send from a responder.
class SubscriptionUpdate
{
public:
  /// Creates a SubscriptionUpdate from a cisco::efm_sdk::Variant.
  /// @param update The cisco::efm_sdk::Variant to create a SubscriptionUpdate from.
  /// @param ec Will be set to an error if a SubscriptionUpdate cannot be created.
  /// @return a SubscriptionUpdate object
  static SubscriptionUpdate make_subscription_update(const cisco::efm_sdk::Variant& update, std::error_code& ec);

  uint32_t sid_{std::numeric_limits<uint32_t>::max()}; ///< The sid of the underlying subscription.
  cisco::efm_sdk::Variant time_stamp_;                 ///< The timestamp of the value as a string.
  cisco::efm_sdk::Variant value_;                      ///< The value itself.
  cisco::efm_sdk::Variant status_; ///< The status of the value. Either an empty string if status is ok or a string with
                                   ///< the values ok, stale or disconnected.
};
}
}
