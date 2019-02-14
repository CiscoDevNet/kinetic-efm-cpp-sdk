// @copyright_start
// Copyright (c) 2018 Cisco and/or its affiliates. All rights reserved.
// @copyright_end

/// @file efm_action_result_stream.h

#pragma once

#include <efm_action_result.h>
#include <efm_node_path.h>

#include <system_error>


namespace cisco
{
namespace efm_sdk
{

class ActionResultStream;
/// Shared pointer for an ActionResultStream
using MutableActionResultStreamPtr = std::shared_ptr<ActionResultStream>;

/// @brief Action result stream for all action result types.

class ActionResultStream final
{
public:
  ~ActionResultStream();

  /// The close handler will be called if the stream is closed. The stream can be closed by the peer or by calling the
  /// close method. A stream with type cisco::efm_sdk::ActionResultType::Values will be called automatically upon
  /// setting the result.
  ///
  /// @param close_handler the handler to be called when the stream is closed
  void set_close_handler(
    std::function<void(const cisco::efm_sdk::NodePath& path, const std::error_code& ec)>&& close_handler);

  /// Returns the path of the action that created the stream.
  /// @return The path the action.
  const cisco::efm_sdk::NodePath& get_path() const;

  /// Sets the result of the stream. The result will determine the type of the stream. All rows of the result will be
  /// sent to the peer. If the type is of cisco::efm_sdk::ActionResultType::Values, the stream will be closed
  /// immediately after setting the result.
  ///
  /// @param action_result the result to set
  /// @return true if the result was set
  bool set_result(cisco::efm_sdk::UniqueActionResultPtr action_result);

  /// Returns the type of this action result stream.
  /// @return The type of the stream.
  cisco::efm_sdk::ActionResultType get_result_type() const;

  /// Returns the streams action stream result if the streams type cisco::efm_sdk::ActionResultType::Stream. The action
  /// result can be used to send more rows of data to the peer. The action stream result is not thread safe and should
  /// not be manipulated from multiple threads.
  /// @throw If the streams type is not cisco::efm_sdk::ActionResultType::Stream.
  /// @throw If the streams result was not set yet.
  /// @return Returns a reference to the cisco::efm_sdk::ActionStreamResult of the stream.
  cisco::efm_sdk::ActionStreamResult& get_result_stream();

  /// Returns the streams action table result if the streams type cisco::efm_sdk::ActionResultType::Table. The action
  /// result can be used to send more rows of data to the peer. The action stream result is not thread safe and should
  /// not be manipulated from multiple threads. It shall be closed by the table handler.
  /// @throw If the streams type is not cisco::efm_sdk::ActionResultType::Table.
  /// @throw If the streams result was not set yet.
  /// @return Returns a reference to the cisco::efm_sdk::ActionTableResult of the stream.
  cisco::efm_sdk::ActionTableResult& get_result_table();

  /// Closes the stream or table. Will do nothing if the type is cisco::efm_sdk::ActionResultType::Values. The stream
  /// should not be used anymore after closing it.
  void close();

  /// Will send all cached rows to the peer if the stream was not already closed. Only non empty rows will be send.
  /// Clears the cache after sending the rows.
  /// @return Returns true if the data could be send. Returns false if the stream was already closed.
  bool commit();

  /// Checks if the stream was already closed.
  /// @return Returns true if the stream was already closed, false otherwise.
  bool is_closed() const;

private:
  friend class ActionResultStreamCreator;
  ActionResultStream(const void*);

  class Impl;
  Impl* impl_;
};
}
}
