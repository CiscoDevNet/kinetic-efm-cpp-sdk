// @copyright_start
// Copyright (c) 2018-2019 Cisco and/or its affiliates. All rights reserved.
// @copyright_end

#include <efm_link.h>
#include <efm_link_options.h>

#include "requester_link.h"


const std::string link_version{LINK_VERSION};

int main(int argc, char* argv[])
{
  cisco::efm_sdk::FileConfigLoader loader;
  cisco::efm_sdk::LinkOptions options("Requester-Link", loader);
  if (!options.parse(argc, argv, std::cerr)) {
    return EXIT_FAILURE;
  }

  std::unique_ptr<cisco::efm_sdk::Link> link;
  try {
    link.reset(new cisco::efm_sdk::Link(std::move(options), cisco::efm_sdk::LinkType::Requester, link_version));
  } catch (const std::exception& ex) {
    std::cerr << "Error: link creation failed - " << ex.what() << std::endl;
    return EXIT_FAILURE;
  }

  std::unique_ptr<RequesterLink> requester_link;
  try {
    requester_link.reset(new RequesterLink(*link));
  } catch (const std::exception& ex) {
    std::cerr << "Error: requester link creation failed - " << ex.what() << std::endl;
    return EXIT_FAILURE;
  }

  link->set_on_initialized_handler(
    std::bind(&RequesterLink::initialize, requester_link.get(), std::placeholders::_1, std::placeholders::_2));
  link->set_on_connected_handler(std::bind(&RequesterLink::connected, requester_link.get(), std::placeholders::_1));
  link->set_on_disconnected_handler(
    std::bind(&RequesterLink::disconnected, requester_link.get(), std::placeholders::_1));
  link->set_on_deinitialized_handler(
    std::bind(&RequesterLink::deinitialize, requester_link.get(), std::placeholders::_1, std::placeholders::_2));

  link->run();

  return EXIT_SUCCESS;
}
