#ifndef APP_HPP
#define APP_HPP

#include "Renderer.hpp"
#include "Core/WFC.hpp"
#include <SDL3/SDL.h>
#include <memory>

/**
 * Énumération des modes de génération
 */
enum class GenerationMode {
    MANUAL,        // Génération étape par étape (touche ESPACE)
    AUTO_SLOW,     // Génération automatique lente (visualisation)
    AUTO_FAST,     // Génération automatique rapide
    INSTANT        // Génération instantanée (tout d'un coup)
};

/**
 * Classe principale de l'application
 * Gère la boucle de jeu, les événements et l'état global
 * 
 * NOTE SDL3 : Utilise SDL3 pour les événements et le timing
 */
class App {
private:
    std::unique_ptr<Renderer> renderer;         // Moteur de rendu
    std::unique_ptr<WaveFunctionCollapse> wfc;  // Algorithme WFC
    
    bool running;                                // État de la boucle principale
    GenerationMode mode;                         // Mode de génération actuel
    
    // Paramètres de la grille
    size_t gridWidth;
    size_t gridHeight;
    
    // Timing pour le mode auto
    Uint64 lastStepTime;                         // Timestamp de la dernière étape
    Uint64 stepDelay;                            // Délai entre les étapes (ms)

public:
    /**
     * Constructeur
     * @param width Largeur de la fenêtre
     * @param height Hauteur de la fenêtre
     * @param gw Largeur de la grille
     * @param gh Hauteur de la grille
     */
    App(int width, int height, size_t gw, size_t gh);

    /**
     * Destructeur
     */
    ~App();

    /**
     * Initialise l'application
     * @return true si l'initialisation réussit
     */
    bool Initialize();

    /**
     * Lance la boucle principale
     */
    void Run();

    /**
     * Arrête l'application
     */
    void Quit();

private:
    /**
     * Gère les événements SDL (clavier, souris, fermeture)
     */
    void HandleEvents();

    /**
     * Met à jour la logique de l'application
     * @param deltaTime Temps écoulé depuis la dernière frame (ms)
     */
    void Update(Uint64 deltaTime);

    /**
     * Effectue le rendu
     */
    void Render();

    /**
     * Réinitialise la génération WFC
     */
    void Reset();

    /**
     * Exécute une étape de WFC
     */
    void StepWFC();

    /**
     * Change le mode de génération
     * @param newMode Nouveau mode
     */
    void SetMode(GenerationMode newMode);

    /**
     * Affiche les contrôles dans la console
     */
    void PrintControls();
};

#endif // APP_HPP