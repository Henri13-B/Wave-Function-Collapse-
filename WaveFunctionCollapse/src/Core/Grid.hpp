#ifndef GRID_HPP
#define GRID_HPP

#include "Cell.hpp"
#include "Tile.hpp"
#include <vector>
#include <utility>
#include <optional>

/**
 * Classe représentant une grille 2D de cellules
 * Chaque cellule peut être dans plusieurs états (superposition)
 * La grille gère les relations de voisinage entre cellules
 */
class Grid {
private:
    size_t width;                      // Largeur de la grille
    size_t height;                     // Hauteur de la grille
    std::vector<std::vector<Cell>> cells;  // Grille 2D de cellules
    size_t tileCount;                  // Nombre de types de tuiles disponibles

public:
    /**
     * Constructeur : initialise une grille de taille donnée
     * @param w Largeur de la grille
     * @param h Hauteur de la grille
     * @param numTiles Nombre de types de tuiles disponibles
     */
    Grid(size_t w, size_t h, size_t numTiles) 
        : width(w), height(h), tileCount(numTiles) {
        
        // Initialiser la grille avec des cellules en superposition maximale
        cells.resize(height);
        for (size_t y = 0; y < height; ++y) {
            cells[y].reserve(width);
            for (size_t x = 0; x < width; ++x) {
                cells[y].emplace_back(numTiles);
            }
        }
    }

    /**
     * Récupère la largeur de la grille
     * @return Largeur
     */
    size_t GetWidth() const { return width; }

    /**
     * Récupère la hauteur de la grille
     * @return Hauteur
     */
    size_t GetHeight() const { return height; }

    /**
     * Récupère une cellule à une position donnée
     * @param x Position X
     * @param y Position Y
     * @return Référence vers la cellule
     */
    Cell& GetCell(size_t x, size_t y) {
        return cells[y][x];
    }

    /**
     * Récupère une cellule (version const)
     * @param x Position X
     * @param y Position Y
     * @return Référence const vers la cellule
     */
    const Cell& GetCell(size_t x, size_t y) const {
        return cells[y][x];
    }

    /**
     * Vérifie si une position est valide dans la grille
     * @param x Position X
     * @param y Position Y
     * @return true si la position est dans les limites
     */
    bool IsValid(int x, int y) const {
        return x >= 0 && x < static_cast<int>(width) && 
               y >= 0 && y < static_cast<int>(height);
    }

    /**
     * Récupère la cellule voisine dans une direction donnée
     * @param x Position X de la cellule de référence
     * @param y Position Y de la cellule de référence
     * @param dir Direction du voisin
     * @return Pointeur vers la cellule voisine, ou nullptr si hors limites
     */
    Cell* GetNeighbor(size_t x, size_t y, Direction dir) {
        int nx = static_cast<int>(x);
        int ny = static_cast<int>(y);

        // Calculer la position du voisin selon la direction
        switch (dir) {
            case Direction::NORTH: ny -= 1; break;
            case Direction::SOUTH: ny += 1; break;
            case Direction::EAST:  nx += 1; break;
            case Direction::WEST:  nx -= 1; break;
            default: return nullptr;
        }

        // Vérifier que le voisin est dans les limites
        if (!IsValid(nx, ny)) {
            return nullptr;
        }

        return &cells[ny][nx];
    }

    /**
     * Récupère la cellule voisine (version const)
     * @param x Position X de la cellule de référence
     * @param y Position Y de la cellule de référence
     * @param dir Direction du voisin
     * @return Pointeur const vers la cellule voisine, ou nullptr si hors limites
     */
    const Cell* GetNeighbor(size_t x, size_t y, Direction dir) const {
        int nx = static_cast<int>(x);
        int ny = static_cast<int>(y);

        switch (dir) {
            case Direction::NORTH: ny -= 1; break;
            case Direction::SOUTH: ny += 1; break;
            case Direction::EAST:  nx += 1; break;
            case Direction::WEST:  nx -= 1; break;
            default: return nullptr;
        }

        if (!IsValid(nx, ny)) {
            return nullptr;
        }

        return &cells[ny][nx];
    }

    /**
     * Récupère les coordonnées du voisin dans une direction
     * @param x Position X de la cellule de référence
     * @param y Position Y de la cellule de référence
     * @param dir Direction du voisin
     * @return Coordonnées (x, y) du voisin, ou std::nullopt si hors limites
     */
    std::optional<std::pair<size_t, size_t>> GetNeighborCoords(size_t x, size_t y, Direction dir) const {
        int nx = static_cast<int>(x);
        int ny = static_cast<int>(y);

        switch (dir) {
            case Direction::NORTH: ny -= 1; break;
            case Direction::SOUTH: ny += 1; break;
            case Direction::EAST:  nx += 1; break;
            case Direction::WEST:  nx -= 1; break;
            default: return std::nullopt;
        }

        if (!IsValid(nx, ny)) {
            return std::nullopt;
        }

        return std::make_pair(static_cast<size_t>(nx), static_cast<size_t>(ny));
    }

    /**
     * Vérifie si toutes les cellules sont collapsed (grille complète)
     * @return true si toutes les cellules sont dans un état déterminé
     */
    bool IsFullyCollapsed() const {
        for (size_t y = 0; y < height; ++y) {
            for (size_t x = 0; x < width; ++x) {
                if (!cells[y][x].IsCollapsed()) {
                    return false;
                }
            }
        }
        return true;
    }

    /**
     * Trouve la cellule avec l'entropie minimale (non collapsed)
     * C'est la cellule la plus "déterminée" qu'on va observer en priorité
     * @return Coordonnées (x, y) de la cellule, ou std::nullopt si toutes sont collapsed
     */
    std::optional<std::pair<size_t, size_t>> FindMinEntropyCell() const {
        size_t minEntropy = SIZE_MAX;
        std::optional<std::pair<size_t, size_t>> result = std::nullopt;

        for (size_t y = 0; y < height; ++y) {
            for (size_t x = 0; x < width; ++x) {
                const Cell& cell = cells[y][x];
                
                // Ignorer les cellules déjà collapsed
                if (cell.IsCollapsed()) {
                    continue;
                }

                size_t entropy = cell.GetEntropy();

                // Trouver la cellule avec l'entropie minimale
                // (le moins de possibilités = la plus contrainte)
                if (entropy < minEntropy && entropy > 0) {
                    minEntropy = entropy;
                    result = std::make_pair(x, y);
                }
            }
        }

        return result;
    }

    /**
     * Réinitialise toute la grille à l'état de superposition maximale
     */
    void Reset() {
        for (size_t y = 0; y < height; ++y) {
            for (size_t x = 0; x < width; ++x) {
                cells[y][x].Reset(tileCount);
            }
        }
    }

    /**
     * Compte le nombre de cellules collapsed
     * @return Nombre de cellules dans un état déterminé
     */
    size_t CountCollapsed() const {
        size_t count = 0;
        for (size_t y = 0; y < height; ++y) {
            for (size_t x = 0; x < width; ++x) {
                if (cells[y][x].IsCollapsed()) {
                    ++count;
                }
            }
        }
        return count;
    }

    /**
     * Récupère une représentation textuelle de la grille (pour debug)
     * @return String représentant l'état de la grille
     */
    std::string ToString() const {
        std::string result;
        for (size_t y = 0; y < height; ++y) {
            for (size_t x = 0; x < width; ++x) {
                result += cells[y][x].ToString() + " ";
            }
            result += "\n";
        }
        return result;
    }
};

#endif // GRID_HPP