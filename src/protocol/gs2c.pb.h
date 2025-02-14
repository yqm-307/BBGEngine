// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: gs2c.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_gs2c_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_gs2c_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3010000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3010001 < PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers. Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/port_undef.inc>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/inlined_string_field.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_gs2c_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_gs2c_2eproto {
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTableField entries[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::AuxillaryParseTableField aux[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTable schema[1]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::FieldMetadata field_metadata[];
  static const ::PROTOBUF_NAMESPACE_ID::internal::SerializationTable serialization_table[];
  static const ::PROTOBUF_NAMESPACE_ID::uint32 offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_gs2c_2eproto;
class GS2C_ON_LOGIN_RSP;
class GS2C_ON_LOGIN_RSPDefaultTypeInternal;
extern GS2C_ON_LOGIN_RSPDefaultTypeInternal _GS2C_ON_LOGIN_RSP_default_instance_;
PROTOBUF_NAMESPACE_OPEN
template<> ::GS2C_ON_LOGIN_RSP* Arena::CreateMaybeMessage<::GS2C_ON_LOGIN_RSP>(Arena*);
PROTOBUF_NAMESPACE_CLOSE

// ===================================================================

class GS2C_ON_LOGIN_RSP :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:GS2C_ON_LOGIN_RSP) */ {
 public:
  GS2C_ON_LOGIN_RSP();
  virtual ~GS2C_ON_LOGIN_RSP();

  GS2C_ON_LOGIN_RSP(const GS2C_ON_LOGIN_RSP& from);
  GS2C_ON_LOGIN_RSP(GS2C_ON_LOGIN_RSP&& from) noexcept
    : GS2C_ON_LOGIN_RSP() {
    *this = ::std::move(from);
  }

  inline GS2C_ON_LOGIN_RSP& operator=(const GS2C_ON_LOGIN_RSP& from) {
    CopyFrom(from);
    return *this;
  }
  inline GS2C_ON_LOGIN_RSP& operator=(GS2C_ON_LOGIN_RSP&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return GetMetadataStatic().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return GetMetadataStatic().reflection;
  }
  static const GS2C_ON_LOGIN_RSP& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const GS2C_ON_LOGIN_RSP* internal_default_instance() {
    return reinterpret_cast<const GS2C_ON_LOGIN_RSP*>(
               &_GS2C_ON_LOGIN_RSP_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(GS2C_ON_LOGIN_RSP& a, GS2C_ON_LOGIN_RSP& b) {
    a.Swap(&b);
  }
  inline void Swap(GS2C_ON_LOGIN_RSP* other) {
    if (other == this) return;
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline GS2C_ON_LOGIN_RSP* New() const final {
    return CreateMaybeMessage<GS2C_ON_LOGIN_RSP>(nullptr);
  }

  GS2C_ON_LOGIN_RSP* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<GS2C_ON_LOGIN_RSP>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const GS2C_ON_LOGIN_RSP& from);
  void MergeFrom(const GS2C_ON_LOGIN_RSP& from);
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  ::PROTOBUF_NAMESPACE_ID::uint8* InternalSerializeWithCachedSizesToArray(
      ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  inline void SharedCtor();
  inline void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(GS2C_ON_LOGIN_RSP* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "GS2C_ON_LOGIN_RSP";
  }
  private:
  inline ::PROTOBUF_NAMESPACE_ID::Arena* GetArenaNoVirtual() const {
    return nullptr;
  }
  inline void* MaybeArenaPtr() const {
    return nullptr;
  }
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_gs2c_2eproto);
    return ::descriptor_table_gs2c_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kFieldHandleIDFieldNumber = 1,
  };
  // int32 Field_HandleID = 1;
  void clear_field_handleid();
  ::PROTOBUF_NAMESPACE_ID::int32 field_handleid() const;
  void set_field_handleid(::PROTOBUF_NAMESPACE_ID::int32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int32 _internal_field_handleid() const;
  void _internal_set_field_handleid(::PROTOBUF_NAMESPACE_ID::int32 value);
  public:

  // @@protoc_insertion_point(class_scope:GS2C_ON_LOGIN_RSP)
 private:
  class _Internal;

  ::PROTOBUF_NAMESPACE_ID::internal::InternalMetadataWithArena _internal_metadata_;
  ::PROTOBUF_NAMESPACE_ID::int32 field_handleid_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_gs2c_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// GS2C_ON_LOGIN_RSP

// int32 Field_HandleID = 1;
inline void GS2C_ON_LOGIN_RSP::clear_field_handleid() {
  field_handleid_ = 0;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 GS2C_ON_LOGIN_RSP::_internal_field_handleid() const {
  return field_handleid_;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 GS2C_ON_LOGIN_RSP::field_handleid() const {
  // @@protoc_insertion_point(field_get:GS2C_ON_LOGIN_RSP.Field_HandleID)
  return _internal_field_handleid();
}
inline void GS2C_ON_LOGIN_RSP::_internal_set_field_handleid(::PROTOBUF_NAMESPACE_ID::int32 value) {
  
  field_handleid_ = value;
}
inline void GS2C_ON_LOGIN_RSP::set_field_handleid(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _internal_set_field_handleid(value);
  // @@protoc_insertion_point(field_set:GS2C_ON_LOGIN_RSP.Field_HandleID)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)


// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_gs2c_2eproto
