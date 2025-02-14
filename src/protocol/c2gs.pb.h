// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: c2gs.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_c2gs_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_c2gs_2eproto

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
#define PROTOBUF_INTERNAL_EXPORT_c2gs_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_c2gs_2eproto {
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTableField entries[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::AuxillaryParseTableField aux[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTable schema[2]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::FieldMetadata field_metadata[];
  static const ::PROTOBUF_NAMESPACE_ID::internal::SerializationTable serialization_table[];
  static const ::PROTOBUF_NAMESPACE_ID::uint32 offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_c2gs_2eproto;
class C2GS_ON_LOGIN_REQ;
class C2GS_ON_LOGIN_REQDefaultTypeInternal;
extern C2GS_ON_LOGIN_REQDefaultTypeInternal _C2GS_ON_LOGIN_REQ_default_instance_;
class C2GS_Test_Echo_Req;
class C2GS_Test_Echo_ReqDefaultTypeInternal;
extern C2GS_Test_Echo_ReqDefaultTypeInternal _C2GS_Test_Echo_Req_default_instance_;
PROTOBUF_NAMESPACE_OPEN
template<> ::C2GS_ON_LOGIN_REQ* Arena::CreateMaybeMessage<::C2GS_ON_LOGIN_REQ>(Arena*);
template<> ::C2GS_Test_Echo_Req* Arena::CreateMaybeMessage<::C2GS_Test_Echo_Req>(Arena*);
PROTOBUF_NAMESPACE_CLOSE

// ===================================================================

class C2GS_Test_Echo_Req :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:C2GS_Test_Echo_Req) */ {
 public:
  C2GS_Test_Echo_Req();
  virtual ~C2GS_Test_Echo_Req();

  C2GS_Test_Echo_Req(const C2GS_Test_Echo_Req& from);
  C2GS_Test_Echo_Req(C2GS_Test_Echo_Req&& from) noexcept
    : C2GS_Test_Echo_Req() {
    *this = ::std::move(from);
  }

  inline C2GS_Test_Echo_Req& operator=(const C2GS_Test_Echo_Req& from) {
    CopyFrom(from);
    return *this;
  }
  inline C2GS_Test_Echo_Req& operator=(C2GS_Test_Echo_Req&& from) noexcept {
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
  static const C2GS_Test_Echo_Req& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const C2GS_Test_Echo_Req* internal_default_instance() {
    return reinterpret_cast<const C2GS_Test_Echo_Req*>(
               &_C2GS_Test_Echo_Req_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(C2GS_Test_Echo_Req& a, C2GS_Test_Echo_Req& b) {
    a.Swap(&b);
  }
  inline void Swap(C2GS_Test_Echo_Req* other) {
    if (other == this) return;
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline C2GS_Test_Echo_Req* New() const final {
    return CreateMaybeMessage<C2GS_Test_Echo_Req>(nullptr);
  }

  C2GS_Test_Echo_Req* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<C2GS_Test_Echo_Req>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const C2GS_Test_Echo_Req& from);
  void MergeFrom(const C2GS_Test_Echo_Req& from);
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
  void InternalSwap(C2GS_Test_Echo_Req* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "C2GS_Test_Echo_Req";
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
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_c2gs_2eproto);
    return ::descriptor_table_c2gs_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kFieldMessageFieldNumber = 2,
    kFieldHandleIDFieldNumber = 1,
  };
  // string Field_Message = 2;
  void clear_field_message();
  const std::string& field_message() const;
  void set_field_message(const std::string& value);
  void set_field_message(std::string&& value);
  void set_field_message(const char* value);
  void set_field_message(const char* value, size_t size);
  std::string* mutable_field_message();
  std::string* release_field_message();
  void set_allocated_field_message(std::string* field_message);
  private:
  const std::string& _internal_field_message() const;
  void _internal_set_field_message(const std::string& value);
  std::string* _internal_mutable_field_message();
  public:

  // int32 Field_HandleID = 1;
  void clear_field_handleid();
  ::PROTOBUF_NAMESPACE_ID::int32 field_handleid() const;
  void set_field_handleid(::PROTOBUF_NAMESPACE_ID::int32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int32 _internal_field_handleid() const;
  void _internal_set_field_handleid(::PROTOBUF_NAMESPACE_ID::int32 value);
  public:

  // @@protoc_insertion_point(class_scope:C2GS_Test_Echo_Req)
 private:
  class _Internal;

  ::PROTOBUF_NAMESPACE_ID::internal::InternalMetadataWithArena _internal_metadata_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr field_message_;
  ::PROTOBUF_NAMESPACE_ID::int32 field_handleid_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_c2gs_2eproto;
};
// -------------------------------------------------------------------

class C2GS_ON_LOGIN_REQ :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:C2GS_ON_LOGIN_REQ) */ {
 public:
  C2GS_ON_LOGIN_REQ();
  virtual ~C2GS_ON_LOGIN_REQ();

  C2GS_ON_LOGIN_REQ(const C2GS_ON_LOGIN_REQ& from);
  C2GS_ON_LOGIN_REQ(C2GS_ON_LOGIN_REQ&& from) noexcept
    : C2GS_ON_LOGIN_REQ() {
    *this = ::std::move(from);
  }

  inline C2GS_ON_LOGIN_REQ& operator=(const C2GS_ON_LOGIN_REQ& from) {
    CopyFrom(from);
    return *this;
  }
  inline C2GS_ON_LOGIN_REQ& operator=(C2GS_ON_LOGIN_REQ&& from) noexcept {
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
  static const C2GS_ON_LOGIN_REQ& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const C2GS_ON_LOGIN_REQ* internal_default_instance() {
    return reinterpret_cast<const C2GS_ON_LOGIN_REQ*>(
               &_C2GS_ON_LOGIN_REQ_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    1;

  friend void swap(C2GS_ON_LOGIN_REQ& a, C2GS_ON_LOGIN_REQ& b) {
    a.Swap(&b);
  }
  inline void Swap(C2GS_ON_LOGIN_REQ* other) {
    if (other == this) return;
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline C2GS_ON_LOGIN_REQ* New() const final {
    return CreateMaybeMessage<C2GS_ON_LOGIN_REQ>(nullptr);
  }

  C2GS_ON_LOGIN_REQ* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<C2GS_ON_LOGIN_REQ>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const C2GS_ON_LOGIN_REQ& from);
  void MergeFrom(const C2GS_ON_LOGIN_REQ& from);
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
  void InternalSwap(C2GS_ON_LOGIN_REQ* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "C2GS_ON_LOGIN_REQ";
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
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_c2gs_2eproto);
    return ::descriptor_table_c2gs_2eproto.file_level_metadata[kIndexInFileMessages];
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

  // @@protoc_insertion_point(class_scope:C2GS_ON_LOGIN_REQ)
 private:
  class _Internal;

  ::PROTOBUF_NAMESPACE_ID::internal::InternalMetadataWithArena _internal_metadata_;
  ::PROTOBUF_NAMESPACE_ID::int32 field_handleid_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_c2gs_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// C2GS_Test_Echo_Req

// int32 Field_HandleID = 1;
inline void C2GS_Test_Echo_Req::clear_field_handleid() {
  field_handleid_ = 0;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 C2GS_Test_Echo_Req::_internal_field_handleid() const {
  return field_handleid_;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 C2GS_Test_Echo_Req::field_handleid() const {
  // @@protoc_insertion_point(field_get:C2GS_Test_Echo_Req.Field_HandleID)
  return _internal_field_handleid();
}
inline void C2GS_Test_Echo_Req::_internal_set_field_handleid(::PROTOBUF_NAMESPACE_ID::int32 value) {
  
  field_handleid_ = value;
}
inline void C2GS_Test_Echo_Req::set_field_handleid(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _internal_set_field_handleid(value);
  // @@protoc_insertion_point(field_set:C2GS_Test_Echo_Req.Field_HandleID)
}

// string Field_Message = 2;
inline void C2GS_Test_Echo_Req::clear_field_message() {
  field_message_.ClearToEmptyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline const std::string& C2GS_Test_Echo_Req::field_message() const {
  // @@protoc_insertion_point(field_get:C2GS_Test_Echo_Req.Field_Message)
  return _internal_field_message();
}
inline void C2GS_Test_Echo_Req::set_field_message(const std::string& value) {
  _internal_set_field_message(value);
  // @@protoc_insertion_point(field_set:C2GS_Test_Echo_Req.Field_Message)
}
inline std::string* C2GS_Test_Echo_Req::mutable_field_message() {
  // @@protoc_insertion_point(field_mutable:C2GS_Test_Echo_Req.Field_Message)
  return _internal_mutable_field_message();
}
inline const std::string& C2GS_Test_Echo_Req::_internal_field_message() const {
  return field_message_.GetNoArena();
}
inline void C2GS_Test_Echo_Req::_internal_set_field_message(const std::string& value) {
  
  field_message_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), value);
}
inline void C2GS_Test_Echo_Req::set_field_message(std::string&& value) {
  
  field_message_.SetNoArena(
    &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:C2GS_Test_Echo_Req.Field_Message)
}
inline void C2GS_Test_Echo_Req::set_field_message(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  
  field_message_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:C2GS_Test_Echo_Req.Field_Message)
}
inline void C2GS_Test_Echo_Req::set_field_message(const char* value, size_t size) {
  
  field_message_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:C2GS_Test_Echo_Req.Field_Message)
}
inline std::string* C2GS_Test_Echo_Req::_internal_mutable_field_message() {
  
  return field_message_.MutableNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline std::string* C2GS_Test_Echo_Req::release_field_message() {
  // @@protoc_insertion_point(field_release:C2GS_Test_Echo_Req.Field_Message)
  
  return field_message_.ReleaseNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline void C2GS_Test_Echo_Req::set_allocated_field_message(std::string* field_message) {
  if (field_message != nullptr) {
    
  } else {
    
  }
  field_message_.SetAllocatedNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), field_message);
  // @@protoc_insertion_point(field_set_allocated:C2GS_Test_Echo_Req.Field_Message)
}

// -------------------------------------------------------------------

// C2GS_ON_LOGIN_REQ

// int32 Field_HandleID = 1;
inline void C2GS_ON_LOGIN_REQ::clear_field_handleid() {
  field_handleid_ = 0;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 C2GS_ON_LOGIN_REQ::_internal_field_handleid() const {
  return field_handleid_;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 C2GS_ON_LOGIN_REQ::field_handleid() const {
  // @@protoc_insertion_point(field_get:C2GS_ON_LOGIN_REQ.Field_HandleID)
  return _internal_field_handleid();
}
inline void C2GS_ON_LOGIN_REQ::_internal_set_field_handleid(::PROTOBUF_NAMESPACE_ID::int32 value) {
  
  field_handleid_ = value;
}
inline void C2GS_ON_LOGIN_REQ::set_field_handleid(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _internal_set_field_handleid(value);
  // @@protoc_insertion_point(field_set:C2GS_ON_LOGIN_REQ.Field_HandleID)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)


// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_c2gs_2eproto
