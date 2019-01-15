// @copyright_start
// Copyright (c) 2018 Cisco and/or its affiliates. All rights reserved.
// @copyright_end

/// @file efm_link_options.h

#pragma once

#include <efm_types.h>

#include <memory>
#include <string>


namespace cisco
{
namespace efm_sdk
{

/// @brief Base class of the link options JSON config loader implementation.
class ConfigLoader
{
public:
  virtual ~ConfigLoader() = default;

  /// Loads the configuration via the do_load_config template method. The configuration returned has to be in JSON
  /// format. If the returned configuration is empty, an empty JSON object will be returned.
  /// @return The loaded configuration as a JSON string.
  std::string load_config();

private:
  /// Does the actual loading of the JSON configuration.
  /// @return The loaded configuration as a JSON string.
  virtual std::string do_load_config() = 0;
};

/// @brief Loads a JSON configuration file.
class FileConfigLoader : public ConfigLoader
{
public:
  /// Constructs the config loader
  FileConfigLoader() = default;

private:
  std::string do_load_config() override;
};


/// @brief Handles all the links options.

/// Options can be passed via the command line or a dslink.json config file. If need be, the loader can be subclassed to
/// enable other configuration sources.
class LinkOptions final
{
public:
  /// Constructs a LinkOptions instance.
  /// @param link_name The name of the link to handle options for.
  /// @param loader The loader implementation to load configurations with.
  LinkOptions(const std::string& link_name, ConfigLoader& loader);

  /// Move constructor.
  /// @param other The LinkOptions instance to move the option from.
  LinkOptions(LinkOptions&& other) noexcept;

  /// Destroys the link instance.
  ~LinkOptions();

  /// Parses command line options for the link. Supported options can be found via the --help (-h) option.
  /// If non supported options are found, a corresponding error is issued on the given ostream and false will be
  /// returned.
  /// @param argc The number of arguments in the argv array.
  /// @param argv The arguments for this link.
  /// @param stream The ostream to output error messages to.
  /// @return true if the command line options could be parsed successfully, otherwise false.
  bool parse(int argc, const char* const argv[], std::ostream& stream);

  /// Returns the configured broker url. If not set via command line nor options file, `http://127.0.0.1:8080/conn` will
  /// be used.
  /// @return The configured broker url
  const std::string& broker_url() const;

  /// Returns the configured link name. If not set via command line nor options file, the name given upon link options
  /// construction will be used.
  /// @return The configured link name
  const std::string& link_name() const;

  /// Returns the configured log level. If not set via command line nor options file, a log level of
  /// cisco::efm_sdk::LogLevel::Info will be used.
  /// @return The configured log level
  LogLevel log_level() const;

  /// Returns the configured permission token. If not set via command line nor options file, the token will be left
  /// unset.
  /// @return The configured token or an empty string if no token was set
  const std::string& token() const;

  /// Returns the configured key file name. If not set via options file, the key file name will be set to `.key`.
  /// @return The configured key file name
  const std::string& key_file_name() const;

  /// Returns the number of worker threads. Default is the number returned by std::thread::hardware_concurrency. If this
  /// is not defined, the default is 1.
  /// @return The number of worker threads
  size_t num_workers() const;

  /// Returns the SSL options.
  /// @return SSL options.
  const SSLOptions& ssl_options() const;

  /// Returns the settings for the redo log. The defaults are defined in the structure itself.
  /// @return The redo log settings.
  const RedoLogSettings& redo_log_settings() const;

  /// Returns the Quality of Service settings. The defaults are defined in the structure itself.
  /// @return The Quality of Service settings.
  const QoSSettings& qos_settings() const;

  /// Returns the maximum length of the internal send queue. If this number of send messages has not been
  /// acknowledged yet, the sending will be paused until at least some of these messages have been acknowledged. The
  /// default is 8.
  /// @return The maximum length of the internal send queue before pausing the sending of messages.
  uint32_t max_send_queue_length() const;

  /// Returns the Serializer settings. The defaults are defined in the structure itself.
  /// @return The Serializer settings.
  const SerializerSettings& serializer_settings() const;

private:
  class Impl;
  std::unique_ptr<Impl> impl_;
};
}
}
