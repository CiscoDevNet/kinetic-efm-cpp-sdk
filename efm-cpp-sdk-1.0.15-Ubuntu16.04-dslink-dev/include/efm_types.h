// @copyright_start
// Copyright (c) 2018 Cisco and/or its affiliates. All rights reserved.
// @copyright_end

/// @file efm_types.h

#pragma once

#include <efm_optional.h>
#include <efm_variant.h>

#include <memory>
#include <string>

namespace cisco
{
namespace efm_sdk
{

/// Defines the type of link.
enum class LinkType
{
  None = 0x00,              ///< The link has no type.
  Requester = 0x01,         ///< The link is a requester.
  Responder = 0x02,         ///< The link is a responder.
  RequesterResponder = 0x03 ///< The link is a requester and responder.
};

/// Combines two LinkType enums using an OR operation.
/// @param lhs The first LinkType to combine.
/// @param rhs The second LinkType to combine.
/// @return The OR combination of the two given LinkType's. If both have the same type, that type will be returned. If
/// both have different type, RequesterResponder will be returned.
LinkType operator|(LinkType lhs, LinkType rhs);

/// Combines two LinkType enums using an AND operation. Can be used to check if a PermissionLevel corresponds to
/// a specific enum value.
/// @param lhs The first LinkType to combine.
/// @param rhs The second LinkType to combine.
/// @return The AND combination of the two given LinkType's.
LinkType operator&(LinkType lhs, LinkType rhs);

/// Produces the inverse LinkType of the given one.
/// @param rhs The LinkType to inverse.
/// @return
/// | Return Type                                | rhs                                      |
/// | :----------------------------------------: | :--------------------------------------: |
/// | cisco::efm_sdk::LinkType::None                 | cisco::efm_sdk::LinkType::None               |
/// | cisco::efm_sdk::LinkType::Responder            | cisco::efm_sdk::LinkType::Requester          |
/// | cisco::efm_sdk::LinkType::Requester            | cisco::efm_sdk::LinkType::Responder          |
/// | cisco::efm_sdk::LinkType::RequesterResponder   | cisco::efm_sdk::LinkType::RequesterResponder |
LinkType inverse(LinkType rhs);

/// Stream insertion operator for LinkType
///
/// @tparam CharT Character type for the ostream
/// @tparam Traits Traits to be used by the ostream
/// @param os The stream to insert the object into
/// @param link_type The LinkType object to insert
/// @return The std::basic_ostream object
template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, const LinkType& link_type)
{
  switch (link_type) {
    case LinkType::None:
      os << "None";
      break;
    case LinkType::Requester:
      os << "Requester";
      break;
    case LinkType::Responder:
      os << "Responder";
      break;
    case LinkType::RequesterResponder:
      os << "Requester/Responder";
      break;
  }
  return os;
}


/// Specifies the level for the logging facility.
enum class LogLevel
{
  None,    ///< No logging will take place.
  Fatal,   ///< Only fatal error messages will be logged.
  Error,   ///< Fatal, and error messages will be logged.
  Warning, ///< Fatal, error, and warning messages will be logged.
  Info,    ///< Fatal, error, warning, and info messages will be logged.
  Debug    ///< Everything will be logged.
};

/// Stream insertion operator for DebugLevel
///
/// @tparam CharT Character type for the ostream
/// @tparam Traits Traits to be used by the ostream
/// @param os The stream to insert the object into
/// @param logging_level The LogLevel object to insert
/// @return The std::basic_ostream object
template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, LogLevel logging_level)
{
  switch (logging_level) {
    case LogLevel::None: {
      os << "NONE";
      break;
    }
    case LogLevel::Info: {
      os << "INFO";
      break;
    }
    case LogLevel::Warning: {
      os << "WARNING";
      break;
    }
    case LogLevel::Error: {
      os << "ERROR";
      break;
    }
    case LogLevel::Fatal: {
      os << "FATAL";
      break;
    }
    case LogLevel::Debug: {
      os << "DEBUG";
      break;
    }
  }
  return os;
}

/// Specifies the debug logging level.
enum class DebugLevel
{
  no = 0, ///< No debug information shall be logged
  l1,     ///< Only debug messages with l1 shall be logged.
  l2,     ///< Debug messages with l2 and lower shall be logged.
  l3,     ///< Debug messages with l3 and lower shall be logged.
  l4,     ///< Debug messages with l4 and lower shall be logged.
  l5      ///< Debug messages with l5 and lower shall be logged.
};

/// Stream insertion operator for DebugLevel
///
/// @tparam CharT Character type for the ostream
/// @tparam Traits Traits to be used by the ostream
/// @param os The stream to insert the object into
/// @param debug_level The DebugLevel object to insert
/// @return The std::basic_ostream object
template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, DebugLevel debug_level)
{
  switch (debug_level) {
    case DebugLevel::no: {
      os << "DEBUGGING IS OFF";
      break;
    }
    case DebugLevel::l1: {
      os << "DEBUG(1)";
      break;
    }
    case DebugLevel::l2: {
      os << "DEBUG(2)";
      break;
    }
    case DebugLevel::l3: {
      os << "DEBUG(3)";
      break;
    }
    case DebugLevel::l4: {
      os << "DEBUG(4)";
      break;
    }
    case DebugLevel::l5: {
      os << "DEBUG(5)";
      break;
    }
  }
  return os;
}


/// Specifies the quality of service for value subscriptions.
enum class QoS
{
  None,      ///< No quality of service. Multiple messages may be rolled-up into a single message.
  Volatile,  ///< Messages will be added to an in memory ring buffer. All messages will be thrown away upon disconnects.
             ///< May loose messages, as the in memory buffer is of finite size.
  Durable,   ///< Messages will be added to an in memory ring buffer. Messages will be kept upon disconnects, but
             ///< will be lost upon shutdown. May loose messages, as the in memory buffer is of finite size.
  Persistent ///< Messages will be added to a disk persistent ring buffer. Messages will be kept upon disconnects and
             ///< will not be lost upon shutdown. May loose messages, as the buffer and disk is of finite size.
};

/// Stream insertion operator for QoS
///
/// @tparam CharT Character type for the ostream
/// @tparam Traits Traits to be used by the ostream
/// @param os The stream to insert the object into
/// @param qos The QoS object to insert
/// @return The std::basic_ostream object
template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, const QoS& qos)
{
  switch (qos) {
    case QoS::None:
      os << "None";
      break;
    case QoS::Volatile:
      os << "Volatile";
      break;
    case QoS::Durable:
      os << "Durable";
      break;
    case QoS::Persistent:
      os << "Persistent";
      break;
  }
  return os;
}


/// Specifies the user permission to use or enforce for various operations.
enum class PermissionLevel
{
  None,   ///< A requester with this permission is not allowed to do anything.
  List,   ///< At least the list permission is used/needed.
  Read,   ///< At read the list permission is used/needed.
  Write,  ///< At write the list permission is used/needed.
  Config, ///< At config the list permission is used/needed.
  Never   ///< Internally used for system configurations.
};

/// Combines two PermissionLevel enums and returns always the higher of both levels.
/// @param lhs The first PermissionLevel to combine.
/// @param rhs The second PermissionLevel to combine.
/// @return The higher of both levels or lhs if both have the same level.
PermissionLevel operator|(PermissionLevel lhs, PermissionLevel rhs);

/// Stream insertion operator for PermissionLevel
///
/// @tparam CharT Character type for the ostream
/// @tparam Traits Traits to be used by the ostream
/// @param os The stream to insert the object into
/// @param level The PermissionLevel object to insert
/// @return The std::basic_ostream object
template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, PermissionLevel level)
{
  switch (level) {
    case PermissionLevel::None:
      os << "None";
      break;
    case PermissionLevel::List:
      os << "List";
      break;
    case PermissionLevel::Read:
      os << "Read";
      break;
    case PermissionLevel::Write:
      os << "Write";
      break;
    case PermissionLevel::Config:
      os << "Config";
      break;
    case PermissionLevel::Never:
      os << "Never";
      break;
  }
  return os;
}


/// Specifies if the EFM set method of a node can be called.
enum class Writable
{
  Never, ///< Set can never be called.
  Write, ///< Set can be called with the "write" permission level.
  Config ///< Set can be called with the "config" permission level.
};

/// Stream insertion operator for Writable
///
/// @tparam CharT Character type for the ostream
/// @tparam Traits Traits to be used by the ostream
/// @param os The stream to insert the object into
/// @param writable The Writable object to insert
/// @return The std::basic_ostream object
template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, Writable writable)
{
  switch (writable) {
    case Writable::Never:
      os << "Never";
      break;
    case Writable::Write:
      os << "Write";
      break;
    case Writable::Config:
      os << "Config";
      break;
  }
  return os;
}


/// Specifies the type for a value
enum class ValueType
{
  None,   ///< The absence of a type.
  Number, ///< The value has number type and can actually be an int, uint, float or double.
  Int,    ///< The value has int type.
  UInt,   ///< The value has uint type.
  String, ///< The value has string type.
  Bool,   ///< The value has boolean type.
  Map,    ///< The value has map type.
  Array,  ///< The value has array type.
  Time,   ///< The value has time type.
  Enum,   ///< The value has enum type.
  Binary, ///< The value has binary type.
  Dynamic ///< The value has dynamic type.
};


/// Specifies the result type of an action.
enum class ActionResultType
{
  None,   ///< The result type was not set yet.
  Values, ///< The result is composed of one row of values.
  Table,  ///< The result is composed of a table of rows of values.
  Stream  ///< The result is composed of rows of values. Rows will be send until the peer closes the invoke action.
};


/// Specifies the streaming mode of an action
enum class ActionStreamingMode
{
  Refresh, ///< The requester will clear its cache and add the new updates.
  Append,  ///< The new updates are appended to the end of the requesters cache.
  Stream ///< Works as append, but the requester can remove data from the beginning of the cached data and thus has fine
         ///< grained control over the cache size.
};

/// @brief The TableModifier specifies for the requester how to handle the new rows.

/// The modifier can specify to insert the rows at a specific offset or to replace a range of rows with the new rows.
/// Not setting the modifier means to just follow the ActionStreamingMode.
struct TableModifier final
{
  /// The modifier specifies the handling for the requester.
  enum Modifier
  {
    None,   ///< Just follow the ActionStreamingMode.
    Insert, ///< Insert the new rows.
    Replace ///< Replace a range with the new rows.
  };

  /// Resets the modifier to its default values.
  void reset()
  {
    modifier_ = None;
    start_ = 0;
    end_ = 0;
  }

  /// The modifier
  Modifier modifier_{None};
  /// The start index of the modifier
  uint64_t start_{0};
  /// The end index of the modifier (only used by Replace)
  uint64_t end_{0};
};

/// @brief The ActionStreamMetaData specifies the meta data for an action stream.

/// The mode specifies the overall behavior of the stream.
/// The modifier can specify to insert the rows at a specific offset or to replace a range of rows with the new rows.
struct ActionStreamMetaData final
{
  /// The mode
  Optional<ActionStreamingMode> mode_;
  /// The modifier
  TableModifier modifier_;
};

/// Specifies in which context a node was created.
enum class NodeCreationContext
{
  Normal,         ///< The node was created during the normal operation of the link.
  Deserialization ///< The node was created while deserializing the nodes.json file.
};


/// Callback type for event callbacks.
enum class CallbackOn
{
  Subscribe,       ///< For (de)registering of a callback when a subscription is established.
  Unsubscribe,     ///< For (de)registering of a callback when a subscription is removed.
  ListOpen,        ///< For (de)registering of a callback when a list is established.
  MetadataChanged, ///< For (de)registering of a callback when the configuration of a node was changed.
  ValueChanged     ///< For (de)registering of a callback when the value of a node was changed.
};

/// Stream insertion operator for CallbackOn
///
/// @tparam CharT Character type for the ostream
/// @tparam Traits Traits to be used by the ostream
/// @param os The stream to insert the object into
/// @param callback_on The request event a user are interested in
/// @return The std::basic_ostream object
template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, CallbackOn callback_on)
{
  switch (callback_on) {
    case CallbackOn::Subscribe:
      os << "Subscription";
      break;
    case CallbackOn::Unsubscribe:
      os << "Unsubscribe";
      break;
    case CallbackOn::ListOpen:
      os << "ListOpen";
      break;
    case CallbackOn::MetadataChanged:
      os << "MetadataChanged";
      break;
    case CallbackOn::ValueChanged:
      os << "ValueChanged";
      break;
  }
  return os;
}


/// Specifies the serialization mode for nodes.
enum class SerializationMode
{
  None,         ///< No serialization will be performed.
  MetadataOnly, ///< Only the metadata of a node will be serialized.
  Everything    ///< The metadata and the value will be serialized.
};

/// Stream insertion operator for SerializationMode
///
/// @tparam CharT Character type for the ostream
/// @tparam Traits Traits to be used by the ostream
/// @param os The stream to insert the object into
/// @param mode The serialization mode.
/// @return The std::basic_ostream object
template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, SerializationMode mode)
{
  switch (mode) {
    case SerializationMode::None:
      os << "None";
      break;
    case SerializationMode::MetadataOnly:
      os << "MetadataOnly";
      break;
    case SerializationMode::Everything:
      os << "Everything";
      break;
  }
  return os;
}


/// The status of a stream.
enum class StreamStatus
{
  None,       ///< No status available.
  Initialize, ///< The stream was initialized.
  Open,       ///< The stream is open.
  Closed      ///< The stream was closed.
};

/// Stream insertion operator for StreamStatus
///
/// @tparam CharT Character type for the ostream
/// @tparam Traits Traits to be used by the ostream
/// @param os The stream to insert the object into
/// @param status The stream status.
/// @return The std::basic_ostream object
template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, StreamStatus status)
{
  switch (status) {
    case StreamStatus::None:
      os << "None";
      break;
    case StreamStatus::Initialize:
      os << "Initialize";
      break;
    case StreamStatus::Open:
      os << "Open";
      break;
    case StreamStatus::Closed:
      os << "Closed";
      break;
  }
  return os;
}

/// The settings for each redo log (persistent queue storage).
/// For each path a QoS 3 subscription exists on a folder is being created.
struct RedoLogSettings
{
  /// The base path all other redo logs will be created in. Only the top folder of the full path will be created
  /// automatically.
  std::string path_{".redo"};

  /// The maximum entries of each redo log file. The log will be cycled when this is reached. This limitation does not
  /// apply if set to 0.
  uint64_t max_entries_per_file_{1024};

  /// The maximum size (in bytes) of each redo log file. The log will be cycled when this is reached. This limitation
  /// does not apply if set to 0.
  uint64_t max_size_per_file_bytes_{0};

  /// The maximum number of files in each redo log folder. The latest log will be deleted if this is reached. This
  /// limitation does not apply if set to 0.
  uint64_t max_files_per_log_{0};

  /// Controls if a flush is performed after each write operation.
  bool flush_after_write_{true};

  /// Controls if consistencies issues of the redo log are being resolved automatically on start-up.
  bool automatic_recovery_{true};

  /// Controls if the data written for values is being encrypted.
  bool write_encrypted_values_{true};

  /// The minimum available disk space threshold (in MB). If the available disk space drops below the threshold the
  /// oldest log will be deleted when the log is cycled. This limitation does not apply if set to 0.
  uint64_t min_available_disk_space_threshold_mb_{50};
};

/// Stream insertion operator for RedoLogSettings
///
/// @tparam CharT Character type for the ostream
/// @tparam Traits Traits to be used by the ostream
/// @param os The stream to insert the object into
/// @param settings The RedoLogSettings object to insert
/// @return The std::basic_ostream object
template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, const RedoLogSettings& settings)
{
  os << "base_path: " << settings.path_ << ", max_entries_per_file: " << settings.max_entries_per_file_
     << ", max_size_per_file_bytes: " << settings.max_size_per_file_bytes_
     << ", max_files_per_log_: " << settings.max_files_per_log_
     << ", flush_after_write: " << settings.flush_after_write_
     << ", automatic_recovery: " << settings.automatic_recovery_
     << ", write_encrypted_values: " << settings.write_encrypted_values_
     << ", min_available_disk_space_threshold_mb: " << settings.min_available_disk_space_threshold_mb_;
}


/// The Quality of Service settings.
struct QoSSettings
{
  /// The default queue length used by Quality of Service levels 1 and up. This needs to be set at least to 1.
  std::size_t default_queue_length_{1024};
};

/// Stream insertion operator for QoSSettings
///
/// @tparam CharT Character type for the ostream
/// @tparam Traits Traits to be used by the ostream
/// @param os The stream to insert the object into
/// @param settings The QoSSettings object to insert
/// @return The std::basic_ostream object
template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, const QoSSettings& settings)
{
  os << "default_queue_length: " << settings.default_queue_length_;
}


/// Options for the SSL configuration
struct SSLOptions
{
  bool self_signed_tls_certificate_allowed_{true}; ///< Specifies if self signed certificates are allowed or not.
  std::string certs_path_{"ca"}; ///< Specifies the location the certificate verification will look for certificates.
  std::string ca_file_{"ca/ca-bundle.crt"}; ///< Specifies the location of the CA certificates files.
  std::string cipher_list_{
    "HIGH:!AES256-SHA:!DHE-RSA-AES256-SHA:!ECDHE-RSA-AES256-SHA:!CAMELLIA:!aNULL"}; ///< Specifies the cipher list
                                                                                    ///< string - @see
                                                                                    ///< https://www.openssl.org/docs/man1.0.2/apps/ciphers.html
  bool verify_peer_{true}; ///< Specifies if the certificate of the peer should be verified
};

/// Stream insertion operator for SSLOptions
///
/// @tparam CharT Character type for the ostream
/// @tparam Traits Traits to be used by the ostream
/// @param os The stream to insert the object into
/// @param settings The SSLOptions object to insert
/// @return The std::basic_ostream object
template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, const SSLOptions& settings)
{
  os << "self_signed_tls_certificate_allowed: " << settings.self_signed_tls_certificate_allowed_
     << ", certs_path: " << settings.certs_path_ << ", ca_file: " << settings.ca_file_
     << ", ca_file: " << settings.ca_file_;
}


/// Settings for the serialization of serializable nodes.
struct SerializerSettings
{
  /// The serialization will be called intermittently with this frequency in ms. A value of 0 will serialize the nodes
  /// without delay.
  uint64_t serialization_frequency_{1000};

  /// Controls if node values shall also be serialized. If set to false no values will be serialized.
  bool serialize_values_{true};
};

/// Stream insertion operator for SerializerSettings
///
/// @tparam CharT Character type for the ostream
/// @tparam Traits Traits to be used by the ostream
/// @param os The stream to insert the object into
/// @param settings The SerializerSettings object to insert
/// @return The std::basic_ostream object
template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, const SerializerSettings& settings)
{
  os << "frequency: " << settings.serialization_frequency_ << ", serialize values: " << settings.serialize_values_;
}
}
}
