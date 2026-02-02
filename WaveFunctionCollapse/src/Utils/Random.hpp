#ifndef RANDOM_HPP
#define RANDOM_HPP

#include <random>
#include <ctime>

/**
 * Classe utilitaire pour la génération de nombres aléatoires
 * Utilise le moteur Mersenne Twister (mt19937) pour une meilleure qualité
 */
class Random {
private:
    // Déclaration des membres statiques (définition dans un .cpp séparé serait mieux,
    // mais ici on utilise inline pour éviter les erreurs de linkage)
    static inline std::mt19937 generator;  // inline évite les multiple definitions
    static inline bool initialized = false;

public:
    /**
     * Initialise le générateur avec une seed
     * @param seed Graine pour le générateur (par défaut: temps actuel)
     */
    static void Init(unsigned int seed = static_cast<unsigned int>(std::time(nullptr))) {
        generator.seed(seed);
        initialized = true;
    }

    /**
     * Génère un entier aléatoire dans un intervalle [min, max]
     * @param min Valeur minimale (inclusive)
     * @param max Valeur maximale (inclusive)
     * @return Entier aléatoire dans [min, max]
     */
    static int Range(int min, int max) {
        if (!initialized) Init();
        std::uniform_int_distribution<int> dist(min, max);
        return dist(generator);
    }

    /**
     * Génère un nombre flottant aléatoire dans [0.0, 1.0]
     * @return Flottant aléatoire
     */
    static float Float() {
        if (!initialized) Init();
        std::uniform_real_distribution<float> dist(0.0f, 1.0f);
        return dist(generator);
    }

    /**
     * Génère un nombre flottant aléatoire dans [min, max]
     * @param min Valeur minimale
     * @param max Valeur maximale
     * @return Flottant aléatoire dans [min, max]
     */
    static float Range(float min, float max) {
        if (!initialized) Init();
        std::uniform_real_distribution<float> dist(min, max);
        return dist(generator);
    }

    /**
     * Retourne true avec une probabilité donnée
     * @param probability Probabilité entre 0.0 et 1.0
     * @return true si le tirage réussit
     */
    static bool Chance(float probability) {
        return Float() < probability;
    }
};

#endif // RANDOM_HPP