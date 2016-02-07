// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: protocol/msg_guid.proto

#ifndef PROTOBUF_protocol_2fmsg_5fguid_2eproto__INCLUDED
#define PROTOBUF_protocol_2fmsg_5fguid_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2006000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2006001 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)

namespace typed {
namespace protocol {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_protocol_2fmsg_5fguid_2eproto();
void protobuf_AssignDesc_protocol_2fmsg_5fguid_2eproto();
void protobuf_ShutdownFile_protocol_2fmsg_5fguid_2eproto();

class RequestGuid;
class ReturnGuid;

// ===================================================================

class RequestGuid : public ::google::protobuf::Message {
 public:
  RequestGuid();
  virtual ~RequestGuid();

  RequestGuid(const RequestGuid& from);

  inline RequestGuid& operator=(const RequestGuid& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const RequestGuid& default_instance();

  void Swap(RequestGuid* other);

  // implements Message ----------------------------------------------

  RequestGuid* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const RequestGuid& from);
  void MergeFrom(const RequestGuid& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // required uint32 request_no = 1;
  inline bool has_request_no() const;
  inline void clear_request_no();
  static const int kRequestNoFieldNumber = 1;
  inline ::google::protobuf::uint32 request_no() const;
  inline void set_request_no(::google::protobuf::uint32 value);

  // @@protoc_insertion_point(class_scope:typed.protocol.RequestGuid)
 private:
  inline void set_has_request_no();
  inline void clear_has_request_no();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  ::google::protobuf::uint32 request_no_;
  friend void  protobuf_AddDesc_protocol_2fmsg_5fguid_2eproto();
  friend void protobuf_AssignDesc_protocol_2fmsg_5fguid_2eproto();
  friend void protobuf_ShutdownFile_protocol_2fmsg_5fguid_2eproto();

  void InitAsDefaultInstance();
  static RequestGuid* default_instance_;
};
// -------------------------------------------------------------------

class ReturnGuid : public ::google::protobuf::Message {
 public:
  ReturnGuid();
  virtual ~ReturnGuid();

  ReturnGuid(const ReturnGuid& from);

  inline ReturnGuid& operator=(const ReturnGuid& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const ReturnGuid& default_instance();

  void Swap(ReturnGuid* other);

  // implements Message ----------------------------------------------

  ReturnGuid* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const ReturnGuid& from);
  void MergeFrom(const ReturnGuid& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // required uint32 seed_value = 1;
  inline bool has_seed_value() const;
  inline void clear_seed_value();
  static const int kSeedValueFieldNumber = 1;
  inline ::google::protobuf::uint32 seed_value() const;
  inline void set_seed_value(::google::protobuf::uint32 value);

  // required uint32 return_no = 2;
  inline bool has_return_no() const;
  inline void clear_return_no();
  static const int kReturnNoFieldNumber = 2;
  inline ::google::protobuf::uint32 return_no() const;
  inline void set_return_no(::google::protobuf::uint32 value);

  // required uint64 start_index = 3;
  inline bool has_start_index() const;
  inline void clear_start_index();
  static const int kStartIndexFieldNumber = 3;
  inline ::google::protobuf::uint64 start_index() const;
  inline void set_start_index(::google::protobuf::uint64 value);

  // @@protoc_insertion_point(class_scope:typed.protocol.ReturnGuid)
 private:
  inline void set_has_seed_value();
  inline void clear_has_seed_value();
  inline void set_has_return_no();
  inline void clear_has_return_no();
  inline void set_has_start_index();
  inline void clear_has_start_index();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  ::google::protobuf::uint32 seed_value_;
  ::google::protobuf::uint32 return_no_;
  ::google::protobuf::uint64 start_index_;
  friend void  protobuf_AddDesc_protocol_2fmsg_5fguid_2eproto();
  friend void protobuf_AssignDesc_protocol_2fmsg_5fguid_2eproto();
  friend void protobuf_ShutdownFile_protocol_2fmsg_5fguid_2eproto();

  void InitAsDefaultInstance();
  static ReturnGuid* default_instance_;
};
// ===================================================================


// ===================================================================

// RequestGuid

// required uint32 request_no = 1;
inline bool RequestGuid::has_request_no() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void RequestGuid::set_has_request_no() {
  _has_bits_[0] |= 0x00000001u;
}
inline void RequestGuid::clear_has_request_no() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void RequestGuid::clear_request_no() {
  request_no_ = 0u;
  clear_has_request_no();
}
inline ::google::protobuf::uint32 RequestGuid::request_no() const {
  // @@protoc_insertion_point(field_get:typed.protocol.RequestGuid.request_no)
  return request_no_;
}
inline void RequestGuid::set_request_no(::google::protobuf::uint32 value) {
  set_has_request_no();
  request_no_ = value;
  // @@protoc_insertion_point(field_set:typed.protocol.RequestGuid.request_no)
}

// -------------------------------------------------------------------

// ReturnGuid

// required uint32 seed_value = 1;
inline bool ReturnGuid::has_seed_value() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void ReturnGuid::set_has_seed_value() {
  _has_bits_[0] |= 0x00000001u;
}
inline void ReturnGuid::clear_has_seed_value() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void ReturnGuid::clear_seed_value() {
  seed_value_ = 0u;
  clear_has_seed_value();
}
inline ::google::protobuf::uint32 ReturnGuid::seed_value() const {
  // @@protoc_insertion_point(field_get:typed.protocol.ReturnGuid.seed_value)
  return seed_value_;
}
inline void ReturnGuid::set_seed_value(::google::protobuf::uint32 value) {
  set_has_seed_value();
  seed_value_ = value;
  // @@protoc_insertion_point(field_set:typed.protocol.ReturnGuid.seed_value)
}

// required uint32 return_no = 2;
inline bool ReturnGuid::has_return_no() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void ReturnGuid::set_has_return_no() {
  _has_bits_[0] |= 0x00000002u;
}
inline void ReturnGuid::clear_has_return_no() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void ReturnGuid::clear_return_no() {
  return_no_ = 0u;
  clear_has_return_no();
}
inline ::google::protobuf::uint32 ReturnGuid::return_no() const {
  // @@protoc_insertion_point(field_get:typed.protocol.ReturnGuid.return_no)
  return return_no_;
}
inline void ReturnGuid::set_return_no(::google::protobuf::uint32 value) {
  set_has_return_no();
  return_no_ = value;
  // @@protoc_insertion_point(field_set:typed.protocol.ReturnGuid.return_no)
}

// required uint64 start_index = 3;
inline bool ReturnGuid::has_start_index() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void ReturnGuid::set_has_start_index() {
  _has_bits_[0] |= 0x00000004u;
}
inline void ReturnGuid::clear_has_start_index() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void ReturnGuid::clear_start_index() {
  start_index_ = GOOGLE_ULONGLONG(0);
  clear_has_start_index();
}
inline ::google::protobuf::uint64 ReturnGuid::start_index() const {
  // @@protoc_insertion_point(field_get:typed.protocol.ReturnGuid.start_index)
  return start_index_;
}
inline void ReturnGuid::set_start_index(::google::protobuf::uint64 value) {
  set_has_start_index();
  start_index_ = value;
  // @@protoc_insertion_point(field_set:typed.protocol.ReturnGuid.start_index)
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace protocol
}  // namespace typed

#ifndef SWIG
namespace google {
namespace protobuf {


}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_protocol_2fmsg_5fguid_2eproto__INCLUDED
