/**
 * @file main.c
 * @brief Example demonstrating FSM for recognizing strings ending with "ab"
 * @author Antonio Bernardini
 * @date 2025
 * 
 * This example creates a finite state machine that recognizes strings
 * ending with the pattern "ab". The FSM has three states:
 * - q0: Initial state (also accepts strings not ending with "ab")
 * - q1: Intermediate state (after reading 'a')
 * - q2: Final/accepting state (after reading "ab")
 * 
 * The FSM demonstrates:
 * - Basic vertex (state) creation and positioning
 * - Edge (transition) creation with labels
 * - Setting initial and final states
 * - Using curved edges to avoid visual overlap
 * - LaTeX generation for publication-quality diagrams
 * - PDF and PNG output generation
 * 
 * @example
 * Input strings and their acceptance:
 * - "ab" -> ACCEPTED (ends with "ab")
 * - "aab" -> ACCEPTED (ends with "ab") 
 * - "abab" -> ACCEPTED (ends with "ab")
 * - "ba" -> REJECTED (doesn't end with "ab")
 * - "a" -> REJECTED (doesn't end with "ab")
 * - "b" -> REJECTED (doesn't end with "ab")
 */

#include "fsm.h"

/**
 * @brief Main function demonstrating string recognition FSM
 * @return 0 on success, 1 on failure
 * @details Creates an FSM that recognizes strings ending with "ab",
 *          generates LaTeX visualization, and outputs both PDF and PNG.
 */
int main(void) {
    // Create a new FSM
    fsm_t *fsm = fsm_create("Example Finite State Machine");
    if (!fsm) {
        fprintf(stderr, "Failed to create FSM\n");
        return 1;
    }

    printf("Creating a simple FSM for recognizing strings ending with 'ab'\n\n");

    // Add vertices (states)
    size_t q0 = fsm_add_vertex(fsm, "q_0", 0, 0);
    size_t q1 = fsm_add_vertex(fsm, "q_1", 4, 0);
    size_t q2 = fsm_add_vertex(fsm, "q_2", 8, 0);

    // Set initial and final states
    fsm_set_initial_state(fsm, q0);
    fsm_set_final_state(fsm, q2);

    // Add edges (transitions)
    size_t e1 = fsm_add_edge(fsm, q0, q0, "b");
    size_t e2 = fsm_add_edge(fsm, q0, q1, "a");
    size_t e3 = fsm_add_edge(fsm, q1, q0, "a");
    size_t e4 = fsm_add_edge(fsm, q1, q2, "b");
    size_t e5 = fsm_add_edge(fsm, q2, q0, "a");
    size_t e6 = fsm_add_edge(fsm, q2, q0, "b");

    // Unused edges for clarity
    (void)e1;
    (void)e2;
    (void)e4;

    // Make some edges curved to avoid overlap
    fsm_set_edge_curved(fsm, e3, 30); // q1 -> q0 curved
    fsm_set_edge_curved(fsm, e5, 45); // q2 -> q0 curved
    fsm_set_edge_curved(fsm, e6, 60); // q2 -> q0 curved more

    // Print FSM information
    fsm_print_info(fsm);

    // Generate LaTeX file
    if (!fsm_generate_latex(fsm, "recognize-strings-ending-with-ab.tex")) {
        printf("LaTeX file \"recognize-strings-ending-with-ab.tex\" generated successfully!\n");
    }

    printf("LaTeX code:\n");
    printf("=============\n");
    fsm_print_latex(fsm);

    // Clean up
    fsm_destroy(fsm);
    return 0;
}
