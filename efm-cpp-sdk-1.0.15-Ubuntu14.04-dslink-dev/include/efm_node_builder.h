// @copyright_start
// Copyright (c) 2018 Cisco and/or its affiliates. All rights reserved.
// @copyright_end

/// @file efm_node_builder.h

#pragma once

#include <efm_action.h>
#include <efm_node_path.h>
#include <efm_variant.h>

#include <chrono>
#include <functional>
#include <memory>


namespace cisco
{
namespace efm_sdk
{

/// @brief Defines the structure of new nodes to add to a responder.

/// The NodeBuilder will not build the nodes itself, but provide a description of a collection of nodes, that shall be
/// created by a responder. All nodes will be created as children of the parent node. The first method to call is always
/// the NodeBuilder::make_node method. It establishes a new node description on which attribute methods like
/// NodeBuilder::display_name etc. can be called. Every attribute method call after a NodeBuilder::make_node call, will
/// be called on the current node description. The next node to create can be initiated by calling
/// NodeBuilder::make_node again.
class NodeBuilder final
{
public:
  /// @brief The description of a node to create.
  struct NodeDescription
  {
    std::string name_;                                        ///< Name of the node
    std::string profile_;                                     ///< Profile of the node
    SerializationMode serializable_{SerializationMode::None}; ///< How this node will be persisted
    std::string display_name_;                                ///< Display name of the node
    ValueType type_{ValueType::None};                         ///< cisco::efm_sdk::ValueType of the node
    std::string enum_values_; ///< Enum values as a coma separated list if NodeDescription::type_ is an enum
    PermissionLevel permission_level_{PermissionLevel::Read};     ///< permissions level of the node
    Writable writable_{Writable::Never};                          ///< cisco::efm_sdk::Writable of the node
    std::function<void(const Variant& value)> writable_callback_; ///< cisco::efm_sdk::Writable callback
    std::function<void(bool subscribe)>
      on_subscribe_; ///< On subscribe callback to be informed about subscribers to the node
    Variant value_;  ///< Initial value of the node
    std::chrono::system_clock::time_point timestamp_{
      std::chrono::system_clock::time_point::max()}; ///< Timestamp of the initial value of the node (defaults to now())
    bool hidden_{false};                             ///< If this node is hidden and not visible in a GUI or not
    IEditorPtr editor_;                              ///< The editor for the value, if available.
    std::shared_ptr<Action> action_;                 ///< The optional action definition of the node
    std::map<std::string, Variant> configs_;         ///< Custom config values
    std::map<std::string, Variant> attributes_;      ///< Custom attribute values
  };

  /// @brief Iterator to iterate over the NodeBuilder node descriptions.
  class ConstIterator
  {
  public:
    /// the iterator type
    using iterator = ConstIterator;
    /// @private
    using iterator_category = std::input_iterator_tag;
    /// @private
    using value_type = const NodeDescription;
    /// @private
    using difference_type = std::ptrdiff_t;
    /// alias for const pointer to NodeDescription
    using pointer = const NodeDescription*;
    /// alias for the const reference to NodeDescription
    using reference = const NodeDescription&;

    /// dereferences the iterator
    /// @return A reference to the object behind the iterator
    reference operator*() const
    {
      return *_iter;
    }

    /// dereferences the iterator
    /// @return A pointer to the object behind the iterator
    pointer operator->() const
    {
      return &(*_iter);
    }

    /// increments the iterator
    /// @return A reference to the incremented iterator
    iterator& operator++()
    {
      ++_iter;
      return *this;
    }

    /// Compare this iterator to another one for equality.
    /// @param other The iterator to compare to this one.
    /// @return true if both iterators are identical, false otherwise.
    bool operator==(const ConstIterator& other) const
    {
      return _iter == other._iter;
    }

    /// Compare this iterator to another one for inequality.
    /// @param other The iterator to compare to this one.
    /// @return true if both iterators are not identical, false otherwise.
    bool operator!=(const ConstIterator& other) const
    {
      return _iter != other._iter;
    }

  private:
    friend class NodeBuilder;

    ConstIterator(const NodeBuilder& builder, bool end = false)
      : _iter(end ? builder.nodes_.end() : builder.nodes_.begin())
    {
    }

    std::vector<NodeDescription>::const_iterator _iter;
  };

  /// Constructs a NodeBuilder instance.
  /// @param parent_path The path of the parent to add this node as a child node to.
  NodeBuilder(const NodePath& parent_path);

  /// Constructs a NodeBuilder instance.
  /// @param parent_path The path of the parent to add this node as a child node to.
  NodeBuilder(NodePath&& parent_path);

  /// This class is not copyable
  NodeBuilder(const NodeBuilder&) = delete;

  /// Default move constructor
  NodeBuilder(NodeBuilder&&) = default;

  /// This class is not assignable
  /// @return A reference to the NodeBuilder object
  NodeBuilder& operator=(const NodeBuilder&) = delete;

  /// Default move assignment
  /// @return A reference to the NodeBuilder object
  NodeBuilder& operator=(NodeBuilder&&) = default;

  /// Starts the definition of a new node. Every attribute method call after this call will be made in the context of
  /// this node.
  /// @throw Will throw an exception if the name is empty or has illegal characters. Illegal characters are: /\?*:|"<>.
  /// @param name Name of the new node. Has to be unique in the collection of new nodes and the child collection of the
  /// parent node.
  /// @param profile The profile of the new node.
  /// @return The NodeBuilder for method chaining
  NodeBuilder& make_node(const std::string& name, const std::string& profile);

  /// Starts the definition of a new node. Every attribute method call after this call will be made in the context of
  /// this node. The profile will be set to 'node' for this node.
  /// @throw Will throw an exception if the name is empty or has illegal characters. Illegal characters are: /\?*:|"<>.
  /// @param name Name of the new node. Has to be unique in the collection of new nodes and the child collection of the
  /// parent node.
  /// @return The NodeBuilder for method chaining
  NodeBuilder& make_node(const std::string& name);

  /// Starts the definition of a new node. Every attribute method call after this call will be made in the context of
  /// this node.
  /// @throw Will throw an exception if the name is empty or has illegal characters. Illegal characters are: /\?*:|"<>.
  /// @param name Name of the new node. Has to be unique in the collection of new nodes and the child collection of the
  /// parent node.
  /// @param profile The profile of the new node.
  /// @return The NodeBuilder for method chaining
  NodeBuilder& make_node(std::string&& name, std::string&& profile);

  /// Starts the definition of a new node. Every attribute method call after this call will be made in the context of
  /// this node. The profile will be set to 'node' for this node.
  /// @throw Will throw an exception if the name is empty or has illegal characters. Illegal characters are: /\?*:|"<>.
  /// @param name Name of the new node. Has to be unique in the collection of new nodes and the child collection of the
  /// parent node.
  /// @return The NodeBuilder for method chaining
  NodeBuilder& make_node(std::string&& name);

  /// Sets the node to be serializable. The node metadata information will be serialized into a persistent store.
  /// @throws If there is no current node available. Call NodeBuilder::make_node first.
  /// @param mode The serialization mode for the node.
  /// @return The NodeBuilder for method chaining
  NodeBuilder& serializable(SerializationMode mode = SerializationMode::Everything);

  /// Sets the display name of the node. The display name will be shown in GUI clients instead of the node name if it
  /// was set.
  /// @throws If there is no current node available. Call NodeBuilder::make_node first.
  /// @param name The name to set as display name.
  /// @return The NodeBuilder for method chaining
  NodeBuilder& display_name(std::string&& name);

  /// Sets the display name of the node. The display name will be shown in GUI clients instead of the node name if it
  /// was set.
  /// @throws If there is no current node available. Call NodeBuilder::make_node first.
  /// @param name The name to set as display name.
  /// @return The NodeBuilder for method chaining
  NodeBuilder& display_name(const std::string& name);

  /// Sets the cisco::efm_sdk::ValueType of the node.
  /// @throws If there is no current node available. Call NodeBuilder::make_node first.
  /// @param type The cisco::efm_sdk::ValueType to set the node to.
  /// @return The NodeBuilder for method chaining
  NodeBuilder& type(ValueType type);

  /// Sets the enum values for an enum or bool cisco::efm_sdk::ValueType. The values have to be supplied as a comma
  /// separated value list. For a cisco::efm_sdk::ValueType::Bool, two strings have to be supplied. The first string is
  /// the label for false and the second for true.
  /// @throws If there is no current node available. Call NodeBuilder::make_node first.
  /// @throws If no type was set yet or the type is not cisco::efm_sdk::ValueType::Enum or
  /// cisco::efm_sdk::ValueType::Bool. Call NodeBuilder::type first.
  /// @param enums The enum values as a comma separated value list.
  /// @return The NodeBuilder for method chaining
  NodeBuilder& enum_values(const std::string& enums);

  /// Sets the enum values for an enum or bool cisco::efm_sdk::ValueType. The values have to be supplied as a comma
  /// separated value list. For a cisco::efm_sdk::ValueType::Bool, two strings have to be supplied. The first string is
  /// the label for false and the second for true.
  /// @throws If there is no current node available. Call NodeBuilder::make_node first.
  /// @throws If no type was set yet or the type is not cisco::efm_sdk::ValueType::Enum or
  /// cisco::efm_sdk::ValueType::Bool. Call NodeBuilder::type first.
  /// @param enums The enum values as a comma separated value list.
  /// @return The NodeBuilder for method chaining
  NodeBuilder& enum_values(std::string&& enums);

  /// Sets the permission level of the node.
  /// @throws If there is no current node available. Call NodeBuilder::make_node first.
  /// @param level The permission level to set.
  /// @return The NodeBuilder for method chaining
  NodeBuilder& permission(PermissionLevel level);

  /// Sets the cisco::efm_sdk::Writable config of the node. If set to Write or Config, the nodes value will be settable
  /// via the \@set action.
  /// @throws If there is no current node available. Call NodeBuilder::make_node first.
  /// @param writable The writable config to set.
  /// @return The NodeBuilder for method chaining
  NodeBuilder& writable(Writable writable);

  /// Sets the cisco::efm_sdk::Writable config of the node. If set to Write or Config, the nodes value will be settable
  /// via the \@set action.
  /// @throws If there is no current node available. Call NodeBuilder::make_node first.
  /// @throws If the writable config is set to Writable::Never, as no callback can be called.
  /// @param writable The writable config to set.
  /// @param callback The callback will be called, if the value is changed by a \@set action.
  /// @return The NodeBuilder for method chaining
  NodeBuilder& writable(Writable writable, std::function<void(const Variant& value)> callback);

  /// Sets the initial value of the node.
  /// @throws If there is no current node available. Call NodeBuilder::make_node first.
  /// @throws If no type was set yet. Call NodeBuilder::type first.
  /// @param value The initial value to set for the node.
  /// @return The NodeBuilder for method chaining
  NodeBuilder& value(Variant&& value);

  /// Sets the initial value of the node.
  /// @throws If there is no current node available. Call NodeBuilder::make_node first.
  /// @throws If no type was set yet. Call NodeBuilder::type first.
  /// @param value The initial value to set for the node.
  /// @return The NodeBuilder for method chaining
  NodeBuilder& value(const Variant& value);

  /// Sets the initial timestamp of the node.
  /// @throws If there is no current node available. Call NodeBuilder::make_node first.
  /// @throws If no type was set yet. Call NodeBuilder::type first.
  /// @param timestamp The initial timestamp to set for the node.
  /// @return The NodeBuilder for method chaining
  NodeBuilder& timestamp(const std::chrono::system_clock::time_point& timestamp);

  /// Sets the on subscribe callback. The callback will be fired upon subscribe or unsubscribe to the node.
  /// @param callback The callback that will be called. The callbacks parameter will be true if a subscribe has been
  /// made and false if an unsubscribe has been made to the node.
  /// @return The NodeBuilder for method chaining
  NodeBuilder& on_subscribe(std::function<void(bool subscribe)> callback);

  /// Sets the node to hidden. It will be included in list operations, but will not be shown in GUI clients.
  /// @throws If there is no current node available. Call NodeBuilder::make_node first.
  /// @return The NodeBuilder for method chaining
  NodeBuilder& hidden();

  /// Sets an editor for the value. Has to be a value node (i.e. node has to have a type) for this method to work.
  /// @throws If there is no current node available. Call NodeBuilder::make_node first.
  /// @throws If no type was set yet. Call NodeBuilder::type first.
  /// @param et The editor to set. Currently, only textarea, password, daterange, and date allowed.
  /// @return NodeBuilder& for daisy chaining
  template <typename Editor_Type>
  NodeBuilder& editor(Editor_Type&& et)
  {
    if (nodes_.empty()) {
      throw exception(error_code::no_node_defined_yet, "cannot set editor");
    }

    auto name = et.name().as_string();
    if (!(name == "textarea" || name == "password" || name == "daterange" || name == "date")) {
      throw exception(error_code::editor_type_not_allowed, "cannot set editor");
    }

    if (et.value_type() != nodes_.back().type_) {
      throw exception(error_code::invalid_editor_type, et.name().as_string());
    }

    nodes_.back().editor_ = make_editor(std::move(et));
    return *this;
  }

  /// Sets the action of the node. Nodes with an action will be shown in pop-up menus in GUI clients.
  /// @throws If there is no current node available. Call NodeBuilder::make_node first.
  /// @param action The action to set for this node.
  /// @return The NodeBuilder for method chaining
  NodeBuilder& action(Action&& action);

  /// Sets the action of the node. Nodes with an action will be shown in pop-up menus in GUI clients.
  /// @throws If there is no current node available. Call NodeBuilder::make_node first.
  /// @param action The action to set for this node.
  /// @return The NodeBuilder for method chaining
  NodeBuilder& action(const Action& action);

  /// Adds a custom config value to the node. Config keys may have a `$` prefix.
  /// @throws If there is no current node available. Call NodeBuilder::make_node first.
  /// @param key The custom config key. If the key was previously added, the value will be overwritten.
  /// @param value The custom config value to set.
  /// @return The NodeBuilder for method chaining
  NodeBuilder& config(const std::string& key, const Variant& value);

  /// Adds a custom config value to the node. Config keys may have a `$` prefix.
  /// @throws If there is no current node available. Call NodeBuilder::make_node first.
  /// @param key The custom config key. If the key was previously added, the value will be overwritten.
  /// @param value The custom config value to set.
  /// @return The NodeBuilder for method chaining
  NodeBuilder& config(const std::string& key, Variant&& value);

  /// Adds a custom attribute value to the node. Attribute keys may have a `@` prefix.
  /// @throws If there is no current node available. Call NodeBuilder::make_node first.
  /// @param key The custom attribute key. If the key was previously added, the value will be overwritten.
  /// @param value The custom attribute value to set.
  /// @return The NodeBuilder for method chaining
  NodeBuilder& attribute(const std::string& key, const Variant& value);

  /// Adds a custom attribute value to the node. Attribute keys may have a `@` prefix.
  /// @throws If there is no current node available. Call NodeBuilder::make_node first.
  /// @param key The custom attribute key. If the key was previously added, the value will be overwritten.
  /// @param value The custom attribute value to set.
  /// @return The NodeBuilder for method chaining
  NodeBuilder& attribute(const std::string& key, Variant&& value);

  /// Returns the parent path of the NodeBuilder. The new nodes will be added as child nodes to the parent node.
  /// @return The parent path of the NodeBuilder.
  const NodePath& parent_path() const
  {
    return parent_path_;
  }

  /// Returns the start iterator for the NodeBuilder node descriptions.
  /// @return Start iterator for the NodeBuilder node descriptions
  ConstIterator begin() const
  {
    return {*this};
  }

  /// Returns the end iterator for the NodeBuilder node descriptions.
  /// @return End iterator for the NodeBuilder node descriptions
  ConstIterator end() const
  {
    return {*this, true};
  }

private:
  /// @private
  void validate(const std::string& name) const;

  NodePath parent_path_;
  std::vector<NodeDescription> nodes_;
};
}
}
