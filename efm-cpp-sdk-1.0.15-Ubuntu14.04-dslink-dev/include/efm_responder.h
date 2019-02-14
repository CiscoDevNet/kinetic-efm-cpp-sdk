// @copyright_start
// Copyright (c) 2018 Cisco and/or its affiliates. All rights reserved.
// @copyright_end

/// @file efm_responder.h

#pragma once

#include <efm_interfaces.h>
#include <efm_node_builder.h>
#include <efm_node_updater.h>

#include <chrono>
#include <memory>


namespace cisco
{
namespace efm_sdk
{

/// @brief Properties of a distinct node
struct NodeInfo
{
  NodePath node_path_;              ///< Path of this node
  std::string name_;                ///< Name of the node
  std::string profile_;             ///< Profile of the node
  bool serializable_{false};        ///< If this node will be persistent
  std::string display_name_;        ///< Display name of the node
  ValueType type_{ValueType::None}; ///< cisco::efm_sdk::ValueType of the node
  std::string enum_values_;         ///< Enum values as a coma separated list if NodeInfo::type_ is an enum
  PermissionLevel permission_{PermissionLevel::Read}; ///< permission level of the node
  Writable writable_{Writable::Never};                ///< cisco::efm_sdk::Writable of the node
  Variant value_;                                     ///< Current value of the node
  bool is_hidden_{false};                             ///< If this node is hidden and not visible in a GUI or not
  bool is_action_{false};                             ///< If this node is an action
  std::chrono::system_clock::time_point timestamp_;   ///< When the value was set
  std::map<std::string, Variant> configs_;            ///< Custom config values
  std::map<std::string, Variant> attributes_;         ///< Custom attribute values
};


/// @brief Description of a node. This class is used for responder function create_node_or_set_value
class NodeCreateOrSetValueDescription
{
public:
  /// Constructor
  ///
  /// @param node_path The parent path of the node to set the value for.
  /// @param profile Profile of the node.
  /// @param display_name The parent path of the node to set the value for.
  /// @param type cisco::efm_sdk::ValueType of the node.
  /// @param value Initial value of the node.
  /// @param timestamp The timestamp of the values actual update time.
  NodeCreateOrSetValueDescription(
    const NodePath& node_path,
    const std::string& profile,
    const std::string& display_name,
    ValueType type,
    const Variant& value,
    std::chrono::system_clock::time_point timestamp = std::chrono::system_clock::now())
    : node_path_(node_path)
    , profile_(profile)
    , display_name_(display_name)
    , type_(type)
    , value_(value)
    , timestamp_(timestamp)
  {
  }

  /// Constructor
  ///
  /// @param node_path The parent path of the node to set the value for.
  /// @param profile Profile of the node.
  /// @param display_name The parent path of the node to set the value for.
  /// @param type cisco::efm_sdk::ValueType of the node.
  /// @param value Initial value of the node.
  /// @param enum_values Enum values as a coma separated list if NodeDescription::type_ is an enum.
  /// @param permission permission level of the node.
  /// @param writable cisco::efm_sdk::Writable of the node.
  /// @param serializable If this node will be persistent.
  /// @param timestamp The timestamp of the values actual update time.
  NodeCreateOrSetValueDescription(
    const NodePath& node_path,
    const std::string& profile,
    const std::string& display_name,
    ValueType type,
    const Variant& value,
    const std::string& enum_values,
    PermissionLevel permission,
    Writable writable,
    bool serializable,
    std::chrono::system_clock::time_point timestamp = std::chrono::system_clock::now())
    : node_path_(node_path)
    , profile_(profile)
    , display_name_(display_name)
    , type_(type)
    , value_(value)
    , timestamp_(timestamp)
    , enum_values_(enum_values)
    , permission_(permission)
    , writable_(writable)
    , serializable_(serializable)
  {
  }
  NodePath node_path_;                              ///< The parent path of the node to set the value for.
  std::string profile_;                             ///< Profile of the node.
  std::string display_name_;                        ///< The parent path of the node to set the value for.
  ValueType type_{ValueType::None};                 ///< cisco::efm_sdk::ValueType of the node.
  Variant value_;                                   ///< Initial value of the node.
  std::chrono::system_clock::time_point timestamp_; ///< The timestamp of the values actual update time.
  std::string enum_values_; ///< Enum values as a coma separated list if NodeDescription::type_ is an enum.
  PermissionLevel permission_{PermissionLevel::Read}; ///< cisco::efm_sdk::Permission of the node.
  Writable writable_{Writable::Never};                ///< cisco::efm_sdk::Writable of the node.
  bool serializable_{false};                          ///< If this node will be persistent.
};

/// @brief This is the responder part of a link. It is used to add and manipulate nodes of the responder node model. The
/// Responder will respond to requests originating from requesters. It will also asynchronously push value and list
/// updates to subscribers.

/// The Responder object will be created and managed by the cisco::efm_sdk::Link instance.
class Responder final
{
public:
  /// This class is not movable
  Responder(Responder&&) = delete;
  /// This class is not move assignable
  /// @return A reference to the Responder object
  Responder& operator=(Responder&&) = delete;
  /// This class is not copyable
  Responder(const Responder&) = delete;
  /// This class is not assignable
  /// @return A reference to the Responder object
  Responder& operator=(const Responder&) = delete;

  /// Destroys the Responder object.
  ~Responder();

  /// Adds a callback for a path pattern. Every time a node is created with a path that matches the path pattern, the
  /// callback will be called. The callback will provide the nodes path and an error code as parameter to the call.This
  /// is useful for example to add action nodes to nodes that are deserialized. The path pattern is used to identify
  /// the entry. If a path pattern already exists, it will be overwritten by the last call. Adding a pattern that only
  /// matches the root path "/" will have no effect.
  /// @param path_pattern The path pattern to match. Follows the DQL syntax for wildcards.
  /// @param callback The callback to call if the path of a created node matches the pattern.
  /// @return true if the callback could be added, false if the path pattern was invalid.
  bool add_node_creation_callback(
    const std::string& path_pattern,
    std::function<void(const NodePath&, NodeCreationContext context, const std::error_code&)>&& callback);

  /// Removes a previously added callback for a path pattern.
  /// @param path_pattern the path pattern to remove the callback for.
  void remove_node_creation_callback(const std::string& path_pattern);

  /// Registers a callback for a distinct event on the responder.
  /// @note When called with the same path/callback_on twice the second call will replace the previous one.
  /// @param path The node path the event might happen on.
  /// @param callback_on The responder event to listen on.
  /// @param responder_callback The callback will be called when the specified event happened on the given path.
  void register_callback(
    const NodePath& path,
    CallbackOn callback_on,
    std::function<void(const cisco::efm_sdk::NodePath&, cisco::efm_sdk::CallbackOn)>&& responder_callback);

  /// Unregisters a previously registered callback
  /// @note When no callback was registered with the specified path/callback_on nothing will happen
  /// @param path The node path the event might happen on.
  /// @param callback_on The responder event to listen on.
  void unregister_callback(const NodePath& path, CallbackOn callback_on);

  /// Add asynchronously the nodes described by the given NodeBuilder to the responders node model. If the nodes
  /// cannot be added (i.e. the parent path does not exist), the callback will issue a corresponding error code. If
  /// successful, the callback will return all newly created paths.
  /// @param builder The node descriptions to add.
  /// @param callback The callback will be called as soon as the add node operation has finished.
  void add_node(
    NodeBuilder&& builder,
    std::function<void(const std::vector<NodePath>&, const std::error_code&)>&& callback);

  /// Remove asynchronously the given path from the responders node model. All child nodes of the node pointed by
  /// the path will also be removed. If the path can't be removed (the path doesn't exist), the callback will return a
  /// corresponding error code.
  /// @param path The path to remove.
  /// @param callback The callback will be called as soon as the remove node operation has finished.
  void remove_node(const NodePath& path, std::function<void(const std::error_code&)>&& callback);

  /// Updates a nodes metadata asynchronously. The path to the node is specified by the NodeUpdater. If the node does
  /// not exist, the callback will return a corresponding error code.
  /// @param updater The node metadata changes to execute on the node.
  /// @param callback The callback will be called as soon as the update node operation has finished.
  void update_node(NodeUpdater&& updater, std::function<void(const std::error_code&)>&& callback);

  /// Checks if the given path exists. As the SDK works in an asynchronous way, the existence of a path can only be a
  /// snapshot of the current status while calling this method. Upon returning from this method, the status can change
  /// anytime asynchronously, including before checking the return value of this method.
  /// @param path The path to check.
  /// @return true if the path exists, otherwise false.
  bool exists(const NodePath& path) const;

  /// Retrieve asynchronously the paths of the children of the given parent path if any available. If the node does not
  /// exist, the callback will return a corresponding error code. If successful, the callback will return the child
  /// paths.
  /// @param parent_path The path to return the children for.
  /// @param callback The callback will be called as soon as the enumerate children operation has finished.
  void enumerate_children(
    const NodePath& parent_path,
    std::function<void(const std::vector<NodePath>&, const std::error_code&)>&& callback);

  /// Retrieve asynchronously the value of the given path. If no value exists for the path, the callback will return an
  /// error.
  /// @param path The path of the node to retrieve the value for.
  /// @param callback The callback will be called as soon as the get value operation has finished. If the error code
  /// flags no error, the value will be set to the current value of the path.
  void get_value(const NodePath& path, std::function<void(const Variant& value, const std::error_code& ec)>&& callback);

  /// Asynchronously sets the value for the given path. If no value for the path exists, the callback will return an
  /// error.
  /// @param path The path of the node to set the value for.
  /// @param value The value to set.
  /// @param timestamp The timestamp of the values actual update time.
  /// @param callback The callback will be called as soon as the set value operation has finished. If the error code
  /// flags no error, the node value will be set to the given value.
  void set_value(
    const NodePath& path,
    Variant&& value,
    const std::chrono::system_clock::time_point& timestamp,
    std::function<void(const std::error_code&)>&& callback);

  /// Asynchronously sets the value for the given path. If no value for the path exists, the callback will return an
  /// error.
  /// @param path The path of the node to set the value for.
  /// @param value The value to set.
  /// @param timestamp The timestamp of the values actual update time.
  /// @param callback The callback will be called as soon as the set value operation has finished. If the error code
  /// flags no error, the node value will be set to the given value.
  void set_value(
    const NodePath& path,
    const Variant& value,
    const std::chrono::system_clock::time_point& timestamp,
    std::function<void(const std::error_code&)>&& callback);

  /// Asynchronously sets the value for the given path. If no value for the path exists, the callback will return an
  /// error. The timestamp of the value will be set to the current date and time.
  /// @param path The path of the node to set the value for.
  /// @param value The value to set.
  /// @param callback The callback will be called as soon as the set value operation has finished. If the error code
  /// flags no error, the node value will be set to the given value.
  void set_value(const NodePath& path, Variant&& value, std::function<void(const std::error_code&)>&& callback);

  /// Asynchronously sets the value for the given path. If no value for the path exists, the callback will return an
  /// error. The timestamp of the value will be set to the current date and time.
  /// @param path The path of the node to set the value for.
  /// @param value The value to set.
  /// @param callback The callback will be called as soon as the set value operation has finished. If the error code
  /// flags no error, the node value will be set to the given value.
  void set_value(const NodePath& path, const Variant& value, std::function<void(const std::error_code&)>&& callback);

  /// Retrieve asynchronously the config value of the given path. If no config value exists for the path, the callback
  /// will return an error.
  /// @param path The path of the node to retrieve the config value for.
  /// @param config_name The name of the config value to retrieve.
  /// @param callback The callback will be called as soon as the get config operation has finished. If the error code
  /// flags no error, the value will be set to the current value of the config.
  void get_config(
    const NodePath& path,
    const std::string& config_name,
    std::function<void(const Variant& value, const std::error_code& ec)>&& callback);

  /// Retrieve asynchronously the attribute value of the given path. If no attribute value exists for the path, the
  /// callback will return an error.
  /// @param path The path of the node to retrieve the attribute value for.
  /// @param attribute_name The name of the attribute value to retrieve.
  /// @param callback The callback will be called as soon as the get attribute operation has finished. If the error code
  /// flags no error, the value will be set to the current value of the attribute.
  void get_attribute(
    const NodePath& path,
    const std::string& attribute_name,
    std::function<void(const Variant& value, const std::error_code& ec)>&& callback);

  /// Retrieve asynchronously all information a node provides by a given path.
  /// @param path The path of the node to retrieve the information for.
  /// @param callback The callback will be called as soon as the get node info operation has finished. If the error code
  /// flags no error, the node_info value contains all information the requested node provides. If the error code
  /// indicates an error, the node_info passed into the callback will contain the path given to the get_node_info call,
  /// even if that path doesn't exist. The rest of the node_info structure will be filled with default values.
  void get_node_info(
    const NodePath& path,
    std::function<void(const NodeInfo& node_info, const std::error_code& ec)>&& callback);

  /// Creates a node if it doesn't exist or set a new value on specified node asynchronously.
  /// @param desc Information about the node to create or a value to set on the specified node.
  /// @param callback The callback will be called as soon as the get node info operation has finished.
  /// If the error code flags no error, the link_path contains the path of the node. If the error code
  /// indicates an error, the link_path contains the path where the error occurs.
  /// even if that path doesn't exist. The rest of the node_info structure will be filled with default values.
  void create_node_or_set_value(
    const cisco::efm_sdk::NodeCreateOrSetValueDescription& desc,
    std::function<void(const cisco::efm_sdk::NodePath& link_path, const std::error_code& ec)>&& callback);

private:
  friend class Link;
  Responder(void* link);

  class Impl;
  Impl* impl_;
};
}
}
