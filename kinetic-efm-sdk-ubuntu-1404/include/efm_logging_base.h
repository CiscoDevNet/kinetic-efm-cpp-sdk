// @copyright_start
// Copyright (c) 2018 Cisco and/or its affiliates. All rights reserved.
// @copyright_end

/// @file efm_logging_base.h

#include <efm_types.h>

#include <sstream>
#include <system_error>

#pragma once

/// @private
void log_message_internal(cisco::efm_sdk::LogLevel log_level, std::ostream&& strm);

/// @private
bool will_log(cisco::efm_sdk::LogLevel log_level);

/// @private
bool will_debug_log(cisco::efm_sdk::DebugLevel debug_level);


/// @private
template <typename T>
void log_message_strm(std::stringstream& strm, const T& t)
{
  strm << ", " << t;
}

/// @private
template <typename T, typename... Rest>
void log_message_strm(std::stringstream& strm, const T& t, Rest... parameters)
{
  strm << ", " << t;
  log_message_strm(strm, parameters...);
}

/// @private
template <typename LogLevel, typename... Parameters>
void log_message(LogLevel log_level, const std::error_code& ec, Parameters... parameters)
{
  std::stringstream strm;
  strm << "(" << ec.value() << "): " << ec.message();
  log_message_strm(strm, parameters...);
  log_message_internal(log_level, std::move(strm));
}

/// @private
template <typename LogLevel>
void log_message(LogLevel log_level, const std::error_code& ec)
{
  std::stringstream strm;
  strm << "(" << ec.value() << "): " << ec.message();
  log_message_internal(log_level, std::move(strm));
}

/// @private
template <typename LogLevel>
void log_message(LogLevel log_level, const std::error_code& ec, std::stringstream& stream)
{
  std::stringstream strm;
  strm << "(" << ec.value() << "): " << ec.message() << " - ";
  strm << stream.str();
  log_message_internal(log_level, std::move(strm));
}

/// @private
void log_debug_message(std::stringstream&& stream);
