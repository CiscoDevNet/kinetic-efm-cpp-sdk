// @copyright_start
// Copyright (c) 2018 Cisco and/or its affiliates. All rights reserved.
// @copyright_end

/// @file efm_node_path.h

#pragma once

#include <functional>
#include <string>


namespace cisco
{
namespace efm_sdk
{

/// @brief The NodePath describes the path in the node tree to a given node.

/// A path is composed of path components. Each component is a sub-path, which has to be separated by '`/`' characters.
/// A valid path would be: `/sys/links/dataflow`. The last component of the path is the name of the node. In this
/// example it is `dataflow`. The parent path is `/sys/links`. The root part is `/`.
class NodePath
{
public:
  class Iterator;

  /// Constructs an empty NodePath.
  NodePath() = default;

  /// Constructs a NodePath from a c string.
  /// @param path Path of the node.
  NodePath(const char* path)
    : path_(path)
  {
  }

  /// Constructs a NodePath from a string.
  /// @param path Path of the node.
  NodePath(const std::string& path)
    : path_(path)
  {
  }

  /// Constructs a NodePath from a string. The string will be moved into the NodePath.
  /// @param path Path of the node.
  NodePath(std::string&& path)
    : path_(std::move(path))
  {
  }

  /// Move constructs a node path from another NodePath.
  /// @param other The NodePath to move into this path.
  NodePath(NodePath&& other)
    : path_(std::move(other.path_))
    , hash_(other.hash_)
  {
    other.path_.clear();
  }

  /// Default copy constructor
  NodePath(const NodePath&) = default;
  /// Default copy assignment
  /// @return A reference to the NodePath object
  NodePath& operator=(const NodePath&) = default;

  /// Checks if the NodePath is empty.
  /// @return true if the path is empty, false otherwise
  bool empty() const
  {
    return path_.empty();
  }

  /// Clears the NodePath and sets it to an empty path.
  void clear()
  {
    hash_ = 0;
    path_.clear();
  }

  /// Assigns a c string to this path.
  /// @param path A c string to set the path from.
  /// @return This NodePath
  NodePath& operator=(const char* path)
  {
    hash_ = 0;
    path_.assign(path);
    return *this;
  }

  /// Assigns a string to this path.
  /// @param path A string to set the path from.
  /// @return This NodePath
  NodePath& operator=(const std::string& path)
  {
    hash_ = 0;
    path_.assign(path);
    return *this;
  }

  /// Assigns a string to this path and moves the string.
  /// @param path A string to set the path from.
  /// @return This NodePath
  NodePath& operator=(std::string&& path)
  {
    hash_ = 0;
    path_ = std::move(path);
    return *this;
  }

  /// Removes the given path from this path if it matches, actually rebasing it to the given path.
  /// Example:
  ///
  /// @code
  ///     cisco::efm_sdk::NodePath p("/downstream/my-link/first_node");
  ///     cisco::efm_sdk::NodePath p1 = p.rebase("/downstream/my-link");
  ///     // p1 == "/first_node"
  /// @endcode
  ///
  /// If the path is identical to this path, `/` is returned. If the given path does not match, an empty path is
  /// returned.
  /// @param rhs The path to rebase this path on.
  /// @return The rebased path if the given path matches part of this path, `/`if the path is identical or an empty path
  /// if it doesn't match.
  ///
  NodePath rebase(const NodePath& rhs) const;

  /// Concatenates two NodePath yielding a new path. Will ensure that there is at most one `/` separator between the two
  /// components. Does never add a trailing `/` to a path.
  /// @param lhs The path to concatenate the rhs path to.
  /// @param rhs The path to concatenate to the lhs path.
  /// @return The concatenated path.
  friend NodePath operator/(const NodePath& lhs, const NodePath& rhs);

  /// Concatenates a NodePath to this path. Will ensure that there is at most one `/` separator between the two
  /// components.
  /// @param rhs The path to concatenate to this path.
  /// @return The concatenated path.
  NodePath& operator/=(const std::string& rhs)
  {
    hash_ = 0;
    *this = *this / rhs;
    return *this;
  }

  /// Compares two NodePath for equality.
  /// @param lhs The first path to compare.
  /// @param rhs The other path to compare.
  /// @return true if the paths are equal, false otherwise
  friend bool operator==(const NodePath& lhs, const NodePath& rhs)
  {
    return lhs.path_ == rhs.path_;
  }

  /// Compares two NodePath for inequality.
  /// @param lhs The first path to compare.
  /// @param rhs The other path to compare.
  /// @return true if the paths are not equal, false otherwise
  friend bool operator!=(const NodePath& lhs, const NodePath& rhs)
  {
    return lhs.path_ != rhs.path_;
  }

  /// Compares two NodePath lexicographically.
  /// @param lhs The first path to compare.
  /// @param rhs The other path to compare.
  /// @return true if the left path is lexicographically smaller than the right one, otherwise false.
  friend bool operator<(const NodePath& lhs, const NodePath& rhs)
  {
    return lhs.path_ < rhs.path_;
  }

  /// Compares two NodePath lexicographically.
  /// @param lhs The first path to compare.
  /// @param rhs The other path to compare.
  /// @return true if the left path is lexicographically bigger than the right one, otherwise false.
  friend bool operator>(const NodePath& lhs, const NodePath& rhs)
  {
    return lhs.path_ > rhs.path_;
  }

  /// Returns the base of the path and set the rest to the remaining part of the path.
  /// Example:
  ///
  /// @code
  ///     cisco::efm_sdk::NodePath p("/downstream/my-link");
  ///     cisco::efm_sdk::NodePath rest;
  ///     cisco::efm_sdk::NodePath base = p.get_base(rest);
  ///     // base == "downstream"
  ///     // rest == "/my-link"
  /// @endcode
  ///
  /// @param rest Will be set to the rest of the path if the base is removed. Will be an empty path if the is no rest.
  /// @return The base of the path without leading `/`, if there is one. Otherwise an empty path.
  NodePath get_base(NodePath& rest) const;

  /// Returns the parent path of this path, which in essence is the path of this node without the name.
  /// @return The parent path of the node.
  NodePath get_parent_path() const;

  /// Returns the name (i.e. the last part) of the path. Will be an empty string for the root node.
  /// @return The name as string of the path
  std::string get_name() const;

  template <class CharT, class Traits>
  friend std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, const NodePath& path);

  /// Computes a hash value for NodePath.
  struct Hash
  {
    /// Computes a hash value for NodePath.
    /// @param path The path to compute the hash for.
    /// @return The computed hash.
    std::size_t operator()(const NodePath& path) const;
  };

  /// Converts the node path to a string.
  /// @return The node path as string.
  const std::string& to_string() const;

  /// Returns the start iterator for the parts of the NodePath
  /// @return Start iterator for the parts of the NodePath
  Iterator begin() const;

  /// Returns the end iterator for the parts of the NodePath
  /// @return End iterator for the parts of the NodePath
  Iterator end() const;

private:
  friend class Iterator;

  /// Returns hash for the node path. The hash is lazily generated and cached until the next modification.
  /// @return The hash of the node path.
  std::size_t get_hash() const;

  std::string path_;
  mutable std::size_t hash_{0};
};


/// @brief Iterator to iterate over the NodePath components.
class NodePath::Iterator
{
public:
  /// the iterator type
  using iterator = Iterator;
  /// @private
  using iterator_category = std::input_iterator_tag;
  /// @private
  using value_type = const NodePath;
  /// @private
  using difference_type = std::ptrdiff_t;
  /// alias for const pointer to NodePath
  using pointer = const NodePath*;
  /// alias for const reference to NodePath
  using reference = const NodePath&;

  /// dereferences the iterator
  /// @return A reference to the object behind the iterator
  reference operator*() const;

  /// dereferences the iterator
  /// @return A pointer to the object behind the iterator
  pointer operator->() const;

  /// increments the iterator
  /// @return A reference to the incremented iterator
  iterator& operator++();

  /// Compare this iterator to another one for equality.
  /// @param other The iterator to compare to this one.
  /// @return true if both iterators are identical, false otherwise.
  bool operator==(const Iterator& other) const;

  /// Compare this iterator to another one for inequality.
  /// @param other The iterator to compare to this one.
  /// @return true if both iterators are not identical, false otherwise.
  bool operator!=(const Iterator& other) const;

private:
  friend class NodePath;

  Iterator(const NodePath& path);

  const std::string* path_{nullptr};
  NodePath element_;
  std::string::size_type pos_{std::string::npos};
};

/// stream insertion operator for NodePath
///
/// @tparam CharT Character type for the ostream
/// @tparam Traits Traits to be used by the ostream
/// @param os the stream to insert the object into
/// @param path the NodePath object to insert
/// @return the std::basic_ostream object
template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, const NodePath& path)
{
  os << path.path_;
  return os;
}
}
}

namespace std
{
/// Computes a hash value for NodePath.
template <>
struct hash<cisco::efm_sdk::NodePath>
{
  /// Computes a hash value for NodePath.
  /// @param path The path to compute the hash for.
  /// @return The computed hash.
  std::size_t operator()(const cisco::efm_sdk::NodePath& path) const noexcept
  {
    return cisco::efm_sdk::NodePath::Hash{}(path);
  }
};
}
