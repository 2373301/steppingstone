// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: protocol/msg_cache.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "protocol/msg_cache.pb.h"

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

namespace protocol {

namespace {

const ::google::protobuf::Descriptor* OpResult_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  OpResult_reflection_ = NULL;
const ::google::protobuf::EnumDescriptor* OpResult_status_t_descriptor_ = NULL;
const ::google::protobuf::Descriptor* db_data_request_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  db_data_request_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_protocol_2fmsg_5fcache_2eproto() {
  protobuf_AddDesc_protocol_2fmsg_5fcache_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "protocol/msg_cache.proto");
  GOOGLE_CHECK(file != NULL);
  OpResult_descriptor_ = file->message_type(0);
  static const int OpResult_offsets_[2] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(OpResult, status_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(OpResult, message_),
  };
  OpResult_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      OpResult_descriptor_,
      OpResult::default_instance_,
      OpResult_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(OpResult, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(OpResult, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(OpResult));
  OpResult_status_t_descriptor_ = OpResult_descriptor_->enum_type(0);
  db_data_request_descriptor_ = file->message_type(1);
  static const int db_data_request_offsets_[6] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(db_data_request, transaction_index_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(db_data_request, transaction_num_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(db_data_request, transaction_id_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(db_data_request, line_id_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(db_data_request, data_stream_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(db_data_request, owner_guid_),
  };
  db_data_request_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      db_data_request_descriptor_,
      db_data_request::default_instance_,
      db_data_request_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(db_data_request, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(db_data_request, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(db_data_request));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_protocol_2fmsg_5fcache_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    OpResult_descriptor_, &OpResult::default_instance());
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    db_data_request_descriptor_, &db_data_request::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_protocol_2fmsg_5fcache_2eproto() {
  delete OpResult::default_instance_;
  delete OpResult_reflection_;
  delete db_data_request::default_instance_;
  delete db_data_request_reflection_;
}

void protobuf_AddDesc_protocol_2fmsg_5fcache_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\030protocol/msg_cache.proto\022\010protocol\"\351\001\n"
    "\010OpResult\022+\n\006status\030\001 \002(\0162\033.protocol.OpR"
    "esult.status_t\022\017\n\007message\030\002 \001(\014\"\236\001\n\010stat"
    "us_t\022\020\n\014STATE_NORMAL\020\000\022\032\n\026STATE_ENTITY_N"
    "OT_FOUND\020\001\022\027\n\023STATE_ENTITY_EXISTS\020\002\022\027\n\023S"
    "TATE_ENTITY_LOCKED\020\005\022\030\n\024STATE_PROTOCOL_E"
    "RROR\020h\022\030\n\024STATE_INTERNAL_ERROR\020i\"\227\001\n\017db_"
    "data_request\022\031\n\021transaction_index\030\001 \002(\r\022"
    "\027\n\017transaction_num\030\002 \002(\r\022\026\n\016transaction_"
    "id\030\003 \002(\r\022\017\n\007line_id\030\004 \001(\r\022\023\n\013data_stream"
    "\030\005 \001(\014\022\022\n\nowner_guid\030\006 \001(\004", 426);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "protocol/msg_cache.proto", &protobuf_RegisterTypes);
  OpResult::default_instance_ = new OpResult();
  db_data_request::default_instance_ = new db_data_request();
  OpResult::default_instance_->InitAsDefaultInstance();
  db_data_request::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_protocol_2fmsg_5fcache_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_protocol_2fmsg_5fcache_2eproto {
  StaticDescriptorInitializer_protocol_2fmsg_5fcache_2eproto() {
    protobuf_AddDesc_protocol_2fmsg_5fcache_2eproto();
  }
} static_descriptor_initializer_protocol_2fmsg_5fcache_2eproto_;

// ===================================================================

const ::google::protobuf::EnumDescriptor* OpResult_status_t_descriptor() {
  protobuf_AssignDescriptorsOnce();
  return OpResult_status_t_descriptor_;
}
bool OpResult_status_t_IsValid(int value) {
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
const OpResult_status_t OpResult::STATE_NORMAL;
const OpResult_status_t OpResult::STATE_ENTITY_NOT_FOUND;
const OpResult_status_t OpResult::STATE_ENTITY_EXISTS;
const OpResult_status_t OpResult::STATE_ENTITY_LOCKED;
const OpResult_status_t OpResult::STATE_PROTOCOL_ERROR;
const OpResult_status_t OpResult::STATE_INTERNAL_ERROR;
const OpResult_status_t OpResult::status_t_MIN;
const OpResult_status_t OpResult::status_t_MAX;
const int OpResult::status_t_ARRAYSIZE;
#endif  // _MSC_VER
#ifndef _MSC_VER
const int OpResult::kStatusFieldNumber;
const int OpResult::kMessageFieldNumber;
#endif  // !_MSC_VER

OpResult::OpResult()
  : ::google::protobuf::Message() {
  SharedCtor();
  // @@protoc_insertion_point(constructor:protocol.OpResult)
}

void OpResult::InitAsDefaultInstance() {
}

OpResult::OpResult(const OpResult& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
  // @@protoc_insertion_point(copy_constructor:protocol.OpResult)
}

void OpResult::SharedCtor() {
  ::google::protobuf::internal::GetEmptyString();
  _cached_size_ = 0;
  status_ = 0;
  message_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

OpResult::~OpResult() {
  // @@protoc_insertion_point(destructor:protocol.OpResult)
  SharedDtor();
}

void OpResult::SharedDtor() {
  if (message_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    delete message_;
  }
  if (this != default_instance_) {
  }
}

void OpResult::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* OpResult::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return OpResult_descriptor_;
}

const OpResult& OpResult::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_protocol_2fmsg_5fcache_2eproto();
  return *default_instance_;
}

OpResult* OpResult::default_instance_ = NULL;

OpResult* OpResult::New() const {
  return new OpResult;
}

void OpResult::Clear() {
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

bool OpResult::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:protocol.OpResult)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoff(127);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required .protocol.OpResult.status_t status = 1;
      case 1: {
        if (tag == 8) {
          int value;
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   int, ::google::protobuf::internal::WireFormatLite::TYPE_ENUM>(
                 input, &value)));
          if (::protocol::OpResult_status_t_IsValid(value)) {
            set_status(static_cast< ::protocol::OpResult_status_t >(value));
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
  // @@protoc_insertion_point(parse_success:protocol.OpResult)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:protocol.OpResult)
  return false;
#undef DO_
}

void OpResult::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:protocol.OpResult)
  // required .protocol.OpResult.status_t status = 1;
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
  // @@protoc_insertion_point(serialize_end:protocol.OpResult)
}

::google::protobuf::uint8* OpResult::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // @@protoc_insertion_point(serialize_to_array_start:protocol.OpResult)
  // required .protocol.OpResult.status_t status = 1;
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
  // @@protoc_insertion_point(serialize_to_array_end:protocol.OpResult)
  return target;
}

int OpResult::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required .protocol.OpResult.status_t status = 1;
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

void OpResult::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const OpResult* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const OpResult*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void OpResult::MergeFrom(const OpResult& from) {
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

void OpResult::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void OpResult::CopyFrom(const OpResult& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool OpResult::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000001) != 0x00000001) return false;

  return true;
}

void OpResult::Swap(OpResult* other) {
  if (other != this) {
    std::swap(status_, other->status_);
    std::swap(message_, other->message_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata OpResult::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = OpResult_descriptor_;
  metadata.reflection = OpResult_reflection_;
  return metadata;
}


// ===================================================================

#ifndef _MSC_VER
const int db_data_request::kTransactionIndexFieldNumber;
const int db_data_request::kTransactionNumFieldNumber;
const int db_data_request::kTransactionIdFieldNumber;
const int db_data_request::kLineIdFieldNumber;
const int db_data_request::kDataStreamFieldNumber;
const int db_data_request::kOwnerGuidFieldNumber;
#endif  // !_MSC_VER

db_data_request::db_data_request()
  : ::google::protobuf::Message() {
  SharedCtor();
  // @@protoc_insertion_point(constructor:protocol.db_data_request)
}

void db_data_request::InitAsDefaultInstance() {
}

db_data_request::db_data_request(const db_data_request& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
  // @@protoc_insertion_point(copy_constructor:protocol.db_data_request)
}

void db_data_request::SharedCtor() {
  ::google::protobuf::internal::GetEmptyString();
  _cached_size_ = 0;
  transaction_index_ = 0u;
  transaction_num_ = 0u;
  transaction_id_ = 0u;
  line_id_ = 0u;
  data_stream_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  owner_guid_ = GOOGLE_ULONGLONG(0);
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

db_data_request::~db_data_request() {
  // @@protoc_insertion_point(destructor:protocol.db_data_request)
  SharedDtor();
}

void db_data_request::SharedDtor() {
  if (data_stream_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    delete data_stream_;
  }
  if (this != default_instance_) {
  }
}

void db_data_request::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* db_data_request::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return db_data_request_descriptor_;
}

const db_data_request& db_data_request::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_protocol_2fmsg_5fcache_2eproto();
  return *default_instance_;
}

db_data_request* db_data_request::default_instance_ = NULL;

db_data_request* db_data_request::New() const {
  return new db_data_request;
}

void db_data_request::Clear() {
#define OFFSET_OF_FIELD_(f) (reinterpret_cast<char*>(      \
  &reinterpret_cast<db_data_request*>(16)->f) - \
   reinterpret_cast<char*>(16))

#define ZR_(first, last) do {                              \
    size_t f = OFFSET_OF_FIELD_(first);                    \
    size_t n = OFFSET_OF_FIELD_(last) - f + sizeof(last);  \
    ::memset(&first, 0, n);                                \
  } while (0)

  if (_has_bits_[0 / 32] & 63) {
    ZR_(transaction_index_, line_id_);
    if (has_data_stream()) {
      if (data_stream_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
        data_stream_->clear();
      }
    }
    owner_guid_ = GOOGLE_ULONGLONG(0);
  }

#undef OFFSET_OF_FIELD_
#undef ZR_

  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool db_data_request::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:protocol.db_data_request)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoff(127);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required uint32 transaction_index = 1;
      case 1: {
        if (tag == 8) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &transaction_index_)));
          set_has_transaction_index();
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(16)) goto parse_transaction_num;
        break;
      }

      // required uint32 transaction_num = 2;
      case 2: {
        if (tag == 16) {
         parse_transaction_num:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &transaction_num_)));
          set_has_transaction_num();
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(24)) goto parse_transaction_id;
        break;
      }

      // required uint32 transaction_id = 3;
      case 3: {
        if (tag == 24) {
         parse_transaction_id:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &transaction_id_)));
          set_has_transaction_id();
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(32)) goto parse_line_id;
        break;
      }

      // optional uint32 line_id = 4;
      case 4: {
        if (tag == 32) {
         parse_line_id:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &line_id_)));
          set_has_line_id();
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(42)) goto parse_data_stream;
        break;
      }

      // optional bytes data_stream = 5;
      case 5: {
        if (tag == 42) {
         parse_data_stream:
          DO_(::google::protobuf::internal::WireFormatLite::ReadBytes(
                input, this->mutable_data_stream()));
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(48)) goto parse_owner_guid;
        break;
      }

      // optional uint64 owner_guid = 6;
      case 6: {
        if (tag == 48) {
         parse_owner_guid:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint64, ::google::protobuf::internal::WireFormatLite::TYPE_UINT64>(
                 input, &owner_guid_)));
          set_has_owner_guid();
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
  // @@protoc_insertion_point(parse_success:protocol.db_data_request)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:protocol.db_data_request)
  return false;
#undef DO_
}

void db_data_request::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:protocol.db_data_request)
  // required uint32 transaction_index = 1;
  if (has_transaction_index()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(1, this->transaction_index(), output);
  }

  // required uint32 transaction_num = 2;
  if (has_transaction_num()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(2, this->transaction_num(), output);
  }

  // required uint32 transaction_id = 3;
  if (has_transaction_id()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(3, this->transaction_id(), output);
  }

  // optional uint32 line_id = 4;
  if (has_line_id()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(4, this->line_id(), output);
  }

  // optional bytes data_stream = 5;
  if (has_data_stream()) {
    ::google::protobuf::internal::WireFormatLite::WriteBytesMaybeAliased(
      5, this->data_stream(), output);
  }

  // optional uint64 owner_guid = 6;
  if (has_owner_guid()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt64(6, this->owner_guid(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
  // @@protoc_insertion_point(serialize_end:protocol.db_data_request)
}

::google::protobuf::uint8* db_data_request::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // @@protoc_insertion_point(serialize_to_array_start:protocol.db_data_request)
  // required uint32 transaction_index = 1;
  if (has_transaction_index()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(1, this->transaction_index(), target);
  }

  // required uint32 transaction_num = 2;
  if (has_transaction_num()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(2, this->transaction_num(), target);
  }

  // required uint32 transaction_id = 3;
  if (has_transaction_id()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(3, this->transaction_id(), target);
  }

  // optional uint32 line_id = 4;
  if (has_line_id()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(4, this->line_id(), target);
  }

  // optional bytes data_stream = 5;
  if (has_data_stream()) {
    target =
      ::google::protobuf::internal::WireFormatLite::WriteBytesToArray(
        5, this->data_stream(), target);
  }

  // optional uint64 owner_guid = 6;
  if (has_owner_guid()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt64ToArray(6, this->owner_guid(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:protocol.db_data_request)
  return target;
}

int db_data_request::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required uint32 transaction_index = 1;
    if (has_transaction_index()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->transaction_index());
    }

    // required uint32 transaction_num = 2;
    if (has_transaction_num()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->transaction_num());
    }

    // required uint32 transaction_id = 3;
    if (has_transaction_id()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->transaction_id());
    }

    // optional uint32 line_id = 4;
    if (has_line_id()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->line_id());
    }

    // optional bytes data_stream = 5;
    if (has_data_stream()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::BytesSize(
          this->data_stream());
    }

    // optional uint64 owner_guid = 6;
    if (has_owner_guid()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt64Size(
          this->owner_guid());
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

void db_data_request::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const db_data_request* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const db_data_request*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void db_data_request::MergeFrom(const db_data_request& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_transaction_index()) {
      set_transaction_index(from.transaction_index());
    }
    if (from.has_transaction_num()) {
      set_transaction_num(from.transaction_num());
    }
    if (from.has_transaction_id()) {
      set_transaction_id(from.transaction_id());
    }
    if (from.has_line_id()) {
      set_line_id(from.line_id());
    }
    if (from.has_data_stream()) {
      set_data_stream(from.data_stream());
    }
    if (from.has_owner_guid()) {
      set_owner_guid(from.owner_guid());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void db_data_request::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void db_data_request::CopyFrom(const db_data_request& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool db_data_request::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000007) != 0x00000007) return false;

  return true;
}

void db_data_request::Swap(db_data_request* other) {
  if (other != this) {
    std::swap(transaction_index_, other->transaction_index_);
    std::swap(transaction_num_, other->transaction_num_);
    std::swap(transaction_id_, other->transaction_id_);
    std::swap(line_id_, other->line_id_);
    std::swap(data_stream_, other->data_stream_);
    std::swap(owner_guid_, other->owner_guid_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata db_data_request::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = db_data_request_descriptor_;
  metadata.reflection = db_data_request_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace protocol

// @@protoc_insertion_point(global_scope)
