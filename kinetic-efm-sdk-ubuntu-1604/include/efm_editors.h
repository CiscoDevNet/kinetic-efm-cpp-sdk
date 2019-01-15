// @copyright_start
// Copyright (c) 2018 Cisco and/or its affiliates. All rights reserved.
// @copyright_end

/// @file efm_editors.h

#pragma once

#include <efm_types.h>
#include <efm_variant.h>


namespace cisco
{
namespace efm_sdk
{
/// Base class for all editors that can be used in an ActionParameter
class ActionParameterEditor
{
};


/// Base class for all editors that can be used in an ActionColumn
class ActionColumnEditor
{
};


namespace editor
{
/// Specifies different editor types for a number editor.
/// Number can be either a single or double. When a number type is used in the action parameter, it supports additional
/// properties.

/// Number editor.
class Number : public ActionParameterEditor
{
public:
  /// Constructs a simple number editor.
  Number() = default;

  /// @return ValueType of the editor.
  ValueType value_type() const noexcept;

  /// Serializes the editor to specified parameter.
  /// @param params The params map to fill.
  /// @return The filled params object.
  Variant& serialize(Variant& params) const;

  /// @returns the name of this editor.
  Variant name() const noexcept;
};


/// Number editor Opens a color editor.
class Color : public ActionParameterEditor
{
public:
  /// Constructor.
  /// @param color Color to set.
  Color(Variant&& color) noexcept
    : color_(std::move(color))
  {
  }

  /// @return ValueType of the editor.
  ValueType value_type() const noexcept;

  /// Serializes the editor to specified parameter.
  /// @param params The params map to fill.
  /// @return The filled params object.
  Variant& serialize(Variant& params) const;

  /// @returns the name of this editor
  Variant name() const noexcept;

private:
  Variant color_;
};


/// Number editor with Min property.
/// Requires a minimum bound.
class Min : public ActionParameterEditor
{
public:
  /// Constructor.
  /// @param min The minimum bound to set.
  Min(Variant&& min)
    : min_(std::move(min))
  {
  }

  /// @return ValueType of the editor.
  ValueType value_type() const noexcept;

  /// Serializes the editor to specified parameter.
  /// @param params The params map to fill.
  /// @return The filled params object.
  Variant& serialize(Variant& params) const;

  /// @returns The name of this editor
  Variant name() const noexcept;

private:
  Variant min_;
};


/// Number editor with "Max" property.
/// Requires a maximum bound.
class Max : public ActionParameterEditor
{
public:
  /// Constructs a max editor with the specified maximum value.
  /// @param max The maximum bound to set.
  Max(Variant&& max)
    : max_(std::move(max))
  {
  }

  /// @return ValueType of the editor.
  ValueType value_type() const noexcept;

  /// Serializes the editor to specified parameter.
  /// @param params The params map to fill.
  /// @return The filled params object.
  Variant& serialize(Variant& params) const;

  /// @returns The name of this editor.
  Variant name() const noexcept;

private:
  Variant max_;
};


/// Number editor with "Min and Max" property.
/// Requires a minimum and maximum bound.
/// Note: Does not work in color editor.
class MinMax : public ActionParameterEditor
{
public:
  /// Constructs a min/max editor with the specified minimum and maximum value.
  /// @param min The minimum bound to set.
  /// @param max The maximum bound to set.
  MinMax(Variant&& min, Variant&& max)
    : min_(std::move(min))
    , max_(std::move(max))
  {
  }

  /// @return ValueType of the editor.
  ValueType value_type() const noexcept;

  /// Serializes the editor to specified parameter.
  /// @param params The params map to fill.
  /// @return The filled params object.
  Variant& serialize(Variant& params) const;

  /// @returns The name of this editor.
  Variant name() const noexcept;

private:
  Variant min_;
  Variant max_;
};


/// Specifies different editor types for a string editor.

/// String editor with password properties.
/// Hides the entered String.
class Password : public ActionParameterEditor
{
public:
  /// Constructor.
  Password() = default;

  /// @return ValueType of the editor.
  ValueType value_type() const noexcept;

  /// Serializes the editor to specified parameter.
  /// @param params The params map to fill.
  /// @return The filled params object.
  Variant& serialize(Variant& params) const;

  /// @returns The name of this editor.
  Variant name() const noexcept;
};


/// String editor with date range properties.
/// Opens a date range editor.
class DateRange : public ActionParameterEditor
{
public:
  /// Constructor.
  DateRange() = default;

  /// @return ValueType of the editor.
  ValueType value_type() const noexcept;

  /// Serializes the editor to specified parameter.
  /// @param params The params map to fill.
  /// @return The filled params object.
  Variant& serialize(Variant& params) const;

  /// @returns The name of this editor.
  Variant name() const noexcept;
};


/// String editor with date property.
/// Opens a date editor.
class Date : public ActionParameterEditor
{
public:
  /// Constructor.
  Date() = default;

  /// @return ValueType of the editor.
  ValueType value_type() const noexcept;

  /// Serializes the editor to specified parameter.
  /// @param params The params map to fill.
  /// @return The filled params object.
  Variant& serialize(Variant& params) const;

  /// @returns The name of this editor.
  Variant name() const noexcept;
};


/// String editor with date text area properties.
/// Allows for multi-line input.
class TextArea : public ActionParameterEditor, public ActionColumnEditor
{
public:
  /// Constructor.
  TextArea() = default;

  /// @return ValueType of the editor.
  ValueType value_type() const noexcept;

  /// Serializes the editor to specified parameter.
  /// @param params The params map to fill.
  /// @return The filled params object.
  Variant& serialize(Variant& params) const;

  /// @returns The name of this editor.
  Variant name() const noexcept;
};


/// Select from a predefined set of values
/// Opens an drop down selection dialog
class Enum : ActionParameterEditor
{
public:
  /// Constructs an enum editor with the specified amount of enums.
  /// @param enums A comma separated list of enum values.
  Enum(std::string&& enums)
    : enums_(std::move(enums))
  {
  }

  /// @return ValueType of the editor.
  ValueType value_type() const noexcept;

  /// Serializes the editor to specified parameter.
  /// @param params The params map to fill.
  /// @return The filled params object.
  Variant& serialize(Variant& params) const;

  /// @returns The name of this editor.
  Variant name() const noexcept;

private:
  Variant enums_;
};


/// Choose a file.
/// Opens a file select dialog.
class FileInput : public ActionParameterEditor
{
public:
  /// Constructor.
  FileInput() = default;

  /// Constructs a file input editor for the specified mime type.
  /// @param mime_type The mime type to set.
  FileInput(Variant&& mime_type)
    : mime_type_(std::move(mime_type))
  {
  }

  /// @return ValueType of the editor.
  ValueType value_type() const noexcept;

  /// Serializes the editor to specified parameter.
  /// @param params The params map to fill.
  /// @return The filled params object.
  Variant& serialize(Variant& params) const;

  /// @returns The name of this editor.
  Variant name() const noexcept;

private:
  Variant mime_type_;
};


/// Downloads a file.
/// Opens a file select dialog.
class Download : public ActionColumnEditor
{
public:
  /// Constructor.
  /// @param filename The filename to use for the download.
  Download(Variant&& filename)
    : filename_(std::move(filename))
  {
  }

  /// @return ValueType of the editor.
  ValueType value_type() const noexcept;

  /// Serializes the editor to specified parameter.
  /// @param params The params map to fill.
  /// @return The filled params object.
  Variant& serialize(Variant& params) const;

  /// @returns The name of this editor.
  Variant name() const noexcept;

private:
  Variant filename_;
};
}

/// Helper class to encapsulate an editor for usage in conjunction with an ActionParameter.
class IEditor
{
public:
  virtual ~IEditor() = default;

  /// @return ValueType of the editor.
  virtual ValueType value_type() const noexcept = 0;

  /// Serializes the editor to specified parameter.
  /// @param params The params map to fill.
  /// @return The filled params object.
  virtual Variant& serialize(Variant& params) const = 0;

  /// @returns The name of this editor.
  virtual Variant name() const noexcept = 0;

protected:
  IEditor() = default;
};


/// This helper contains the concrete editor.
/// Hence the helper can have different types.
template <typename T>
class ConcreteEditor : public IEditor
{
public:
  /// Constructs an editor with the specified type.
  /// @param etype The editor type to wrap.
  explicit constexpr ConcreteEditor(T&& etype)
    : type_(std::move(etype))
  {
  }

  /// @return ValueType of the underlying editor.
  ValueType value_type() const noexcept override
  {
    return type_.value_type();
  }

  /// Serializes the underlying editor to specified parameter.
  /// @param params The params map to fill.
  /// @return The filled params object.
  Variant& serialize(Variant& params) const override
  {
    return type_.serialize(params);
  }

  /// @returns The name of the underlying editor.
  Variant name() const noexcept override
  {
    return type_.name();
  }

private:
  T type_;
};

/// Convenience definition for the editor interface.
using IEditorPtr = std::shared_ptr<IEditor>;

/// Creates a concrete editor.
/// @param t The editor to create.
/// @return A smart pointer to the created editor.
template <typename T>
IEditorPtr make_editor(T&& t)
{
  return std::make_shared<ConcreteEditor<T>>(ConcreteEditor<T>(std::move(t)));
}
}
}
