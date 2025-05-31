/**
 * @file fsm.h
 * @brief Finite State Machine Designer Library
 * @author Antonio Bernardini  
 * @date 2025
 * 
 * This library provides a comprehensive C API for creating, manipulating, and
 * visualizing Finite State Machines (FSMs). It integrates with a safe string
 * library for memory safety and generates professional LaTeX/TikZ output for
 * publication-quality diagrams.
 * 
 * Key features:
 * - Memory-safe string handling with automatic cleanup
 * - Professional LaTeX/TikZ diagram generation  
 * - Support for Greek letter symbols in labels
 * - Flexible vertex positioning and edge styling
 * - Curved edges and self-loops for complex FSMs
 * - PDF and PNG output generation
 * 
 * @example
 * @code
 * // Create a simple FSM
 * fsm_t *fsm = fsm_create("Example FSM");
 * int q0 = fsm_add_vertex(fsm, "q_0", 0, 0);
 * int q1 = fsm_add_vertex(fsm, "q_1", 4, 0);
 * fsm_set_initial_state(fsm, q0);
 * fsm_set_final_state(fsm, q1);
 * fsm_add_edge(fsm, q0, q1, "a");
 * fsm_generate_latex(fsm, "example");
 * fsm_destroy(fsm);
 * @endcode
 */
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/stat.h>
#include <string.h>

#include "sstring.h"

// Initial capacity constants for dynamic arrays
#define INITIAL_VERTICES_CAPACITY 10
#define INITIAL_EDGES_CAPACITY    20
#define GROWTH_FACTOR              2

// Greek letter support constants
#define GREEK_ALPHA     "\\alpha"
#define GREEK_BETA      "\\beta"
#define GREEK_GAMMA     "\\gamma"
#define GREEK_DELTA     "\\delta"
#define GREEK_EPSILON   "\\epsilon"
#define GREEK_ZETA      "\\zeta"
#define GREEK_ETA       "\\eta"
#define GREEK_THETA     "\\theta"
#define GREEK_IOTA      "\\iota"
#define GREEK_KAPPA     "\\kappa"
#define GREEK_LAMBDA    "\\lambda"
#define GREEK_MU        "\\mu"
#define GREEK_NU        "\\nu"
#define GREEK_XI        "\\xi"
#define GREEK_OMICRON   "\\omicron"
#define GREEK_PI        "\\pi"
#define GREEK_RHO       "\\rho"
#define GREEK_SIGMA     "\\sigma"
#define GREEK_TAU       "\\tau"
#define GREEK_UPSILON   "\\upsilon"
#define GREEK_PHI       "\\phi"
#define GREEK_CHI       "\\chi"
#define GREEK_PSI       "\\psi"
#define GREEK_OMEGA     "\\omega"

// Capital Greek letters
#define GREEK_ALPHA_CAP     "\\Alpha"
#define GREEK_BETA_CAP      "\\Beta"
#define GREEK_GAMMA_CAP     "\\Gamma"
#define GREEK_DELTA_CAP     "\\Delta"
#define GREEK_EPSILON_CAP   "\\Epsilon"
#define GREEK_ZETA_CAP      "\\Zeta"
#define GREEK_ETA_CAP       "\\Eta"
#define GREEK_THETA_CAP     "\\Theta"
#define GREEK_IOTA_CAP      "\\Iota"
#define GREEK_KAPPA_CAP     "\\Kappa"
#define GREEK_LAMBDA_CAP    "\\Lambda"
#define GREEK_MU_CAP        "\\Mu"
#define GREEK_NU_CAP        "\\Nu"
#define GREEK_XI_CAP        "\\Xi"
#define GREEK_OMICRON_CAP   "\\Omicron"
#define GREEK_PI_CAP        "\\Pi"
#define GREEK_RHO_CAP       "\\Rho"
#define GREEK_SIGMA_CAP     "\\Sigma"
#define GREEK_TAU_CAP       "\\Tau"
#define GREEK_UPSILON_CAP   "\\Upsilon"
#define GREEK_PHI_CAP       "\\Phi"
#define GREEK_CHI_CAP       "\\Chi"
#define GREEK_PSI_CAP       "\\Psi"
#define GREEK_OMEGA_CAP     "\\Omega"

/**
 * @brief Represents a vertex (state) in the finite state machine
 * @details Contains all properties needed to define and visualize a state,
 *          including position, appearance, and semantic flags
 */
typedef struct {
    int id;                          ///< Unique identifier for the vertex
    string_t *label;                 ///< Safe string for state label
    double x, y;                     ///< Position coordinates for visualization
    double radius;                   ///< Vertex radius for drawing
    bool is_initial;                 ///< Flag indicating if this is the initial state
    bool is_final;                   ///< Flag indicating if this is a final/accepting state
} vertex_t;

/**
 * @brief Represents an edge (transition) in the finite state machine
 * @details Contains all properties needed to define and visualize a transition,
 *          including source, destination, label, and styling options
 */
typedef struct {
    int id;                          ///< Unique identifier for the edge
    int from_vertex_id;              ///< Source vertex ID
    int to_vertex_id;                ///< Destination vertex ID
    string_t *label;                 ///< Safe string for transition label
    bool is_curved;                  ///< Flag for curved edge rendering
    double curve_angle;              ///< Angle for curved edge (in degrees)
    int is_self_loop;                ///< Flag indicating if this is a self-loop
} edge_t;

/**
 * @brief Main finite state machine structure
 * @details Contains all vertices, edges, and metadata for a complete FSM,
 *          with dynamic memory allocation for vertices and edges arrays.
 *          Memory grows automatically as needed using realloc.
 */
typedef struct {
    string_t *name;                  ///< Safe string for FSM name/description
    vertex_t *vertices;              ///< Dynamic array of vertices (states)
    edge_t *edges;                   ///< Dynamic array of edges (transitions)
    int vertex_count;                ///< Current number of vertices
    int edge_count;                  ///< Current number of edges
    int vertex_capacity;             ///< Allocated capacity for vertices array
    int edge_capacity;               ///< Allocated capacity for edges array
    int next_vertex_id;              ///< Next available vertex ID
    int next_edge_id;                ///< Next available edge ID
} fsm_t;

/*
 * ========================================
 * FSM Creation and Management Functions
 * ========================================
 */

/**
 * @brief Create a new finite state machine
 * @param name Name/description for the FSM (null-terminated string)
 * @return Pointer to newly created FSM structure, or NULL on failure
 * @details Allocates memory for a new FSM and initializes all fields.
 *          The name is safely copied using the string library.
 *          Memory must be freed using fsm_destroy().
 */
fsm_t *fsm_create(const char *name);

/**
 * @brief Destroy an FSM and free all associated memory
 * @param fsm Pointer to FSM to destroy (can be NULL)
 * @details Safely deallocates all memory including vertex labels,
 *          edge labels, FSM name, and the FSM structure itself.
 *          Safe to call with NULL pointer.
 */
void fsm_destroy(fsm_t *fsm);

/*
 * ========================================
 * Vertex (State) Management Functions
 * ========================================
 */

/**
 * @brief Add a new vertex (state) to the FSM
 * @param fsm Pointer to the FSM
 * @param label Label for the vertex (null-terminated string)
 * @param x X coordinate for visualization
 * @param y Y coordinate for visualization
 * @return Vertex ID on success, -1 on failure
 * @details Creates a new vertex with default properties (radius=0.8,
 *          not initial, not final). The label is safely copied.
 */
int fsm_add_vertex(fsm_t *fsm, const char *label, const double x, const double y);

/**
 * @brief Set the radius of a specific vertex
 * @param fsm Pointer to the FSM
 * @param vertex_id ID of the vertex to modify
 * @param radius New radius value for the vertex
 * @return 0 on success, -1 on failure (invalid FSM or vertex ID)
 * @details Changes the visual radius of the vertex for rendering.
 *          Useful for emphasizing important states.
 */
int fsm_set_vertex_radius(fsm_t *fsm, const int vertex_id, const double radius);

/**
 * @brief Mark a vertex as the initial state
 * @param fsm Pointer to the FSM
 * @param vertex_id ID of the vertex to mark as initial
 * @return 0 on success, -1 on failure (invalid FSM or vertex ID)
 * @details Sets the specified vertex as initial and clears the initial
 *          flag from all other vertices (only one initial state allowed).
 */
int fsm_set_initial_state(fsm_t *fsm, const int vertex_id);

/**
 * @brief Mark a vertex as a final (accepting) state
 * @param fsm Pointer to the FSM
 * @param vertex_id ID of the vertex to mark as final
 * @return 0 on success, -1 on failure (invalid FSM or vertex ID)
 * @details Sets the final flag for the specified vertex.
 *          Multiple final states are allowed.
 */
int fsm_set_final_state(fsm_t *fsm, const int vertex_id);

/**
 * @brief Get a vertex by its ID
 * @param fsm Pointer to the FSM
 * @param vertex_id ID of the vertex to retrieve
 * @return Pointer to vertex structure, or NULL if not found
 * @details Returns a direct pointer to the vertex structure for
 *          reading or modification. Use with caution.
 */
vertex_t *fsm_get_vertex(fsm_t *fsm, const int vertex_id);

/*
 * ========================================
 * Edge (Transition) Management Functions
 * ========================================
 */

/**
 * @brief Add a new edge (transition) to the FSM
 * @param fsm Pointer to the FSM
 * @param from_vertex_id Source vertex ID
 * @param to_vertex_id Destination vertex ID
 * @param label Label for the transition (null-terminated string)
 * @return Edge ID on success, -1 on failure
 * @details Creates a new edge between two existing vertices.
 *          Automatically detects self-loops. The label is safely copied.
 *          Both vertices must exist before creating the edge.
 */
int fsm_add_edge(fsm_t *fsm, const int from_vertex_id, const int to_vertex_id, const char *label);

/**
 * @brief Set an edge to be curved with a specific angle
 * @param fsm Pointer to the FSM
 * @param edge_id ID of the edge to modify
 * @param curve_angle Curve angle in degrees
 * @return 0 on success, -1 on failure (invalid FSM or edge ID)
 * @details Makes the edge curved for better visualization when
 *          multiple edges exist between the same vertices.
 */
int fsm_set_edge_curved(fsm_t *fsm, const int edge_id, const double curve_angle);

/**
 * @brief Get an edge by its ID
 * @param fsm Pointer to the FSM
 * @param edge_id ID of the edge to retrieve
 * @return Pointer to edge structure, or NULL if not found
 * @details Returns a direct pointer to the edge structure for
 *          reading or modification. Use with caution.
 */
edge_t *fsm_get_edge(fsm_t *fsm, const int edge_id);

/*
 * ========================================
 * LaTeX Generation and Output Functions
 * ========================================
 */

/**
 * @brief Generate LaTeX/TikZ code and save to file
 * @param fsm Pointer to the FSM
 * @param filename Base filename (without extension) for output
 * @return 0 on success, -1 on failure
 * @details Generates complete LaTeX document with TikZ code for
 *          FSM visualization. Creates both .tex file and Makefile
 *          for PDF/PNG generation in a 'latex/' subdirectory.
 */
int fsm_generate_latex(fsm_t *fsm, const char *filename);

/**
 * @brief Print LaTeX/TikZ code to standard output
 * @param fsm Pointer to the FSM
 * @details Prints the complete LaTeX document to stdout for
 *          debugging or piping to other tools.
 */
void fsm_print_latex(fsm_t *fsm);

/*
 * ========================================
 * Utility and Debug Functions
 * ========================================
 */

/**
 * @brief Print detailed information about the FSM
 * @param fsm Pointer to the FSM
 * @details Prints comprehensive information including FSM name,
 *          all vertices with their properties, and all edges with
 *          their properties. Useful for debugging and verification.
 */
void fsm_print_info(fsm_t *fsm);
