/**
 * @file main.c
 * @brief Advanced FSM example for counting even number of ones in binary strings
 * @author Antonio Bernardini
 * @date 2025
 * 
 * This example demonstrates a more complex finite state machine that processes
 * binary strings and accepts those containing an even number of '1' characters.
 * The FSM showcases advanced features including:
 * 
 * **States:**
 * - Even: Accepts strings with even number of 1s (initial and final state)
 * - Odd: Tracks strings with odd number of 1s
 * - Error: Demonstrates epsilon transitions and error handling
 * 
 * **Advanced Features Demonstrated:**
 * - Custom vertex radius settings for visual emphasis
 * - Greek letter symbols (epsilon transitions)
 * - Curved edges for complex transition visualization
 * - Self-loops with custom curve angles
 * - Error state with catch-all transitions
 * - Professional LaTeX output with TikZ styling
 * 
 * **State Transitions:**
 * - On '0': Stay in current state (doesn't affect parity)
 * - On '1': Switch between Even and Odd states
 * - Epsilon transitions: Demonstrate non-deterministic behavior
 * 
 * @example
 * Input strings and their acceptance:
 * - "00" -> ACCEPTED (0 ones = even)
 * - "11" -> ACCEPTED (2 ones = even)
 * - "101" -> REJECTED (3 ones = odd)
 * - "1010" -> ACCEPTED (2 ones = even)
 * - "1111" -> ACCEPTED (4 ones = even)
 * - "" (empty) -> ACCEPTED (0 ones = even)
 * 
 * @note This example uses the GREEK_EPSILON constant to demonstrate
 *       support for mathematical notation in FSM transitions.
 */

#include "fsm.h"

/**
 * @brief Main function demonstrating binary string processing FSM
 * @return 0 on success, 1 on failure
 * @details Creates a complex FSM for binary string analysis with advanced
 *          visualization features including curved edges, custom vertex sizes,
 *          and epsilon transitions. Generates publication-quality LaTeX output.
 */
int main(void) {
    // Create a more complex FSM for binary string processing
    fsm_t *fsm = fsm_create("Binary String Processor");
    if (!fsm) {
        fprintf(stderr, "Failed to create FSM\n");
        return 1;
    }

    printf("Creating a complex FSM for processing binary strings\n");
    printf("This FSM accepts strings with an even number of 1s\n\n");

    // Add vertices with custom positioning
    int even = fsm_add_vertex(fsm, "Even", 0, 0);
    int odd = fsm_add_vertex(fsm, "Odd", 6, 0);
    int error = fsm_add_vertex(fsm, "Error", 3, -4);

    // Set vertex properties
    fsm_set_vertex_radius(fsm, even, 1.0);
    fsm_set_vertex_radius(fsm, odd, 1.0);
    fsm_set_vertex_radius(fsm, error, 0.8);

    // Set initial and final states
    fsm_set_initial_state(fsm, even);
    fsm_set_final_state(fsm, even);

    // Add transitions
    int e1 = fsm_add_edge(fsm, even, even, "0");            // Stay in even on 0
    int e2 = fsm_add_edge(fsm, even, odd, "1");             // Go to odd on 1
    int e3 = fsm_add_edge(fsm, odd, odd, "0");              // Stay in odd on 0
    int e4 = fsm_add_edge(fsm, odd, even, "1");             // Go to even on 1
    int e5 = fsm_add_edge(fsm, even, error, GREEK_EPSILON); // Epsilon transition to error
    int e6 = fsm_add_edge(fsm, odd, error, GREEK_EPSILON);  // Epsilon transition to error
    int e7 = fsm_add_edge(fsm, error, error, "0/1");        // Error state loops

    // Unused edges for clarity
    (void)e2;
    (void)e5;
    (void)e6;

    // Make some edges curved for better visualization
    fsm_set_edge_curved(fsm, e1, 60); // Self-loop curve for even state
    fsm_set_edge_curved(fsm, e3, 60); // Self-loop curve for odd state
    fsm_set_edge_curved(fsm, e4, 30); // Curve the odd->even transition
    fsm_set_edge_curved(fsm, e7, 45); // Curve the error self-loop

    // Print FSM information
    fsm_print_info(fsm);

    // Generate LaTeX file
    if (fsm_generate_latex(fsm, "even-number-of-ones.tex") == 0) {
        printf("LaTeX file \"even-number-of-ones.tex\" generated successfully!\n");
    }

    printf("Generated LaTeX TikZ code:\n");
    printf("===========================\n");
    fsm_print_latex(fsm);

    // Clean up
    fsm_destroy(fsm);

    return 0;
}
