#include "App.hpp"
#include <iostream>

/**
 * Point d'entrée du programme
 * 
 * @param argc Nombre d'arguments
 * @param argv Tableau des arguments
 * @return Code de sortie (0 = succès)
 */
int main(int argc, char* argv[]) {
    std::cout << R"(
╔══════════════════════════════════════════════════════╗
║                                                      ║
║     🌊 WAVE FUNCTION COLLAPSE                        ║
║        Générateur Procédural de Terrain             ║
║        Powered by SDL3                               ║
║                                                      ║
╚══════════════════════════════════════════════════════╝
)" << std::endl;

    // Paramètres de l'application
    const int WINDOW_WIDTH = 800;
    const int WINDOW_HEIGHT = 800;
    const size_t GRID_WIDTH = 40;
    const size_t GRID_HEIGHT = 40;

    std::cout << "📊 Configuration:" << std::endl;
    std::cout << "   - Fenêtre: " << WINDOW_WIDTH << "x" << WINDOW_HEIGHT << " pixels" << std::endl;
    std::cout << "   - Grille: " << GRID_WIDTH << "x" << GRID_HEIGHT << " cellules" << std::endl;
    std::cout << "   - Bibliothèque: SDL3" << std::endl;
    std::cout << std::endl;

    // Créer l'application
    App app(WINDOW_WIDTH, WINDOW_HEIGHT, GRID_WIDTH, GRID_HEIGHT);

    // Initialiser
    if (!app.Initialize()) {
        std::cerr << "❌ Échec de l'initialisation de l'application" << std::endl;
        return 1;
    }

    // Lancer la boucle principale
    app.Run();

    std::cout << "\n👋 Merci d'avoir utilisé WFC !" << std::endl;
    return 0;
}

/**
 * ARCHITECTURE DU PROGRAMME :
 * 
 * main.cpp
 *    │
 *    └─> App (application principale)
 *          │
 *          ├─> Renderer (SDL3)
 *          │     └─> Dessine la grille
 *          │
 *          └─> WaveFunctionCollapse (algorithme)
 *                │
 *                ├─> Grid (grille de cellules)
 *                ├─> TileSet (règles)
 *                └─> Propagator (contraintes)
 * 
 * FLUX D'EXÉCUTION :
 * 
 * 1. main() crée App
 * 2. App.Initialize() prépare tout
 * 3. App.Run() lance la boucle :
 *    - HandleEvents() : gère clavier/souris
 *    - Update()       : exécute WFC
 *    - Render()       : dessine
 * 4. Quitter avec ESC ou fermeture fenêtre
 * 
 * TAILLE DE LA GRILLE :
 * - Petite (20x20)  : Génération rapide, idéal pour tester
 * - Moyenne (40x40) : Bon équilibre visualisation/performance
 * - Grande (100x100): Détails fins mais génération plus longue
 * 
 * Pour changer la taille, modifiez GRID_WIDTH et GRID_HEIGHT
 * 
 * SDL3 NOTES :
 * - Ce programme utilise SDL3, pas SDL2
 * - Différences principales : événements, timing, création de fenêtre
 * - Compatibilité : Compile avec clang++/g++ et SDL3
 */