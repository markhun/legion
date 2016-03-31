/* Copyright 2016 Stanford University
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#ifndef __LEGION_C_H__
#define __LEGION_C_H__

/**
 * \file legion_c.h
 * Legion C API
 */

// ******************** IMPORTANT **************************
//
// This file is PURE C, **NOT** C++.
//
// ******************** IMPORTANT **************************

#include "legion_config.h"
#include "lowlevel_config.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

  // -----------------------------------------------------------------------
  // Proxy Types
  // -----------------------------------------------------------------------

// #define NEW_OPAQUE_TYPE(T) typedef void * T
#define NEW_OPAQUE_TYPE(T) typedef struct T { void *impl; } T
  NEW_OPAQUE_TYPE(legion_runtime_t);
  NEW_OPAQUE_TYPE(legion_context_t);
  NEW_OPAQUE_TYPE(legion_coloring_t);
  NEW_OPAQUE_TYPE(legion_domain_coloring_t);
  NEW_OPAQUE_TYPE(legion_domain_point_coloring_t);
  NEW_OPAQUE_TYPE(legion_multi_domain_point_coloring_t);
  NEW_OPAQUE_TYPE(legion_index_space_allocator_t);
  NEW_OPAQUE_TYPE(legion_field_allocator_t);
  NEW_OPAQUE_TYPE(legion_argument_map_t);
  NEW_OPAQUE_TYPE(legion_predicate_t);
  NEW_OPAQUE_TYPE(legion_future_t);
  NEW_OPAQUE_TYPE(legion_future_map_t);
  NEW_OPAQUE_TYPE(legion_task_launcher_t);
  NEW_OPAQUE_TYPE(legion_index_launcher_t);
  NEW_OPAQUE_TYPE(legion_inline_launcher_t);
  NEW_OPAQUE_TYPE(legion_copy_launcher_t);
  NEW_OPAQUE_TYPE(legion_must_epoch_launcher_t);
  NEW_OPAQUE_TYPE(legion_physical_region_t);
  NEW_OPAQUE_TYPE(legion_accessor_generic_t);
  NEW_OPAQUE_TYPE(legion_accessor_array_t);
  NEW_OPAQUE_TYPE(legion_index_iterator_t);
  NEW_OPAQUE_TYPE(legion_task_t);
  NEW_OPAQUE_TYPE(legion_inline_t);
  NEW_OPAQUE_TYPE(legion_mappable_t);
  NEW_OPAQUE_TYPE(legion_region_requirement_t);
  NEW_OPAQUE_TYPE(legion_machine_t);
  NEW_OPAQUE_TYPE(legion_mapper_t);
  NEW_OPAQUE_TYPE(legion_default_mapper_t);
  NEW_OPAQUE_TYPE(legion_processor_query_t);
  NEW_OPAQUE_TYPE(legion_memory_query_t);
  NEW_OPAQUE_TYPE(legion_machine_query_interface_t);
#undef NEW_OPAQUE_TYPE

  /**
   * @see ptr_t
   */
  typedef struct legion_ptr_t {
    unsigned value;
  } legion_ptr_t;

#ifdef POINTS_ARE_64BIT
typedef ptrdiff_t coord_t;
#else
typedef int coord_t;
#endif

#define NEW_POINT_TYPE(T, DIM) typedef struct T { coord_t x[DIM]; } T
  NEW_POINT_TYPE(legion_point_1d_t, 1);
  NEW_POINT_TYPE(legion_point_2d_t, 2);
  NEW_POINT_TYPE(legion_point_3d_t, 3);
#undef NEW_POINT_TYPE

#define NEW_RECT_TYPE(T, PT) typedef struct T { PT lo, hi; } T
  NEW_RECT_TYPE(legion_rect_1d_t, legion_point_1d_t);
  NEW_RECT_TYPE(legion_rect_2d_t, legion_point_2d_t);
  NEW_RECT_TYPE(legion_rect_3d_t, legion_point_3d_t);
#undef NEW_RECT_TYPE

#define NEW_BLOCKIFY_TYPE(T, PT) typedef struct T { PT block_size; } T
  NEW_BLOCKIFY_TYPE(legion_blockify_1d_t, legion_point_1d_t);
  NEW_BLOCKIFY_TYPE(legion_blockify_2d_t, legion_point_2d_t);
  NEW_BLOCKIFY_TYPE(legion_blockify_3d_t, legion_point_3d_t);
#undef NEW_BLOCKIFY_TYPE

  /**
   * @see LegionRuntime::HighLevel::Domain
   */
  typedef struct legion_domain_t {
    legion_lowlevel_id_t is_id;
    int dim;
    coord_t rect_data[2 * MAX_RECT_DIM];
  } legion_domain_t;

  /**
   * @see LegionRuntime::HighLevel::DomainPoint
   */
  typedef struct legion_domain_point_t {
    int dim;
    coord_t point_data[MAX_POINT_DIM];
  } legion_domain_point_t;

  /**
   * @see LegionRuntime::HighLevel::IndexSpace
   */
  typedef struct legion_index_space_t {
    legion_index_space_id_t id;
    legion_index_tree_id_t tid;
  } legion_index_space_t;

  /**
   * @see LegionRuntime::HighLevel::IndexPartition
   */
  typedef struct legion_index_partition_t {
    legion_index_partition_id_t id;
    legion_index_tree_id_t tid;
  } legion_index_partition_t;

  /**
   * @see LegionRuntime::HighLevel::IndexAllocator
   */
  typedef struct legion_index_allocator_t {
    legion_index_space_t index_space;
    legion_index_space_allocator_t allocator;
  } legion_index_allocator_t;

  /**
   * @see LegionRuntime::HighLevel::FieldSpace
   */
  typedef struct legion_field_space_t {
    legion_field_space_id_t id;
  } legion_field_space_t;

  /**
   * @see LegionRuntime::HighLevel::LogicalRegion
   */
  typedef struct legion_logical_region_t {
    legion_region_tree_id_t tree_id;
    legion_index_space_t index_space;
    legion_field_space_t field_space;
  } legion_logical_region_t;

  /**
   * @see LegionRuntime::HighLevel::LogicalPartition
   */
  typedef struct legion_logical_partition_t {
    legion_region_tree_id_t tree_id;
    legion_index_partition_t index_partition;
    legion_field_space_t field_space;
  } legion_logical_partition_t;

  /**
   * @see LegionRuntime::HighLevel::TaskArgument
   */
  typedef struct legion_task_argument_t {
    void *args;
    size_t arglen;
  } legion_task_argument_t;

  /**
   * @see LegionRuntime::HighLevel::TaskResult
   */
  typedef struct legion_task_result_t {
    void *value;
    size_t value_size;
  } legion_task_result_t;

  /**
   * @see LegionRuntime::Accessor::ByteOffset
   */
  typedef struct legion_byte_offset_t {
    int offset;
  } legion_byte_offset_t;

  /**
   * @see LegionRuntime::HighLevel::InputArgs
   */
  typedef struct legion_input_args_t {
    char **argv;
    int argc;
  } legion_input_args_t;

  /**
   * @see LegionRuntime::HighLevel::TaskConfigOptions
   */
  typedef struct legion_task_config_options_t {
    bool leaf /* = false */;
    bool inner /* = false */;
    bool idempotent /* = false */;
  }  legion_task_config_options_t;

  /**
   * @see LegionRuntime::HighLevel::Processor
   */
  typedef struct legion_processor_t {
    legion_lowlevel_id_t id;
  } legion_processor_t;

  /**
   * @see LegionRuntime::HighLevel::Memory
   */
  typedef struct legion_memory_t {
    legion_lowlevel_id_t id;
  } legion_memory_t;

  /**
   * @see LegionRuntime::HighLevel::Mapper::DomainSplit
   */
  typedef struct legion_domain_split_t {
    legion_domain_t domain;
    legion_processor_t proc;
    bool recurse;
    bool stealable;
  } legion_domain_split_t;

  /**
   * @see LegionRuntime::HighLevel::PhaseBarrier
   */
  typedef struct legion_phase_barrier_t {
    // From Realm::Event
    legion_lowlevel_id_t id;
    legion_lowlevel_event_gen_t gen;
    // From Realm::Barrier
    legion_lowlevel_barrier_timestamp_t timestamp;
  } legion_phase_barrier_t;

  /**
   * @see LegionRuntime::HighLevel::DynamicCollective
   */
  typedef struct legion_dynamic_collective_t {
    // From LegionRuntime::HighLevel::PhaseBarrier
    //   From Realm::Event
    legion_lowlevel_id_t id;
    legion_lowlevel_event_gen_t gen;
    //   From Realm::Barrier
    legion_lowlevel_barrier_timestamp_t timestamp;
    // From LegionRuntime::HighLevel::DynamicCollective
    legion_reduction_op_id_t redop;
  } legion_dynamic_collective_t;

  /**
   * Interface for a Legion C registration callback.
   */
  typedef
    void (*legion_registration_callback_pointer_t)(
      legion_machine_t /* machine */,
      legion_runtime_t /* runtime */,
      const legion_processor_t * /* local_procs */,
      unsigned /* num_local_procs */);

  /**
   * Interface for a Legion C task returning void.
   */
  typedef
    void (*legion_task_pointer_void_t)(
      const legion_task_t /* task */,
      const legion_physical_region_t * /* regions */,
      unsigned /* num_regions */,
      legion_context_t /* ctx */,
      legion_runtime_t /* runtime */);

  /**
   * Interface for a Legion C task returning an opaque, arbitrary size
   * result.
   */
  typedef
    legion_task_result_t (*legion_task_pointer_t)(
      const legion_task_t /* task */,
      const legion_physical_region_t * /* regions */,
      unsigned /* num_regions */,
      legion_context_t /* ctx */,
      legion_runtime_t /* runtime */);

  /**
   * Interface for a Legion C task returning an opaque 4-byte result.
   */
  typedef
    uint32_t (*legion_task_pointer_uint32_t)(
      const legion_task_t /* task */,
      const legion_physical_region_t * /* regions */,
      unsigned /* num_regions */,
      legion_context_t /* ctx */,
      legion_runtime_t /* runtime */);

  /**
   * Interface for a Legion C task returning an opaque 8-byte result.
   */
  typedef
    uint64_t (*legion_task_pointer_uint64_t)(
      const legion_task_t /* task */,
      const legion_physical_region_t * /* regions */,
      unsigned /* num_regions */,
      legion_context_t /* ctx */,
      legion_runtime_t /* runtime */);

  /**
   * Interface for a Legion C task that is wrapped (i.e. this is the Realm
   * task interface)
   */
  typedef legion_lowlevel_task_pointer_t legion_task_pointer_wrapped_t;

  /**
   * Interface for a Legion C projection functor (Logical Region
   * upper bound).
   */
  typedef
    legion_logical_region_t (*legion_projection_functor_logical_region_t)(
      legion_runtime_t /* runtime */,
      legion_context_t /* context */,
      legion_task_t /* task */,
      unsigned /* index */,
      legion_logical_region_t /* upper_bound */,
      legion_domain_point_t /* point */);

  /**
   * Interface for a Legion C projection functor (Logical Partition
   * upper bound).
   */
  typedef
    legion_logical_region_t (*legion_projection_functor_logical_partition_t)(
      legion_runtime_t /* runtime */,
      legion_context_t /* context */,
      legion_task_t /* task */,
      unsigned /* index */,
      legion_logical_partition_t /* upper_bound */,
      legion_domain_point_t /* point */);

  // -----------------------------------------------------------------------
  // Pointer Operations
  // -----------------------------------------------------------------------

  /**
   * @see ptr_t::nil()
   */
  inline legion_ptr_t
  legion_ptr_nil(void)
  {
    legion_ptr_t ptr;
    ptr.value = (unsigned)-1;
    return ptr;
  }

  /**
   * @see ptr_t::is_null()
   */
  inline bool
  legion_ptr_is_null(legion_ptr_t ptr)
  {
    return ptr.value == (unsigned)-1;
  }

  /**
   * @see LegionRuntime::HighLevel::Runtime::safe_cast(
   *        Context, ptr_t, LogicalRegion)
   */
  legion_ptr_t
  legion_ptr_safe_cast(legion_runtime_t runtime,
                       legion_context_t ctx,
                       legion_ptr_t pointer,
                       legion_logical_region_t region);

  // -----------------------------------------------------------------------
  // Domain Operations
  // -----------------------------------------------------------------------

  /**
   * @see LegionRuntime::HighLevel::Domain::from_rect()
   */
  legion_domain_t
  legion_domain_from_rect_1d(legion_rect_1d_t r);

  /**
   * @see LegionRuntime::HighLevel::Domain::from_rect()
   */
  legion_domain_t
  legion_domain_from_rect_2d(legion_rect_2d_t r);

  /**
   * @see LegionRuntime::HighLevel::Domain::from_rect()
   */
  legion_domain_t
  legion_domain_from_rect_3d(legion_rect_3d_t r);

  /**
   * @see LegionRuntime::HighLevel::Domain::get_rect()
   */
  legion_rect_1d_t
  legion_domain_get_rect_1d(legion_domain_t d);

  /**
   * @see LegionRuntime::HighLevel::Domain::get_rect()
   */
  legion_rect_2d_t
  legion_domain_get_rect_2d(legion_domain_t d);

  /**
   * @see LegionRuntime::HighLevel::Domain::get_rect()
   */
  legion_rect_3d_t
  legion_domain_get_rect_3d(legion_domain_t d);

  /**
   * @see LegionRuntime::HighLevel::Domain::get_volume()
   */
  size_t
  legion_domain_get_volume(legion_domain_t d);

  /**
   * @see LegionRuntime::HighLevel::Domain::Domain(
   *        LegionRuntime::HighLevel::IndexSpace)
   */
  legion_domain_t
  legion_domain_from_index_space(legion_runtime_t runtime_,
                                 legion_context_t ctx_,
                                 legion_index_space_t is);

  // -----------------------------------------------------------------------
  // Domain Point Operations
  // -----------------------------------------------------------------------

  /**
   * @see LegionRuntime::HighLevel::DomainPoint::from_point()
   */
  legion_domain_point_t
  legion_domain_point_from_point_1d(legion_point_1d_t p);

  /**
   * @see LegionRuntime::HighLevel::DomainPoint::from_point()
   */
  legion_domain_point_t
  legion_domain_point_from_point_2d(legion_point_2d_t p);

  /**
   * @see LegionRuntime::HighLevel::DomainPoint::from_point()
   */
  legion_domain_point_t
  legion_domain_point_from_point_3d(legion_point_3d_t p);

  /**
   * @see LegionRuntime::HighLevel::Runtime::safe_cast(
   *        Context, DomainPoint, LogicalRegion)
   */
  legion_domain_point_t
  legion_domain_point_safe_cast(legion_runtime_t runtime,
                                legion_context_t ctx,
                                legion_domain_point_t point,
                                legion_logical_region_t region);

  // -----------------------------------------------------------------------
  // Coloring Operations
  // -----------------------------------------------------------------------

  /**
   * @return Caller takes ownership of return value.
   *
   * @see LegionRuntime::HighLevel::Coloring
   */
  legion_coloring_t
  legion_coloring_create(void);

  /**
   * @param handle Caller must have ownership of parameter `handle`.
   *
   * @see LegionRuntime::HighLevel::Coloring
   */
  void
  legion_coloring_destroy(legion_coloring_t handle);

  /**
   * @see LegionRuntime::HighLevel::Coloring
   */
  void
  legion_coloring_ensure_color(legion_coloring_t handle,
                               legion_color_t color);

  /**
   * @see LegionRuntime::HighLevel::Coloring
   */
  void
  legion_coloring_add_point(legion_coloring_t handle,
                            legion_color_t color,
                            legion_ptr_t point);

  /**
   * @see LegionRuntime::HighLevel::Coloring
   */
  void
  legion_coloring_delete_point(legion_coloring_t handle,
                               legion_color_t color,
                               legion_ptr_t point);

  /**
   * @see LegionRuntime::HighLevel::Coloring
   */
  bool
  legion_coloring_has_point(legion_coloring_t handle,
                            legion_color_t color,
                            legion_ptr_t point);

  /**
   * @see LegionRuntime::HighLevel::Coloring
   */
  void
  legion_coloring_add_range(legion_coloring_t handle,
                            legion_color_t color,
                            legion_ptr_t start,
                            legion_ptr_t end /**< inclusive */);

  // -----------------------------------------------------------------------
  // Domain Coloring Operations
  // -----------------------------------------------------------------------

  /**
   * @return Caller takes ownership of return value.
   *
   * @see LegionRuntime::HighLevel::DomainColoring
   */
  legion_domain_coloring_t
  legion_domain_coloring_create(void);

  /**
   * @param handle Caller must have ownership of parameter `handle`.
   *
   * @see LegionRuntime::HighLevel::DomainColoring
   */
  void
  legion_domain_coloring_destroy(legion_domain_coloring_t handle);

  /**
   * @see LegionRuntime::HighLevel::DomainColoring
   */
  void
  legion_domain_coloring_color_domain(legion_domain_coloring_t handle,
                                      legion_color_t color,
                                      legion_domain_t domain);

  /**
   * @see LegionRuntime::HighLevel::DomainColoring
   */
  legion_domain_t
  legion_domain_coloring_get_color_space(legion_domain_coloring_t handle);

  // -----------------------------------------------------------------------
  // Domain Point Coloring Operations
  // -----------------------------------------------------------------------

  /**
   * @return Caller takes ownership of return value.
   *
   * @see LegionRuntime::HighLevel::DomainPointColoring
   */
  legion_domain_point_coloring_t
  legion_domain_point_coloring_create(void);

  /**
   * @param handle Caller must have ownership of parameter `handle`.
   *
   * @see LegionRuntime::HighLevel::DomainPointColoring
   */
  void
  legion_domain_point_coloring_destroy(
    legion_domain_point_coloring_t handle);

  /**
   * @see LegionRuntime::HighLevel::DomainPointColoring
   */
  void
  legion_domain_point_coloring_color_domain(
    legion_domain_point_coloring_t handle,
    legion_domain_point_t color,
    legion_domain_t domain);

  // -----------------------------------------------------------------------
  // Multi-Domain Point Coloring Operations
  // -----------------------------------------------------------------------

  /**
   * @return Caller takes ownership of return value.
   *
   * @see LegionRuntime::HighLevel::MultiDomainPointColoring
   */
  legion_multi_domain_point_coloring_t
  legion_multi_domain_point_coloring_create(void);

  /**
   * @param handle Caller must have ownership of parameter `handle`.
   *
   * @see LegionRuntime::HighLevel::MultiDomainPointColoring
   */
  void
  legion_multi_domain_point_coloring_destroy(
    legion_multi_domain_point_coloring_t handle);

  /**
   * @see LegionRuntime::HighLevel::MultiDomainPointColoring
   */
  void
  legion_multi_domain_point_coloring_color_domain(
    legion_multi_domain_point_coloring_t handle,
    legion_color_t color,
    legion_domain_t domain);

  // -----------------------------------------------------------------------
  // Index Space Operations
  // ----------------------------------------------------------------------

  /**
   * @return Caller takes ownership of return value.
   *
   * @see LegionRuntime::HighLevel::Runtime::create_index_space(Context, size_t)
   */
  legion_index_space_t
  legion_index_space_create(legion_runtime_t runtime,
                            legion_context_t ctx,
                            size_t max_num_elmts);

  /**
   * @return Caller takes ownership of return value.
   *
   * @see LegionRuntime::HighLevel::Runtime::create_index_space(Context, Domain)
   */
  legion_index_space_t
  legion_index_space_create_domain(legion_runtime_t runtime,
                                   legion_context_t ctx,
                                   legion_domain_t domain);

  /**
   * @param handle Caller must have ownership of parameter `handle`.
   *
   * @see LegionRuntime::HighLevel::Runtime::get_index_space_domain()
   */
  legion_domain_t
  legion_index_space_get_domain(legion_runtime_t runtime,
                                legion_context_t ctx,
                                legion_index_space_t handle);

  /**
   * @param handle Caller must have ownership of parameter `handle`.
   *
   * @see LegionRuntime::HighLevel::Runtime::destroy_index_space()
   */
  void
  legion_index_space_destroy(legion_runtime_t runtime,
                             legion_context_t ctx,
                             legion_index_space_t handle);

  /**
   * @see LegionRuntime::HighLevel::Runtime::attach_name()
   */
  void
  legion_index_space_attach_name(legion_runtime_t runtime,
                                 legion_index_space_t handle,
                                 const char *name,
                                 bool is_mutable /* = false */);

  /**
   * @see LegionRuntime::HighLevel::Runtime::retrieve_name()
   */
  void
  legion_index_space_retrieve_name(legion_runtime_t runtime,
                                   legion_index_space_t handle,
                                   const char **result);

  // -----------------------------------------------------------------------
  // Index Partition Operations
  // -----------------------------------------------------------------------

  /**
   * @return Caller takes ownership of return value.
   *
   * @see LegionRuntime::HighLevel::Runtime::create_index_partition(
   *        Context, IndexSpace, Coloring, bool, int)
   */
  legion_index_partition_t
  legion_index_partition_create_coloring(
    legion_runtime_t runtime,
    legion_context_t ctx,
    legion_index_space_t parent,
    legion_coloring_t coloring,
    bool disjoint,
    int part_color /* = AUTO_GENERATE_ID */);

  /**
   * @return Caller takes ownership of return value.
   *
   * @see LegionRuntime::HighLevel::Runtime::create_index_partition(
   *        Context, IndexSpace, Domain, DomainColoring, bool, int)
   */
  legion_index_partition_t
  legion_index_partition_create_domain_coloring(
    legion_runtime_t runtime,
    legion_context_t ctx,
    legion_index_space_t parent,
    legion_domain_t color_space,
    legion_domain_coloring_t coloring,
    bool disjoint,
    int part_color /* = AUTO_GENERATE_ID */);

  /**
   * @return Caller takes ownership of return value.
   *
   * @see LegionRuntime::HighLevel::Runtime::create_index_partition(
   *        Context, IndexSpace, Domain, DomainPointColoring, PartitionKind, int)
   */
  legion_index_partition_t
  legion_index_partition_create_domain_point_coloring(
    legion_runtime_t runtime,
    legion_context_t ctx,
    legion_index_space_t parent,
    legion_domain_t color_space,
    legion_domain_point_coloring_t coloring,
    legion_partition_kind_t part_kind /* = COMPUTE_KIND */,
    int color /* = AUTO_GENERATE_ID */);

  /**
   * @return Caller takes ownership of return value.
   *
   * @see LegionRuntime::HighLevel::Runtime::create_index_partition(
   *        Context, IndexSpace, Domain, MultiDomainPointColoring, PartitionKind, int)
   */
  legion_index_partition_t
  legion_index_partition_create_multi_domain_point_coloring(
    legion_runtime_t runtime,
    legion_context_t ctx,
    legion_index_space_t parent,
    legion_domain_t color_space,
    legion_multi_domain_point_coloring_t coloring,
    legion_partition_kind_t part_kind /* = COMPUTE_KIND */,
    int color /* = AUTO_GENERATE_ID */);

  /**
   * @return Caller takes ownership of return value.
   *
   * @see LegionRuntime::HighLevel::Runtime::create_index_partition<T>(
   *        Context, IndexSpace, const T&, int)
   */
  legion_index_partition_t
  legion_index_partition_create_blockify_1d(
    legion_runtime_t runtime,
    legion_context_t ctx,
    legion_index_space_t parent,
    legion_blockify_1d_t blockify,
    int part_color /* = AUTO_GENERATE_ID */);

  /**
   * @return Caller takes ownership of return value.
   *
   * @see LegionRuntime::HighLevel::Runtime::create_index_partition<T>(
   *        Context, IndexSpace, const T&, int)
   */
  legion_index_partition_t
  legion_index_partition_create_blockify_2d(
    legion_runtime_t runtime,
    legion_context_t ctx,
    legion_index_space_t parent,
    legion_blockify_2d_t blockify,
    int part_color /* = AUTO_GENERATE_ID */);

  /**
   * @return Caller takes ownership of return value.
   *
   * @see LegionRuntime::HighLevel::Runtime::create_index_partition<T>(
   *        Context, IndexSpace, const T&, int)
   */
  legion_index_partition_t
  legion_index_partition_create_blockify_3d(
    legion_runtime_t runtime,
    legion_context_t ctx,
    legion_index_space_t parent,
    legion_blockify_3d_t blockify,
    int part_color /* = AUTO_GENERATE_ID */);

  /**
   * @return Caller takes ownership of return value.
   *
   * @see LegionRuntime::HighLevel::Runtime::create_equal_partition()
   */
  legion_index_partition_t
  legion_index_partition_create_equal(legion_runtime_t runtime,
                                      legion_context_t ctx,
                                      legion_index_space_t parent,
                                      legion_domain_t color_space,
                                      size_t granularity,
                                      int color /* = AUTO_GENERATE_ID */,
                                      bool allocable /* = false */);

  /**
   * @return Caller takes ownership of return value.
   *
   * @see LegionRuntime::HighLevel::Runtime::create_partition_by_union()
   */
  legion_index_partition_t
  legion_index_partition_create_by_union(
    legion_runtime_t runtime,
    legion_context_t ctx,
    legion_index_space_t parent,
    legion_index_partition_t handle1,
    legion_index_partition_t handle2,
    legion_partition_kind_t part_kind /* = COMPUTE_KIND */,
    int color /* = AUTO_GENERATE_ID */,
    bool allocable /* = false */);

  /**
   * @return Caller takes ownership of return value.
   *
   * @see LegionRuntime::HighLevel::Runtime::create_partition_by_intersection()
   */
  legion_index_partition_t
  legion_index_partition_create_by_intersection(
    legion_runtime_t runtime,
    legion_context_t ctx,
    legion_index_space_t parent,
    legion_index_partition_t handle1,
    legion_index_partition_t handle2,
    legion_partition_kind_t part_kind /* = COMPUTE_KIND */,
    int color /* = AUTO_GENERATE_ID */,
    bool allocable /* = false */);

  /**
   * @return Caller takes ownership of return value.
   *
   * @see LegionRuntime::HighLevel::Runtime::create_partition_by_difference()
   */
  legion_index_partition_t
  legion_index_partition_create_by_difference(
    legion_runtime_t runtime,
    legion_context_t ctx,
    legion_index_space_t parent,
    legion_index_partition_t handle1,
    legion_index_partition_t handle2,
    legion_partition_kind_t part_kind /* = COMPUTE_KIND */,
    int color /* = AUTO_GENERATE_ID */,
    bool allocable /* = false */);

  /**
   * @return Caller takes ownership of return value.
   *
   * @see LegionRuntime::HighLevel::Runtime::create_partition_by_field()
   */
  legion_index_partition_t
  legion_index_partition_create_by_field(legion_runtime_t runtime,
                                         legion_context_t ctx,
                                         legion_logical_region_t handle,
                                         legion_logical_region_t parent,
                                         legion_field_id_t fid,
                                         legion_domain_t color_space,
                                         int color /* = AUTO_GENERATE_ID */,
                                         bool allocable /* = false */);

  /**
   * @return Caller takes ownership of return value.
   *
   * @see LegionRuntime::HighLevel::Runtime::create_partition_by_image()
   */
  legion_index_partition_t
  legion_index_partition_create_by_image(
    legion_runtime_t runtime,
    legion_context_t ctx,
    legion_index_space_t handle,
    legion_logical_partition_t projection,
    legion_logical_region_t parent,
    legion_field_id_t fid,
    legion_domain_t color_space,
    legion_partition_kind_t part_kind /* = COMPUTE_KIND */,
    int color /* = AUTO_GENERATE_ID */,
    bool allocable /* = false */);

  /**
   * @return Caller takes ownership of return value.
   *
   * @see LegionRuntime::HighLevel::Runtime::create_partition_by_preimage()
   */
  legion_index_partition_t
  legion_index_partition_create_by_preimage(
    legion_runtime_t runtime,
    legion_context_t ctx,
    legion_index_partition_t projection,
    legion_logical_region_t handle,
    legion_logical_region_t parent,
    legion_field_id_t fid,
    legion_domain_t color_space,
    legion_partition_kind_t part_kind /* = COMPUTE_KIND */,
    int color /* = AUTO_GENERATE_ID */,
    bool allocable /* = false */);

  /**
   * @return Caller does **NOT** take ownership of return value.
   *
   * @see LegionRuntime::HighLevel::Runtime::get_index_subspace()
   */
  legion_index_space_t
  legion_index_partition_get_index_subspace(legion_runtime_t runtime,
                                            legion_context_t ctx,
                                            legion_index_partition_t handle,
                                            legion_color_t color);

  /**
   * @return Caller does **NOT** take ownership of return value.
   *
   * @see LegionRuntime::HighLevel::Runtime::get_index_subspace()
   */
  legion_index_space_t
  legion_index_partition_get_index_subspace_domain_point(
    legion_runtime_t runtime,
    legion_context_t ctx,
    legion_index_partition_t handle,
    legion_domain_point_t color);

  /**
   * @see LegionRuntime::HighLevel::Runtime::has_index_subspace()
   */
  bool
  legion_index_partition_has_index_subspace_domain_point(
    legion_runtime_t runtime,
    legion_context_t ctx,
    legion_index_partition_t handle,
    legion_domain_point_t color);

  /**
   * @see LegionRuntime::HighLevel::Runtime::get_index_partition_color_space()
   */
  legion_domain_t
  legion_index_partition_get_color_space(legion_runtime_t runtime,
                                         legion_context_t ctx,
                                         legion_index_partition_t handle);

  /**
   * @see LegionRuntime::HighLevel::Runtime::get_index_partition_color()
   */
  legion_color_t
  legion_index_partition_get_color(legion_runtime_t runtime,
                                   legion_context_t ctx,
                                   legion_index_partition_t handle);

  /**
   * @return Caller does **NOT** take ownership of return value.
   *
   * @see LegionRuntime::HighLevel::Runtime::get_parent_index_space()
   */
  legion_index_space_t
  legion_index_partition_get_parent_index_space(legion_runtime_t runtime,
                                                legion_context_t ctx,
                                                legion_index_partition_t handle);

  /**
   * @param handle Caller must have ownership of parameter `handle`.
   *
   * @see LegionRuntime::HighLevel::Runtime::destroy_index_space()
   */
  void
  legion_index_partition_destroy(legion_runtime_t runtime,
                                legion_context_t ctx,
                                 legion_index_partition_t handle);

  /**
   * @see LegionRuntime::HighLevel::Runtime::attach_name()
   */
  void
  legion_index_partition_attach_name(legion_runtime_t runtime,
                                     legion_index_partition_t handle,
                                     const char *name,
                                     bool is_mutable /* = false */);

  /**
   * @see LegionRuntime::HighLevel::Runtime::retrieve_name()
   */
  void
  legion_index_partition_retrieve_name(legion_runtime_t runtime,
                                       legion_index_partition_t handle,
                                       const char **result);

  // -----------------------------------------------------------------------
  // Field Space Operations
  // -----------------------------------------------------------------------

  /**
   * @return Caller takes ownership of return value.
   *
   * @see LegionRuntime::HighLevel::Runtime::create_field_space()
   */
  legion_field_space_t
  legion_field_space_create(legion_runtime_t runtime,
                            legion_context_t ctx);

  /**
   * @param handle Caller must have ownership of parameter `handle`.
   *
   * @see LegionRuntime::HighLevel::Runtime::destroy_field_space()
   */
  void
  legion_field_space_destroy(legion_runtime_t runtime,
                             legion_context_t ctx,
                             legion_field_space_t handle);

  /**
   * @see LegionRuntime::HighLevel::Runtime::attach_name()
   */
  void
  legion_field_space_attach_name(legion_runtime_t runtime,
                                 legion_field_space_t handle,
                                 const char *name,
                                 bool is_mutable /* = false */);

  /**
   * @see LegionRuntime::HighLevel::Runtime::retrieve_name()
   */
  void
  legion_field_space_retrieve_name(legion_runtime_t runtime,
                                   legion_field_space_t handle,
                                   const char **result);

  /**
   * @see LegionRuntime::HighLevel::Runtime::retrieve_name()
   */
  void
  legion_field_id_attach_name(legion_runtime_t runtime,
                              legion_field_space_t handle,
                              legion_field_id_t id,
                              const char *name,
                              bool is_mutable /* = false */);

  /**
   * @see LegionRuntime::HighLevel::Runtime::attach_name()
   */
  void
  legion_field_id_retrieve_name(legion_runtime_t runtime,
                                legion_field_space_t handle,
                                legion_field_id_t id,
                                const char **result);

  // -----------------------------------------------------------------------
  // Logical Region Operations
  // -----------------------------------------------------------------------

  /**
   * @return Caller takes ownership of return value.
   *
   * @see LegionRuntime::HighLevel::Runtime::create_logical_region()
   */
  legion_logical_region_t
  legion_logical_region_create(legion_runtime_t runtime,
                               legion_context_t ctx,
                               legion_index_space_t index,
                               legion_field_space_t fields);

  /**
   * @param handle Caller must have ownership of parameter `handle`.
   *
   * @see LegionRuntime::HighLevel::Runtime::destroy_logical_region()
   */
  void
  legion_logical_region_destroy(legion_runtime_t runtime,
                                legion_context_t ctx,
                                legion_logical_region_t handle);

  /**
   * @see LegionRuntime::HighLevel::Runtime::get_logical_region_color()
   */
  legion_color_t
  legion_logical_region_get_color(legion_runtime_t runtime,
                                  legion_context_t ctx,
                                  legion_logical_region_t handle);

  /**
   * @see LegionRuntime::HighLevel::Runtime::has_parent_logical_partition()
   */
  bool
  legion_logical_region_has_parent_logical_partition(
    legion_runtime_t runtime,
    legion_context_t ctx,
    legion_logical_region_t handle);

  /**
   * @see LegionRuntime::HighLevel::Runtime::get_parent_logical_partition()
   */
  legion_logical_partition_t
  legion_logical_region_get_parent_logical_partition(
    legion_runtime_t runtime,
    legion_context_t ctx,
    legion_logical_region_t handle);

  /**
   * @see LegionRuntime::HighLevel::Runtime::attach_name()
   */
  void
  legion_logical_region_attach_name(legion_runtime_t runtime,
                                    legion_logical_region_t handle,
                                    const char *name,
                                    bool is_mutable /* = false */);

  /**
   * @see LegionRuntime::HighLevel::Runtime::retrieve_name()
   */
  void
  legion_logical_region_retrieve_name(legion_runtime_t runtime,
                                      legion_logical_region_t handle,
                                      const char **result);

  // -----------------------------------------------------------------------
  // Logical Region Tree Traversal Operations
  // -----------------------------------------------------------------------

  /**
   * @return Caller takes ownership of return value.
   *
   * @see LegionRuntime::HighLevel::Runtime::get_logical_partition()
   */
  legion_logical_partition_t
  legion_logical_partition_create(legion_runtime_t runtime,
                                  legion_context_t ctx,
                                  legion_logical_region_t parent,
                                  legion_index_partition_t handle);

  /**
   * @return Caller takes ownership of return value.
   *
   * @see LegionRuntime::HighLevel::Runtime::get_logical_partition_by_tree()
   */
  legion_logical_partition_t
  legion_logical_partition_create_by_tree(legion_runtime_t runtime,
                                          legion_context_t ctx,
                                          legion_index_partition_t handle,
                                          legion_field_space_t fspace,
                                          legion_region_tree_id_t tid);

  /**
   * @param handle Caller must have ownership of parameter `handle`.
   *
   * @see LegionRuntime::HighLevel::Runtime::destroy_logical_partition()
   */
  void
  legion_logical_partition_destroy(legion_runtime_t runtime,
                                   legion_context_t ctx,
                                   legion_logical_partition_t handle);

  /**
   * @return Caller does **NOT** take ownership of return value.
   *
   * @see LegionRuntime::HighLevel::Runtime::get_logical_subregion()
   */
  legion_logical_region_t
  legion_logical_partition_get_logical_subregion(
    legion_runtime_t runtime,
    legion_context_t ctx,
    legion_logical_partition_t parent,
    legion_index_space_t handle);

  /**
   * @return Caller does **NOT** take ownership of return value.
   *
   * @see LegionRuntime::HighLevel::Runtime::get_logical_subregion_by_color()
   */
  legion_logical_region_t
  legion_logical_partition_get_logical_subregion_by_color(
    legion_runtime_t runtime,
    legion_context_t ctx,
    legion_logical_partition_t parent,
    legion_color_t c);

  /**
   * @return Caller does **NOT** take ownership of return value.
   *
   * @see LegionRuntime::HighLevel::Runtime::get_logical_subregion_by_color()
   */
  legion_logical_region_t
  legion_logical_partition_get_logical_subregion_by_color_domain_point(
    legion_runtime_t runtime,
    legion_context_t ctx,
    legion_logical_partition_t parent,
    legion_domain_point_t c);

  /**
   * @see LegionRuntime::HighLevel::Runtime::has_logical_subregion_by_color()
   */
  bool
  legion_logical_partition_has_logical_subregion_by_color_domain_point(
    legion_runtime_t runtime,
    legion_context_t ctx,
    legion_logical_partition_t parent,
    legion_domain_point_t c);

  /**
   * @return Caller does **NOT** take ownership of return value.
   *
   * @see LegionRuntime::HighLevel::Runtime::get_logical_subregion_by_tree()
   */
  legion_logical_region_t
  legion_logical_partition_get_logical_subregion_by_tree(
    legion_runtime_t runtime,
    legion_context_t ctx,
    legion_index_space_t handle,
    legion_field_space_t fspace,
    legion_region_tree_id_t tid);

  /**
   * @see LegionRuntime::HighLevel::Runtime::get_parent_logical_region()
   */
  legion_logical_region_t
  legion_logical_partition_get_parent_logical_region(
    legion_runtime_t runtime,
    legion_context_t ctx,
    legion_logical_partition_t handle);

  /**
   * @see LegionRuntime::HighLevel::Runtime::attach_name()
   */
  void
  legion_logical_partition_attach_name(legion_runtime_t runtime,
                                       legion_logical_partition_t handle,
                                       const char *name,
                                       bool is_mutable /* = false */);

  /**
   * @see LegionRuntime::HighLevel::Runtime::retrieve_name()
   */
  void
  legion_logical_partition_retrieve_name(legion_runtime_t runtime,
                                         legion_logical_partition_t handle,
                                         const char **result);

  // -----------------------------------------------------------------------
  // Region Requirement Operations
  // -----------------------------------------------------------------------

  /**
   * @see LegionRuntime::HighLevel::RegionRequirement::region
   */
  legion_logical_region_t
  legion_region_requirement_get_region(legion_region_requirement_t handle);

  /**
   * @see LegionRuntime::HighLevel::RegionRequirement::parent
   */
  legion_logical_region_t
  legion_region_requirement_get_parent(legion_region_requirement_t handle);

  /**
   * @see LegionRuntime::HighLevel::RegionRequirement::partition
   */
  legion_logical_partition_t
  legion_region_requirement_get_partition(legion_region_requirement_t handle);

  /**
   * @see LegionRuntime::HighLevel::RegionRequirement::privilege_fields
   */
  unsigned
  legion_region_requirement_get_privilege_fields_size(
      legion_region_requirement_t handle);

  /**
   * @param fields Caller should give a buffer of the size fields_size
   *
   * @param fields_size the size of the buffer fields
   *
   * @return returns privilege fields in the region requirement.
   *         The return might be truncated if the buffer size is
   *         smaller than the number of privilege fields.
   *
   * @see LegionRuntime::HighLevel::RegionRequirement::privilege_fields
   */
  void
  legion_region_requirement_get_privilege_fields(
      legion_region_requirement_t handle,
      legion_field_id_t* fields,
      unsigned fields_size);

  /**
   * @return returns the i-th privilege field in the region requirement.
   *         note that this function takes O(n) time due to the underlying
   *         data structure does not provide an indexing operation.
   *
   * @see LegionRuntime::HighLevel::RegionRequirement::privilege_fields
   */
  legion_field_id_t
  legion_region_requirement_get_privilege_field(
      legion_region_requirement_t handle,
      unsigned idx);

  /**
   * @see LegionRuntime::HighLevel::RegionRequirement::instance_fields
   */
  unsigned
  legion_region_requirement_get_instance_fields_size(
      legion_region_requirement_t handle);

  /**
   * @param fields Caller should give a buffer of the size fields_size
   *
   * @param fields_size the size of the buffer fields
   *
   * @return returns instance fields in the region requirement.
   *         The return might be truncated if the buffer size is
   *         smaller than the number of instance fields.
   *
   * @see LegionRuntime::HighLevel::RegionRequirement::instance_fields
   */
  void
  legion_region_requirement_get_instance_fields(
      legion_region_requirement_t handle,
      legion_field_id_t* fields,
      unsigned fields_size);

  /**
   * @return returns the i-th instance field in the region requirement.
   *
   * @see LegionRuntime::HighLevel::RegionRequirement::instance_fields
   */
  legion_field_id_t
  legion_region_requirement_get_instance_field(
      legion_region_requirement_t handle,
      unsigned idx);

  /**
   * @see LegionRuntime::HighLevel::RegionRequirement::privilege
   */
  legion_privilege_mode_t
  legion_region_requirement_get_privilege(legion_region_requirement_t handle);

  /**
   * @see LegionRuntime::HighLevel::RegionRequirement::prop
   */
  legion_coherence_property_t
  legion_region_requirement_get_prop(legion_region_requirement_t handle);

  /**
   * @see LegionRuntime::HighLevel::RegionRequirement::redop
   */
  legion_reduction_op_id_t
  legion_region_requirement_get_redop(legion_region_requirement_t handle);

  /**
   * @see LegionRuntime::HighLevel::RegionRequirement::tag
   */
  legion_mapping_tag_id_t
  legion_region_requirement_get_tag(legion_region_requirement_t handle);

  /**
   * @see LegionRuntime::HighLevel::RegionRequirement::handle_type
   */
  legion_handle_type_t
  legion_region_requirement_get_handle_type(legion_region_requirement_t handle);

  /**
   * @see LegionRuntime::HighLevel::RegionRequirement::projection
   */
  legion_projection_id_t
  legion_region_requirement_get_projection(legion_region_requirement_t handle);

  /**
   * @see LegionRuntime::HighLevel::RegionRequirement::virtual_map
   */
  bool
  legion_region_requirement_get_virtual_map(legion_region_requirement_t handle);

  /**
   * @see LegionRuntime::HighLevel::RegionRequirement::virtual_map
   */
  void
  legion_region_requirement_set_virtual_map(
    legion_region_requirement_t handle,
    bool value);

  /**
   * @see LegionRuntime::HighLevel::RegionRequirement::early_map
   */
  bool
  legion_region_requirement_get_early_map(legion_region_requirement_t handle);

  /**
   * @see LegionRuntime::HighLevel::RegionRequirement::early_map
   */
  void
  legion_region_requirement_set_early_map(
    legion_region_requirement_t handle,
    bool value);

  /**
   * @see LegionRuntime::HighLevel::RegionRequirement::enable_WAR_optimization
   */
  bool
  legion_region_requirement_get_enable_WAR_optimization(
    legion_region_requirement_t handle);

  /**
   * @see LegionRuntime::HighLevel::RegionRequirement::enable_WAR_optimization
   */
  void
  legion_region_requirement_set_enable_WAR_optimization(
    legion_region_requirement_t handle,
    bool value);

  /**
   * @see LegionRuntime::HighLevel::RegionRequirement::reduction_list
   */
  bool
  legion_region_requirement_get_reduction_list(
    legion_region_requirement_t handle);

  /**
   * @see LegionRuntime::HighLevel::RegionRequirement::reduction_list
   */
  void
  legion_region_requirement_set_reduction_list(
    legion_region_requirement_t handle,
    bool value);

  /**
   * @see LegionRuntime::HighLevel::RegionRequirement::make_persistent
   */
  unsigned
  legion_region_requirement_get_make_persistent(
    legion_region_requirement_t handle);

  /**
   * @see LegionRuntime::HighLevel::RegionRequirement::make_persistent
   */
  void
  legion_region_requirement_set_make_persistent(
    legion_region_requirement_t handle,
    unsigned value);

  /**
   * @see LegionRuntime::HighLevel::RegionRequirement::blocking_factor
   */
  unsigned
  legion_region_requirement_get_blocking_factor(
    legion_region_requirement_t handle);

  /**
   * @see LegionRuntime::HighLevel::RegionRequirement::blocking_factor
   */
  void
  legion_region_requirement_set_blocking_factor(
    legion_region_requirement_t handle,
    unsigned value);

  /**
   * @see LegionRuntime::HighLevel::RegionRequirement::max_blocking_factor
   */
  unsigned
  legion_region_requirement_get_max_blocking_factor(
    legion_region_requirement_t handle);

  /**
   * @see LegionRuntime::HighLevel::RegionRequirement::target_ranking
   */
  void
  legion_region_requirement_add_target_ranking(
    legion_region_requirement_t handle,
    legion_memory_t memory);

  // -----------------------------------------------------------------------
  // Allocator and Argument Map Operations
  // -----------------------------------------------------------------------

  /**
   * @return Caller takes ownership of return value.
   *
   * @see LegionRuntime::HighLevel::Runtime::create_index_allocator()
   */
  legion_index_allocator_t
  legion_index_allocator_create(legion_runtime_t runtime,
                                legion_context_t ctx,
                                legion_index_space_t handle);

  /**
   * @param handle Caller must have ownership of parameter `handle`.
   *
   * @see LegionRuntime::HighLevel::IndexAllocator::~IndexAllocator()
   */
  void
  legion_index_allocator_destroy(legion_index_allocator_t handle);

  /**
   * @see LegionRuntime::HighLevel::IndexAllocator::alloc()
   */
  legion_ptr_t
  legion_index_allocator_alloc(legion_index_allocator_t allocator,
                               unsigned num_elements /* = 1 */);

  /**
   * @see LegionRuntime::HighLevel::IndexAllocator::free()
   */
  void
  legion_index_allocator_free(legion_index_allocator_t allocator,
                              legion_ptr_t ptr,
                              unsigned num_elements /* = 1 */);

  /**
   * @return Caller takes ownership of return value.
   *
   * @see LegionRuntime::HighLevel::Runtime::create_field_allocator()
   */
  legion_field_allocator_t
  legion_field_allocator_create(legion_runtime_t runtime,
                                legion_context_t ctx,
                                legion_field_space_t handle);

  /**
   * @param handle Caller must have ownership of parameter `handle`.
   *
   * @see LegionRuntime::HighLevel::FieldAllocator::~FieldAllocator()
   */
  void
  legion_field_allocator_destroy(legion_field_allocator_t handle);

  /**
   * @see LegionRuntime::HighLevel::FieldAllocator::allocate_field()
   */
  legion_field_id_t
  legion_field_allocator_allocate_field(
    legion_field_allocator_t allocator,
    size_t field_size,
    legion_field_id_t desired_fieldid /* = AUTO_GENERATE_ID */);

  /**
   * @see LegionRuntime::HighLevel::FieldAllocator::free_field()
   */
  void
  legion_field_allocator_free_field(legion_field_allocator_t allocator,
                                    legion_field_id_t fid);

  /**
   * @see LegionRuntime::HighLevel::FieldAllocator::allocate_local_field()
   */
  legion_field_id_t
  legion_field_allocator_allocate_local_field(
    legion_field_allocator_t allocator,
    size_t field_size,
    legion_field_id_t desired_fieldid /* = AUTO_GENERATE_ID */);

  /**
   * @return Caller takes ownership of return value.
   *
   * @see LegionRuntime::HighLevel::ArgumentMap::ArgumentMap()
   */
  legion_argument_map_t
  legion_argument_map_create();

  /**
   * @see LegionRuntime::HighLevel::ArgumentMap::set_point()
   */
  void
  legion_argument_map_set_point(legion_argument_map_t map,
                                legion_domain_point_t dp,
                                legion_task_argument_t arg,
                                bool replace /* = true */);

  /**
   * @param handle Caller must have ownership of parameter `handle`.
   *
   * @see LegionRuntime::HighLevel::ArgumentMap::~ArgumentMap()
   */
  void
  legion_argument_map_destroy(legion_argument_map_t handle);

  // -----------------------------------------------------------------------
  // Predicate Operations
  // -----------------------------------------------------------------------

  /**
   * @param handle Caller must have ownership of parameter `handle`.
   *
   * @see LegionRuntime::HighLevel::Predicate::~Predicate()
   */
  void
  legion_predicate_destroy(legion_predicate_t handle);

  /**
   * @return Caller does **NOT** take ownership of return value.
   *
   * @see LegionRuntime::HighLevel::Predicate::TRUE_PRED
   */
  const legion_predicate_t
  legion_predicate_true(void);

  /**
   * @return Caller does **NOT** take ownership of return value.
   *
   * @see LegionRuntime::HighLevel::Predicate::FALSE_PRED
   */
  const legion_predicate_t
  legion_predicate_false(void);

  // -----------------------------------------------------------------------
  // Phase Barrier Operations
  // -----------------------------------------------------------------------

  /**
   * @return Caller takes ownership of return value.
   *
   * @see LegionRuntime::HighLevel::HighLevelRuntime::create_phase_barrier()
   */
  legion_phase_barrier_t
  legion_phase_barrier_create(legion_runtime_t runtime,
                              legion_context_t ctx,
                              unsigned arrivals);

  /**
   * @param handle Caller must have ownership of parameter `handle`.
   *
   * @see LegionRuntime::HighLevel::PhaseBarrier::destroy_phase_barrier()
   */
  void
  legion_phase_barrier_destroy(legion_runtime_t runtime,
                               legion_context_t ctx,
                               legion_phase_barrier_t handle);

  /**
   * @return Caller does **NOT** take ownership of return value.
   *
   * @see LegionRuntime::HighLevel::HighLevelRuntime::advance_phase_barrier()
   */
  legion_phase_barrier_t
  legion_phase_barrier_advance(legion_runtime_t runtime,
                               legion_context_t ctx,
                               legion_phase_barrier_t handle);

  // -----------------------------------------------------------------------
  // Dynamic Collective Operations
  // -----------------------------------------------------------------------

  /**
   * @return Caller takes ownership of return value.
   *
   * @see LegionRuntime::HighLevel::HighLevelRuntime::create_dynamic_collective()
   */
  legion_dynamic_collective_t
  legion_dynamic_collective_create(legion_runtime_t runtime,
                                   legion_context_t ctx,
                                   unsigned arrivals,
                                   legion_reduction_op_id_t redop,
                                   const void *init_value,
                                   size_t init_size);

  /**
   * @param handle Caller must have ownership of parameter `handle`.
   *
   * @see LegionRuntime::HighLevel::PhaseBarrier::destroy_dynamic_collective()
   */
  void
  legion_dynamic_collective_destroy(legion_runtime_t runtime,
                                    legion_context_t ctx,
                                    legion_dynamic_collective_t handle);

  /**
   * @see LegionRuntime::HighLevel::HighLevelRuntime::arrive_dynamic_collective()
   */
  void
  legion_dynamic_collective_arrive(legion_runtime_t runtime,
                                   legion_context_t ctx,
                                   legion_dynamic_collective_t handle,
                                   const void *buffer,
                                   size_t size,
                                   unsigned count /* = 1 */);

  /**
   * @see LegionRuntime::HighLevel::HighLevelRuntime::defer_dynamic_collective_arrival()
   */
  void
  legion_dynamic_collective_defer_arrival(legion_runtime_t runtime,
                                          legion_context_t ctx,
                                          legion_dynamic_collective_t handle,
                                          legion_future_t f,
                                          unsigned count /* = 1 */);

  /**
   * @return Caller takes ownership of return value.
   *
   * @see LegionRuntime::HighLevel::HighLevelRuntime::get_dynamic_collective_result()
   */
  legion_future_t
  legion_dynamic_collective_get_result(legion_runtime_t runtime,
                                       legion_context_t ctx,
                                       legion_dynamic_collective_t handle);

  /**
   * @return Caller does **NOT** take ownership of return value.
   *
   * @see LegionRuntime::HighLevel::HighLevelRuntime::advance_dynamic_collective()
   */
  legion_dynamic_collective_t
  legion_dynamic_collective_advance(legion_runtime_t runtime,
                                    legion_context_t ctx,
                                    legion_dynamic_collective_t handle);

  // -----------------------------------------------------------------------
  // Future Operations
  // -----------------------------------------------------------------------

  /**
   * @param buffer Makes a copy of parameter `buffer`.
   *
   * @return Caller takes ownership of return value.
   *
   * @see LegionRuntime::HighLevel::Future::from_value()
   */
  legion_future_t
  legion_future_from_buffer(legion_runtime_t runtime,
                            const void *buffer,
                            size_t size);

  /**
   * @return Caller takes ownership of return value.
   *
   * @see LegionRuntime::HighLevel::Future::from_value()
   */
  legion_future_t
  legion_future_from_uint32(legion_runtime_t runtime,
                            uint32_t value);

  /**
   * @return Caller takes ownership of return value.
   *
   * @see LegionRuntime::HighLevel::Future::from_value()
   */
  legion_future_t
  legion_future_from_uint64(legion_runtime_t runtime,
                            uint64_t value);

  /**
   * @return Caller takes ownership of return value.
   *
   * @see LegionRuntime::HighLevel::Future::from_value()
   */
  legion_future_t
  legion_future_from_bytes(legion_runtime_t runtime,
			   const void *buffer,
			   size_t size);

  /**
   * @param handle Caller must have ownership of parameter `handle`.
   *
   * @see LegionRuntime::HighLevel::Future::~Future()
   */
  void
  legion_future_destroy(legion_future_t handle);

  /**
   * @see LegionRuntime::HighLevel::Future::get_void_result()
   */
  void
  legion_future_get_void_result(legion_future_t handle);

  /**
   * @return Caller takes ownership of return value.
   *
   * @see LegionRuntime::HighLevel::Future::get_result()
   */
  legion_task_result_t
  legion_future_get_result(legion_future_t handle);

  /**
   * @see LegionRuntime::HighLevel::Future::get_result()
   */
  uint32_t
  legion_future_get_result_uint32(legion_future_t handle);

  /**
   * @see LegionRuntime::HighLevel::Future::get_result()
   */
  uint64_t
  legion_future_get_result_uint64(legion_future_t handle);

  /**
   * @see LegionRuntime::HighLevel::Future::get_untyped_pointer()
   */
  void
  legion_future_get_result_bytes(legion_future_t handle_, void *buffer, size_t size);

  /**
   * @see LegionRuntime::HighLevel::Future::is_empty()
   */
  bool
  legion_future_is_empty(legion_future_t handle,
                         bool block /* = false */);

  /**
   * @see LegionRuntime::HighLevel::Future::get_untyped_pointer()
   */
  const void *
  legion_future_get_untyped_pointer(legion_future_t handle);

  /**
   * @return Caller takes ownership of return value.
   *
   * @see LegionRuntime::HighLevel::Runtime::execute_task()
   */
  legion_task_result_t
  legion_task_result_create(const void *handle, size_t size);

  /**
   * @param handle Caller must have ownership of parameter `handle`.
   *
   * @see LegionRuntime::HighLevel::Runtime::execute_task()
   */
  void
  legion_task_result_destroy(legion_task_result_t handle);

  // -----------------------------------------------------------------------
  // Future Map Operations
  // -----------------------------------------------------------------------

  /**
   * @param handle Caller must have ownership of parameter `handle`.
   *
   * @see LegionRuntime::HighLevel::FutureMap::~FutureMap()
   */
  void
  legion_future_map_destroy(legion_future_map_t handle);

  /**
   * @see LegionRuntime::HighLevel::FutureMap::wait_all_results()
   */
  void
  legion_future_map_wait_all_results(legion_future_map_t handle);

  /**
   * @return Caller takes ownership of return value.
   *
   * @see LegionRuntime::HighLevel::Future::get_future()
   */
  legion_future_t
  legion_future_map_get_future(legion_future_map_t handle,
                               legion_domain_point_t point);

  /**
   * @return Caller takes ownership of return value.
   *
   * @see LegionRuntime::HighLevel::Future::get_result()
   */
  legion_task_result_t
  legion_future_map_get_result(legion_future_map_t handle,
                               legion_domain_point_t point);

  // -----------------------------------------------------------------------
  // Task Launch Operations
  // -----------------------------------------------------------------------

  /**
   * @return Caller takes ownership of return value.
   *
   * @see LegionRuntime::HighLevel::TaskLauncher::TaskLauncher()
   */
  legion_task_launcher_t
  legion_task_launcher_create(
    legion_task_id_t tid,
    legion_task_argument_t arg,
    legion_predicate_t pred /* = legion_predicate_true() */,
    legion_mapper_id_t id /* = 0 */,
    legion_mapping_tag_id_t tag /* = 0 */);

  /**
   * @param handle Caller must have ownership of parameter `handle`.
   *
   * @see LegionRuntime::HighLevel::TaskLauncher::~TaskLauncher()
   */
  void
  legion_task_launcher_destroy(legion_task_launcher_t handle);

  /**
   * @return Caller takes ownership of return value.
   *
   * @see LegionRuntime::HighLevel::Runtime::execute_task()
   */
  legion_future_t
  legion_task_launcher_execute(legion_runtime_t runtime,
                               legion_context_t ctx,
                               legion_task_launcher_t launcher);

  /**
   * @see LegionRuntime::HighLevel::TaskLauncher::add_region_requirement()
   */
  unsigned
  legion_task_launcher_add_region_requirement_logical_region(
    legion_task_launcher_t launcher,
    legion_logical_region_t handle,
    legion_privilege_mode_t priv,
    legion_coherence_property_t prop,
    legion_logical_region_t parent,
    legion_mapping_tag_id_t tag /* = 0 */,
    bool verified /* = false*/);

  /**
   * @see LegionRuntime::HighLevel::TaskLauncher::add_region_requirement()
   */
  unsigned
  legion_task_launcher_add_region_requirement_logical_region_reduction(
    legion_task_launcher_t launcher,
    legion_logical_region_t handle,
    legion_reduction_op_id_t redop,
    legion_coherence_property_t prop,
    legion_logical_region_t parent,
    legion_mapping_tag_id_t tag /* = 0 */,
    bool verified /* = false*/);

  /**
   * @see LegionRuntime::HighLevel::TaskLauncher::add_field()
   */
  void
  legion_task_launcher_add_field(legion_task_launcher_t launcher,
                                 unsigned idx,
                                 legion_field_id_t fid,
                                 bool inst /* = true */);

  /**
   * @see LegionRuntime::HighLevel::RegionRequirement::add_flags()
   */
  void
  legion_task_launcher_add_flags(legion_task_launcher_t launcher,
                                 unsigned idx,
                                 enum legion_region_flags_t flags);

  /**
   * @see LegionRuntime::HighLevel::RegionRequirement::flags
   */
  void
  legion_task_launcher_intersect_flags(legion_task_launcher_t launcher,
                                       unsigned idx,
                                       enum legion_region_flags_t flags);

  /**
   * @see LegionRuntime::HighLevel::TaskLauncher::add_index_requirement()
   */
  unsigned
  legion_task_launcher_add_index_requirement(
    legion_task_launcher_t launcher,
    legion_index_space_t handle,
    legion_allocate_mode_t priv,
    legion_index_space_t parent,
    bool verified /* = false*/);

  /**
   * @see LegionRuntime::HighLevel::TaskLauncher::add_future()
   */
  void
  legion_task_launcher_add_future(legion_task_launcher_t launcher,
                                  legion_future_t future);

  /**
   * @see LegionRuntime::HighLevel::TaskLauncher::add_wait_barrier()
   */
  void
  legion_task_launcher_add_wait_barrier(legion_task_launcher_t launcher,
                                        legion_phase_barrier_t bar);

  /**
   * @see LegionRuntime::HighLevel::TaskLauncher::add_arrival_barrier()
   */
  void
  legion_task_launcher_add_arrival_barrier(legion_task_launcher_t launcher,
                                           legion_phase_barrier_t bar);

  /**
   * @return Caller takes ownership of return value.
   *
   * @see LegionRuntime::HighLevel::IndexLauncher::IndexLauncher()
   */
  legion_index_launcher_t
  legion_index_launcher_create(
    legion_task_id_t tid,
    legion_domain_t domain,
    legion_task_argument_t global_arg,
    legion_argument_map_t map,
    legion_predicate_t pred /* = legion_predicate_true() */,
    bool must /* = false */,
    legion_mapper_id_t id /* = 0 */,
    legion_mapping_tag_id_t tag /* = 0 */);

  /**
   * @param handle Caller must have ownership of parameter `handle`.
   *
   * @see LegionRuntime::HighLevel::IndexLauncher::~IndexLauncher()
   */
  void
  legion_index_launcher_destroy(legion_index_launcher_t handle);

  /**
   * @return Caller takes ownership of return value.
   *
   * @see LegionRuntime::HighLevel::Runtime::execute_index_space(Context, const IndexLauncher &)
   */
  legion_future_map_t
  legion_index_launcher_execute(legion_runtime_t runtime,
                               legion_context_t ctx,
                               legion_index_launcher_t launcher);

  /**
   * @return Caller takes ownership of return value.
   *
   * @see LegionRuntime::HighLevel::Runtime::execute_index_space(Context, const IndexLauncher &, ReductionOpID)
   */
  legion_future_t
  legion_index_launcher_execute_reduction(legion_runtime_t runtime,
                                          legion_context_t ctx,
                                          legion_index_launcher_t launcher,
                                          legion_reduction_op_id_t redop);

  /**
   * @see LegionRuntime::HighLevel::IndexLauncher::add_region_requirement()
   */
  unsigned
  legion_index_launcher_add_region_requirement_logical_region(
    legion_index_launcher_t launcher,
    legion_logical_region_t handle,
    legion_projection_id_t proj /* = 0 */,
    legion_privilege_mode_t priv,
    legion_coherence_property_t prop,
    legion_logical_region_t parent,
    legion_mapping_tag_id_t tag /* = 0 */,
    bool verified /* = false*/);

  /**
   * @see LegionRuntime::HighLevel::IndexLauncher::add_region_requirement()
   */
  unsigned
  legion_index_launcher_add_region_requirement_logical_partition(
    legion_index_launcher_t launcher,
    legion_logical_partition_t handle,
    legion_projection_id_t proj /* = 0 */,
    legion_privilege_mode_t priv,
    legion_coherence_property_t prop,
    legion_logical_region_t parent,
    legion_mapping_tag_id_t tag /* = 0 */,
    bool verified /* = false*/);

  /**
   * @see LegionRuntime::HighLevel::IndexLauncher::add_region_requirement()
   */
  unsigned
  legion_index_launcher_add_region_requirement_logical_region_reduction(
    legion_index_launcher_t launcher,
    legion_logical_region_t handle,
    legion_projection_id_t proj /* = 0 */,
    legion_reduction_op_id_t redop,
    legion_coherence_property_t prop,
    legion_logical_region_t parent,
    legion_mapping_tag_id_t tag /* = 0 */,
    bool verified /* = false*/);

  /**
   * @see LegionRuntime::HighLevel::IndexLauncher::add_region_requirement()
   */
  unsigned
  legion_index_launcher_add_region_requirement_logical_partition_reduction(
    legion_index_launcher_t launcher,
    legion_logical_partition_t handle,
    legion_projection_id_t proj /* = 0 */,
    legion_reduction_op_id_t redop,
    legion_coherence_property_t prop,
    legion_logical_region_t parent,
    legion_mapping_tag_id_t tag /* = 0 */,
    bool verified /* = false*/);

  /**
   * @see LegionRuntime::HighLevel::IndexLaunchxer::add_field()
   */
  void
  legion_index_launcher_add_field(legion_index_launcher_t launcher,
                                 unsigned idx,
                                 legion_field_id_t fid,
                                 bool inst /* = true */);

  /**
   * @see LegionRuntime::HighLevel::RegionRequirement::add_flags()
   */
  void
  legion_index_launcher_add_flags(legion_index_launcher_t launcher,
                                  unsigned idx,
                                  enum legion_region_flags_t flags);

  /**
   * @see LegionRuntime::HighLevel::RegionRequirement::flags
   */
  void
  legion_index_launcher_intersect_flags(legion_index_launcher_t launcher,
                                        unsigned idx,
                                        enum legion_region_flags_t flags);

  /**
   * @see LegionRuntime::HighLevel::IndexLauncher::add_index_requirement()
   */
  unsigned
  legion_index_launcher_add_index_requirement(
    legion_index_launcher_t launcher,
    legion_index_space_t handle,
    legion_allocate_mode_t priv,
    legion_index_space_t parent,
    bool verified /* = false*/);

  /**
   * @see LegionRuntime::HighLevel::IndexLauncher::add_future()
   */
  void
  legion_index_launcher_add_future(legion_index_launcher_t launcher,
                                   legion_future_t future);

  /**
   * @see LegionRuntime::HighLevel::IndexLauncher::add_wait_barrier()
   */
  void
  legion_index_launcher_add_wait_barrier(legion_index_launcher_t launcher,
                                         legion_phase_barrier_t bar);

  /**
   * @see LegionRuntime::HighLevel::IndexLauncher::add_arrival_barrier()
   */
  void
  legion_index_launcher_add_arrival_barrier(legion_index_launcher_t launcher,
                                            legion_phase_barrier_t bar);

  // -----------------------------------------------------------------------
  // Inline Mapping Operations
  // -----------------------------------------------------------------------

  /**
   * @return Caller takes ownership of return value.
   *
   * @see LegionRuntime::HighLevel::InlineLauncher::InlineLauncher()
   */
  legion_inline_launcher_t
  legion_inline_launcher_create_logical_region(
    legion_logical_region_t handle,
    legion_privilege_mode_t priv,
    legion_coherence_property_t prop,
    legion_logical_region_t parent,
    legion_mapping_tag_id_t region_tag /* = 0 */,
    bool verified /* = false*/,
    legion_mapper_id_t id /* = 0 */,
    legion_mapping_tag_id_t launcher_tag /* = 0 */);

  /**
   * @param handle Caller must have ownership of parameter `handle`.
   *
   * @see LegionRuntime::HighLevel::InlineLauncher::~InlineLauncher()
   */
  void
  legion_inline_launcher_destroy(legion_inline_launcher_t handle);

  /**
   * @return Caller takes ownership of return value.
   *
   * @see LegionRuntime::HighLevel::Runtime::map_region()
   */
  legion_physical_region_t
  legion_inline_launcher_execute(legion_runtime_t runtime,
                                 legion_context_t ctx,
                                 legion_inline_launcher_t launcher);

  /**
   * @see LegionRuntime::HighLevel::InlineLauncher::add_field()
   */
  void
  legion_inline_launcher_add_field(legion_inline_launcher_t launcher,
                                   legion_field_id_t fid,
                                   bool inst /* = true */);

  /**
   * @see LegionRuntime::HighLevel::Runtime::remap_region()
   */
  void
  legion_runtime_remap_region(legion_runtime_t runtime,
                              legion_context_t ctx,
                              legion_physical_region_t region);

  /**
   * @see LegionRuntime::HighLevel::Runtime::unmap_region()
   */
  void
  legion_runtime_unmap_region(legion_runtime_t runtime,
                              legion_context_t ctx,
                              legion_physical_region_t region);

  /**
   * @see LegionRuntime::HighLevel::Runtime::unmap_all_regions()
   */
  void
  legion_runtime_unmap_all_regions(legion_runtime_t runtime,
                                   legion_context_t ctx);

  /**
   * @see LegionRuntime::HighLevel::Runtime::fill_field()
   */
  void
  legion_runtime_fill_field(
    legion_runtime_t runtime,
    legion_context_t ctx,
    legion_logical_region_t handle,
    legion_logical_region_t parent,
    legion_field_id_t fid,
    const void *value,
    size_t value_size,
    legion_predicate_t pred /* = legion_predicate_true() */);

  /**
   * @see LegionRuntime::HighLevel::Runtime::fill_field()
   */
  void
  legion_runtime_fill_field_future(
    legion_runtime_t runtime,
    legion_context_t ctx,
    legion_logical_region_t handle,
    legion_logical_region_t parent,
    legion_field_id_t fid,
    legion_future_t f,
    legion_predicate_t pred /* = legion_predicate_true() */);

  // -----------------------------------------------------------------------
  // Copy Operations
  // -----------------------------------------------------------------------

  /**
   * @return Caller takes ownership of return value.
   *
   * @see LegionRuntime::HighLevel::CopyLauncher::CopyLauncher()
   */
  legion_copy_launcher_t
  legion_copy_launcher_create(
    legion_predicate_t pred /* = legion_predicate_true() */,
    legion_mapper_id_t id /* = 0 */,
    legion_mapping_tag_id_t launcher_tag /* = 0 */);

  /**
   * @param handle Caller must have ownership of parameter `handle`.
   *
   * @see LegionRuntime::HighLevel::CopyLauncher::~CopyLauncher()
   */
  void
  legion_copy_launcher_destroy(legion_copy_launcher_t handle);

  /**
   * @return Caller takes ownership of return value.
   *
   * @see LegionRuntime::HighLevel::Runtime::issue_copy_operation()
   */
  void
  legion_copy_launcher_execute(legion_runtime_t runtime,
                               legion_context_t ctx,
                               legion_copy_launcher_t launcher);

  /**
   * @see LegionRuntime::HighLevel::CopyLauncher::add_copy_requirements()
   */
  unsigned
  legion_copy_launcher_add_src_region_requirement_logical_region(
    legion_copy_launcher_t launcher,
    legion_logical_region_t handle,
    legion_privilege_mode_t priv,
    legion_coherence_property_t prop,
    legion_logical_region_t parent,
    legion_mapping_tag_id_t tag /* = 0 */,
    bool verified /* = false*/);

  /**
   * @see LegionRuntime::HighLevel::CopyLauncher::add_copy_requirements()
   */
  unsigned
  legion_copy_launcher_add_dst_region_requirement_logical_region(
    legion_copy_launcher_t launcher,
    legion_logical_region_t handle,
    legion_privilege_mode_t priv,
    legion_coherence_property_t prop,
    legion_logical_region_t parent,
    legion_mapping_tag_id_t tag /* = 0 */,
    bool verified /* = false*/);

  /**
   * @see LegionRuntime::HighLevel::CopyLauncher::add_region_requirement()
   */
  unsigned
  legion_copy_launcher_add_dst_region_requirement_logical_region_reduction(
    legion_copy_launcher_t launcher,
    legion_logical_region_t handle,
    legion_reduction_op_id_t redop,
    legion_coherence_property_t prop,
    legion_logical_region_t parent,
    legion_mapping_tag_id_t tag /* = 0 */,
    bool verified /* = false*/);

  /**
   * @see LegionRuntime::HighLevel::CopyLauncher::add_field()
   */
  void
  legion_copy_launcher_add_src_field(legion_copy_launcher_t launcher,
                                     unsigned idx,
                                     legion_field_id_t fid,
                                     bool inst /* = true */);

  /**
   * @see LegionRuntime::HighLevel::CopyLauncher::add_field()
   */
  void
  legion_copy_launcher_add_dst_field(legion_copy_launcher_t launcher,
                                     unsigned idx,
                                     legion_field_id_t fid,
                                     bool inst /* = true */);

  /**
   * @see LegionRuntime::HighLevel::CopyLauncher::add_wait_barrier()
   */
  void
  legion_copy_launcher_add_wait_barrier(legion_copy_launcher_t launcher,
                                        legion_phase_barrier_t bar);

  /**
   * @see LegionRuntime::HighLevel::CopyLauncher::add_arrival_barrier()
   */
  void
  legion_copy_launcher_add_arrival_barrier(legion_copy_launcher_t launcher,
                                           legion_phase_barrier_t bar);

  // -----------------------------------------------------------------------
  // Must Epoch Operations
  // -----------------------------------------------------------------------

  /**
   * @return Caller takes ownership of return value.
   *
   * @see LegionRuntime::HighLevel::MustEpochLauncher::MustEpochLauncher()
   */
  legion_must_epoch_launcher_t
  legion_must_epoch_launcher_create(
    legion_mapper_id_t id /* = 0 */,
    legion_mapping_tag_id_t launcher_tag /* = 0 */);

  /**
   * @param handle Caller must have ownership of parameter `handle`.
   *
   * @see LegionRuntime::HighLevel::MustEpochLauncher::~MustEpochLauncher()
   */
  void
  legion_must_epoch_launcher_destroy(legion_must_epoch_launcher_t handle);

  /**
   * @return Caller takes ownership of return value.
   *
   * @see LegionRuntime::HighLevel::HighLevelRuntime::execute_must_epoch()
   */
  legion_future_map_t
  legion_must_epoch_launcher_execute(legion_runtime_t runtime,
                                     legion_context_t ctx,
                                     legion_must_epoch_launcher_t launcher);

  /**
   * @param handle Caller must have ownership of parameter `handle`.
   *
   * @see LegionRuntime::HighLevel::Must_EpochLauncher::add_single_task()
   */
  void
  legion_must_epoch_launcher_add_single_task(
    legion_must_epoch_launcher_t launcher,
    legion_domain_point_t point,
    legion_task_launcher_t handle);

  /**
   * @param handle Caller must have ownership of parameter `handle`.
   *
   * @see LegionRuntime::HighLevel::Must_EpochLauncher::add_index_task()
   */
  void
  legion_must_epoch_launcher_add_index_task(
    legion_must_epoch_launcher_t launcher,
    legion_index_launcher_t handle);

  // -----------------------------------------------------------------------
  // Fence Operations
  // -----------------------------------------------------------------------

  /**
   * @see LegionRuntime::HighLevel::Runtime::issue_mapping_fence()
   */
  void
  legion_runtime_issue_mapping_fence(legion_runtime_t runtime,
                                     legion_context_t ctx);

  /**
   * @see LegionRuntime::HighLevel::Runtime::issue_execution_fence()
   */
  void
  legion_runtime_issue_execution_fence(legion_runtime_t runtime,
                                       legion_context_t ctx);

  // -----------------------------------------------------------------------
  // Tracing Operations
  // -----------------------------------------------------------------------

  /**
   * @see LegionRuntime::HighLevel::Runtime::begin_trace()
   */
  void
  legion_runtime_begin_trace(legion_runtime_t runtime,
                             legion_context_t ctx,
                             legion_trace_id_t tid);

  /**
   * @see LegionRuntime::HighLevel::Runtime::end_trace()
   */
  void
  legion_runtime_end_trace(legion_runtime_t runtime,
                           legion_context_t ctx,
                           legion_trace_id_t tid);

  // -----------------------------------------------------------------------
  // Miscellaneous Operations
  // -----------------------------------------------------------------------

  /**
   * @see LegionRuntime::HighLevel::Runtime::get_executing_processor()
   */
  legion_processor_t
  legion_runtime_get_executing_processor(legion_runtime_t runtime,
                                         legion_context_t ctx);

  // -----------------------------------------------------------------------
  // Physical Data Operations
  // -----------------------------------------------------------------------

  /**
   * @param handle Caller must have ownership of parameter `handle`.
   *
   * @see LegionRuntime::HighLevel::PhysicalRegion::~PhysicalRegion()
   */
  void
  legion_physical_region_destroy(legion_physical_region_t handle);

  /**
   * @see LegionRuntime::HighLevel::PhysicalRegion::is_mapped()
   */
  bool
  legion_physical_region_is_mapped(legion_physical_region_t handle);

  /**
   * @see LegionRuntime::HighLevel::PhysicalRegion::wait_until_valid()
   */
  void
  legion_physical_region_wait_until_valid(legion_physical_region_t handle);

  /**
   * @see LegionRuntime::HighLevel::PhysicalRegion::is_valid()
   */
  bool
  legion_physical_region_is_valid(legion_physical_region_t handle);

  /**
   * @see LegionRuntime::HighLevel::PhysicalRegion::get_logical_region()
   */
  legion_logical_region_t
  legion_physical_region_get_logical_region(legion_physical_region_t handle);

  /**
   * @see LegionRuntime::HighLevel::PhysicalRegion::get_fields()
   */
  size_t
  legion_physical_region_get_field_count(legion_physical_region_t handle);
  legion_field_id_t
  legion_physical_region_get_field_id(legion_physical_region_t handle, size_t index);

  /**
   * Safe for use only with instances with a single field.
   *
   * @return Caller takes ownership of return value.
   *
   * @see LegionRuntime::HighLevel::PhysicalRegion::get_accessor()
   */
  legion_accessor_generic_t
  legion_physical_region_get_accessor_generic(legion_physical_region_t handle);

  /**
   * @return Caller takes ownership of return value.
   *
   * @see LegionRuntime::HighLevel::PhysicalRegion::get_field_accessor()
   */
  legion_accessor_generic_t
  legion_physical_region_get_field_accessor_generic(
    legion_physical_region_t handle,
    legion_field_id_t fid);

  /**
   * Safe for use only with instances with a single field.
   *
   * @return Caller takes ownership of return value.
   *
   * @see LegionRuntime::HighLevel::PhysicalRegion::get_field_accessor()
   */
  legion_accessor_array_t
  legion_physical_region_get_accessor_array(
    legion_physical_region_t handle);

  /**
   * @return Caller takes ownership of return value.
   *
   * @see LegionRuntime::HighLevel::PhysicalRegion::get_field_accessor()
   */
  legion_accessor_array_t
  legion_physical_region_get_field_accessor_array(
    legion_physical_region_t handle,
    legion_field_id_t fid);

  /**
   * @param handle Caller must have ownership of parameter `handle`.
   */
  void
  legion_accessor_generic_destroy(legion_accessor_generic_t handle);

  /**
   * @see LegionRuntime::Accessor::Generic::Untyped::read_untyped()
   */
  void
  legion_accessor_generic_read(legion_accessor_generic_t handle,
                               legion_ptr_t ptr,
                               void *dst,
                               size_t bytes);

  /**
   * @see LegionRuntime::Accessor::Generic::Untyped::write_untyped()
   */
  void
  legion_accessor_generic_write(legion_accessor_generic_t handle,
                                legion_ptr_t ptr,
                                const void *src,
                                size_t bytes);

  /**
   * @see LegionRuntime::Accessor::Generic::Untyped::read_untyped()
   */
  void
  legion_accessor_generic_read_domain_point(legion_accessor_generic_t handle,
                                            legion_domain_point_t dp,
                                            void *dst,
                                            size_t bytes);

  /**
   * @see LegionRuntime::Accessor::Generic::Untyped::write_untyped()
   */
  void
  legion_accessor_generic_write_domain_point(legion_accessor_generic_t handle,
                                             legion_domain_point_t dp,
                                             const void *src,
                                             size_t bytes);

  /**
   * @see LegionRuntime::Accessor::Generic::Untyped::raw_span_ptr()
   */
  void *
  legion_accessor_generic_raw_span_ptr(legion_accessor_generic_t handle,
                                       legion_ptr_t ptr,
                                       size_t req_count,
                                       size_t *act_count,
                                       legion_byte_offset_t *stride);

  /**
   * @see LegionRuntime::Accessor::Generic::Untyped::raw_rect_ptr()
   */
  void *
  legion_accessor_generic_raw_rect_ptr_1d(legion_accessor_generic_t handle,
                                          legion_rect_1d_t rect,
                                          legion_rect_1d_t *subrect,
                                          legion_byte_offset_t *offsets);

  /**
   * @see LegionRuntime::Accessor::Generic::Untyped::raw_rect_ptr()
   */
  void *
  legion_accessor_generic_raw_rect_ptr_2d(legion_accessor_generic_t handle,
                                          legion_rect_2d_t rect,
                                          legion_rect_2d_t *subrect,
                                          legion_byte_offset_t *offsets);

  /**
   * @see LegionRuntime::Accessor::Generic::Untyped::raw_rect_ptr()
   */
  void *
  legion_accessor_generic_raw_rect_ptr_3d(legion_accessor_generic_t handle,
                                          legion_rect_3d_t rect,
                                          legion_rect_3d_t *subrect,
                                          legion_byte_offset_t *offsets);

  /**
   * @see LegionRuntime::Accessor::Generic::Untyped::get_soa_parameters()
   */
  bool
  legion_accessor_generic_get_soa_parameters(legion_accessor_generic_t handle,
                                             void **base,
                                             size_t *stride);

  /**
   * @param handle Caller must have ownership of parameter `handle`.
   */
  void
  legion_accessor_array_destroy(legion_accessor_array_t handle);

  /**
   * @see LegionRuntime::Accessor::SOA::Untyped::elem_ptr()
   */
  void
  legion_accessor_array_read(legion_accessor_array_t handle,
                             legion_ptr_t ptr,
                             void *dst,
                             size_t bytes);

  /**
   * @see LegionRuntime::Accessor::SOA::Untyped::elem_ptr()
   */
  void
  legion_accessor_array_write(legion_accessor_array_t handle,
                              legion_ptr_t ptr,
                              const void *src,
                              size_t bytes);

  /**
   * @see LegionRuntime::Accessor::SOA::Untyped::elem_ptr()
   */
  void *
  legion_accessor_array_ref(legion_accessor_array_t handle,
                            legion_ptr_t ptr);

  /**
   * @return Caller takes ownership of return value.
   *
   * @see LegionRuntime::HighLevel::IndexIterator::IndexIterator()
   */
  legion_index_iterator_t
  legion_index_iterator_create(legion_runtime_t runtime,
                               legion_context_t context,
                               legion_index_space_t handle);

  /**
   * @param handle Caller must have ownership of parameter `handle`.
   *
   * @see LegionRuntime::HighLevel::IndexIterator::~IndexIterator()
   */
  void
  legion_index_iterator_destroy(legion_index_iterator_t handle);

  /**
   * @see LegionRuntime::HighLevel::IndexIterator::has_next()
   */
  bool
  legion_index_iterator_has_next(legion_index_iterator_t handle);

  /**
   * @see LegionRuntime::HighLevel::IndexIterator::next()
   */
  legion_ptr_t
  legion_index_iterator_next(legion_index_iterator_t handle);

  /**
   * @see LegionRuntime::HighLevel::IndexIterator::next_span()
   */
  legion_ptr_t
  legion_index_iterator_next_span(legion_index_iterator_t handle,
                                  size_t *act_count,
                                  size_t req_count /* = -1 */);

  // -----------------------------------------------------------------------
  // Task Operations
  // -----------------------------------------------------------------------

  /**
   * @see LegionRuntime::HighLevel::Runtime::attach_name()
   */
  void
  legion_task_id_attach_name(legion_runtime_t runtime,
                             legion_task_id_t task_id,
                             const char *name,
                             bool is_mutable /* = false */);

  /**
   * @see LegionRuntime::HighLevel::Runtime::retrieve_name()
   */
  void
  legion_task_id_retrieve_name(legion_runtime_t runtime,
                               legion_task_id_t task_id,
                               const char **result);

  /**
   * @see LegionRuntime::HighLevel::Task::args
   */
  void *
  legion_task_get_args(legion_task_t task);

  /**
   * @see LegionRuntime::HighLevel::Task::arglen
   */
  size_t
  legion_task_get_arglen(legion_task_t task);

  /**
   * @see LegionRuntime::HighLevel::Task::index_domain
   */
  legion_domain_t
  legion_task_get_index_domain(legion_task_t task);

  /**
   * @see LegionRuntime::HighLevel::Task::index_point
   */
  legion_domain_point_t
  legion_task_get_index_point(legion_task_t task);

  /**
   * @see LegionRuntime::HighLevel::Task::is_index_space
   */
  bool
  legion_task_get_is_index_space(legion_task_t task);

  /**
   * @see LegionRuntime::HighLevel::Task::local_args
   */
  void *
  legion_task_get_local_args(legion_task_t task);

  /**
   * @see LegionRuntime::HighLevel::Task::local_arglen
   */
  size_t
  legion_task_get_local_arglen(legion_task_t task);

  /**
   * @see LegionRuntime::HighLevel::Task::regions
   */
  unsigned
  legion_task_get_regions_size(legion_task_t task);

  /**
   * @return Caller does **NOT** take ownership of return value.
   *
   * @see LegionRuntime::HighLevel::Task::regions
   */
  legion_region_requirement_t
  legion_task_get_region(legion_task_t task, unsigned idx);

  /**
   * @see LegionRuntime::HighLevel::Task::futures
   */
  unsigned
  legion_task_get_futures_size(legion_task_t task);

  /**
   * @see LegionRuntime::HighLevel::Task::futures
   */
  legion_future_t
  legion_task_get_future(legion_task_t task, unsigned idx);

  /**
   * @see LegionRuntime::HighLevel::Task::task_id
   */
  legion_task_id_t
  legion_task_get_task_id(legion_task_t task);

  /**
   * @see LegionRuntime::HighLevel::Task::target_proc
   */
  legion_processor_t
  legion_task_get_target_proc(legion_task_t task);

  /**
   * @see LegionRuntime::HighLevel::Task::variants::name
   */
  const char *
  legion_task_get_name(legion_task_t task);

  /**
   * @see LegionRuntime::HighLevel::Task::target_proc
   */
  void
  legion_task_set_target_proc(legion_task_t task, legion_processor_t proc);

  /**
   * @see LegionRuntime::HighLevel::Task::additional_procs
   */
  void
  legion_task_add_additional_proc(legion_task_t task, legion_processor_t proc);

  // -----------------------------------------------------------------------
  // -----------------------------------------------------------------------
  // Inline Operations
  // -----------------------------------------------------------------------

  /**
   * @return Caller does **NOT** take ownership of return value.
   *
   * @see LegionRuntime::HighLevel::Inline::requirement
   */
  legion_region_requirement_t
  legion_inline_get_requirement(legion_inline_t inline_operation);

  // -----------------------------------------------------------------------
  // Start-up Operations
  // -----------------------------------------------------------------------

  /**
   * @see LegionRuntime::HighLevel::Runtime::start()
   */
  int
  legion_runtime_start(int argc,
                       char **argv,
                       bool background /* = false */);

  /**
   * @see LegionRuntime::HighLevel::Runtime::wait_for_shutdown()
   */
  void
  legion_runtime_wait_for_shutdown(void);

  /**
   * @see LegionRuntime::HighLevel::Runtime::set_top_level_task_id()
   */
  void
  legion_runtime_set_top_level_task_id(legion_task_id_t top_id);

  /**
   * @see LegionRuntime::HighLevel::Runtime::get_input_args()
   */
  const legion_input_args_t
  legion_runtime_get_input_args(void);

  /**
   * @see LegionRuntime::HighLevel::Runtime::set_registration_callback()
   */
  void
  legion_runtime_set_registration_callback(
    legion_registration_callback_pointer_t callback);

  /**
   * @see LegionRuntime::HighLevel::Runtime::replace_default_mapper()
   */
  void
  legion_runtime_replace_default_mapper(
    legion_runtime_t runtime,
    legion_mapper_t mapper,
    legion_processor_t proc);

  /**
   * @see LegionRuntime::HighLevel::Runtime::register_legion_task()
   */
  legion_task_id_t
  legion_runtime_register_task_void(
    legion_task_id_t id,
    legion_processor_kind_t proc_kind,
    bool single,
    bool index,
    legion_variant_id_t vid /* = AUTO_GENERATE_ID */,
    legion_task_config_options_t options,
    const char *task_name /* = NULL*/,
    legion_task_pointer_void_t task_pointer);

  /**
   * @see LegionRuntime::HighLevel::Runtime::register_legion_task()
   */
  legion_task_id_t
  legion_runtime_register_task(
    legion_task_id_t id,
    legion_processor_kind_t proc_kind,
    bool single,
    bool index,
    legion_variant_id_t vid /* = AUTO_GENERATE_ID */,
    legion_task_config_options_t options,
    const char *task_name /* = NULL*/,
    legion_task_pointer_t task_pointer);

  /**
   * @see LegionRuntime::HighLevel::Runtime::register_legion_task()
   */
  legion_task_id_t
  legion_runtime_register_task_uint32(
    legion_task_id_t id,
    legion_processor_kind_t proc_kind,
    bool single,
    bool index,
    legion_variant_id_t vid /* = AUTO_GENERATE_ID */,
    legion_task_config_options_t options,
    const char *task_name /* = NULL*/,
    legion_task_pointer_uint32_t task_pointer);

  /**
   * @see LegionRuntime::HighLevel::Runtime::register_legion_task()
   */
  legion_task_id_t
  legion_runtime_register_task_uint64(
    legion_task_id_t id,
    legion_processor_kind_t proc_kind,
    bool single,
    bool index,
    legion_variant_id_t vid /* = AUTO_GENERATE_ID */,
    legion_task_config_options_t options,
    const char *task_name /* = NULL*/,
    legion_task_pointer_uint64_t task_pointer);

  /**
   * @see LegionRuntime::HighLevel::Runtime::register_task_variant()
   */
  legion_task_id_t
  legion_runtime_register_task_variant_fnptr(
    legion_runtime_t runtime,
    legion_task_id_t id,
    legion_processor_kind_t proc_kind,
    legion_task_config_options_t options,
    const char *task_name,
    const void *userdata,
    size_t userlen,
    legion_task_pointer_wrapped_t wrapped_task_pointer);

  legion_task_id_t
  legion_runtime_register_task_variant_llvmir(
    legion_runtime_t runtime,
    legion_task_id_t id,
    legion_processor_kind_t proc_kind,
    bool global,
    legion_task_config_options_t options,
    const char *task_name,
    const void *userdata,
    size_t userlen,
    const char *llvmir,
    const char *entry_symbol);

  /**
   * @see LegionRuntime::HighLevel::Runtime::preregister_task_variant()
   */
  legion_task_id_t
  legion_runtime_preregister_task_variant_fnptr(
    legion_task_id_t id,
    legion_processor_kind_t proc_kind,
    legion_task_config_options_t options,
    const char *task_name,
    const void *userdata,
    size_t userlen,
    legion_task_pointer_wrapped_t wrapped_task_pointer);

  legion_task_id_t
  legion_runtime_preregister_task_variant_llvmir(
    legion_task_id_t id,
    legion_processor_kind_t proc_kind,
    legion_task_config_options_t options,
    const char *task_name,
    const void *userdata,
    size_t userlen,
    const char *llvmir,
    const char *entry_symbol);

  /**
   * @see LegionRuntime::LegionTaskWrapper::legion_task_preamble()
   */
  void
  legion_task_preamble(
    const void *data,
    size_t datalen,
    legion_lowlevel_id_t proc_id,
    legion_task_t *taskptr,
    const legion_physical_region_t **regionptr,
    unsigned * num_regions_ptr,
    legion_context_t * ctxptr,
    legion_runtime_t * runtimeptr);

  /**
   * @see LegionRuntime::LegionTaskWrapper::legion_task_postamble()
   */
  void
  legion_task_postamble(
    legion_runtime_t runtime,
    legion_context_t ctx,
    const void *retval,
    size_t retsize);

  /**
   * @see LegionRuntime::HighLevel::Runtime::register_projection_functor()
   */
  void
  legion_runtime_register_projection_functor(
    legion_projection_id_t id,
    legion_projection_functor_logical_region_t region_functor,
    legion_projection_functor_logical_partition_t partition_functor);

  // -----------------------------------------------------------------------
  // Timing Operations
  // -----------------------------------------------------------------------

  /**
   * @see LegionRuntime::TimeStamp::get_current_time_in_micros()
   */
  unsigned long long
  legion_get_current_time_in_micros(void);

  // -----------------------------------------------------------------------
  // Machine Operations
  // -----------------------------------------------------------------------

  /**
   * @return Caller takes ownership of return value.
   *
   * @see Realm::Machine::get_machine()
   */
  legion_machine_t
  legion_machine_create();

  /**
   * @param handle Caller must have ownership of parameter `handle`.
   *
   * @see Realm::Machine::~Machine()
   */
  void
  legion_machine_destroy(legion_machine_t handle);

  /**
   * @see Realm::Machine::get_all_processors()
   */
  void
  legion_machine_get_all_processors(
    legion_machine_t machine,
    legion_processor_t *processors,
    size_t processors_size);

  /**
   * @see Realm::Machine::get_all_processors()
   */
  size_t
  legion_machine_get_all_processors_size(legion_machine_t machine);

  /**
   * @see Realm::Machine::get_all_memories()
   */
  void
  legion_machine_get_all_memories(
    legion_machine_t machine,
    legion_memory_t *memories,
    size_t memories_size);

  /**
   * @see Realm::Machine::get_all_memories()
   */
  size_t
  legion_machine_get_all_memories_size(legion_machine_t machine);

  // -----------------------------------------------------------------------
  // Processor Operations
  // -----------------------------------------------------------------------

  /**
   * @see Realm::Processor::kind()
   */
  legion_processor_kind_t
  legion_processor_kind(legion_processor_t proc);

  /**
   * @see Realm::Processor::address_space()
   */
  legion_address_space_t
  legion_processor_address_space(legion_processor_t proc);

  // -----------------------------------------------------------------------
  // Memory Operations
  // -----------------------------------------------------------------------

  /**
   * @see Realm::Memory::kind()
   */
  legion_memory_kind_t
  legion_memory_kind(legion_memory_t mem);

  /**
   * @see Realm::Memory::address_space()
   */
  legion_address_space_t
  legion_memory_address_space(legion_memory_t mem);

  // -----------------------------------------------------------------------
  // Processor Query Operations
  // -----------------------------------------------------------------------

  /**
   * @return Caller takes ownership of return value.
   *
   * @see Realm::Machine::ProcessorQuery::ProcessorQuery()
   */
  legion_processor_query_t
  legion_processor_query_create(legion_machine_t machine);

  /**
   * @return Caller takes ownership of return value.
   *
   * @see Realm::Machine::ProcessorQuery::ProcessorQuery()
   */
  legion_processor_query_t
  legion_processor_query_create_copy(legion_processor_query_t query);

  /**
   * @param handle Caller must have ownership of parameter `handle`.
   *
   * @see Realm::Machine::ProcessorQuery::~ProcessorQuery()
   */
  void
  legion_processor_query_destroy(legion_processor_query_t handle);

  /**
   * Note: Mutates `query`.
   *
   * @see Realm::Machine::ProcessorQuery::only_kind()
   */
  void
  legion_processor_query_only_kind(legion_processor_query_t query,
                                   legion_processor_kind_t kind);

  /**
   * Note: Mutates `query`.
   *
   * @see Realm::Machine::ProcessorQuery::local_address_space()
   */
  void
  legion_processor_query_local_address_space(legion_processor_query_t query);

  /**
   * Note: Mutates `query`.
   *
   * @see Realm::Machine::ProcessorQuery::same_address_space_as()
   */
  void
  legion_processor_query_same_address_space_as_processor(legion_processor_query_t query,
                                                         legion_processor_t proc);

  /**
   * Note: Mutates `query`.
   *
   * @see Realm::Machine::ProcessorQuery::same_address_space_as()
   */
  void
  legion_processor_query_same_address_space_as_memory(legion_processor_query_t query,
                                                      legion_memory_t mem);

  /**
   * Note: Mutates `query`.
   *
   * @see Realm::Machine::ProcessorQuery::has_affinity_to()
   */
  void
  legion_processor_query_has_affinity_to_memory(legion_processor_query_t query,
                                                legion_memory_t mem,
                                                unsigned min_bandwidth /* = 0 */,
                                                unsigned max_latency /* = 0 */);

  /**
   * Note: Mutates `query`.
   *
   * @see Realm::Machine::ProcessorQuery::best_affinity_to()
   */
  void
  legion_processor_query_best_affinity_to_memory(legion_processor_query_t query,
                                                 legion_memory_t mem,
                                                 int bandwidth_weight /* = 0 */,
                                                 int latency_weight /* = 0 */);

  /**
   * @see Realm::Machine::ProcessorQuery::count()
   */
  size_t
  legion_processor_query_count(legion_processor_query_t query);

  /**
   * @see Realm::Machine::ProcessorQuery::first()
   */
  legion_processor_t
  legion_processor_query_first(legion_processor_query_t query);

  /**
   * @see Realm::Machine::ProcessorQuery::next()
   */
  legion_processor_t
  legion_processor_query_next(legion_processor_query_t query,
                              legion_processor_t after);

  /**
   * @see Realm::Machine::ProcessorQuery::random()
   */
  legion_processor_t
  legion_processor_query_random(legion_processor_query_t query);

  // -----------------------------------------------------------------------
  // Memory Query Operations
  // -----------------------------------------------------------------------

  /**
   * @return Caller takes ownership of return value.
   *
   * @see Realm::Machine::MemoryQuery::MemoryQuery()
   */
  legion_memory_query_t
  legion_memory_query_create(legion_machine_t machine);

  /**
   * @return Caller takes ownership of return value.
   *
   * @see Realm::Machine::MemoryQuery::MemoryQuery()
   */
  legion_memory_query_t
  legion_memory_query_create_copy(legion_memory_query_t query);

  /**
   * @param handle Caller must have ownership of parameter `handle`.
   *
   * @see Realm::Machine::MemoryQuery::~MemoryQuery()
   */
  void
  legion_memory_query_destroy(legion_memory_query_t handle);

  /**
   * Note: Mutates `query`.
   *
   * @see Realm::Machine::MemoryQuery::only_kind()
   */
  void
  legion_memory_query_only_kind(legion_memory_query_t query,
                                legion_memory_kind_t kind);

  /**
   * Note: Mutates `query`.
   *
   * @see Realm::Machine::MemoryQuery::local_address_space()
   */
  void
  legion_memory_query_local_address_space(legion_memory_query_t query);

  /**
   * Note: Mutates `query`.
   *
   * @see Realm::Machine::MemoryQuery::same_address_space_as()
   */
  void
  legion_memory_query_same_address_space_as_processor(legion_memory_query_t query,
                                                      legion_processor_t proc);

  /**
   * Note: Mutates `query`.
   *
   * @see Realm::Machine::MemoryQuery::same_address_space_as()
   */
  void
  legion_memory_query_same_address_space_as_memory(legion_memory_query_t query,
                                                   legion_memory_t mem);

  /**
   * Note: Mutates `query`.
   *
   * @see Realm::Machine::MemoryQuery::has_affinity_to()
   */
  void
  legion_memory_query_has_affinity_to_processor(legion_memory_query_t query,
                                                legion_processor_t proc,
                                                unsigned min_bandwidth /* = 0 */,
                                                unsigned max_latency /* = 0 */);

  /**
   * Note: Mutates `query`.
   *
   * @see Realm::Machine::MemoryQuery::has_affinity_to()
   */
  void
  legion_memory_query_has_affinity_to_memory(legion_memory_query_t query,
                                             legion_memory_t mem,
                                             unsigned min_bandwidth /* = 0 */,
                                             unsigned max_latency /* = 0 */);

  /**
   * Note: Mutates `query`.
   *
   * @see Realm::Machine::MemoryQuery::best_affinity_to()
   */
  void
  legion_memory_query_best_affinity_to_processor(legion_memory_query_t query,
                                                 legion_processor_t proc,
                                                 int bandwidth_weight /* = 0 */,
                                                 int latency_weight /* = 0 */);

  /**
   * Note: Mutates `query`.
   *
   * @see Realm::Machine::MemoryQuery::best_affinity_to()
   */
  void
  legion_memory_query_best_affinity_to_memory(legion_memory_query_t query,
                                              legion_memory_t mem,
                                              int bandwidth_weight /* = 0 */,
                                              int latency_weight /* = 0 */);

  /**
   * @see Realm::Machine::MemoryQuery::count()
   */
  size_t
  legion_memory_query_count(legion_memory_query_t query);

  /**
   * @see Realm::Machine::MemoryQuery::first()
   */
  legion_memory_t
  legion_memory_query_first(legion_memory_query_t query);

  /**
   * @see Realm::Machine::MemoryQuery::next()
   */
  legion_memory_t
  legion_memory_query_next(legion_memory_query_t query,
                           legion_memory_t after);

  /**
   * @see Realm::Machine::MemoryQuery::random()
   */
  legion_memory_t
  legion_memory_query_random(legion_memory_query_t query);

  // -----------------------------------------------------------------------
  // Default Mapper Operations
  // -----------------------------------------------------------------------

  /**
   * @see LegionRuntime::HighLevel::DefaultMapper::map_task()
   */
  bool
  legion_default_mapper_map_task(
    legion_default_mapper_t mapper,
    legion_task_t task);

  /**
   * @see LegionRuntime::HighLevel::DefaultMapper::map_inline()
   */
  bool
  legion_default_mapper_map_inline(
    legion_default_mapper_t mapper,
    legion_inline_t inline_operation);

#ifdef __cplusplus
}
#endif

#endif // __LEGION_C_H__
