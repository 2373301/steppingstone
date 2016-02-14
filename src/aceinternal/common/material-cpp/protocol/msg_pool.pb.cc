// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: protocol/msg_pool.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "protocol/msg_pool.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace typed {
namespace cached {
namespace protocol {

namespace {

const ::google::protobuf::Descriptor* Result_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  Result_reflection_ = NULL;
const ::google::protobuf::EnumDescriptor* Result_status_t_descriptor_ = NULL;

}  // namespace


void protobuf_AssignDesc_protocol_2fmsg_5fpool_2eproto() {
  protobuf_AddDesc_protocol_2fmsg_5fpool_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "protocol/msg_pool.proto");
  GOOGLE_CHECK(file != NULL);
  Result_descriptor_ = file->message_type(0);
  static const int Result_offsets_[2] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Result, status_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Result, message_),
  };
  Result_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      Result_descriptor_,
      Result::default_instance_,
      Result_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Result, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Result, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(Result));
  Result_status_t_descriptor_ = Result_descriptor_->enum_type(0);
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_protocol_2fmsg_5fpool_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    Result_descriptor_, &Result::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_protocol_2fmsg_5fpool_2eproto() {
  delete Result::default_instance_;
  delete Result_reflection_;
}

void protobuf_AddDesc_protocol_2fmsg_5fpool_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\027protocol/msg_pool.proto\022\025typed.cached."
    "protocol\"\362\001\n\006Result\0226\n\006status\030\001 \002(\0162&.ty"
    "ped.cached.protocol.Result.status_t\022\017\n\007m"
    "essage\030\002 \001(\014\"\236\001\n\010status_t\022\020\n\014STATE_NORMA"
    "L\020\000\022\032\n\026STATE_ENTITY_NOT_FOUND\020\001\022\027\n\023STATE"
    "_ENTITY_EXISTS\020\002\022\027\n\023STATE_ENTITY_LOCKED\020"
    "\005\022\030\n\024STATE_PROTOCOL_ERROR\020h\022\030\n\024STATE_INT"
    "ERNAL_ERROR\020i", 293);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "protocol/msg_pool.proto", &protobuf_RegisterTypes);
  Result::default_instance_ = new Result();
  Result::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_protocol_2fmsg_5fpool_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_protocol_2fmsg_5fpool_2eproto {
  StaticDescriptorInitializer_protocol_2fmsg_5fpool_2eproto() {
    protobuf_AddDesc_protocol_2fmsg_5fpool_2eproto();
  }
} static_descriptor_initializer_protocol_2fmsg_5fpool_2eproto_;

// ===================================================================

const ::google::protobuf::EnumDescriptor* Result_status_t_descriptor() {
  protobuf_AssignDescriptorsOnce();
  return Result_status_t_descriptor_;
}
bool Result_status_t_IsValid(int value) {
  switch(value) {
    case 0:
    case 1:
    case 2:
    case 5:
    case 104:
    case 105:
      return true;
    default:
      return false;
  }
}

#ifndef _MSC_VER
const Result_status_t Result::STATE_NORMAL;
const Result_status_t Result::STATE_ENTITY_NOT_FOUND;
const Result_status_t Result::STATE_ENTITY_EXISTS;
const Result_status_t Result::STATE_ENTITY_LOCKED;
const Result_status_t Result::STATE_PROTOCOL_ERROR;
const Result_status_t Result::STATE_INTERNAL_ERROR;
const Result_status_t Result::status_t_MIN;
const Result_status_t Result::status_t_MAX;
const int Result::status_t_ARRAYSIZE;
#endif  // _MSC_VER
#ifndef _MSC_VER
const int Result::kStatusFieldNumber;
const int Result::kMessageFieldNumber;
#endif  // !_MSC_VER

Result::Result()
  : ::google::protobuf::Message() {
  SharedCtor();
  // @@protoc_insertion_point(constructor:typed.cached.protocol.Result)
}

void Result::InitAsDefaultInstance() {
}

Result::Result(const Result& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
  // @@protoc_insertion_point(copy_constructor:typed.cached.protocol.Result)
}

void Result::SharedCtor() {
  ::google::protobuf::internal::GetEmptyString();
  _cached_size_ = 0;
  status_ = 0;
  message_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

Result::~Result() {
  // @@protoc_insertion_point(destructor:typed.cached.protocol.Result)
  SharedDtor();
}

void Result::SharedDtor() {
  if (message_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    delete message_;
  }
  if (this != default_instance_) {
  }
}

void Result::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* Result::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return Result_descriptor_;
}

const Result& Result::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_protocol_2fmsg_5fpool_2eproto();
  return *default_instance_;
}

Result* Result::default_instance_ = NULL;

Result* Result::New() const {
  return new Result;
}

void Result::Clear() {
  if (_has_bits_[0 / 32] & 3) {
    status_ = 0;
    if (has_message()) {
      if (message_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
        message_->clear();
      }
    }
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool Result::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:typed.cached.protocol.Result)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoff(127);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required .typed.cached.protocol.Result.status_t status = 1;
      case 1: {
        if (tag == 8) {
          int value;
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   int, ::google::protobuf::internal::WireFormatLite::TYPE_ENUM>(
                 input, &value)));
          if (::typed::cached::protocol::Result_status_t_IsValid(value)) {
            set_status(static_cast< ::typed::cached::protocol::Result_status_t >(value));
          } else {
            mutable_unknown_fields()->AddVarint(1, value);
          }
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(18)) goto parse_message;
        break;
      }

      // optional bytes message = 2;
      case 2: {
        if (tag == 18) {
         parse_message:
          DO_(::google::protobuf::internal::WireFormatLite::ReadBytes(
                input, this->mutable_message()));
        } else {
          goto handle_unusual;
        }
        if (input->ExpectAtEnd()) goto success;
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0 ||
            ::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:typed.cached.protocol.Result)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:typed.cached.protocol.Result)
  return false;
#undef DO_
}

void Result::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:typed.cached.protocol.Result)
  // required .typed.cached.protocol.Result.status_t status = 1;
  if (has_status()) {
    ::google::protobuf::internal::WireFormatLite::WriteEnum(
      1, this->status(), output);
  }

  // optional bytes message = 2;
  if (has_message()) {
    ::google::protobuf::internal::WireFormatLite::WriteBytesMaybeAliased(
      2, this->message(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
  // @@protoc_insertion_point(serialize_end:typed.cached.protocol.Result)
}

::google::protobuf::uint8* Result::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // @@protoc_insertion_point(serialize_to_array_start:typed.cached.protocol.Result)
  // required .typed.cached.protocol.Result.status_t status = 1;
  if (has_status()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteEnumToArray(
      1, this->status(), target);
  }

  // optional bytes message = 2;
  if (has_message()) {
    target =
      ::google::protobuf::internal::WireFormatLite::WriteBytesToArray(
        2, this->message(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:typed.cached.protocol.Result)
  return target;
}

int Result::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required .typed.cached.protocol.Result.status_t status = 1;
    if (has_status()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::EnumSize(this->status());
    }

    // optional bytes message = 2;
    if (has_message()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::BytesSize(
          this->message());
    }

  }
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void Result::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const Result* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const Result*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void Result::MergeFrom(const Result& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_status()) {
      set_status(from.status());
    }
    if (from.has_message()) {
      set_message(from.message());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void Result::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void Result::CopyFrom(const Result& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Result::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000001) != 0x00000001) return false;

  return true;
}

void Result::Swap(Result* other) {
  if (other != this) {
    std::swap(status_, other->status_);
    std::swap(message_, other->message_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata Result::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = Result_descriptor_;
  metadata.reflection = Result_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace protocol
}  // namespace cached
}  // namespace typed

// @@protoc_insertion_point(global_scope)