/**
 * @file fsm.c
 * @brief Implementation of the Finite State Machine Designer Library
 * @author Antonio Bernardini
 * @date 2025
 * 
 * This file contains the complete implementation of the FSM library,
 * including all functions for FSM creation, manipulation, and LaTeX
 * generation. It integrates with the safe string library for memory
 * safety and provides robust error handling throughout.
 */

#include "fsm.h"

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

fsm_t *fsm_create(const char *name) {
    fsm_t *fsm = (fsm_t *)malloc(sizeof(fsm_t));
    if (!fsm) {
        fprintf(stderr, "Error: Failed to allocate memory for fsm_t\n");
        return NULL;
    }

    fsm->name = string_create_from_cstr(name);
    if (!fsm->name) {
        fprintf(stderr, "Error: Failed to create name string\n");
        free(fsm);
        return NULL;
    }

    // Initialize dynamic arrays
    fsm->vertex_capacity = INITIAL_VERTICES_CAPACITY;
    fsm->edge_capacity = INITIAL_EDGES_CAPACITY;
    
    fsm->vertices = (vertex_t *)malloc(fsm->vertex_capacity * sizeof(vertex_t));
    if (!fsm->vertices) {
        fprintf(stderr, "Error: Failed to allocate memory for vertices array\n");
        string_destroy(fsm->name);
        free(fsm);
        return NULL;
    }
    
    fsm->edges = (edge_t *)malloc(fsm->edge_capacity * sizeof(edge_t));
    if (!fsm->edges) {
        fprintf(stderr, "Error: Failed to allocate memory for edges array\n");
        free(fsm->vertices);
        string_destroy(fsm->name);
        free(fsm);
        return NULL;
    }

    fsm->vertex_count = 0;
    fsm->edge_count = 0;
    fsm->next_vertex_id = 1;
    fsm->next_edge_id = 1;

    return fsm;
}

/**
 * @brief Destroy an FSM and free all associated memory
 * @param fsm Pointer to FSM to destroy (can be NULL)
 * @details Safely deallocates all memory including vertex labels,
 *          edge labels, FSM name, and the FSM structure itself.
 *          Safe to call with NULL pointer.
 */
void fsm_destroy(fsm_t *fsm) {
    if (fsm) {
        // Clean up FSM name
        string_destroy(fsm->name);

        // Clean up vertex labels
        for (int i = 0; i < fsm->vertex_count; ++i) {
            string_destroy(fsm->vertices[i].label);
        }

        // Clean up edge labels
        for (int i = 0; i < fsm->edge_count; ++i) {
            string_destroy(fsm->edges[i].label);
        }

        // Free dynamic arrays
        free(fsm->vertices);
        free(fsm->edges);
        
        free(fsm);
    }
}

/*
 * ========================================
 * Dynamic Array Management Helper Functions
 * ========================================
 */

/**
 * @brief Grow the vertices array capacity if needed
 * @param fsm Pointer to the FSM
 * @return 0 on success, -1 on failure
 * @details Uses realloc to double the vertices array capacity when full
 */
static int fsm_grow_vertices_if_needed(fsm_t *fsm) {
    if (fsm->vertex_count >= fsm->vertex_capacity) {
        int new_capacity = fsm->vertex_capacity * GROWTH_FACTOR;
        vertex_t *new_vertices = (vertex_t *)realloc(fsm->vertices, new_capacity * sizeof(vertex_t));
        
        if (!new_vertices) {
            fprintf(stderr, "Error: Failed to reallocate memory for vertices array\n");
            return -1;
        }
        
        fsm->vertices = new_vertices;
        fsm->vertex_capacity = new_capacity;
    }
    return 0;
}

/**
 * @brief Grow the edges array capacity if needed
 * @param fsm Pointer to the FSM
 * @return 0 on success, -1 on failure
 * @details Uses realloc to double the edges array capacity when full
 */
static int fsm_grow_edges_if_needed(fsm_t *fsm) {
    if (fsm->edge_count >= fsm->edge_capacity) {
        int new_capacity = fsm->edge_capacity * GROWTH_FACTOR;
        edge_t *new_edges = (edge_t *)realloc(fsm->edges, new_capacity * sizeof(edge_t));
        
        if (!new_edges) {
            fprintf(stderr, "Error: Failed to reallocate memory for edges array\n");
            return -1;
        }
        
        fsm->edges = new_edges;
        fsm->edge_capacity = new_capacity;
    }
    return 0;
}

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
int fsm_add_vertex(fsm_t *fsm, const char *label, const double x, const double y) {
    if (!fsm) {
        return -1;
    }

    // Grow the vertices array if needed
    if (fsm_grow_vertices_if_needed(fsm) != 0) {
        return -1;
    }

    vertex_t *vertex = &fsm->vertices[fsm->vertex_count];
    vertex->id = fsm->next_vertex_id++;

    vertex->label = string_create_from_cstr(label);
    if (!vertex->label) {
        fprintf(stderr, "Error: Failed to create vertex label string\n");
        return -1;
    }

    vertex->x = x;
    vertex->y = y;
    vertex->radius = 0.8; // Default radius
    vertex->is_initial = false;
    vertex->is_final = false;

    fsm->vertex_count++;
    return vertex->id;
}

/**
 * @brief Set the radius of a specific vertex
 * @param fsm Pointer to the FSM
 * @param vertex_id ID of the vertex to modify
 * @param radius New radius value for the vertex
 * @return 0 on success, -1 on failure (invalid FSM or vertex ID)
 * @details Changes the visual radius of the vertex for rendering.
 *          Useful for emphasizing important states.
 */
int fsm_set_vertex_radius(fsm_t *fsm, const int vertex_id, const double radius) {
    if (!fsm) return -1;

    for (int i = 0; i < fsm->vertex_count; ++i) {
        if (fsm->vertices[i].id == vertex_id) {
            fsm->vertices[i].radius = radius;
            return 0;
        }
    }
    return -1;
}

/**
 * @brief Mark a vertex as the initial state
 * @param fsm Pointer to the FSM
 * @param vertex_id ID of the vertex to mark as initial
 * @return 0 on success, -1 on failure (invalid FSM or vertex ID)
 * @details Sets the specified vertex as initial and clears the initial
 *          flag from all other vertices (only one initial state allowed).
 */
int fsm_set_initial_state(fsm_t *fsm, const int vertex_id) {
    if (!fsm) return -1;

    // First, clear all initial states
    for (int i = 0; i < fsm->vertex_count; ++i) {
        fsm->vertices[i].is_initial = false;
    }

    // Set the specified vertex as initial
    for (int i = 0; i < fsm->vertex_count; ++i) {
        if (fsm->vertices[i].id == vertex_id) {
            fsm->vertices[i].is_initial = true;
            return 0;
        }
    }
    return -1;
}

/**
 * @brief Mark a vertex as a final (accepting) state
 * @param fsm Pointer to the FSM
 * @param vertex_id ID of the vertex to mark as final
 * @return 0 on success, -1 on failure (invalid FSM or vertex ID)
 * @details Sets the final flag for the specified vertex.
 *          Multiple final states are allowed.
 */
int fsm_set_final_state(fsm_t *fsm, const int vertex_id) {
    if (!fsm) return -1;

    for (int i = 0; i < fsm->vertex_count; ++i) {
        if (fsm->vertices[i].id == vertex_id) {
            fsm->vertices[i].is_final = true;
            return 0;
        }
    }
    return -1;
}

/**
 * @brief Get a vertex by its ID
 * @param fsm Pointer to the FSM
 * @param vertex_id ID of the vertex to retrieve
 * @return Pointer to vertex structure, or NULL if not found
 * @details Returns a direct pointer to the vertex structure for
 *          reading or modification. Use with caution.
 */
vertex_t *fsm_get_vertex(fsm_t *fsm, const int vertex_id) {
    if (!fsm) return NULL;

    for (int i = 0; i < fsm->vertex_count; ++i) {
        if (fsm->vertices[i].id == vertex_id) {
            return &fsm->vertices[i];
        }
    }
    return NULL;
}

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
int fsm_add_edge(fsm_t *fsm, const int from_vertex_id, const int to_vertex_id, const char *label) {
    if (!fsm) {
        return -1;
    }

    // Verify that both vertices exist
    vertex_t *from_vertex = fsm_get_vertex(fsm, from_vertex_id);
    vertex_t *to_vertex = fsm_get_vertex(fsm, to_vertex_id);

    if (!from_vertex || !to_vertex) {
        return -1;
    }

    // Grow the edges array if needed
    if (fsm_grow_edges_if_needed(fsm) != 0) {
        return -1;
    }

    edge_t *edge = &fsm->edges[fsm->edge_count];
    edge->id = fsm->next_edge_id++;
    edge->from_vertex_id = from_vertex_id;
    edge->to_vertex_id = to_vertex_id;

    edge->label = string_create_from_cstr(label);
    if (!edge->label) {
        fprintf(stderr, "Error: Failed to create edge label string\n");
        return -1;
    }

    edge->is_curved = 0;
    edge->curve_angle = 0.0;
    edge->is_self_loop = (from_vertex_id == to_vertex_id);

    fsm->edge_count++;
    return edge->id;
}

/**
 * @brief Set an edge to be curved with a specific angle
 * @param fsm Pointer to the FSM
 * @param edge_id ID of the edge to modify
 * @param curve_angle Curve angle in degrees
 * @return 0 on success, -1 on failure (invalid FSM or edge ID)
 * @details Makes the edge curved for better visualization when
 *          multiple edges exist between the same vertices.
 */
int fsm_set_edge_curved(fsm_t *fsm, const int edge_id, const double curve_angle) {
    if (!fsm) return -1;

    for (int i = 0; i < fsm->edge_count; ++i) {
        if (fsm->edges[i].id == edge_id) {
            fsm->edges[i].is_curved = true;
            fsm->edges[i].curve_angle = curve_angle;
            return 0;
        }
    }
    return -1;
}

/**
 * @brief Get an edge by its ID
 * @param fsm Pointer to the FSM
 * @param edge_id ID of the edge to retrieve
 * @return Pointer to edge structure, or NULL if not found
 * @details Returns a direct pointer to the edge structure for
 *          reading or modification. Use with caution.
 */
edge_t *fsm_get_edge(fsm_t *fsm, const int edge_id) {
    if (!fsm) return NULL;

    for (int i = 0; i < fsm->edge_count; ++i) {
        if (fsm->edges[i].id == edge_id) {
            return &fsm->edges[i];
        }
    }
    return NULL;
}

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
int fsm_generate_latex(fsm_t *fsm, const char *filename) {
    if (!fsm || !filename) {
        return -1;
    }

    // Create latex directory in current working directory
    const char *latex_dir = "latex";
    char full_path[512];
    char makefile_path[512];

    // Create latex directory if it doesn't exist in current directory
    struct stat st = {0};
    if (stat(latex_dir, &st) == -1) {
        if (mkdir(latex_dir, 0755) != 0) {
            fprintf(stderr, "Error: Cannot create latex directory\n");
            return -1;
        }
        printf("Created latex directory: %s\n", latex_dir);
    }

    // Extract base name without extension using safe strings
    string_t *filename_str = string_create_from_cstr(filename);
    if (!filename_str) {
        return -1;
    }

    // Remove .tex extension if present
    size_t tex_pos = string_find_cstr(filename_str, ".tex", 0);
    if (tex_pos != STRING_NPOS && tex_pos + 4 == string_length(filename_str)) {
        string_erase(filename_str, tex_pos, 4);
    }

    // Construct full path: latex/filename.tex
    string_t *full_path_str = string_create();
    if (!full_path_str) {
        string_destroy(filename_str);
        return -1;
    }

    string_result_t result = string_format(full_path_str, "%s/%s.tex", latex_dir, string_cstr(filename_str));
    if (result != STRING_SUCCESS) {
        string_destroy(filename_str);
        string_destroy(full_path_str);
        return -1;
    }

    // Copy to char buffer for fopen
    string_copy_to_buffer(full_path_str, full_path, sizeof(full_path));

    FILE *file = fopen(full_path, "w");
    if (!file) {
        fprintf(stderr, "Error: Cannot open file %s for writing\n", full_path);
        string_destroy(filename_str);
        string_destroy(full_path_str);
        return -1;
    }

    // Write LaTeX header
    fprintf(file, "\\documentclass[tikz]{standalone}\n");
    fprintf(file, "\\usepackage{tikz}\n");
    fprintf(file, "\\usetikzlibrary{automata,positioning,arrows.meta}\n");
    fprintf(file, "\\tikzset{\n");
    fprintf(file, "    >=Stealth,\n");
    fprintf(file, "    every edge/.style={draw,->,>=Stealth,thick},\n");
    fprintf(file, "    every loop/.style={draw,->,>=Stealth,thick}\n");
    fprintf(file, "}\n");
    fprintf(file, "\\begin{document}\n\n");
    fprintf(file, "\\begin{tikzpicture}[shorten >=1pt,auto,node distance=3cm,\n");
    fprintf(file, "                    semithick,\n");
    fprintf(file, "                    state/.style={circle,draw,minimum size=1.5cm}]\n\n");

    // Write vertices
    fprintf(file, "%% States\n");
    for (int i = 0; i < fsm->vertex_count; i++) {
        vertex_t *v = &fsm->vertices[i];

        fprintf(file, "\\node[state");
        if (v->is_initial) {
            fprintf(file, ",initial");
        }
        if (v->is_final) {
            fprintf(file, ",accepting");
        }
        fprintf(file, "] (q%d) at (%.2fcm,%.2fcm) {$%s$};\n", v->id, v->x, v->y, string_cstr(v->label));
    }

    fprintf(file, "\n%% Transitions\n");

    // Write edges
    for (int i = 0; i < fsm->edge_count; i++) {
        edge_t *e = &fsm->edges[i];

        if (e->is_self_loop) {
            // Self-loop
            fprintf(file, "\\path (q%d) edge [loop above] node {$%s$} (q%d);\n", e->from_vertex_id, string_cstr(e->label), e->to_vertex_id);
        } else if (e->is_curved) {
            // Curved edge
            fprintf(file,
                    "\\path (q%d) edge [bend left=%.0f] node {$%s$} (q%d);\n",
                    e->from_vertex_id,
                    e->curve_angle,
                    string_cstr(e->label),
                    e->to_vertex_id);
        } else {
            // Straight edge
            fprintf(file, "\\path (q%d) edge node {$%s$} (q%d);\n", e->from_vertex_id, string_cstr(e->label), e->to_vertex_id);
        }
    }

    // Write LaTeX footer
    fprintf(file, "\n\\end{tikzpicture}\n");
    fprintf(file, "\\end{document}\n");

    fclose(file);
    printf("LaTeX code generated successfully: %s\n", full_path);

    // Create Makefile in the latex directory
    string_t *makefile_path_str = string_create();
    if (!makefile_path_str) {
        string_destroy(filename_str);
        string_destroy(full_path_str);
        return 0; // Continue even if Makefile creation fails
    }

    string_result_t makefile_result = string_format(makefile_path_str, "%s/Makefile", latex_dir);
    if (makefile_result != STRING_SUCCESS) {
        string_destroy(filename_str);
        string_destroy(full_path_str);
        string_destroy(makefile_path_str);
        return 0; // Continue even if Makefile creation fails
    }

    string_copy_to_buffer(makefile_path_str, makefile_path, sizeof(makefile_path));
    FILE *makefile = fopen(makefile_path, "w");
    if (!makefile) {
        fprintf(stderr, "Warning: Cannot create Makefile at %s\n", makefile_path);
        string_destroy(filename_str);
        string_destroy(full_path_str);
        string_destroy(makefile_path_str);
    } else {
        fprintf(makefile, "COMPILER=lualatex\n");
        fprintf(makefile, "OUTPUT_PDF=%s\n", string_cstr(filename_str));
        fprintf(makefile, "INPUT_FILE=%s.tex\n\n", string_cstr(filename_str));
        
        fprintf(makefile, "all: pdf png\n\n");
        
        fprintf(makefile, "pdf:\n");
        fprintf(makefile, "\t$(COMPILER) -jobname=$(OUTPUT_PDF) $(INPUT_FILE)\n");
        fprintf(makefile, "\t$(COMPILER) -jobname=$(OUTPUT_PDF) $(INPUT_FILE)\n\n");
        
        fprintf(makefile, "png: pdf\n");
        fprintf(makefile, "\tpdftoppm $(OUTPUT_PDF).pdf $(OUTPUT_PDF) -png\n\n");
        
        fprintf(makefile, "clean:\n");
        fprintf(makefile, "\t$(RM) *.aux *.log *.out *.fls *.fdb_latexmk *.toc *.gz\n\n");
        
        fprintf(makefile, "clean_pdf:\n");
        fprintf(makefile, "\t$(RM) $(OUTPUT_PDF).pdf\n\n");
        
        fprintf(makefile, "clean_png:\n");
        fprintf(makefile, "\t$(RM) $(OUTPUT_PDF)-*.png\n\n");
        
        fprintf(makefile, "clean_all: clean clean_pdf clean_png\n\n");
        
        fprintf(makefile, ".PHONY: all pdf png clean clean_pdf clean_png clean_all\n");

        fclose(makefile);
        printf("Makefile created successfully: %s\n", makefile_path);

        string_destroy(filename_str);
        string_destroy(full_path_str);
        string_destroy(makefile_path_str);
    }

    return 0;
}

/**
 * @brief Print LaTeX/TikZ code to standard output
 * @param fsm Pointer to the FSM
 * @details Prints the complete LaTeX document to stdout for
 *          debugging or piping to other tools.
 */
void fsm_print_latex(fsm_t *fsm) {
    if (!fsm) return;

    printf("\\documentclass{article}\n");
    printf("\\usepackage{tikz}\n");
    printf("\\usetikzlibrary{automata,positioning,arrows.meta}\n");
    printf("\\tikzset{\n");
    printf("    >=Stealth,\n");
    printf("    every edge/.style={draw,->,>=Stealth,thick},\n");
    printf("    every loop/.style={draw,->,>=Stealth,thick}\n");
    printf("}\n");
    printf("\\begin{document}\n\n");
    printf("\\begin{tikzpicture}[shorten >=1pt,auto,node distance=3cm,\n");
    printf("                    semithick,\n");
    printf("                    state/.style={circle,draw,minimum size=1.5cm}]\n\n");

    // Print vertices
    printf("%% States\n");
    for (int i = 0; i < fsm->vertex_count; i++) {
        vertex_t *v = &fsm->vertices[i];

        printf("\\node[state");
        if (v->is_initial) {
            printf(",initial");
        }
        if (v->is_final) {
            printf(",accepting");
        }
        printf("] (q%d) at (%.2fcm,%.2fcm) {$%s$};\n", v->id, v->x, v->y, string_cstr(v->label));
    }

    printf("\n%% Transitions\n");

    // Print edges
    for (int i = 0; i < fsm->edge_count; i++) {
        edge_t *e = &fsm->edges[i];

        if (e->is_self_loop) {
            printf("\\path (q%d) edge [loop above] node {$%s$} (q%d);\n", e->from_vertex_id, string_cstr(e->label), e->to_vertex_id);
        } else if (e->is_curved) {
            printf(
                "\\path (q%d) edge [bend left=%.0f] node {$%s$} (q%d);\n", e->from_vertex_id, e->curve_angle, string_cstr(e->label), e->to_vertex_id);
        } else {
            printf("\\path (q%d) edge node {$%s$} (q%d);\n", e->from_vertex_id, string_cstr(e->label), e->to_vertex_id);
        }
    }

    printf("\n\\end{tikzpicture}\n");
    printf("\\end{document}\n");
}

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
void fsm_print_info(fsm_t *fsm) {
    if (!fsm) {
        printf("fsm_t is NULL\n");
        return;
    }

    printf("=== fsm_t: %s ===\n", string_cstr(fsm->name));
    printf("Vertices: %d\n", fsm->vertex_count);
    printf("Edges: %d\n\n", fsm->edge_count);

    printf("--- Vertices ---\n");
    for (int i = 0; i < fsm->vertex_count; i++) {
        vertex_t *v = &fsm->vertices[i];
        printf("ID: %d, Label: \"%s\", Position: (%.2f, %.2f), Radius: %.2f", v->id, string_cstr(v->label), v->x, v->y, v->radius);
        if (v->is_initial) printf(", INITIAL");
        if (v->is_final) printf(", FINAL");
        printf("\n");
    }

    printf("\n--- Edges ---\n");
    for (int i = 0; i < fsm->edge_count; i++) {
        edge_t *e = &fsm->edges[i];
        printf("ID: %d, From: %d, To: %d, Label: \"%s\"", e->id, e->from_vertex_id, e->to_vertex_id, string_cstr(e->label));
        if (e->is_curved) printf(", CURVED (%.0f°)", e->curve_angle);
        if (e->is_self_loop) printf(", SELF-LOOP");
        printf("\n");
    }
    printf("\n");
}
