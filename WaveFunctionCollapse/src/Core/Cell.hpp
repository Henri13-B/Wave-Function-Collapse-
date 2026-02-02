#ifndef CELL_HPP
#define CELL_HPP

#include "Tile.hpp"
#include <set>
#include <optional>

/**
 * Classe représentant une cellule de la grille
 * Une cellule peut être dans plusieurs états simultanément (superposition)
 * jusqu'à ce qu'elle soit "observée" (collapse)
 */
class Cell {
private:
    std::set<TileType> possibleStates;  // États possibles pour cette cellule
    bool collapsed;                      // true si la cellule a été observée
    TileType finalState;                 // État final après collapse

public:
    /**
     * Constructeur : initialise avec tous les états possibles
     * @param tileCount Nombre de types de tuiles disponibles
     */
    Cell(size_t tileCount) : collapsed(false), finalState(TileType::WATER) {
        // Au départ, tous les états sont possibles (superposition maximale)
        for (size_t i = 0; i < tileCount; ++i) {
            possibleStates.insert(static_cast<TileType>(i));
        }
    }

    /**
     * Vérifie si la cellule est dans un état déterminé
     * @return true si la cellule a été observée
     */
    bool IsCollapsed() const {
        return collapsed;
    }

    /**
     * Récupère l'entropie de la cellule (nombre d'états possibles)
     * Plus l'entropie est faible, plus la cellule est "déterminée"
     * @return Nombre d'états possibles
     */
    size_t GetEntropy() const {
        return possibleStates.size();
    }

    /**
     * Récupère les états possibles de la cellule
     * @return Ensemble des types de tuiles possibles
     */
    const std::set<TileType>& GetPossibleStates() const {
        return possibleStates;
    }

    /**
     * Récupère l'état final de la cellule (si collapsed)
     * @return État final, ou std::nullopt si pas encore collapsed
     */
    std::optional<TileType> GetState() const {
        if (collapsed) {
            return finalState;
        }
        return std::nullopt;
    }

    /**
     * "Observe" la cellule : choisit un état parmi les possibles
     * C'est le "collapse" de la fonction d'onde
     * @param state État choisi (doit être dans possibleStates)
     * @return true si le collapse a réussi
     */
    bool Collapse(TileType state) {
        // Vérifie que l'état choisi est possible
        if (possibleStates.find(state) == possibleStates.end()) {
            return false;
        }

        collapsed = true;
        finalState = state;
        possibleStates.clear();
        possibleStates.insert(state);
        return true;
    }

    /**
     * Restreint les états possibles selon une contrainte extérieure
     * Utilisé lors de la propagation des contraintes
     * @param allowedStates Nouveaux états autorisés
     * @return true si la cellule a changé (et n'est pas en contradiction)
     */
    bool Constrain(const std::set<TileType>& allowedStates) {
        if (collapsed) {
            // Si déjà collapsed, vérifie que l'état est toujours valide
            return allowedStates.find(finalState) != allowedStates.end();
        }

        // Calcule l'intersection : garde seulement les états à la fois
        // possibles ET autorisés
        std::set<TileType> newStates;
        for (TileType state : possibleStates) {
            if (allowedStates.find(state) != allowedStates.end()) {
                newStates.insert(state);
            }
        }

        // Si aucun état possible, c'est une contradiction
        if (newStates.empty()) {
            return false;
        }

        // Vérifie si la cellule a changé
        bool changed = (newStates.size() != possibleStates.size());
        possibleStates = newStates;

        // Si un seul état reste, on peut auto-collapse
        if (possibleStates.size() == 1) {
            collapsed = true;
            finalState = *possibleStates.begin();
        }

        return changed;
    }

    /**
     * Réinitialise la cellule à l'état de superposition maximale
     * @param tileCount Nombre de types de tuiles disponibles
     */
    void Reset(size_t tileCount) {
        collapsed = false;
        possibleStates.clear();
        for (size_t i = 0; i < tileCount; ++i) {
            possibleStates.insert(static_cast<TileType>(i));
        }
    }

    /**
     * Récupère une représentation textuelle pour debug
     * @return String représentant l'état de la cellule
     */
    std::string ToString() const {
        if (collapsed) {
            return "[" + std::to_string(static_cast<int>(finalState)) + "]";
        }
        return "{" + std::to_string(possibleStates.size()) + "}";
    }
};

#endif // CELL_HPP