// @copyright_start
// Copyright (c) 2018 Cisco and/or its affiliates. All rights reserved.
// @copyright_end

/// @file efm_node_updater.h

#pragma once

#include <efm_action.h>
#include <efm_node_path.h>
#include <efm_optional.h>

#include <functional>
#include <memory>
#include <set>


namespace cisco
{
namespace efm_sdk
{

/// @brief Defines the structure of a node metadata update of an existing responder node.

/// The NodeUpdater will not update the metadata of the node itself, but provide a node metadata update description.
/// Every attribute method called will affect the node update description. Only attributes set in the update description
/// will be changed in the corresponding node. All other attributes will stay unchanged.
class NodeUpdater final
{
public:
  /// @brief The description of a node to update.
  struct NodeUpdateDescription
  {
    Optional<std::string> display_name_; ///< Display name of the node
    Optional<ValueType> type_;           ///< cisco::efm_sdk::ValueType of the node
    Optional<std::string> enum_values_;  ///< Enum values as a coma separated list if NodeDescription::type_ is an enum
    Optional<PermissionLevel> permission_;                        ///< permission level of the node
    Optional<Writable> writable_;                                 ///< cisco::efm_sdk::Writable of the node
    std::function<void(const Variant& value)> writable_callback_; ///< cisco::efm_sdk::Writable callback
    std::function<void(bool subscribe)>
      on_subscribe_; ///< On subscribe callback to be informed about subscribers to the node

    Optional<bool> hidden_;                    ///< If this node is hidden and not visible in a GUI or not
    Optional<std::shared_ptr<Action>> action_; ///< The optional action definition of the node

    std::map<std::string, Variant> configs_;    ///< Added or changed custom config values
    std::set<std::string> remove_configs_;      ///< Removed custom config values
    std::map<std::string, Variant> attributes_; ///< Added or changed custom attribute values
    std::set<std::string> remove_attributes_;   ///< Removed custom attribute values
  };

  /// Constructs a NodeUpdater instance.
  /// @param node_path The path of the node to update the metadata.
  NodeUpdater(const NodePath& node_path);

  /// Constructs a NodeUpdater instance.
  /// @param node_path The path of the node to update the metadata.
  NodeUpdater(NodePath&& node_path);

  /// This class is not copyable
  NodeUpdater(const NodeUpdater&) = delete;
  /// Default move constructor
  NodeUpdater(NodeUpdater&&) = default;
  /// This class is not assignable
  /// @return A reference to the NodeUpdater object
  NodeUpdater& operator=(const NodeUpdater&) = delete;
  /// Default move assignment
  /// @return A reference to the NodeUpdater object
  NodeUpdater& operator=(NodeUpdater&&) = default;

  /// Sets the display name of the node. The display name will be shown in GUI clients instead of the node name if it
  /// was set.
  /// @param name The name to set as display name.
  /// @return The NodeUpdater for method chaining
  NodeUpdater& display_name(std::string&& name);

  /// Sets the display name of the node. The display name will be shown in GUI clients instead of the node name if it
  /// was set.
  /// @param name The name to set as display name.
  /// @return The NodeUpdater for method chaining
  NodeUpdater& display_name(const std::string& name);

  /// Sets the cisco::efm_sdk::ValueType of the node.
  /// @param type The cisco::efm_sdk::ValueType to set the node to.
  /// @return The NodeUpdater for method chaining
  NodeUpdater& type(ValueType type);

  /// Sets the enum values for an enum cisco::efm_sdk::ValueType. The values have to be supplied as a comma separated
  /// value list.
  /// @param enums The enum values as a comma separated value list.
  /// @return The NodeUpdater for method chaining
  NodeUpdater& enum_values(const std::string& enums);

  /// Sets the enum values for an enum cisco::efm_sdk::ValueType. The values have to be supplied as a comma separated
  /// value list.
  /// @param enums The enum values as a comma separated value list.
  /// @return The NodeUpdater for method chaining
  NodeUpdater& enum_values(std::string&& enums);

  /// Sets the permission level of the node.
  /// @param permission The permission level to set.
  /// @return The NodeUpdater for method chaining
  NodeUpdater& permission(PermissionLevel permission);

  /// Sets the cisco::efm_sdk::Writable config of the node.
  /// @param writable The writable config to set.
  /// @return The NodeUpdater for method chaining
  NodeUpdater& writable(Writable writable);

  /// Sets the cisco::efm_sdk::Writable config of the node. If set to Write or Config, the nodes value will be settable
  /// via the \@set action.
  /// @param writable The writable config to set.
  /// @param callback The callback will be called, if the value is changed by a \@set action.
  /// @return The NodeUpdater for method chaining
  NodeUpdater& writable(Writable writable, std::function<void(const Variant& value)> callback);

  /// Sets the on subscribe callback. The callback will be fired upon subscribe or unsubscribe to the node.
  /// @param callback The callback that will be called. The callbacks parameter will be true if a subscribe has been
  /// made and false if an unsubscribe has been made to the node.
  /// @return The NodeUpdater for method chaining
  NodeUpdater& on_subscribe(std::function<void(bool subscribe)> callback);

  /// Sets the node to hidden. It will be included in list operations, but will not be shown in GUI clients.
  /// @return The NodeUpdater for method chaining
  NodeUpdater& hidden();

  /// Sets the node to not be hidden anymore. It will be included in list operations if hidden, but will not be shown in
  /// GUI clients.
  /// @return The NodeUpdater for method chaining
  NodeUpdater& clear_hidden();

  /// Sets the action of the node. Nodes with an action will be shown in pop-up menus in GUI clients.
  /// @param action The action to set for this node.
  /// @return The NodeUpdater for method chaining
  NodeUpdater& action(Action&& action);

  /// Sets the action of the node. Nodes with an action will be shown in pop-up menus in GUI clients.
  /// @param action The action to set for this node.
  /// @return The NodeUpdater for method chaining
  NodeUpdater& action(const Action& action);

  /// Removes the action of the node. The node will have no action anymore.
  /// @return The NodeUpdater for method chaining
  NodeUpdater& remove_action();

  /// Adds or changes a custom config node value. Config keys may have a `$` prefix.
  /// @param key The custom config key. If the key was previously added, the value will be overwritten.
  /// @param value The custom config value to set.
  /// @return The NodeUpdater for method chaining
  NodeUpdater& config(const std::string& key, const Variant& value);

  /// Adds or changes a custom config node value. Config keys may have a `$` prefix.
  /// @param key The custom config key. If the key was previously added, the value will be overwritten.
  /// @param value The custom config value to set.
  /// @return The NodeUpdater for method chaining
  NodeUpdater& config(const std::string& key, Variant&& value);

  /// Removes the corresponding config from the node.
  /// @param key The key of the config to remove.
  /// @return The NodeUpdater for method chaining
  NodeUpdater& remove_config(const std::string& key);

  /// Adds or changes a custom attribute node value. Attribute keys my have a `@` prefix.
  /// @param key The custom attribute key. If the key was previously added, the value will be overwritten.
  /// @param value The custom attribute value to set.
  /// @return The NodeUpdater for method chaining
  NodeUpdater& attribute(const std::string& key, const Variant& value);

  /// Adds or changes a custom attribute node value. Attribute keys my have a `@` prefix.
  /// @param key The custom attribute key. If the key was previously added, the value will be overwritten.
  /// @param value The custom attribute value to set.
  /// @return The NodeUpdater for method chaining
  NodeUpdater& attribute(const std::string& key, Variant&& value);

  /// Removes the corresponding attribute from the node.
  /// @param key The key of the attribute to remove.
  /// @return The NodeUpdater for method chaining
  NodeUpdater& remove_attribute(const std::string& key);

  /// Returns the path of the node to update.
  /// @return The path of the node to update.
  const NodePath& get_path() const
  {
    return node_path_;
  }

  /// Returns the description of the node to update.
  /// @return The description of the node to update.
  const NodeUpdateDescription& get_description() const
  {
    return node_update_description_;
  }

private:
  NodePath node_path_;
  NodeUpdateDescription node_update_description_;
};
}
}
