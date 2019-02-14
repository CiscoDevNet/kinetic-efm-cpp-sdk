// @copyright_start
// Copyright (c) 2018 Cisco and/or its affiliates. All rights reserved.
// @copyright_end

#include "error_code.h"

#include <string>


/// @private
class responder_error_category_t : public std::error_category
{
public:
  const char* name() const noexcept override
  {
    return "error";
  }

  std::string message(int condition) const override
  {
    auto err = static_cast<responder_error_code>(condition);

    switch (err) {
      case responder_error_code::build_with_version:
        return "Build using cpp SDK version";
      case responder_error_code::connected:
        return "Link connected";
      case responder_error_code::disconnected:
        return "Link disconnected";
      case responder_error_code::subscribed_text:
        return "Subscribed to /text";
      case responder_error_code::unsubscribed_text:
        return "Unsubscribed from /text";
      case responder_error_code::set_text:
        return "Text value set to";
    }

    return "<Unknown error>";
  }
};


const std::error_category& responder_error_category()
{
  static const responder_error_category_t instance{};
  return instance;
}
