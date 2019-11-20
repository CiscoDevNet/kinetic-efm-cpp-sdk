// @copyright_start
// Copyright (c) 2018-2019 Cisco and/or its affiliates. All rights reserved.
// @copyright_end

#include <efm_link_options.h>
#include <efm_logging.h>

#include "error_code.h"
#include "responder_link.h"

#include <iostream>


const std::string link_version{LINK_VERSION};

int main(int argc, char* argv[])
{
  cisco::efm_sdk::FileConfigLoader loader;
  cisco::efm_sdk::LinkOptions options("Responder-Link", loader);
  if (!options.parse(argc, argv, std::cerr)) {
    return EXIT_FAILURE;
  }

  std::unique_ptr<cisco::efm_sdk::Link> link;
  try {
    link.reset(new cisco::efm_sdk::Link(std::move(options), cisco::efm_sdk::LinkType::Responder, link_version));
  } catch (const std::exception& ex) {
    std::cerr << "Error: link creation failed - " << ex.what() << std::endl;
    return EXIT_FAILURE;
  }

  LOG_EFM_INFO(::responder_error_code::build_with_version, link->get_version_info());

  std::unique_ptr<ResponderLink> responder_link;
  try {
    responder_link.reset(new ResponderLink(*link));
  } catch (const std::exception& ex) {
    std::cerr << "Error: responder link creation failed - " << ex.what() << std::endl;
    return EXIT_FAILURE;
  }

  link->set_on_initialized_handler(
    std::bind(&ResponderLink::initialize, responder_link.get(), std::placeholders::_1, std::placeholders::_2));
  link->set_on_connected_handler(std::bind(&ResponderLink::connected, responder_link.get(), std::placeholders::_1));
  link->set_on_disconnected_handler(
    std::bind(&ResponderLink::disconnected, responder_link.get(), std::placeholders::_1));
  link->set_on_deinitialized_handler(
    std::bind(&ResponderLink::deinitialize, responder_link.get(), std::placeholders::_1, std::placeholders::_2));

  link->run();

  return EXIT_SUCCESS;
}
