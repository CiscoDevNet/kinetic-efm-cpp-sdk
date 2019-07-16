// @copyright_start
// Copyright (c) 2018-2019 Cisco and/or its affiliates. All rights reserved.
// @copyright_end

/// @file requester_link.h

#pragma once

#include <efm_link.h>

#include <iostream>

using namespace cisco::efm_sdk;

/// @brief The requester link example demonstrates the EFM SDK API for requester implementations.

class RequesterLink
{
public:
  /// Constructs the requester link implementation.
  /// @param link The link to work with.
  RequesterLink(cisco::efm_sdk::Link& link)
    : link_(link)
    , requester_(link_.requester())
  {
  }

  /// The initialize callback that will be called as soon as the initialization including serialization is complete.
  /// @param link_name The name of the link.
  /// @param ec The error code will be set to an error if the initialization failed.
  void initialize(const std::string& link_name, const std::error_code& ec)
  {
    std::cerr << "RequesterLink - initialize" << std::endl;
    if (!ec) {
      std::cerr << "RequesterLink - Requester link initialized - " << link_name << std::endl;
    } else {
      std::cerr << "Requester link initialization failed - " << ec.message() << std::endl;
    }
  }

  /// Called every time the link connects to the broker.
  /// @param ec The error code will be set to an error if the connect failed.
  void connected(const std::error_code& ec)
  {
    std::cerr << "RequesterLink - connected" << std::endl;
    if (!ec) {
      std::cerr << "RequesterLink - Requester link connected!" << std::endl;

      requester_.list(
        "/downstream/Responder-Link",
        std::bind(&RequesterLink::on_list_response, this, std::placeholders::_1, std::placeholders::_2));

      requester_.subscribe(
        "/downstream/Responder-Link/seq",
        QoS::Persistent,
        std::bind(
          &RequesterLink::on_subscription_update,
          this,
          std::placeholders::_1,
          std::placeholders::_2,
          std::placeholders::_3),
        std::bind(&RequesterLink::on_subscribe_response, this, std::placeholders::_1));

      /// This node will not be available at the first start of the requester. It can be added to the responder via
      /// the 'Add Node' action of the /downstream/Responder-Link/nodes node in the dataflow editor. The node named
      /// 'subscription' has to be added with a 'Value Type' other than none. After the nodes have been added, a value
      /// can be set via the \@set action of the /downstream/Responder-Link/nodes/test/subscription node. This value
      /// should then be displayed in the requesters output as subscription update.
      requester_.subscribe(
        "/downstream/Responder-Link/nodes/test/subscription",
        QoS::None,
        std::bind(
          &RequesterLink::on_subscription_update,
          this,
          std::placeholders::_1,
          std::placeholders::_2,
          std::placeholders::_3),
        std::bind(&RequesterLink::on_subscribe_response, this, std::placeholders::_1));

      const cisco::efm_sdk::Variant::MapType m = {{"Input", "Cisco Kinect DSLink C++ SDK Example"}};
      requester_.invoke(
        "/downstream/Responder-Link/echo",
        m,
        cisco::efm_sdk::PermissionLevel::Read,
        std::bind(&RequesterLink::on_invoke_response, this, std::placeholders::_1, std::placeholders::_2));
    } else {
      std::cerr << "Requester link connect failed - " << ec.message() << std::endl;
    }
  }

  /// Called every time the link is disconnected from the broker.
  /// @param ec The error code will be set to an error if the disconnect failed.
  void disconnected(const std::error_code& ec)
  {
    std::cerr << "RequesterLink - disconnected" << std::endl;
    if (ec) {
      std::cerr << "Requester link disconnect failed - " << ec.message() << std::endl;
    }
  }

  /// The deinitialize callback that will be called as soon as the link is being stopped.
  /// @param link_name The name of the link.
  /// @param ec The error code will be set to an error if the deinitialization failed.
  void deinitialize(const std::string& link_name, const std::error_code& ec)
  {
    std::cerr << "RequesterLink - deinitialize" << std::endl;
    if (!ec) {
      std::cerr << "RequesterLink - Requester link deinitialized - " << link_name << std::endl;
    } else {
      std::cerr << "Requester link deinitialization failed - " << ec.message() << std::endl;
    }
  }

  /// Callback will be called upon list responses from the peer.
  /// @param list The list response object.
  /// @param ec The error code will be set to an error if the list response failed.
  void on_list_response(const ListResponse& list, const std::error_code& ec)
  {
    if (!ec) {
      std::cerr << "RequesterLink - on_list_response - " << list.updates_ << std::endl;
    } else {
      std::cerr << "Requester list failed - " << ec.message() << std::endl;
    }
  }

  /// Callback will be called upon invoke responses from the peer.
  /// @param invoke The invoke response object.
  /// @param ec The error code will be set to an error if the invoke failed.
  void on_invoke_response(const InvokeResponse& invoke, const std::error_code& ec)
  {
    if (!ec) {
      std::cerr << "RequesterLink - on_invoke_response - " << invoke.updates_ << std::endl;
    } else {
      std::cerr << "Requester invoke failed - " << ec.message() << std::endl;
    }
  }

  /// Callback will be called upon subscribe responses from the peer. After the callback, the peer will start to send
  /// subscription updates.
  /// @param ec The error code will be set to an error if the subscribe failed.
  void on_subscribe_response(const std::error_code& ec)
  {
    if (!ec) {
      std::cerr << "RequesterLink - on_subscribe_response" << std::endl;
    } else {
      std::cerr << "Requester subscribe failed - " << ec.message() << std::endl;
    }
  }

  /// Callback will be called upon subscribe update responses from the peer. The peer will send updates until the
  /// requester unsubscribes from the paths.
  /// @param path The path of the node sending the subscription value update.
  /// @param update The update object with the new value and timestamp.
  /// @param ec The error code will be set to an error if the subscription update failed.
  void on_subscription_update(const NodePath& path, const SubscriptionUpdate& update, const std::error_code& ec)
  {
    if (!ec) {
      std::cerr << "RequesterLink - on_subscription_update - " << path << " - " << update.value_ << " "
                << update.time_stamp_ << std::endl;
    } else {
      std::cerr << "Requester subscription update failure - " << ec.message() << std::endl;
    }
  }

private:
  cisco::efm_sdk::Link& link_;
  Requester& requester_;
};
