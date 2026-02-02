#ifndef WFC_HPP
#define WFC_HPP

#include "Grid.hpp"
#include "Tile.hpp"
#include "Propagation.hpp"
#include "../Utils/Random.hpp"
#include <memory>

/**
 * Énumération représentant l'état de l'algorithme WFC
 */
enum class WFCState {
    NOT_STARTED,    // Algorithme pas encore lancé
    RUNNING,        // En cours de génération
    COMPLETED,      // Génération terminée avec succès
    CONTRADICTION   // Contradiction détectée (échec)
};

/**
 * Classe implémentant l'algorithme Wave Function Collapse
 * 
 * Algorithme principal :
 * 1. Tant que des cellules ne sont pas collapsed :
 *    a. Trouver la cellule avec l'entropie minimale
 *    b. "Observer" cette cellule (choisir un état aléatoire)
 *    c. Propager les contraintes aux voisins
 * 2. Si contradiction, échec
 * 3. Si toutes les cellules sont collapsed, succès
 */
class WaveFunctionCollapse {
private:
    std::unique_ptr<Grid> grid;           // Grille de cellules
    std::unique_ptr<TileSet> tileSet;     // Ensemble des règles de tuiles
    std::unique_ptr<Propagator> propagator; // Gestionnaire de propagation
    
    WFCState state;                        // État actuel de l'algorithme
    size_t stepCount;                      // Nombre d'étapes effectuées

public:
    /**
     * Constructeur
     * @param width Largeur de la grille
     * @param height Hauteur de la grille
     */
    WaveFunctionCollapse(size_t width, size_t height);

    /**
     * Destructeur
     */
    ~WaveFunctionCollapse();

    /**
     * Initialise l'algorithme (ou réinitialise)
     */
    void Initialize();

    /**
     * Exécute une étape de l'algorithme WFC
     * @return true si l'étape a réussi, false sinon
     */
    bool Step();

    /**
     * Exécute l'algorithme complet jusqu'à la fin
     * @param maxSteps Nombre maximum d'étapes (sécurité contre boucle infinie)
     * @return true si la génération a réussi
     */
    bool Run(size_t maxSteps = 10000);

    /**
     * Récupère l'état actuel de l'algorithme
     * @return État (NOT_STARTED, RUNNING, COMPLETED, CONTRADICTION)
     */
    WFCState GetState() const { return state; }

    /**
     * Récupère la grille
     * @return Référence constante vers la grille
     */
    const Grid& GetGrid() const { return *grid; }

    /**
     * Récupère le nombre d'étapes effectuées
     * @return Nombre d'étapes
     */
    size_t GetStepCount() const { return stepCount; }

    /**
     * Vérifie si la génération est terminée
     * @return true si terminée (succès ou échec)
     */
    bool IsFinished() const {
        return state == WFCState::COMPLETED || state == WFCState::CONTRADICTION;
    }

    /**
     * Vérifie si la génération a réussi
     * @return true si toutes les cellules sont collapsed sans contradiction
     */
    bool IsSuccessful() const {
        return state == WFCState::COMPLETED;
    }

private:
    /**
     * Observe une cellule (collapse de la fonction d'onde)
     * Choisit aléatoirement un état parmi les possibles
     * 
     * @param x Position X de la cellule
     * @param y Position Y de la cellule
     * @return true si l'observation a réussi
     */
    bool ObserveCell(size_t x, size_t y);

    /**
     * Choisit aléatoirement un état parmi un ensemble
     * @param possibleStates Ensemble des états possibles
     * @return État choisi
     */
    TileType ChooseRandomState(const std::set<TileType>& possibleStates);
};

#endif // WFC_HPP