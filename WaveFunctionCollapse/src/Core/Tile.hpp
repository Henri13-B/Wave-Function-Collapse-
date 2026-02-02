#ifndef TILE_HPP
#define TILE_HPP

#include <vector>
#include <set>
#include <string>

/**
 * Enum représentant les différents types de tuiles
 * Chaque type aura une couleur et des règles de voisinage spécifiques
 */
enum class TileType {
    WATER = 0,      // Eau (bleu)
    SAND = 1,       // Sable (jaune)
    GRASS = 2,      // Herbe (vert)
    FOREST = 3,     // Forêt (vert foncé)
    MOUNTAIN = 4,   // Montagne (gris)
    COUNT           // Nombre total de types (pour itérations)
};

/**
 * Enum représentant les 4 directions cardinales
 */
enum class Direction {
    NORTH = 0,
    EAST = 1,
    SOUTH = 2,
    WEST = 3,
    COUNT = 4
};

/**
 * Structure représentant une tuile avec ses règles de compatibilité
 * Une tuile sait quels types de tuiles peuvent être placés à côté d'elle
 */
struct Tile {
    TileType type;                                    // Type de cette tuile
    std::string name;                                 // Nom lisible (debug)
    
    // Pour chaque direction, ensemble des types de tuiles compatibles
    // neighbors[Direction::NORTH] = {WATER, SAND} signifie que cette tuile
    // peut avoir WATER ou SAND au nord
    std::set<TileType> neighbors[static_cast<int>(Direction::COUNT)];

    /**
     * Constructeur
     * @param t Type de la tuile
     * @param n Nom de la tuile
     */
    Tile(TileType t, const std::string& n) : type(t), name(n) {}

    /**
     * Ajoute une compatibilité dans une direction
     * @param dir Direction concernée
     * @param compatible Type de tuile compatible dans cette direction
     */
    void AddNeighbor(Direction dir, TileType compatible) {
        neighbors[static_cast<int>(dir)].insert(compatible);
    }

    /**
     * Vérifie si un type de tuile est compatible dans une direction
     * @param dir Direction à vérifier
     * @param other Type de tuile à tester
     * @return true si compatible
     */
    bool IsCompatible(Direction dir, TileType other) const {
        const auto& validNeighbors = neighbors[static_cast<int>(dir)];
        return validNeighbors.find(other) != validNeighbors.end();
    }

    /**
     * Récupère tous les voisins compatibles dans une direction
     * @param dir Direction concernée
     * @return Ensemble des types compatibles
     */
    const std::set<TileType>& GetCompatibleNeighbors(Direction dir) const {
        return neighbors[static_cast<int>(dir)];
    }
};

/**
 * Classe utilitaire pour gérer l'ensemble des règles de tuiles
 * C'est ici qu'on définit quelles tuiles peuvent être à côté d'autres
 */
class TileSet {
private:
    std::vector<Tile> tiles;  // Ensemble de toutes les tuiles disponibles

public:
    /**
     * Constructeur : initialise les tuiles avec leurs règles
     */
    TileSet() {
        InitializeTiles();
    }

    /**
     * Récupère une tuile par son type
     * @param type Type de tuile recherché
     * @return Référence vers la tuile
     */
    const Tile& GetTile(TileType type) const {
        return tiles[static_cast<int>(type)];
    }

    /**
     * Retourne le nombre total de types de tuiles
     * @return Nombre de tuiles
     */
    size_t Count() const {
        return tiles.size();
    }

private:
    /**
     * Initialise les tuiles et leurs règles de voisinage
     * C'est ici qu'on définit la "logique" de notre monde
     */
    void InitializeTiles() {
        // Création des 5 types de tuiles
        tiles.emplace_back(TileType::WATER, "Water");
        tiles.emplace_back(TileType::SAND, "Sand");
        tiles.emplace_back(TileType::GRASS, "Grass");
        tiles.emplace_back(TileType::FOREST, "Forest");
        tiles.emplace_back(TileType::MOUNTAIN, "Mountain");

        // Définition des règles de compatibilité
        // Principe : on crée une transition naturelle eau -> sable -> herbe -> forêt -> montagne
        
        // WATER (eau) : peut être entouré d'eau ou de sable
        for (int d = 0; d < static_cast<int>(Direction::COUNT); ++d) {
            tiles[0].AddNeighbor(static_cast<Direction>(d), TileType::WATER);
            tiles[0].AddNeighbor(static_cast<Direction>(d), TileType::SAND);
        }

        // SAND (sable) : transition entre eau et terre
        for (int d = 0; d < static_cast<int>(Direction::COUNT); ++d) {
            tiles[1].AddNeighbor(static_cast<Direction>(d), TileType::WATER);
            tiles[1].AddNeighbor(static_cast<Direction>(d), TileType::SAND);
            tiles[1].AddNeighbor(static_cast<Direction>(d), TileType::GRASS);
        }

        // GRASS (herbe) : peut être avec sable, herbe ou forêt
        for (int d = 0; d < static_cast<int>(Direction::COUNT); ++d) {
            tiles[2].AddNeighbor(static_cast<Direction>(d), TileType::SAND);
            tiles[2].AddNeighbor(static_cast<Direction>(d), TileType::GRASS);
            tiles[2].AddNeighbor(static_cast<Direction>(d), TileType::FOREST);
        }

        // FOREST (forêt) : entre herbe et montagne
        for (int d = 0; d < static_cast<int>(Direction::COUNT); ++d) {
            tiles[3].AddNeighbor(static_cast<Direction>(d), TileType::GRASS);
            tiles[3].AddNeighbor(static_cast<Direction>(d), TileType::FOREST);
            tiles[3].AddNeighbor(static_cast<Direction>(d), TileType::MOUNTAIN);
        }

        // MOUNTAIN (montagne) : peut être avec forêt ou montagne
        for (int d = 0; d < static_cast<int>(Direction::COUNT); ++d) {
            tiles[4].AddNeighbor(static_cast<Direction>(d), TileType::FOREST);
            tiles[4].AddNeighbor(static_cast<Direction>(d), TileType::MOUNTAIN);
        }
    }
};

#endif // TILE_HPP