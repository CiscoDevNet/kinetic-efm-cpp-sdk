// @copyright_start
// Copyright (c) 2018 Cisco and/or its affiliates. All rights reserved.
// @copyright_end

/// @file efm_action_result.h

#pragma once

#include <efm_error_code.h>
#include <efm_exception.h>
#include <efm_types.h>
#include <efm_variant.h>

#include <memory>


namespace cisco
{
namespace efm_sdk
{

class ActionResult;
/// Unique pointer for an ActionResult
using UniqueActionResultPtr = std::unique_ptr<ActionResult>;

/// @brief Action result base class.

class ActionResult
{
public:
  /// Marker for successful action invocations
  struct SuccessType final
  {
  };
  /// Marker for erroneous action invocations
  struct ErrorType final
  {
  };

  virtual ~ActionResult() = default;

  /// Default copy assignment
  /// @return A reference to the ActionResult object
  ActionResult& operator=(const ActionResult&) = default;
  /// Default copy constructor
  ActionResult(const ActionResult&) = default;
  /// Default move assignment
  /// @return A reference to the ActionResult object
  ActionResult& operator=(ActionResult&&) = default;
  /// Default move constructor
  ActionResult(ActionResult&&) = default;

  /// Checks if this ActionResult was an error or not.
  /// @return true, if the ActionResult was an error, otherwise false
  bool is_error() const
  {
    return result_ == false || !error_text_.empty();
  }

  /// Checks if this ActionResult was an error or not.
  /// @return true, if the ActionResult was an error, otherwise false
  explicit operator bool() const
  {
    return result_;
  }

  /// Returns the actions result type.
  /// @return The actions result type.
  virtual ActionResultType get_result_type() const = 0;

  /// Sets an error text. If an error text is set, it will be send to the peer as error response. The stream will also
  /// be closed automatically. No values will be send to the peer.
  /// @param error_text The error text to send to the peer.
  void set_error_text(const std::string& error_text)
  {
    error_text_ = error_text;
  }

  /// Gets the error text.
  /// @return The error text.
  const std::string& get_error_text() const
  {
    return error_text_;
  }

protected:
  /// Constructs an ActionResult for successful action invocations
  ActionResult(const SuccessType&)
    : result_{true}
  {
  }

  /// Constructs an ActionResult for erroneous action invocations
  ActionResult(const ErrorType&)
    : result_{false}
  {
  }

private:
  bool result_{false};
  std::string error_text_;
};


/// @brief Action result type for a single row of values.

/// Should be subclassed for custom result types.
class ActionValuesResult : public ActionResult
{
public:
  /// Action result row signature
  using Row = std::vector<Variant>;

  /// Constructs an ActionValuesResult for successful action invocations
  ActionValuesResult(const SuccessType&)
    : ActionResult{ActionResult::SuccessType()}
  {
  }

  /// Constructs an ActionValuesResult for erroneous action invocations
  ActionValuesResult(const ErrorType&)
    : ActionResult{ActionResult::ErrorType()}
  {
  }

  /// Adds a value to the ActionValuesResult row. The value shall have the correct type for the current column. The
  /// types have been set by the cisco::efm_sdk::ActionColumn set in the action node.
  /// @param value The value to set for the current result column.
  /// @return The ActionValuesResult for method chaining
  ActionValuesResult& add_value(Variant&& value)
  {
    row_.emplace_back(std::move(value));
    return *this;
  }

  /// Adds a value to the ActionValuesResult row. The value shall match the correct type for the current column. The
  /// types have been configured by the cisco::efm_sdk::ActionColumn set in the action node.
  /// @param value The value to set for the current result column.
  /// @return The ActionValuesResult for method chaining
  ActionValuesResult& add_value(const Variant& value)
  {
    row_.emplace_back(value);
    return *this;
  }

  /// Returns the filled result row. The rows column count and types have to match the configured columns of the action
  /// node.
  /// @return a reference to the Row
  const Row& get_row() const
  {
    return row_;
  }

  /// @private
  Row& get_row()
  {
    return row_;
  }

  /// Will return the result type ActionResultType::Values.
  /// @return ActionResultType::Values.
  ActionResultType get_result_type() const override
  {
    return ActionResultType::Values;
  }

private:
  Row row_;
};


/// @brief Action result type for a multiple rows of values. The stream has to be closed by the peer.

/// Should be subclassed for custom result types.
class ActionStreamResult : public ActionResult
{
public:
  /// Action result row type
  using Row = std::vector<Variant>;
  /// Action result rows type
  using Rows = std::vector<Row>;

  /// Constructs an ActionResult for successful action invocations. The default mode is ActionStreamingMode::Stream.
  ActionStreamResult(const SuccessType&)
    : ActionResult{ActionResult::SuccessType()}
  {
  }


  /// Constructs an ActionResult for erroneous action invocations. The default mode is ActionStreamingMode::Stream.
  ActionStreamResult(const ErrorType&)
    : ActionResult{ActionResult::ErrorType()}
  {
  }

  /// Sets the streaming mode of this stream. For every send operation the last set mode will be used.
  /// @param mode The mode to use for this stream
  /// @return A reference to the ActionStreamResult for method chaining
  ActionStreamResult& set_mode(ActionStreamingMode mode)
  {
    mode_ = mode;
    return *this;
  }

  /// Appends a new empty row to the rows of the action result. Has to be called before calling add_value.
  /// @return A reference to the ActionStreamResult for method chaining
  ActionStreamResult& next_row()
  {
    if (rows_.empty() || !rows_.back().empty()) {
      rows_.emplace_back(Row{});
    }
    return *this;
  }

  /// Adds a value to the current ActionStreamResult row. The value shall have the correct type for the current column.
  /// The types have been set by the cisco::efm_sdk::ActionColumn set in the action node.
  /// @param value The value to set for the current result column.
  /// @return A reference to the ActionStreamResult for method chaining
  ActionStreamResult& add_value(Variant&& value)
  {
    if (rows_.empty()) {
      rows_.emplace_back(Row{});
    }

    rows_.back().emplace_back(std::move(value));
    return *this;
  }

  /// Adds a value to the current ActionStreamResult row. The value shall match the correct type for the current
  /// column. The types have been configured by the cisco::efm_sdk::ActionColumn set in the action node.
  /// @param value The value to set for the current result column.
  /// @return A reference to the ActionStreamResult for method chaining
  ActionStreamResult& add_value(const Variant& value)
  {
    if (rows_.empty()) {
      rows_.emplace_back(Row{});
    }

    rows_.back().emplace_back(value);
    return *this;
  }

  /// Returns the filled result rows. The columns count and types have to match the configured columns of the action
  /// node.
  /// @return A reference to the action result rows.
  const Rows& get_rows() const
  {
    return rows_;
  }

  /// @private
  Rows& get_rows()
  {
    return rows_;
  }

  /// Will return the result type ActionResultType::Values.
  /// @return ActionResultType::Stream.
  ActionResultType get_result_type() const override
  {
    return ActionResultType::Stream;
  }

  /// Returns the streaming mode of the result. Default is 'stream'.
  /// @return The streaming mode for this result stream.
  ActionStreamingMode get_mode() const
  {
    return mode_;
  }

  /// Clears the rows of the action result. The ActionStreamResult::rows method will return an empty container.
  virtual void clear()
  {
    rows_.clear();
  }

private:
  Rows rows_;
  ActionStreamingMode mode_{ActionStreamingMode::Stream};
};


/// @brief Action result type for a table of row values.

/// Should be subclassed for custom result types.
class ActionTableResult : public ActionStreamResult
{
public:
  /// Constructs an ActionResult for successful action invocations. The default mode is ActionStreamingMode::Stream.
  ActionTableResult(const SuccessType&)
    : ActionStreamResult{ActionResult::SuccessType()}
  {
  }

  /// Constructs an ActionResult for erroneous action invocations. The default mode is ActionStreamingMode::Stream.
  ActionTableResult(const ErrorType&)
    : ActionStreamResult{ActionResult::ErrorType()}
  {
  }

  /// Sets the insert table modifier. The new rows will be inserted after the given offset.
  /// @param offset The offset after which the new rows will be inserted.
  /// @return A reference to the ActionTableResult for method chaining
  ActionTableResult& set_insert_modifier(uint64_t offset)
  {
    modifier_.modifier_ = TableModifier::Insert;
    modifier_.start_ = offset;
    return *this;
  }

  /// Sets the replace table modifier. The new rows will replace the rows ranging from row index start to end. The end
  /// index shall not be smaller than the start index.
  /// @exception If the end index is smaller than the start index.
  /// @param start The row start index for the replacement.
  /// @param end The row end index for the replacement.
  /// @return A reference to the ActionTableResult for method chaining
  ActionTableResult& set_replace_modifier(uint64_t start, uint64_t end)
  {
    if (end < start) {
      throw exception(error_code::invalid_table_replace_modifier_index);
    }
    modifier_.modifier_ = TableModifier::Replace;
    modifier_.start_ = start;
    modifier_.end_ = end;
    return *this;
  }

  /// Will return the result type ActionResultType::Values.
  /// @return ActionResultType::Table.
  ActionResultType get_result_type() const override
  {
    return ActionResultType::Table;
  }

  /// Returns the TableModifier for the handling of the current rows.
  /// @return A reference to the TableModifier.
  const TableModifier& get_modifier() const
  {
    return modifier_;
  }

  /// Clears the rows of the action result. The ActionTableResult::rows method will return an empty container. Will
  /// also reset the TableModifier.
  void clear() override
  {
    ActionStreamResult::clear();
    modifier_.reset();
  }

private:
  TableModifier modifier_;
};

static const ActionResult::SuccessType ActionSuccess = ActionResult::SuccessType();
static const ActionResult::ErrorType ActionError = ActionResult::ErrorType();
}
}
