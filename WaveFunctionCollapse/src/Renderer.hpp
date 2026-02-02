#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <SDL3/SDL.h>
#include "Core/Grid.hpp"
#include "Core/Tile.hpp"
#include <string>

/**
 * Structure représentant une couleur RGB
 */
struct Color {
    uint8_t r, g, b, a;

    Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 255)
        : r(red), g(green), b(blue), a(alpha) {}
};

/**
 * Classe gérant le rendu graphique avec SDL3
 * Affiche la grille WFC avec des rectangles colorés
 * 
 * NOTE SDL3 : SDL3 utilise des pointeurs opaques et des fonctions
 * qui retournent des bool au lieu de int
 */
class Renderer {
private:
    SDL_Window* window;           // Fenêtre SDL3
    SDL_Renderer* renderer;       // Renderer SDL3
    
    int windowWidth;              // Largeur de la fenêtre
    int windowHeight;             // Hauteur de la fenêtre
    
    int cellWidth;                // Largeur d'une cellule en pixels
    int cellHeight;               // Hauteur d'une cellule en pixels

    // Palette de couleurs pour chaque type de tuile
    Color colorWater;
    Color colorSand;
    Color colorGrass;
    Color colorForest;
    Color colorMountain;
    Color colorUnknown;           // Pour cellules non-collapsed

public:
    /**
     * Constructeur
     * @param title Titre de la fenêtre
     * @param width Largeur de la fenêtre
     * @param height Hauteur de la fenêtre
     */
    Renderer(const std::string& title, int width, int height);

    /**
     * Destructeur : libère les ressources SDL3
     */
    ~Renderer();

    /**
     * Initialise SDL3 et crée la fenêtre
     * @return true si l'initialisation réussit
     */
    bool Initialize();

    /**
     * Nettoie les ressources SDL3
     */
    void Cleanup();

    /**
     * Efface l'écran avec une couleur de fond
     */
    void Clear();

    /**
     * Affiche le rendu à l'écran
     */
    void Present();

    /**
     * Dessine la grille WFC
     * @param grid Grille à dessiner
     */
    void DrawGrid(const Grid& grid);

    /**
     * Calcule la taille des cellules en fonction de la grille
     * @param gridWidth Largeur de la grille
     * @param gridHeight Hauteur de la grille
     */
    void CalculateCellSize(size_t gridWidth, size_t gridHeight);

    /**
     * Récupère la couleur associée à un type de tuile
     * @param type Type de tuile
     * @return Couleur correspondante
     */
    Color GetColorForTile(TileType type) const;

    /**
     * Dessine un rectangle rempli
     * @param x Position X (pixels)
     * @param y Position Y (pixels)
     * @param w Largeur (pixels)
     * @param h Hauteur (pixels)
     * @param color Couleur du rectangle
     */
    void DrawRect(int x, int y, int w, int h, const Color& color);

    /**
     * Dessine le contour d'un rectangle
     * @param x Position X (pixels)
     * @param y Position Y (pixels)
     * @param w Largeur (pixels)
     * @param h Hauteur (pixels)
     * @param color Couleur du contour
     */
    void DrawRectOutline(int x, int y, int w, int h, const Color& color);

    /**
     * Vérifie si le renderer est initialisé
     * @return true si prêt à dessiner
     */
    bool IsReady() const {
        return window != nullptr && renderer != nullptr;
    }
};

#endif // RENDERER_HPP