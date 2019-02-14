// @copyright_start
// Copyright (c) 2018 Cisco and/or its affiliates. All rights reserved.
// @copyright_end

/// @file efm_action.h

#pragma once

#include <efm_action_column.h>
#include <efm_action_parameter.h>
#include <efm_action_result_stream.h>
#include <efm_error_code.h>
#include <efm_node_path.h>

#include <functional>
#include <system_error>


namespace cisco
{
namespace efm_sdk
{

/// @brief Defines the action of a node.
class Action final
{
public:
  /// Action callback signature
  using action_callback = std::function<void(
    const MutableActionResultStreamPtr& stream,
    const NodePath& parent_path,
    const Variant& params,
    const std::error_code& ec)>;

  /// Constructs an Action. The default action result type is ActionResultType::Values.
  /// @param permission The permission level of the Action.
  /// @param callback The callback to call upon action invocation.
  Action(PermissionLevel permission, action_callback&& callback)
    : permission_(permission)
    , callback_(std::move(callback))
  {
    if (permission_ < PermissionLevel::Read) {
      throw exception(error_code::invalid_permission_specified);
    }
  }

  /// Default copy assignment
  /// @return A reference to the Action object
  Action& operator=(const Action&) = default;
  /// Default copy constructor
  Action(const Action&) = default;
  /// Default move assignment
  /// @return A reference to the Action object
  Action& operator=(Action&&) = default;
  /// Default move constructor
  Action(Action&&) = default;

  /// Returns the Action's callback
  /// @return The action callback
  action_callback get_callback() const
  {
    return callback_;
  }

  /// Adds the Action to the action group with the given subtitle.
  /// @param group The group to add this action to. Actions with the same group name will be displayed together in a
  /// popup menu with each actions subtitle as menu.
  /// @param group_subtitle The group subtitle to set for this action
  /// @return The Action for method chaining
  Action& group(std::string&& group, std::string&& group_subtitle)
  {
    group_ = std::move(group);
    group_subtitle_ = std::move(group_subtitle);
    return *this;
  }

  /// Adds an input parameter to the Action. The parameter will be displayed in the action dialog and has to be filled
  /// in before the action invocation.
  /// @param param The input parameter to add.
  /// @return The Action for method chaining
  Action& add_param(ActionParameter&& param)
  {
    params_.emplace_back(std::move(param));
    return *this;
  }

  /// Adds an input parameter to the Action. The parameter will be displayed in the action dialog and has to be filled
  /// in before the action invocation.
  /// @param param The input parameter to add.
  /// @return The Action for method chaining
  Action& add_param(ActionParameter& param)
  {
    params_.emplace_back(param);
    return *this;
  }

  /// Adds a collection of input parameters to the Action.
  /// @param params The parameter collection to add.
  /// @return The Action for method chaining
  Action& params(ActionParameters&& params)
  {
    params_ = std::move(params);
    return *this;
  }

  /// Adds a collection of input parameters to the Action.
  /// @param params The parameter collection to add.
  /// @return The Action for method chaining
  Action& params(const ActionParameters& params)
  {
    params_ = params;
    return *this;
  }

  /// Adds a result column to the Action. The result columns will be displayed in the action dialog and will be filled
  /// with the actions result.
  /// @param column The result column to add.
  /// @return The Action for method chaining
  Action& add_column(ActionColumn&& column)
  {
    columns_.emplace_back(std::move(column));
    return *this;
  }

  /// Adds a result column to the action. The result columns will be displayed in the action dialog and will be filled
  /// with the actions result.
  /// @param column The result column to add.
  /// @return The action for method chaining
  Action& add_column(const ActionColumn& column)
  {
    columns_.emplace_back(column);
    return *this;
  }

  /// Adds a collection of result columns to the Action.
  /// @param columns The column collection to add.
  /// @return The Action for method chaining
  Action& columns(ActionColumns&& columns)
  {
    columns_ = std::move(columns);
    return *this;
  }

  /// Adds a collection of result columns to the Action.
  /// @param columns The column collection to add.
  /// @return The Action for method chaining
  Action& columns(const ActionColumns& columns)
  {
    columns_ = columns;
    return *this;
  }

  /// Sets the action result type to ActionResultType::Stream.
  /// @return The Action for method chaining
  Action& set_stream()
  {
    result_type_ = ActionResultType::Stream;
    return *this;
  }

  /// Sets the action result type to ActionResultType::Table.
  /// @return The Action for method chaining
  Action& set_table()
  {
    result_type_ = ActionResultType::Table;
    return *this;
  }

  /// Returns the permission level of the Action.
  /// @return The permission level of the Action
  PermissionLevel permission() const
  {
    return permission_;
  }

  /// Returns the group the Action belongs to.
  /// @return The group the Action belongs to or an empty string if no group was set.
  const std::string& group() const
  {
    return group_;
  }

  /// Returns the group subtitle of the Action belongs to.
  /// @return The group subtitle of the Action or an empty string if no group subtitle was set.
  const std::string& group_subtitle() const
  {
    return group_subtitle_;
  }

  /// Returns the input parameter of the Action.
  /// @return The input parameters or an empty collection if no input parameter were set.
  const ActionParameters& parameters() const
  {
    return params_;
  }

  /// Returns the result columns of the Action.
  /// @return The result columns or an empty collection if no result columns were set.
  const ActionColumns& columns() const
  {
    return columns_;
  }

  /// Returns the action results type.
  /// @return The action results type.
  ActionResultType result_type() const
  {
    return result_type_;
  }

private:
  PermissionLevel permission_{PermissionLevel::Read};
  ActionParameters params_;
  ActionColumns columns_;
  std::string group_;
  std::string group_subtitle_;
  ActionResultType result_type_{ActionResultType::Values};
  action_callback callback_;
};
}
}
