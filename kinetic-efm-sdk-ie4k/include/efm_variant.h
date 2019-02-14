// @copyright_start
// Copyright (c) 2018 Cisco and/or its affiliates. All rights reserved.
// @copyright_end

/// @file efm_variant.h

#pragma once

#include <efm_error_code.h>
#include <efm_exception.h>

#include <cmath>
#include <iomanip>
#include <map>
#include <string>
#include <vector>

namespace cisco
{
namespace efm_sdk
{

using std::map;
using std::string;
using std::vector;

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif

/// @brief The Variant is the basic container for the value handling.

/// It can stand for simple value types like a string, an integer or a double, but also for an array or map (or
/// sometimes called object). The variant also allows some basic operations on the values.
///
/// If you are working with pointers you always need to explicitly create an object. There is no implicit conversion
/// from pointers. String literals can be implicitly converted though.
///
/// To ensure that your code is portable please always use the standardized integral type aliases (e.g. int16_t,
/// uint32_t) to interface with this class.
template <uint16_t size>
class VariantBase
{
public:
  /// Array type for Variant arrays
  using ArrayType = std::vector<VariantBase<size>>;
  /// Map type for Variant maps
  using MapType = std::map<std::string, VariantBase<size>>;

  /// Specifies the type of the variant.
  enum VariantType
  {
    Null,   ///< The absence of a type.
    Bool,   ///< The variant has boolean type. The underlying type is bool.
    Int,    ///< The variant has integer type. The underlying type is int64_t.
    UInt,   ///< The variant has unsigned integer type. The underlying type is uint64_t.
    Double, ///< The variant has double type. The underlying type is double.
    String, ///< The variant has string type.
    Map,    ///< The variant has map type.
    Array   ///< The variant has array type.
  };

  /// Constructs an empty variant. VariantBase::is_null will be true. The type will be VariantBase::Null.
  VariantBase()
    : type_(Null)
  {
  }

  /// Constructs a variant of the given type. The value will be set to a default value, depending on the type:
  /// | Variant Type | Default value |
  /// | :----------: | :-----------: |
  /// | Null         |       -       |
  /// | Bool         |     false     |
  /// | Int          |       0       |
  /// | UInt         |       0       |
  /// | Double       |      0.0      |
  /// | String       |     empty     |
  /// | Map          |       -       |
  /// | Array        |       -       |
  /// @param type The type to create a variant for.
  VariantBase(VariantType type)
    : type_(type)
  {
    create_default(type);
  }

  /// Constructs a variant of boolean type. VariantBase::is_null will be false. The type will be VariantBase::Bool.
  /// VariantBase::as_bool will return the given value.
  /// @tparam T The type the template is given.
  /// @param b The bool value to set the variant to.
  template <typename T, typename std::enable_if<std::is_same<T, bool>::value, bool>::type = 0>
  VariantBase(T b)
    : type_(Bool)
  {
    new (raw_) bool(b);
  }

  /// Constructs a variant of unsigned integer type. VariantBase::is_null will be false. The type will be
  /// VariantBase::UInt. VariantBase::as_uint will return the given value.
  /// @param n The unsigned integer value to set the variant to.
  VariantBase(uint8_t n)
    : type_(UInt)
  {
    new (raw_) uint64_t(n);
  }

  /// Constructs a variant of unsigned integer type. VariantBase::is_null will be false. The type will be
  /// VariantBase::UInt. VariantBase::as_uint will return the given value.
  /// @param n The unsigned integer value to set the variant to.
  VariantBase(uint16_t n)
    : type_(UInt)
  {
    new (raw_) uint64_t(n);
  }

  /// Constructs a variant of unsigned integer type. VariantBase::is_null will be false. The type will be
  /// VariantBase::UInt. VariantBase::as_uint will return the given value.
  /// @param n The unsigned integer value to set the variant to.
  VariantBase(uint32_t n)
    : type_(UInt)
  {
    new (raw_) uint64_t(n);
  }

  /// Constructs a variant of unsigned integer type. VariantBase::is_null will be false. The type will be
  /// VariantBase::UInt. VariantBase::as_uint will return the given value.
  /// @param n The unsigned integer value to set the variant to.
  VariantBase(uint64_t n)
    : type_(UInt)
  {
    new (raw_) uint64_t(n);
  }

  /// Constructs a variant of integer type. VariantBase::is_null will be false. The type will be VariantBase::Int.
  /// VariantBase::as_int will return the given value.
  /// @param n The integer value to set the variant to.
  VariantBase(int8_t n)
    : type_(Int)
  {
    new (raw_) int64_t(n);
  }

  /// Constructs a variant of integer type. VariantBase::is_null will be false. The type will be VariantBase::Int.
  /// VariantBase::as_int will return the given value.
  /// @param n The integer value to set the variant to.
  VariantBase(int16_t n)
    : type_(Int)
  {
    new (raw_) int64_t(n);
  }

  /// Constructs a variant of integer type. VariantBase::is_null will be false. The type will be VariantBase::Int.
  /// VariantBase::as_int will return the given value.
  /// @param n The integer value to set the variant to.
  VariantBase(int32_t n)
    : type_(Int)
  {
    new (raw_) int64_t(n);
  }

  /// Constructs a variant of integer type. VariantBase::is_null will be false. The type will be VariantBase::Int.
  /// VariantBase::as_int will return the given value.
  /// @param n The integer value to set the variant to.
  VariantBase(int64_t n)
    : type_(Int)
  {
    new (raw_) int64_t(n);
  }

  /// Constructs a variant of double type. VariantBase::is_null will be false. The type will be VariantBase::Double.
  /// VariantBase::as_double will return the given value.
  /// @param f The float value to set the variant to.
  /// @throw cisco::efm_sdk::exception If value is NaN (not a number) or Infinity
  VariantBase(float f)
    : type_(Double)
  {
    if (!std::isfinite(f)) {
      throw exception(error_code::efm_variant_error, "not a valid float");
    }
    new (raw_) double(static_cast<double>(f));
  }

  /// Constructs a variant of double type. VariantBase::is_null will be false. The type will be VariantBase::Double.
  /// VariantBase::as_double will return the given value.
  /// @param f The float value to set the variant to.
  /// @throw cisco::efm_sdk::exception If value is NaN (not a number) or Infinity
  VariantBase(double f)
    : type_(Double)
  {
    if (!std::isfinite(f)) {
      throw exception(error_code::efm_variant_error, "not a valid double");
    }
    new (raw_) double(f);
  }

  /// Constructs a variant of string type. VariantBase::is_null will be false. The type will be VariantBase::String.
  /// VariantBase::as_string will return the given value.
  /// @param s The string value to set the variant to.
  VariantBase(const std::string& s)
    : type_(String)
  {
    new (raw_) std::string(s);
  }

  /// Constructs a variant of string type. VariantBase::is_null will be false. The type will be VariantBase::String.
  /// VariantBase::as_string will return the given value.
  /// @param s The string value to set the variant to.
  VariantBase(std::string&& s)
    : type_(String)
  {
    new (raw_) std::string(std::move(s));
  }

  /// Constructs a variant from a char pointer.
  ///
  /// If the pointer is valid - VariantBase::is_null will be false. The type will be VariantBase::String.
  /// VariantBase::as_string will return the given value.
  ///
  /// If the pointer is null - VariantBase::is_null will be true. The type will be VariantBase::Null.
  /// VariantBase::as_string will fail.
  //
  /// @param s The string value to set the variant to.
  explicit VariantBase(const char* s)
    : type_(Null)
  {
    if (s != nullptr) {
      type_ = String;
      new (raw_) std::string(s);
    }
  }

  /// Constructs a variant from a char pointer.
  ///
  /// If the pointer is valid - VariantBase::is_null will be false. The type will be VariantBase::String.
  /// VariantBase::as_string will return the given value.
  ///
  /// If the pointer is null - VariantBase::is_null will be true. The type will be VariantBase::Null.
  /// VariantBase::as_string will fail.
  //
  /// @param s The string value to set the variant to.
  explicit VariantBase(char* s)
    : type_(Null)
  {
    if (s != nullptr) {
      type_ = String;
      new (raw_) std::string(s);
    }
  }

  /// Constructs a variant of string type. VariantBase::is_null will be false. The type will be VariantBase::String.
  /// VariantBase::as_double will return the given value.
  /// @tparam char_count The character count of the char array.
  /// @param s The string value to set the variant to.
  template <size_t char_count>
  VariantBase(const char (&s)[char_count])
    : type_(String)
  {
    new (raw_) std::string(s);
  }

  /// Constructs a variant of array type. VariantBase::is_null will be false. The type will be VariantBase::Array.
  /// VariantBase::as_array will return the given value.
  /// @param array The array to set the variant to.
  VariantBase(const ArrayType& array)
    : type_(Array)
  {
    new (raw_) ArrayType(array);
  }

  /// Constructs a variant of array type. VariantBase::is_null will be false. The type will be VariantBase::Array.
  /// VariantBase::as_array will return the given value.
  /// @param array The array to set the variant to.
  VariantBase(ArrayType&& array)
    : type_(Array)
  {
    new (raw_) ArrayType(std::move(array));
  }

  /// Constructs a variant of map type. VariantBase::is_null will be false. The type will be VariantBase::Map.
  /// VariantBase::as_map will return the given value.
  /// @param map The map to set the variant to.
  VariantBase(const MapType& map)
    : type_(Map)
  {
    new (raw_) MapType(map);
  }

  /// Constructs a variant of map type. VariantBase::is_null will be false. The type will be VariantBase::Map.
  /// VariantBase::as_map will return the given value.
  /// @param map The map to set the variant to.
  VariantBase(MapType&& map)
    : type_(Map)
  {
    new (raw_) MapType(std::move(map));
  }

  /// Destroys the variant
  ~VariantBase()
  {
    destroy();
  }

  /// Constructs a variant from another variant, moving its internal representation.
  /// @param other Variant to move into this variant.
  VariantBase(VariantBase<size>&& other)
    : type_(other.type_)
  {
    other.type_ = Null;

    switch (type_) {
      case Null:
        break;
      case Bool:
      case Int:
      case UInt:
      case Double:
        std::copy(other.raw_, other.raw_ + size, raw_);
        break;
      case String:
        new (raw_) std::string(std::move(*reinterpret_cast<std::string*>(other.raw_)));
        break;
      case Array:
        new (raw_) ArrayType(std::move(*reinterpret_cast<ArrayType*>(other.raw_)));
        break;
      case Map:
        new (raw_) MapType(std::move(*reinterpret_cast<MapType*>(other.raw_)));
        break;
    }
  }

  /// Constructs a variant from another variant.
  /// @param other The variant to use to initialize this variant.
  VariantBase(const VariantBase<size>& other)
    : type_(other.type_)
  {
    switch (other.type_) {
      case Null:
        break;
      case Bool:
      case Int:
      case UInt:
      case Double:
        std::copy(other.raw_, other.raw_ + size, raw_);
        break;
      case String:
        new (raw_) std::string(other.as_string());
        break;
      case Array:
        new (raw_) ArrayType(other.as_array());
        break;
      case Map:
        new (raw_) MapType(other.as_map());
        break;
    }
  }

  /// Assigns the given variant to this one.
  /// @param other The variant to assign to this one.
  /// @return A reference to this variant.
  VariantBase& operator=(const VariantBase<size>& other)
  {
    if (this == &other) {
      return *this;
    }

    destroy();
    type_ = other.type_;
    switch (other.type_) {
      case Null:
        break;
      case Bool:
      case Int:
      case UInt:
      case Double:
        std::copy(other.raw_, other.raw_ + size, raw_);
        break;
      case String:
        new (raw_) std::string(other.as_string());
        break;
      case Array:
        new (raw_) ArrayType(other.as_array());
        break;
      case Map:
        new (raw_) MapType(other.as_map());
        break;
    }
    return *this;
  }

  /// Assigns the given moved variant to this one.
  /// @param other The moved variant to assign to this one.
  /// @return A reference to this variant.
  VariantBase& operator=(VariantBase<size>&& other)
  {
    if (this == &other) {
      return *this;
    }

    destroy();
    type_ = other.type_;
    other.type_ = Null;

    switch (type_) {
      case Null:
        break;
      case Bool:
      case Int:
      case UInt:
      case Double:
        std::copy(other.raw_, other.raw_ + size, raw_);
        break;
      case String:
        new (raw_) std::string(std::move(*reinterpret_cast<std::string*>(other.raw_)));
        break;
      case Array:
        new (raw_) ArrayType(std::move(*reinterpret_cast<ArrayType*>(other.raw_)));
        break;
      case Map:
        new (raw_) MapType(std::move(*reinterpret_cast<MapType*>(other.raw_)));
        break;
    }
    return *this;
  }

  /// Compare this variant to another one for equality.
  /// @param other The variant to compare to this one.
  /// @return true if both variants are identical, false otherwise.
  bool operator==(const VariantBase<size>& other) const
  {
    return equals(other);
  }

  /// Compare this variant to another one for inequality.
  /// @param other The variant to compare to this one.
  /// @return true if both variants are not identical, false otherwise.
  bool operator!=(const VariantBase<size>& other) const
  {
    return !equals(other);
  }

  /// Checks if the variant is of null type.
  /// @return true if it is of null type, otherwise false.
  bool is_null() const
  {
    return type_ == Null;
  }

  /// Returns the VariantBase::VariantType of this variant.
  /// @return The type of this variant.
  VariantType type() const
  {
    return type_;
  }

  /// Returns the boolean value of the variant if the variants type is VariantBase::Bool.
  /// @exception If the type is not VariantBase::Bool.
  /// @return The boolean value of the variant.
  bool as_bool() const
  {
    if (type_ != Bool) {
      throw exception(error_code::efm_variant_error, "not a bool");
    }
    return *reinterpret_cast<const bool*>(raw_);
  }

  /// Returns the integer value of the variant if the variants type is VariantBase::Int.
  /// @exception If the type is not VariantBase::Int.
  /// @return The integer value of the variant.
  int64_t as_int() const
  {
    if (type_ != Int && type_ != UInt) {
      throw exception(error_code::efm_variant_error, "not an int");
    }
    return *reinterpret_cast<const int64_t*>(raw_);
  }

  /// Returns the unsigned integer value of the variant if the variants type is VariantBase::UInt.
  /// @exception If the type is not VariantBase::UInt.
  /// @return The unsigned integer value of the variant.
  uint64_t as_uint() const
  {
    if (type_ != UInt && type_ != Int) {
      throw exception(error_code::efm_variant_error, "not an uint");
    }
    return *reinterpret_cast<const uint64_t*>(raw_);
  }

  /// Returns the double value of the variant if the variants type is VariantBase::Double.
  /// @exception If the type is not VariantBase::Double.
  /// @return The double value of the variant.
  double as_double() const
  {
    if (type_ != Double) {
      throw exception(error_code::efm_variant_error, "not a double");
    }
    return *reinterpret_cast<const double*>(raw_);
  }

  /// Returns the string value of the variant if the variants type is VariantBase::String.
  /// @exception If the type is not VariantBase::String.
  /// @return The string value of the variant.
  const std::string& as_string() const
  {
    if (type_ != String) {
      throw exception(error_code::efm_variant_error, "not a string");
    }
    return *reinterpret_cast<const std::string*>(raw_);
  }

  /// Returns a non-mutable array of variants if the variants type is VariantBase::Array.
  /// @exception If the type is not VariantBase::Array.
  /// @return An array of variants.
  const std::vector<VariantBase<size>>& as_array() const
  {
    if (type_ != Array) {
      throw exception(error_code::efm_variant_error, "not an array");
    }
    return *reinterpret_cast<const ArrayType*>(raw_);
  }

  /// Returns a non-mutable map of variants if the variants type is VariantBase::Map.
  /// @exception If the type is not VariantBase::Map.
  /// @return An map of variants.
  const std::map<std::string, VariantBase<size>>& as_map() const
  {
    if (type_ != Map) {
      throw exception(error_code::efm_variant_error, "not a map");
    }
    return *reinterpret_cast<const MapType*>(raw_);
  }

  /// Returns a mutable array of variants if the variants type is VariantBase::Array.
  /// @exception If the type is not VariantBase::Array.
  /// @return An array of variants.
  std::vector<VariantBase<size>>& as_array()
  {
    if (type_ != Array) {
      throw exception(error_code::efm_variant_error, "not an array");
    }
    return *reinterpret_cast<ArrayType*>(raw_);
  }

  /// Returns a mutable map of variants if the variants type is VariantBase::Map.
  /// @exception If the type is not VariantBase::Map.
  /// @return An map of variants.
  std::map<std::string, VariantBase<size>>& as_map()
  {
    if (type_ != Map) {
      throw exception(error_code::efm_variant_error, "not a map");
    }
    return *reinterpret_cast<MapType*>(raw_);
  }

  /// Retrieves a mutable element at the given array index position if the variants type is VariantBase::Array.
  /// @exception If the type is not VariantBase::Array.
  /// @exception If the index is out of range.
  /// @param i The index of the element to retrieve.
  /// @return The element at the given index.
  VariantBase<size>& get(typename ArrayType::size_type i)
  {
    ArrayType& arr = as_array();
    if (i >= arr.size()) {
      throw exception(error_code::efm_variant_error, "index out of bounds");
    }
    return arr[i];
  }

  /// Retrieves an non-mutable element at the given array index position if the variants type is VariantBase::Array.
  /// @exception If the type is not VariantBase::Array.
  /// @exception If the index is out of range.
  /// @param i The index of the element to retrieve.
  /// @return The element at the given index.
  const VariantBase<size>& get(typename ArrayType::size_type i) const
  {
    const ArrayType& arr = as_array();
    if (i >= arr.size()) {
      throw exception(error_code::efm_variant_error, "index out of bounds");
    }
    return arr[i];
  }

  /// Retrieves a mutable element at the given array index position if the variants type is VariantBase::Array.
  /// @exception If the type is not VariantBase::Array.
  /// @exception If the index is out of range.
  /// @param i The index of the element to retrieve.
  /// @return The element at the given index.
  VariantBase<size>& operator[](typename ArrayType::size_type i)
  {
    return get(i);
  }

  /// Retrieves an non-mutable element at the given array index position if the variants type is VariantBase::Array.
  /// @exception If the type is not VariantBase::Array.
  /// @exception If the index is out of range.
  /// @param i The index of the element to retrieve.
  /// @return The element at the given index.
  const VariantBase<size>& operator[](typename ArrayType::size_type i) const
  {
    return get(i);
  }

  /// Retrieves a mutable element for the given key from a variant map, if the variants type is VariantBase::Map.
  /// @exception If the type is not VariantBase::Map.
  /// @param key The key of the element to retrieve.
  /// @return The element for the key or a nullptr.
  VariantBase<size>* get(const std::string& key)
  {
    MapType& m = as_map();
    auto it = m.find(key);
    if (it == m.end()) {
      return nullptr;
    }
    return &it->second;
  }

  /// Retrieves a non-mutable element for the given key from a variant map, if the variants type is VariantBase::Map.
  /// @exception If the type is not VariantBase::Map.
  /// @param key The key of the element to retrieve.
  /// @return The element for the key or a nullptr.
  const VariantBase<size>* get(const std::string& key) const
  {
    const MapType& m = as_map();
    const auto it = m.find(key);
    if (it == m.end()) {
      return nullptr;
    }
    return &it->second;
  }

  /// Retrieves a mutable element for the given key from a variant map, if the variants type is VariantBase::Map.
  /// @exception If the type is not VariantBase::Map.
  /// @param key The key of the element to retrieve.
  /// @return The element for the key.
  /// @throw cisco::efm_sdk::exception If the key does not exist.
  VariantBase<size>& operator[](const std::string& key)
  {
    auto* v = get(key);
    if (v == nullptr) {
      throw exception(error_code::efm_variant_error, "key not found");
    }
    return *v;
  }

  /// Retrieves a non-mutable element for the given key from a variant map, if the variants type is VariantBase::Map.
  /// @exception If the type is not VariantBase::Map.
  /// @param key The key of the element to retrieve.
  /// @return The element for the key.
  /// @throw cisco::efm_sdk::exception If the key does not exist.
  const VariantBase<size>& operator[](const std::string& key) const
  {
    const auto* v = get(key);
    if (v == nullptr) {
      throw exception(error_code::efm_variant_error, "key not found");
    }
    return *v;
  }

private:
  inline void create_default(VariantType type)
  {
    switch (type) {
      case Null:
        break;
      case Bool:
        new (raw_) bool(false);
        break;
      case Int:
        new (raw_) int64_t(0);
        break;
      case UInt:
        new (raw_) uint64_t(0);
        break;
      case Double:
        new (raw_) double(0.0);
        break;
      case String:
        new (raw_) std::string();
        break;
      case Map:
        new (raw_) MapType();
        break;
      case Array:
        new (raw_) ArrayType();
        break;
    }
  }

  void destroy()
  {
    switch (type_) {
      case Null:
      case Bool:
      case Int:
      case UInt:
      case Double:
        // do nothing
        break;
      case String: {
        std::string& s = *reinterpret_cast<std::string*>(raw_);
        s.~string();
        break;
      }
      case Array: {
        ArrayType& v = *reinterpret_cast<ArrayType*>(raw_);
        v.~vector<VariantBase<size>>();
        break;
      }
      case Map: {
        MapType& m = *reinterpret_cast<MapType*>(raw_);
        m.~map<std::string, VariantBase<size>>();
        break;
      }
    }
  }

  bool equals(const VariantBase<size>& other) const
  {
    if (this->type() != other.type()) {
      if (!((this->type() == UInt && other.type() == Int) || (this->type() == Int && other.type() == UInt))) {
        return false;
      }
    }

    switch (type()) {
      case Null:
        return true;
      case Bool:
        return as_bool() == other.as_bool();
      case Int:
        return as_int() == other.as_int();
      case UInt:
        return as_uint() == other.as_uint();
      case Double:
        return as_double() == other.as_double();
      case String:
        return as_string() == other.as_string();
      case Array:
        return as_array() == other.as_array();
      case Map:
        return as_map() == other.as_map();
    }
    return false;
  }

  char raw_[size];
  VariantType type_;
};

/// @private
struct VariantHelper
{
  /// @private
  union sizer
  {
    char c1[sizeof(bool)];
    char c2[sizeof(int64_t)];
    char c3[sizeof(uint64_t)];
    char c4[sizeof(double)];
    char c5[sizeof(std::string)];
    char c6[sizeof(std::vector<VariantBase<1>>)];
    char c7[sizeof(std::map<std::string, VariantBase<1>>)];
  };
};

/// @private
using Variant = VariantBase<sizeof(VariantHelper::sizer)>;

/// stream insertion operator for VariantType
///
/// @tparam CharT Character type for the ostream
/// @tparam Traits Traits to be used by the ostream
/// @param os The stream to insert the object into
/// @param value the VariantType to insert
/// @return The std::basic_ostream object
template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, const Variant::VariantType value)
{
  switch (value) {
    case Variant::VariantType::Null:
      os << "null";
      break;
    case Variant::VariantType::Bool:
      os << "bool";
      break;
    case Variant::VariantType::Int:
    case Variant::VariantType::UInt:
    case Variant::VariantType::Double:
      os << "number";
      break;
    case Variant::VariantType::String:
      os << "string";
      break;
    case Variant::VariantType::Map:
      os << "map";
      break;
    case Variant::VariantType::Array:
      os << "array";
      break;
  }

  return os;
}

/// stream insertion operator for Variant
///
/// @tparam CharT Character type for the ostream
/// @tparam Traits Traits to be used by the ostream
/// @param os The stream to insert the object into
/// @param value the Variant object to insert
/// @return The std::basic_ostream object
template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, const Variant& value)
{
  switch (value.type()) {
    case Variant::Null:
      os << "null";
      break;
    case Variant::Array: {
      os << "[";
      bool first{true};
      for (const auto& val : value.as_array()) {
        if (first) {
          first = false;
        } else {
          os << ",";
        }
        os << val;
      }
      os << "]";
      break;
    }
    case Variant::Bool:
      os << (value.as_bool() ? "true" : "false");
      break;
    case Variant::Double:
      os << std::fixed << std::setprecision(6) << std::setfill('0') << value.as_double();
      break;
    case Variant::Int:
      os << value.as_int();
      break;
    case Variant::UInt:
      os << value.as_uint();
      break;
    case Variant::String:
      os << "\"" << value.as_string() << "\"";
      break;
    case Variant::Map: {
      os << "{";
      bool first{true};
      for (const auto& element : value.as_map()) {
        if (first) {
          first = false;
        } else {
          os << ",";
        }
        os << "\"" << element.first << "\" : " << element.second;
      }
      os << "}";
      break;
    }
  }
  return os;
}
}
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif
}
