// @copyright_start
// Copyright (c) 2018 Cisco and/or its affiliates. All rights reserved.
// @copyright_end

#include <efm_link_options.h>
#include <efm_logging.h>

#include "error_code.h"
#include "responder_link.h"

#include <iostream>


int main(int argc, char* argv[])
{
  cisco::efm_sdk::FileConfigLoader loader;
  cisco::efm_sdk::LinkOptions options("Responder-Link", loader);
  if (!options.parse(argc, argv, std::cerr)) {
    return EXIT_FAILURE;
  }

  cisco::efm_sdk::Link link(std::move(options), cisco::efm_sdk::LinkType::Responder);
  LOG_EFM_INFO(::responder_error_code::build_with_version, link.get_version_info());

  ResponderLink responder_link(link);

  link.set_on_initialized_handler(
    std::bind(&ResponderLink::initialize, &responder_link, std::placeholders::_1, std::placeholders::_2));
  link.set_on_connected_handler(std::bind(&ResponderLink::connected, &responder_link, std::placeholders::_1));
  link.set_on_disconnected_handler(std::bind(&ResponderLink::disconnected, &responder_link, std::placeholders::_1));

  link.run();

  return EXIT_SUCCESS;
}
