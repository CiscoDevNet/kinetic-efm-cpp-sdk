// @copyright_start
// Copyright (c) 2018 Cisco and/or its affiliates. All rights reserved.
// @copyright_end

#include <efm_link.h>
#include <efm_link_options.h>

#include "requester_link.h"

int main(int argc, char* argv[])
{
  cisco::efm_sdk::FileConfigLoader loader;
  cisco::efm_sdk::LinkOptions options("Requester-Link", loader);
  if (!options.parse(argc, argv, std::cerr)) {
    return EXIT_FAILURE;
  }

  cisco::efm_sdk::Link link(std::move(options), cisco::efm_sdk::LinkType::Requester);

  RequesterLink requesterLink(link);

  link.set_on_initialized_handler(
    std::bind(&RequesterLink::initialize, &requesterLink, std::placeholders::_1, std::placeholders::_2));
  link.set_on_connected_handler(std::bind(&RequesterLink::connected, &requesterLink, std::placeholders::_1));
  link.set_on_disconnected_handler(std::bind(&RequesterLink::disconnected, &requesterLink, std::placeholders::_1));

  link.run();

  return EXIT_SUCCESS;
}
