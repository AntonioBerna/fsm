/**
 * @file main.c
 * @brief Simple Calculator FSM Example
 * @author Antonio Bernardini
 * @date 2025
 * 
 * This FSM validates simple arithmetic expressions like:
 * - "1+2"     -> ACCEPTED
 * - "5*3-1"   -> ACCEPTED  
 * - "+1"      -> REJECTED (can't start with operator)
 * - "1+"      -> REJECTED (can't end with operator)
 * - "1++2"    -> REJECTED (consecutive operators)
 */

#include "fsm.h"

int main(void) {
    // Create the FSM
    fsm_t *fsm = fsm_create("Simple Calculator Validator");
    if (!fsm) {
        fprintf(stderr, "Failed to create FSM\n");
        return 1;
    }

    printf("Creating Simple Calculator FSM\n");
    printf("Validates arithmetic expressions like '1+2*3'\n\n");

    // Add states with strategic positioning
    int start = fsm_add_vertex(fsm, "Start", 0, 0);
    int digit = fsm_add_vertex(fsm, "Digit", 4, 0);
    int operator = fsm_add_vertex(fsm, "Operator", 8, 0);
    int error = fsm_add_vertex(fsm, "Error", 4, -3);

    // Configure state properties
    fsm_set_vertex_radius(fsm, start, 0.8);
    fsm_set_vertex_radius(fsm, digit, 1.0);
    fsm_set_vertex_radius(fsm, operator, 1.0);
    fsm_set_vertex_radius(fsm, error, 0.8);

    // Set initial and final states
    fsm_set_initial_state(fsm, start);
    fsm_set_final_state(fsm, digit); // Only accept if ending on digit

    // Add transitions
    int e1 = fsm_add_edge(fsm, start, digit, "0-9");
    int e2 = fsm_add_edge(fsm, start, error, "+,-,*,/");
    int e3 = fsm_add_edge(fsm, digit, operator, "+,-,*,/");
    int e4 = fsm_add_edge(fsm, digit, digit, "0-9");
    int e5 = fsm_add_edge(fsm, operator, digit, "0-9");
    int e6 = fsm_add_edge(fsm, operator, error, "+,-,*,/");
    int e7 = fsm_add_edge(fsm, error, error, "any");

    // Unused edges for clarity
    (void)e1;

    // Style the edges for better visualization
    fsm_set_edge_curved(fsm, e2, -30); // Curve start->error
    fsm_set_edge_curved(fsm, e3, 15);  // Curve digit->operator
    fsm_set_edge_curved(fsm, e4, 60);  // Self-loop on digit
    fsm_set_edge_curved(fsm, e5, 15);  // Curve digit->operator
    fsm_set_edge_curved(fsm, e6, 30);  // Curve operator->error
    fsm_set_edge_curved(fsm, e7, 45);  // Self-loop on error

    // Print FSM information
    fsm_print_info(fsm);

    // Generate LaTeX output
    if (fsm_generate_latex(fsm, "simple-calculator.tex") == 0) {
        printf("LaTeX file generated successfully!\n");
    }

    // Preview the generated code
    printf("\nGenerated LaTeX TikZ code:\n");
    printf("==========================\n");
    fsm_print_latex(fsm);

    // Cleanup
    fsm_destroy(fsm);
    return 0;
}
