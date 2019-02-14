// @copyright_start
// Copyright (c) 2018 Cisco and/or its affiliates. All rights reserved.
// @copyright_end

/// @file efm_action_column.h

#pragma once

#include <efm_editors.h>
#include <efm_types.h>
#include <efm_variant.h>

namespace cisco
{
namespace efm_sdk
{

/// @brief Defines an action result column.
class ActionColumn final
{
public:
  /// Constructs an action result column.
  /// @param name The name of the result column.
  /// @param type The cisco::efm_sdk::ValueType of the result column.
  ActionColumn(std::string name, ValueType type)
    : name_(std::move(name))
    , type_(type)
  {
  }

  /// Default copy assignment
  /// @return A reference to the ActionColumn object
  ActionColumn& operator=(const ActionColumn&) = default;
  /// Default copy constructor
  ActionColumn(const ActionColumn&) = default;
  /// Default move assignment
  /// @return A reference to the ActionColumn object
  ActionColumn& operator=(ActionColumn&&) = default;
  /// Default move constructor
  ActionColumn(ActionColumn&&) = default;

  /// Returns the name of the ActionColumn.
  /// @return The name of this ActionColumn
  const std::string& name() const
  {
    return name_;
  }

  /// Returns the cisco::efm_sdk::ValueType of the ActionColumn.
  /// @return The cisco::efm_sdk::ValueType of this ActionColumn
  ValueType type() const
  {
    return type_;
  }

  /// Returns the enum values if the cisco::efm_sdk::ValueType is an enum.
  /// @return The enum values if the cisco::efm_sdk::ValueType is an enum, an empty string otherwise.
  const std::string& enum_values() const
  {
    return enum_values_;
  }

  /// Returns the default value of the ActionColumn.
  /// @return The default value of this ActionColumn. Will return a cisco::efm_sdk::Variant::Null, if no default value
  /// was set.
  const Variant& default_value() const
  {
    return default_;
  }

  /// Sets the enum values for an enum cisco::efm_sdk::ValueType. The values have to be supplied as a comma separated
  /// value list.
  /// @param enums The enum values as a comma separated value list.
  /// @return The ActionColumn for method chaining
  ActionColumn& enum_values(std::string&& enums)
  {
    enum_values_ = std::move(enums);
    return *this;
  }

  /// Sets the default value for the ActionColumn. The default value will be used, if no value was set in the action
  /// result.
  /// @param value The value to set as default.
  /// @return The v for method chaining
  ActionColumn& default_value(Variant&& value)
  {
    default_ = std::move(value);
    return *this;
  }

  /// @param et action column editor, e.g. download
  /// @return ActionColumn& for daisy chaining
  template <typename Editor_Type>
  ActionColumn& editor(Editor_Type&& et)
  {
    static_assert(std::is_base_of<ActionColumnEditor, Editor_Type>::value, "Editor has to be an action column editor");

    if (et.value_type() != type_) {
      throw exception(error_code::invalid_editor_type, et.name().as_string());
    }

    editor_ = make_editor(std::move(et));
    return *this;
  }

  /// @return returns configured editor
  const IEditorPtr& editor() const noexcept
  {
    return editor_;
  }

private:
  std::string name_;
  ValueType type_{ValueType::None};
  std::string enum_values_;
  Variant default_;
  std::string description_;
  IEditorPtr editor_;
};

/// @private
using ActionColumns = std::vector<ActionColumn>;
}
}
