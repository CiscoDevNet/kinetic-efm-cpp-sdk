// @copyright_start
// Copyright (c) 2018 Cisco and/or its affiliates. All rights reserved.
// @copyright_end

/// @file efm_requester.h

#pragma once

#include <efm_interfaces.h>
#include <efm_node_builder.h>
#include <efm_subscription_update.h>

#include <memory>


namespace cisco
{
namespace efm_sdk
{

/// @brief This is the requester part of a cisco::efm_sdk::Link. It is used to originate requests and receive
/// asynchronously responses from responders.

/// The Requester object will be created and managed by the cisco::efm_sdk::Link instance.
class Requester final
{
  /// Requester subscribe callback signature
  /// @param ec The error code defines if this action was successful or not.
  using on_subscribe_response = std::function<void(const std::error_code& ec)>;
  /// Requester list callback signature
  /// @param ec The error code defines if this action was successful or not.
  using on_list_response = std::function<void(const ListResponse& list, const std::error_code& ec)>;
  /// Requester invoke callback signature
  /// @param ec The error code defines if this action was successful or not.
  using on_invoke_response = std::function<void(const InvokeResponse& invoke, const std::error_code& ec)>;
  /// Requester set callback signature
  /// @param ec The error code defines if this action was successful or not.
  using on_set_response = std::function<void(const std::error_code& ec)>;
  /// Requester remove callback signature
  /// @param ec The error code defines if this action was successful or not.
  using on_remove_response = std::function<void(const std::error_code& ec)>;
  /// Requester unsubscribe callback signature
  /// @param ec The error code defines if this action was successful or not.
  using on_unsubscribe_response = std::function<void(const std::error_code& ec)>;
  /// Requester close callback signature
  /// @param ec The error code defines if this action was successful or not.
  using on_close_response = std::function<void(const std::error_code& ec)>;
  /// Requester subscription update callback signature
  /// @param path The path that originated the value update.
  /// @param update The actual value update.
  /// @param ec The error code defines if this action was successful or not.
  using on_subscription_update = std::function<
    void(const cisco::efm_sdk::NodePath& path, const SubscriptionUpdate& update, const std::error_code& ec)>;

public:
  /// This class is not movable
  Requester(Requester&&) = delete;
  /// This class is not move assignable
  /// @return A reference to the Requester object
  Requester& operator=(Requester&&) = delete;
  /// This class is not copyable
  Requester(const Requester&) = delete;
  /// This class is not assignable
  /// @return A reference to the Requester object
  Requester& operator=(const Requester&) = delete;

  /// Destroys the Requester object.
  ~Requester();

  /// Subscribes asynchronously to the given path of a responder. The responder will send value updates asynchronously
  /// to the supplied subscription_update callback. If subscribing multiple times to the same path, the last set
  /// subscription_update callback will be used to handle value updates.
  /// @param path The path to subscribe to.
  /// @param qos The cisco::efm_sdk::QoS value to use.
  /// @param update_callback This callback will be called for every value update pushed by the responder.
  /// @param callback Will be called as soon as the subscription was established.
  void subscribe(
    const cisco::efm_sdk::NodePath& path,
    cisco::efm_sdk::QoS qos,
    on_subscription_update&& update_callback,
    on_subscribe_response&& callback);

  /// Lists asynchronously the given path of a responder node model. The responder will send list updates asynchronously
  /// to the supplied list_update callback as long as the list operation is not closed.
  /// @param path The path to list.
  /// @param callback The callback to call for updates.
  void list(const NodePath& path, on_list_response&& callback);

  /// Invokes asynchronously the action corresponding to the given responder path if it exists. The action result will
  /// be returned asynchronously via the invoke_response callback.
  /// @param path The path to the responder action.
  /// @param params The parameter to send for the action.
  /// @param permission The permission level to use to call the action.
  /// @param callback The callback to call for the action result.
  void invoke(const NodePath& path, const Variant& params, PermissionLevel permission, on_invoke_response&& callback);

  /// Sets asynchronously a value, attribute or config on the given path.
  /// @param path The path to set.
  /// @param value The value to set.
  /// @param permission The cisco::efm_sdk::Permission to use for the set.
  /// @param callback The callback will be called as soon as the set has been requested.
  void set(
    const cisco::efm_sdk::NodePath& path,
    const cisco::efm_sdk::Variant& value,
    PermissionLevel permission,
    on_set_response&& callback);

  /// Removes asynchronously an attribute or config on the given path.
  /// @param path The path to remove.
  /// @param callback The callback will be called as soon as the remove has been requested.
  void remove(const cisco::efm_sdk::NodePath& path, on_remove_response&& callback);

  /// Unsubscribes asynchronously from a previously established subscription.
  /// @param paths The paths to unsubscribe from.
  /// @param callback The callback will be called as soon as the unsubscribe operation finishes.
  void unsubscribe(const std::vector<NodePath>& paths, on_unsubscribe_response&& callback);

  /// Closes asynchronously a stream.
  /// @param callback The callback will be called as soon as the close operation finishes.
  void close(on_close_response&& callback);

private:
  friend class Link;
  Requester(void* link);

  class Impl;
  Impl* impl_;
};
}
}
