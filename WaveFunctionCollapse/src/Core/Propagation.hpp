#ifndef PROPAGATION_HPP
#define PROPAGATION_HPP

#include "Grid.hpp"
#include "Tile.hpp"
#include <queue>
#include <set>
#include <utility>

/**
 * Classe gérant la propagation des contraintes dans la grille
 * Quand une cellule est observée (collapse), ses voisins doivent
 * réduire leurs possibilités pour rester compatibles
 * 
 * Utilise un algorithme de propagation par file (BFS)
 */
class Propagator {
private:
    const TileSet& tileSet;  // Référence vers les règles de tuiles

    /**
     * Récupère la direction opposée
     * @param dir Direction d'origine
     * @return Direction opposée
     */
    Direction GetOppositeDirection(Direction dir) const {
        switch (dir) {
            case Direction::NORTH: return Direction::SOUTH;
            case Direction::SOUTH: return Direction::NORTH;
            case Direction::EAST:  return Direction::WEST;
            case Direction::WEST:  return Direction::EAST;
            default: return Direction::NORTH;
        }
    }

public:
    /**
     * Constructeur
     * @param ts Référence vers le TileSet contenant les règles
     */
    Propagator(const TileSet& ts) : tileSet(ts) {}

    /**
     * Propage les contraintes depuis une cellule donnée
     * Utilise une file pour propager les changements de proche en proche
     * 
     * @param grid Grille à modifier
     * @param startX Position X de départ
     * @param startY Position Y de départ
     * @return true si la propagation réussit, false si contradiction
     */
    bool Propagate(Grid& grid, size_t startX, size_t startY) {
        // File de cellules à traiter (coordonnées x, y)
        std::queue<std::pair<size_t, size_t>> toProcess;
        
        // Ensemble des cellules déjà dans la file (évite les doublons)
        std::set<std::pair<size_t, size_t>> inQueue;

        // Ajouter la cellule de départ
        toProcess.push({startX, startY});
        inQueue.insert({startX, startY});

        // Tant qu'il y a des cellules à traiter
        while (!toProcess.empty()) {
            // Récupérer la prochaine cellule
            auto [x, y] = toProcess.front();
            toProcess.pop();
            inQueue.erase({x, y});

            const Cell& currentCell = grid.GetCell(x, y);

            // Si la cellule n'a plus de possibilités, c'est une contradiction
            if (currentCell.GetEntropy() == 0) {
                return false;
            }

            // Pour chaque direction (nord, est, sud, ouest)
            for (int d = 0; d < static_cast<int>(Direction::COUNT); ++d) {
                Direction dir = static_cast<Direction>(d);
                
                // Récupérer le voisin dans cette direction
                auto neighborCoords = grid.GetNeighborCoords(x, y, dir);
                if (!neighborCoords.has_value()) {
                    continue;  // Pas de voisin (bord de la grille)
                }

                auto [nx, ny] = neighborCoords.value();
                Cell& neighbor = grid.GetCell(nx, ny);

                // Si le voisin est déjà collapsed, pas besoin de le contraindre
                if (neighbor.IsCollapsed()) {
                    continue;
                }

                // Calculer les états autorisés pour le voisin
                // basés sur les états possibles de la cellule courante
                std::set<TileType> allowedStates = ComputeAllowedStates(
                    currentCell.GetPossibleStates(),
                    dir
                );

                // Si aucun état n'est autorisé, contradiction
                if (allowedStates.empty()) {
                    return false;
                }

                // Contraindre le voisin avec les nouveaux états autorisés
                bool changed = neighbor.Constrain(allowedStates);

                // Si le voisin a changé et n'est pas déjà dans la file
                if (changed && inQueue.find({nx, ny}) == inQueue.end()) {
                    toProcess.push({nx, ny});
                    inQueue.insert({nx, ny});
                }
            }
        }

        return true;  // Propagation réussie sans contradiction
    }

private:
    /**
     * Calcule les états autorisés pour une cellule voisine
     * basés sur les états possibles de la cellule courante
     * 
     * @param currentStates États possibles de la cellule courante
     * @param direction Direction vers le voisin
     * @return Ensemble des états autorisés pour le voisin
     */
    std::set<TileType> ComputeAllowedStates(
        const std::set<TileType>& currentStates,
        Direction direction
    ) const {
        std::set<TileType> allowedStates;

        // Pour chaque état possible de la cellule courante
        for (TileType currentType : currentStates) {
            const Tile& currentTile = tileSet.GetTile(currentType);
            
            // Récupérer les voisins compatibles dans cette direction
            const std::set<TileType>& compatibleNeighbors = 
                currentTile.GetCompatibleNeighbors(direction);
            
            // Ajouter tous les voisins compatibles aux états autorisés
            allowedStates.insert(
                compatibleNeighbors.begin(),
                compatibleNeighbors.end()
            );
        }

        return allowedStates;
    }

public:
    /**
     * Propage les contraintes depuis toutes les cellules collapsed
     * Utile pour initialiser la grille ou après un reset partiel
     * 
     * @param grid Grille à traiter
     * @return true si la propagation réussit, false si contradiction
     */
    bool PropagateAll(Grid& grid) {
        for (size_t y = 0; y < grid.GetHeight(); ++y) {
            for (size_t x = 0; x < grid.GetWidth(); ++x) {
                if (grid.GetCell(x, y).IsCollapsed()) {
                    if (!Propagate(grid, x, y)) {
                        return false;
                    }
                }
            }
        }
        return true;
    }

    /**
     * Vérifie si la grille est dans un état cohérent
     * (toutes les cellules ont au moins une possibilité)
     * 
     * @param grid Grille à vérifier
     * @return true si la grille est cohérente
     */
    bool IsConsistent(const Grid& grid) const {
        for (size_t y = 0; y < grid.GetHeight(); ++y) {
            for (size_t x = 0; x < grid.GetWidth(); ++x) {
                if (grid.GetCell(x, y).GetEntropy() == 0) {
                    return false;
                }
            }
        }
        return true;
    }
};

#endif // PROPAGATION_HPP