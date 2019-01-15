// @copyright_start
// Copyright (c) 2018 Cisco and/or its affiliates. All rights reserved.
// @copyright_end

/// @file efm_logging.h

#include <efm_logging_base.h>

#include <sstream>

#pragma once

/// \cond HIDDEN_SYMBOLS
#define GET_3RD_ARG(arg1, arg2, arg3, ...) arg3

#define LOG_EFM_MACRO_CHOOSER(LOG1, LOG2, ...) GET_3RD_ARG(__VA_ARGS__, LOG2, LOG1, )


#define LOG_EFM_INFO_2(error_code, stream)                                                                             \
  do {                                                                                                                 \
    if (will_log(cisco::efm_sdk::LogLevel::Info)) {                                                                    \
      std::stringstream strm;                                                                                          \
      strm << stream;                                                                                                  \
      log_message(cisco::efm_sdk::LogLevel::Info, error_code, strm);                                                   \
    }                                                                                                                  \
  } while (false)

#define LOG_EFM_INFO_1(error_code)                                                                                     \
  do {                                                                                                                 \
    if (will_log(cisco::efm_sdk::LogLevel::Info)) {                                                                    \
      log_message(cisco::efm_sdk::LogLevel::Info, error_code);                                                         \
    }                                                                                                                  \
  } while (false)
/// \endcond

/// Log macro for SDK logging information
/// 1. param: A std::error_code
/// 2. param: A stream of parameters
/// Usage: LOG_EFM_INFO(error_code <, param1 << param2 << param3>)
#define LOG_EFM_INFO(...) LOG_EFM_MACRO_CHOOSER(LOG_EFM_INFO_1, LOG_EFM_INFO_2, __VA_ARGS__)(__VA_ARGS__)


/// \cond HIDDEN_SYMBOLS
#define LOG_EFM_WARNING_2(error_code, stream)                                                                          \
  do {                                                                                                                 \
    if (will_log(cisco::efm_sdk::LogLevel::Warning)) {                                                                 \
      std::stringstream strm;                                                                                          \
      strm << stream;                                                                                                  \
      log_message(cisco::efm_sdk::LogLevel::Warning, error_code, strm);                                                \
    }                                                                                                                  \
  } while (false)

#define LOG_EFM_WARNING_1(error_code)                                                                                  \
  do {                                                                                                                 \
    if (will_log(cisco::efm_sdk::LogLevel::Warning)) {                                                                 \
      log_message(cisco::efm_sdk::LogLevel::Warning, error_code);                                                      \
    }                                                                                                                  \
  } while (false)
/// \endcond

/// Log macro to log SDK warnings
/// 1. param: A std::error_code
/// 2. param: A stream of parameters
/// Usage: LOG_EFM_WARNING(error_code <, param1 << param2 << param3>)
#define LOG_EFM_WARNING(...) LOG_EFM_MACRO_CHOOSER(LOG_EFM_WARNING_1, LOG_EFM_WARNING_2, __VA_ARGS__)(__VA_ARGS__)


/// \cond HIDDEN_SYMBOLS
#define LOG_EFM_ERROR_2(error_code, stream)                                                                            \
  do {                                                                                                                 \
    if (will_log(cisco::efm_sdk::LogLevel::Error)) {                                                                   \
      std::stringstream strm;                                                                                          \
      strm << stream;                                                                                                  \
      log_message(cisco::efm_sdk::LogLevel::Error, error_code, strm);                                                  \
    }                                                                                                                  \
  } while (false)

#define LOG_EFM_ERROR_1(error_code)                                                                                    \
  do {                                                                                                                 \
    if (will_log(cisco::efm_sdk::LogLevel::Error)) {                                                                   \
      log_message(cisco::efm_sdk::LogLevel::Error, error_code);                                                        \
    }                                                                                                                  \
  } while (false)
/// \endcond

/// Log macro to log SDK errors
/// 1. param: A std::error_code
/// 2. param: A stream of parameters
/// Usage: LOG_EFM_ERROR(error_code <, param1 << param2 << param3>)
#define LOG_EFM_ERROR(...) LOG_EFM_MACRO_CHOOSER(LOG_EFM_ERROR_1, LOG_EFM_ERROR_2, __VA_ARGS__)(__VA_ARGS__)


/// \cond HIDDEN_SYMBOLS
#define LOG_EFM_FATAL_2(error_code, stream)                                                                            \
  do {                                                                                                                 \
    if (will_log(cisco::efm_sdk::LogLevel::Fatal)) {                                                                   \
      std::stringstream strm;                                                                                          \
      strm << stream;                                                                                                  \
      log_message(cisco::efm_sdk::LogLevel::Fatal, error_code, strm);                                                  \
    }                                                                                                                  \
  } while (false)

#define LOG_EFM_FATAL_1(error_code)                                                                                    \
  do {                                                                                                                 \
    if (will_log(cisco::efm_sdk::LogLevel::Fatal)) {                                                                   \
      log_message(cisco::efm_sdk::LogLevel::Fatal, error_code);                                                        \
    }                                                                                                                  \
  } while (false)
/// \endcond

/// Log macro to log SDK fatal errors
/// 1. param: A std::error_code
/// 2. param: A stream of parameters
/// Usage: LOG_EFM_FATAL(error_code <, param1 << param2 << param3>)
#define LOG_EFM_FATAL(...) LOG_EFM_MACRO_CHOOSER(LOG_EFM_FATAL_1, LOG_EFM_FATAL_2, __VA_ARGS__)(__VA_ARGS__)


/// Log macro to log SDK debug messages.
/// @param debug_class The debug class name.
/// @param debug_level The level to log this message with.
/// @param stream A stream with parameters,
/// Usage: LOG_EFM_DEBUG(debug_class, debug_level, param1 << param2 << param3)
#define LOG_EFM_DEBUG(debug_class, debug_level, stream)                                                                \
  do {                                                                                                                 \
    if (will_debug_log(debug_level)) {                                                                                 \
      std::stringstream strm;                                                                                          \
      strm << ": " << debug_class << " - " << stream;                                                                  \
      log_debug_message(std::move(strm));                                                                              \
    }                                                                                                                  \
  } while (false)
