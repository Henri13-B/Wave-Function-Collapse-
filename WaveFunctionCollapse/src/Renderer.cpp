#include "Renderer.hpp"
#include <iostream>
#include <algorithm>

/**
 * Constructeur : initialise les couleurs et paramètres
 */
Renderer::Renderer(const std::string& title, int width, int height)
    : window(nullptr)
    , renderer(nullptr)
    , windowWidth(width)
    , windowHeight(height)
    , cellWidth(20)
    , cellHeight(20)
    // Palette de couleurs
    , colorWater(0, 105, 148)           // Bleu océan
    , colorSand(238, 214, 175)          // Beige sable
    , colorGrass(34, 139, 34)           // Vert prairie
    , colorForest(0, 100, 0)            // Vert foncé forêt
    , colorMountain(105, 105, 105)      // Gris montagne
    , colorUnknown(50, 50, 50)          // Gris foncé (non-collapsed)
{
}

/**
 * Destructeur
 */
Renderer::~Renderer() {
    Cleanup();
}

/**
 * Initialise SDL3
 * NOTE SDL3 : SDL_Init retourne bool (true = succès, false = échec)
 */
bool Renderer::Initialize() {
    // Initialiser SDL3 - ATTENTION : SDL_Init retourne bool en SDL3
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "❌ Erreur SDL_Init: " << SDL_GetError() << std::endl;
        return false;
    }

    // Créer la fenêtre - SDL3 utilise SDL_CreateWindow qui retourne nullptr en cas d'erreur
    window = SDL_CreateWindow(
        "Wave Function Collapse",
        windowWidth,
        windowHeight,
        SDL_WINDOW_RESIZABLE  // SDL3 : pas de SDL_WINDOW_SHOWN nécessaire
    );

    if (!window) {
        std::cerr << "❌ Erreur SDL_CreateWindow: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    // Créer le renderer - SDL3 : signature différente
    // SDL_CreateRenderer prend juste window et name (peut être nullptr)
    renderer = SDL_CreateRenderer(window, nullptr);

    if (!renderer) {
        std::cerr << "❌ Erreur SDL_CreateRenderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }

    // SDL3 : Activer VSync (optionnel mais recommandé)
    SDL_SetRenderVSync(renderer, 1);

    std::cout << "✅ SDL3 initialisé avec succès" << std::endl;
    return true;
}

/**
 * Nettoie les ressources SDL3
 */
void Renderer::Cleanup() {
    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }

    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }

    SDL_Quit();
    std::cout << "🧹 SDL3 nettoyé" << std::endl;
}

/**
 * Efface l'écran
 * SDL3 : SDL_SetRenderDrawColor retourne bool
 */
void Renderer::Clear() {
    // Fond noir
    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
    SDL_RenderClear(renderer);
}

/**
 * Affiche le rendu
 * SDL3 : Pas de changement ici
 */
void Renderer::Present() {
    SDL_RenderPresent(renderer);
}

/**
 * Calcule la taille des cellules
 */
void Renderer::CalculateCellSize(size_t gridWidth, size_t gridHeight) {
    // Calculer la taille maximale des cellules qui rentre dans la fenêtre
    cellWidth = windowWidth / static_cast<int>(gridWidth);
    cellHeight = windowHeight / static_cast<int>(gridHeight);

    // S'assurer que les cellules sont carrées
    int minSize = std::min(cellWidth, cellHeight);
    cellWidth = minSize;
    cellHeight = minSize;

    std::cout << "📐 Taille des cellules: " << cellWidth << "x" << cellHeight << std::endl;
}

/**
 * Récupère la couleur d'une tuile
 */
Color Renderer::GetColorForTile(TileType type) const {
    switch (type) {
        case TileType::WATER:    return colorWater;
        case TileType::SAND:     return colorSand;
        case TileType::GRASS:    return colorGrass;
        case TileType::FOREST:   return colorForest;
        case TileType::MOUNTAIN: return colorMountain;
        default:                 return colorUnknown;
    }
}

/**
 * Dessine un rectangle rempli
 * SDL3 : SDL_RenderFillRect retourne bool
 */
void Renderer::DrawRect(int x, int y, int w, int h, const Color& color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    
    // SDL3 : SDL_FRect pour les rectangles flottants
    SDL_FRect rect = {
        static_cast<float>(x), 
        static_cast<float>(y), 
        static_cast<float>(w), 
        static_cast<float>(h)
    };
    
    SDL_RenderFillRect(renderer, &rect);
}

/**
 * Dessine le contour d'un rectangle
 * SDL3 : SDL_RenderRect retourne bool
 */
void Renderer::DrawRectOutline(int x, int y, int w, int h, const Color& color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    
    SDL_FRect rect = {
        static_cast<float>(x), 
        static_cast<float>(y), 
        static_cast<float>(w), 
        static_cast<float>(h)
    };
    
    SDL_RenderRect(renderer, &rect);
}

/**
 * Dessine la grille WFC
 */
void Renderer::DrawGrid(const Grid& grid) {
    size_t width = grid.GetWidth();
    size_t height = grid.GetHeight();

    // Calculer la taille des cellules si pas encore fait
    if (cellWidth <= 0 || cellHeight <= 0) {
        CalculateCellSize(width, height);
    }

    // Dessiner chaque cellule
    for (size_t y = 0; y < height; ++y) {
        for (size_t x = 0; x < width; ++x) {
            const Cell& cell = grid.GetCell(x, y);

            // Calculer la position en pixels
            int pixelX = static_cast<int>(x) * cellWidth;
            int pixelY = static_cast<int>(y) * cellHeight;

            Color cellColor = colorUnknown;

            if (cell.IsCollapsed()) {
                // Cellule collapsed : afficher sa couleur finale
                auto state = cell.GetState();
                if (state.has_value()) {
                    cellColor = GetColorForTile(state.value());
                }
            } else {
                // Cellule en superposition : couleur basée sur l'entropie
                // Plus l'entropie est faible, plus c'est clair
                size_t entropy = cell.GetEntropy();
                float ratio = static_cast<float>(entropy) / 5.0f; // 5 = nombre max de tuiles
                
                uint8_t brightness = static_cast<uint8_t>(50 + 150 * ratio);
                cellColor = Color(brightness, brightness, brightness);
            }

            // Dessiner la cellule
            DrawRect(pixelX, pixelY, cellWidth, cellHeight, cellColor);

            // Dessiner une grille (contour noir fin)
            DrawRectOutline(pixelX, pixelY, cellWidth, cellHeight, Color(0, 0, 0, 100));
        }
    }
}

/**
 * NOTES SUR SDL3 vs SDL2 :
 * 
 * CHANGEMENTS PRINCIPAUX :
 * 1. SDL_Init() retourne bool (true = succès) au lieu de int
 * 2. SDL_CreateWindow() a une signature différente (pas de x,y ni flags SHOWN)
 * 3. SDL_CreateRenderer() prend (window, name) au lieu de (window, index, flags)
 * 4. SDL_SetRenderVSync() pour activer VSync
 * 5. SDL_FRect utilisé au lieu de SDL_Rect (float au lieu de int)
 * 6. Beaucoup de fonctions retournent bool au lieu de int
 * 
 * PALETTE DE COULEURS :
 * - WATER (eau)      : #006994 - Bleu océan
 * - SAND (sable)     : #EED6AF - Beige clair
 * - GRASS (herbe)    : #228B22 - Vert prairie
 * - FOREST (forêt)   : #006400 - Vert foncé
 * - MOUNTAIN (mont.) : #696969 - Gris pierre
 * 
 * Cette palette crée des transitions visuelles naturelles :
 * eau → sable → herbe → forêt → montagne
 */