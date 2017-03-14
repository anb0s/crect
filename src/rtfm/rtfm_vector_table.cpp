
#include "kvasir/mpl/mpl.hpp"
#include "rtfm/rtfm_srp.hpp"
#include "rtfm/rtfm_vector_table.hpp"

/**
 * @brief Unhandled exception definition.
 */
extern "C" void _unhandled_exception(void)
{
  while(1);
}

/**
 * @brief Save vector table to the correct location.
 */
__attribute__((used, section(".isr_vector")))
constexpr const vector_table system_vectors = generate_vector_table(
    std::make_integer_sequence<
        unsigned, sizeof(vector_table::mcu_vectors) /
                      sizeof(rtfm::details::isr_function_pointer)>::value);
