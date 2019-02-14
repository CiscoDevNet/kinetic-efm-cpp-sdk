// @copyright_start
// Copyright (c) 2018 Cisco and/or its affiliates. All rights reserved.
// @copyright_end

/// @file efm_action_parameter.h

#pragma once

#include <efm_editors.h>
#include <efm_error_code.h>
#include <efm_exception.h>
#include <efm_types.h>
#include <efm_variant.h>

namespace cisco
{
namespace efm_sdk
{

/// @brief Defines an action input parameter.
class ActionParameter final
{
public:
  /// Constructs an action input parameter. These parameters will be displayed as input parameters in an action dialog.
  /// @param name The name of the input parameter. It will be displayed as label for the parameter in the action dialog.
  /// @param type cisco::efm_sdk::ValueType for the parameter.
  ActionParameter(std::string name, ValueType type)
    : name_(std::move(name))
    , type_(type)
  {
  }

  /// Default copy assignment
  /// @return A reference to the ActionParameter object
  ActionParameter& operator=(const ActionParameter&) = default;
  /// Default copy constructor
  ActionParameter(const ActionParameter&) = default;
  /// Default move assignment
  /// @return A reference to the ActionParameter object
  ActionParameter& operator=(ActionParameter&&) = default;
  /// Default move constructor
  ActionParameter(ActionParameter&&) = default;

  /// Returns the name of the ActionParameter.
  /// @return The name of the ActionParameter.
  const std::string& name() const
  {
    return name_;
  }

  /// Returns the cisco::efm_sdk::ValueType of the ActionParameter.
  /// @return The cisco::efm_sdk::ValueType of the ActionParameter.
  ValueType type() const
  {
    return type_;
  }

  /// Returns the default value of the ActionParameter.
  /// @return The default value of this ActionParameter. Will return a cisco::efm_sdk::Variant::Null, if no default
  /// value was set.
  const Variant& default_value() const
  {
    return default_;
  }

  /// Returns the enum values if the cisco::efm_sdk::ValueType is an enum.
  /// @return The enum values if the cisco::efm_sdk::ValueType is an enum, an empty string otherwise.
  const std::string& enum_values() const
  {
    return enum_values_;
  }

  /// Returns the placeholder for the ActionParameter.
  /// @return The placeholder for the ActionParameter or an empty string if no placeholder was set.
  const std::string& placeholder() const
  {
    return placeholder_;
  }

  /// Returns the description for the ActionParameter.
  /// @return The description for the ActionParameter or an empty string if no description was set.
  const std::string& description() const
  {
    return description_;
  }

  /// Sets the default value for the ActionParameter. The default value will be used, if no value was set in the action
  /// invocation.
  /// @param value The value to set as default.
  /// @return The ActionParameter for method chaining
  ActionParameter& default_value(Variant&& value)
  {
    default_ = std::move(value);
    return *this;
  }

  /// Sets the enum values for an enum cisco::efm_sdk::ValueType. The values have to be supplied as a comma separated
  /// value list.
  /// @param enums The enum values as a comma separated value list.
  /// @return The ActionParameter for method chaining
  ActionParameter& enum_values(std::string&& enums)
  {
    enum_values_ = std::move(enums);
    return *this;
  }

  /// Sets the placeholder for the ActionParameter. The placeholder can only be set for
  /// cisco::efm_sdk::ValueType::String types. It will be displayed in the edit field of the action dialog input
  /// parameter before the field is edited.
  /// @param placeholder The placeholder to set.
  /// @return The ActionParameter for method chaining
  ActionParameter& placeholder(std::string&& placeholder)
  {
    placeholder_ = std::move(placeholder);
    return *this;
  }

  /// Sets the description for ActionParameter. The description will be shown as a tooltip for the input parameter.
  /// @param description The description to set.
  /// @return The ActionParameter for method chaining
  ActionParameter& description(std::string&& description)
  {
    description_ = std::move(description);
    return *this;
  }

  /// @param et Editor, e.g. Min, Max, Color, ...
  /// @return ActionParameter& for daisy chaining
  template <typename Editor_Type>
  ActionParameter& editor(Editor_Type&& et)
  {
    static_assert(
      std::is_base_of<ActionParameterEditor, Editor_Type>::value, "Editor have to be an action parameter editor");

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
  Variant default_;
  std::string enum_values_;
  std::string placeholder_;
  std::string description_;
  IEditorPtr editor_;
};

/// @private
using ActionParameters = std::vector<ActionParameter>;
}
}
