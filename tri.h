#ifndef TRI_H
#define TRI_H

#include <stddef.h>
#include <time.h>

// Structure pour stocker le temps d'exécution
typedef struct {
    double time_taken;
} TriStats;

// Déclarations des fonctions de tri pour les entiers
void tri_bulle(int arr[], size_t n, TriStats *stats);
void tri_insertion(int arr[], size_t n, TriStats *stats);
void tri_shell(int arr[], size_t n, TriStats *stats);
void tri_quick_wrapper(int arr[], size_t n, TriStats *stats);

// Déclarations des fonctions de tri pour les réels (float)
void tri_bulle_float(float arr[], size_t n, TriStats *stats);
void tri_insertion_float(float arr[], size_t n, TriStats *stats);
void tri_shell_float(float arr[], size_t n, TriStats *stats);
void tri_quick_wrapper_float(float arr[], size_t n, TriStats *stats);

// Déclarations des fonctions de tri pour les caractères (char)
void tri_bulle_char(char arr[], size_t n, TriStats *stats);
void tri_insertion_char(char arr[], size_t n, TriStats *stats);
void tri_shell_char(char arr[], size_t n, TriStats *stats);
void tri_quick_wrapper_char(char arr[], size_t n, TriStats *stats);

// Déclarations des fonctions de tri pour les chaînes (char*)
void tri_bulle_string(char *arr[], size_t n, TriStats *stats);
void tri_insertion_string(char *arr[], size_t n, TriStats *stats);
void tri_shell_string(char *arr[], size_t n, TriStats *stats);
void tri_quick_wrapper_string(char *arr[], size_t n, TriStats *stats);

#endif // TRI_H
