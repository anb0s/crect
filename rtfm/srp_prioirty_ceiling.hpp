

#pragma once

/****************************************************************************
 *
 * This file is intended to be included after the definition of the system's
 * job list.
 *
 ****************************************************************************/

#include "util/utils.hpp"
#include "rtfm/job_resource.hpp"

namespace rtfm
{
namespace details
{

/**
 * @brief Alias for the entire system's resource tree.
 */
using resource_tree = make_resource_tree<system_job_list>::result;

/**
 * @brief Finds a resource in the resource tree.
 *
 * @tparam Resource  Resource to find.
 */
template <typename Resource>
using find_resource = brigand::front< brigand::find< resource_tree,
                                   brigand::bind<
                                     details::_same_id,
                                     typename Resource::ID,
                                     brigand::_1
                                   > > >;

template <typename... Ts>
struct job_to_priority
{
  static_assert(util::always_false<Ts...>::value, "Should not come here");
};

/**
 * @brief Converts a job to an integral_constant containing the job's priority.
 *
 * @tparam ID   Job unique ID.
 * @tparam PRIO Priority.
 * @tparam ISR  ISR definition.
 * @tparam Ts   Parameter pack of resources.
 */
template <int ID, int PRIO, typename ISR, typename... Ts>
struct job_to_priority< Job<ID, PRIO, ISR, Ts...> > : brigand::integral_constant<int, PRIO>
{
};

/**
 * @brief Extracts the priorities of the jobs connected to the resource.
 *
 * @tparam Resource  Resource to get priority list from.
 */
template <typename Resource>
using resource_to_priority_list =
                brigand::flatten< brigand::transform<
                  brigand::flatten< typename find_resource<Resource>::jobs >,
                  job_to_priority<brigand::_1>
                > >;

} /* END namespace details */

/**
 * @brief Extracts the priority ceiling of the jobs connected to the resource.
 *
 * @tparam Resource  Resource to get priority ceiling for.
 */
template <typename Resource>
using get_priority_ceiling =
                brigand::fold<
                  details::resource_to_priority_list<Resource>,
                  brigand::integral_constant<int, 0>,
                  brigand::max<brigand::_state, brigand::_element>
                >;

} /* END namespace rtfm */
