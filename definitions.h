#ifndef POWERS_H
#define POWERS_H

// Entier unique attribué à chaque pion

#define TYPE_KORO 0
#define TYPE_KITSUNE 1
#define TYPE_TANUKI 2
#define TYPE_KODOMA 3
#define TYPE_SAMURAI 4

// IA : Puissance des pions

#define AI_SAMURAI_POWER 10
#define AI_TANUKI_POWER 7
#define AI_KODOMA_POWER 3
#define AI_KITSUNE_POWER 5

// IA : Algorithme Minimax

#define AI_MAXTHREADS 4 // Nombre de threads executés à la fois
#define AI_DEPTH 6 // Profondeur
#define AI_MAX 10000
#define AI_MIN -10000

// IA : Fonction d'évaluation

#define AI_GLOBALPOWER 10 // Importance accordée aux pions vivants
#define AI_RESERVEPOWER 7 // Importance accordée à la réserve
#define AI_ZONEPOWER 0.4 // Importance accordée au contrôle du plâteau

#endif // POWERS_H
