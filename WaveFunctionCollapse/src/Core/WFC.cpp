#include "WFC.hpp"
#include <iostream>
#include <vector>

/**
 * Constructeur : initialise les composants
 */
WaveFunctionCollapse::WaveFunctionCollapse(size_t width, size_t height)
    : state(WFCState::NOT_STARTED), stepCount(0) {
    
    // Créer le TileSet avec les règles
    tileSet = std::make_unique<TileSet>();
    
    // Créer la grille
    grid = std::make_unique<Grid>(width, height, tileSet->Count());
    
    // Créer le propagateur
    propagator = std::make_unique<Propagator>(*tileSet);
    
    // Initialiser le générateur aléatoire
    Random::Init();
}

/**
 * Destructeur
 */
WaveFunctionCollapse::~WaveFunctionCollapse() {
    // Les unique_ptr se détruisent automatiquement
}

/**
 * Initialise (ou réinitialise) l'algorithme
 */
void WaveFunctionCollapse::Initialize() {
    // Réinitialiser la grille
    grid->Reset();
    
    // Réinitialiser l'état
    state = WFCState::NOT_STARTED;
    stepCount = 0;
    
    std::cout << "✅ WFC initialisé : grille " 
              << grid->GetWidth() << "x" << grid->GetHeight() << std::endl;
}

/**
 * Exécute une étape de l'algorithme
 */
bool WaveFunctionCollapse::Step() {
    // Si déjà terminé, ne rien faire
    if (IsFinished()) {
        return false;
    }

    // Marquer comme en cours
    if (state == WFCState::NOT_STARTED) {
        state = WFCState::RUNNING;
    }

    // 1. Vérifier si la grille est complète
    if (grid->IsFullyCollapsed()) {
        state = WFCState::COMPLETED;
        std::cout << "✅ Génération terminée en " << stepCount << " étapes" << std::endl;
        return true;
    }

    // 2. Trouver la cellule avec l'entropie minimale
    auto minEntropyCell = grid->FindMinEntropyCell();
    
    if (!minEntropyCell.has_value()) {
        // Pas de cellule à observer (toutes sont collapsed)
        state = WFCState::COMPLETED;
        return true;
    }

    auto [x, y] = minEntropyCell.value();

    // 3. Observer la cellule (collapse)
    if (!ObserveCell(x, y)) {
        // Échec de l'observation (pas d'états possibles)
        state = WFCState::CONTRADICTION;
        std::cerr << "❌ Contradiction à l'étape " << stepCount 
                  << " à la position (" << x << ", " << y << ")" << std::endl;
        return false;
    }

    // 4. Propager les contraintes
    if (!propagator->Propagate(*grid, x, y)) {
        // Échec de la propagation (contradiction)
        state = WFCState::CONTRADICTION;
        std::cerr << "❌ Contradiction lors de la propagation à l'étape " 
                  << stepCount << std::endl;
        return false;
    }

    // Incrémenter le compteur d'étapes
    ++stepCount;

    return true;
}

/**
 * Exécute l'algorithme complet
 */
bool WaveFunctionCollapse::Run(size_t maxSteps) {
    std::cout << "🚀 Démarrage de la génération WFC..." << std::endl;

    // Réinitialiser si nécessaire
    if (state != WFCState::NOT_STARTED) {
        Initialize();
    }

    // Exécuter jusqu'à la fin ou jusqu'à maxSteps
    while (!IsFinished() && stepCount < maxSteps) {
        if (!Step()) {
            return false;  // Échec
        }
    }

    // Vérifier si on a dépassé le nombre max d'étapes
    if (stepCount >= maxSteps) {
        std::cerr << "⚠️  Nombre maximum d'étapes atteint (" << maxSteps << ")" << std::endl;
        state = WFCState::CONTRADICTION;
        return false;
    }

    return IsSuccessful();
}

/**
 * Observe une cellule : choisit un état parmi les possibles
 */
bool WaveFunctionCollapse::ObserveCell(size_t x, size_t y) {
    Cell& cell = grid->GetCell(x, y);

    // Vérifier que la cellule n'est pas déjà collapsed
    if (cell.IsCollapsed()) {
        return true;
    }

    // Récupérer les états possibles
    const std::set<TileType>& possibleStates = cell.GetPossibleStates();

    // Si aucun état possible, c'est une contradiction
    if (possibleStates.empty()) {
        return false;
    }

    // Choisir aléatoirement un état
    TileType chosenState = ChooseRandomState(possibleStates);

    // Effectuer le collapse
    bool success = cell.Collapse(chosenState);

    if (success) {
        std::cout << "🔍 Étape " << stepCount << ": Cellule (" << x << ", " << y 
                  << ") collapsed vers état " << static_cast<int>(chosenState) 
                  << " (" << possibleStates.size() << " possibilités)" << std::endl;
    }

    return success;
}

/**
 * Choisit aléatoirement un état parmi un ensemble
 */
TileType WaveFunctionCollapse::ChooseRandomState(const std::set<TileType>& possibleStates) {
    // Convertir le set en vector pour accès par indice
    std::vector<TileType> states(possibleStates.begin(), possibleStates.end());

    // Choisir un indice aléatoire
    int randomIndex = Random::Range(0, static_cast<int>(states.size()) - 1);

    return states[randomIndex];
}

/**
 * Note sur l'algorithme :
 * 
 * Le WFC fonctionne comme suit :
 * 
 * 1. INITIALISATION
 *    - Toutes les cellules sont en superposition (tous les états possibles)
 * 
 * 2. BOUCLE PRINCIPALE
 *    a. Trouver la cellule avec l'entropie minimale (le moins de possibilités)
 *       → Heuristique : observer les cellules les plus contraintes en premier
 *    
 *    b. OBSERVATION (collapse)
 *       → Choisir aléatoirement un état parmi les possibles
 *       → La cellule passe de superposition à un état déterminé
 *    
 *    c. PROPAGATION
 *       → Mettre à jour les voisins pour qu'ils restent compatibles
 *       → Utilise une file BFS pour propager de proche en proche
 *       → Si un voisin change, ajouter ses voisins à la file
 * 
 * 3. CONDITIONS D'ARRÊT
 *    - Succès : toutes les cellules sont collapsed
 *    - Échec : une cellule n'a plus de possibilités (contradiction)
 * 
 * EXEMPLE D'EXÉCUTION :
 * 
 * Étape 0 (initial) :
 * [WSGFM] [WSGFM] [WSGFM]
 * [WSGFM] [WSGFM] [WSGFM]
 * [WSGFM] [WSGFM] [WSGFM]
 * 
 * Étape 1 (observer cellule centrale) :
 * [WSG..] [WSG..] [WSG..]
 * [WSG..] [ GRASS] [WSG..]
 * [WSG..] [WSG..] [WSG..]
 * 
 * Étape 2 (propager) :
 * [.SG..] [.SG..] [.SG..]
 * [.SG..] [ GRASS] [.SG..]
 * [.SG..] [.SG..] [.SG..]
 * 
 * ... jusqu'à ce que toutes les cellules soient déterminées
 */