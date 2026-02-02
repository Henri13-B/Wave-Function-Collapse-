#include "App.hpp"
#include <iostream>

/**
 * Constructeur
 */
App::App(int width, int height, size_t gw, size_t gh)
    : running(false)
    , mode(GenerationMode::MANUAL)
    , gridWidth(gw)
    , gridHeight(gh)
    , lastStepTime(0)
    , stepDelay(200)  // 200ms entre chaque étape en mode auto lent
{
    renderer = std::make_unique<Renderer>("Wave Function Collapse", width, height);
    wfc = std::make_unique<WaveFunctionCollapse>(gridWidth, gridHeight);
}

/**
 * Destructeur
 */
App::~App() {
}

/**
 * Initialise l'application
 */
bool App::Initialize() {
    std::cout << "🚀 Initialisation de l'application WFC..." << std::endl;

    // Initialiser le renderer
    if (!renderer->Initialize()) {
        std::cerr << "❌ Échec de l'initialisation du renderer" << std::endl;
        return false;
    }

    // Calculer la taille des cellules
    renderer->CalculateCellSize(gridWidth, gridHeight);

    // Initialiser WFC
    wfc->Initialize();

    // Afficher les contrôles
    PrintControls();

    running = true;
    return true;
}

/**
 * Boucle principale
 * SDL3 : Utilise SDL_GetTicksNS() pour le timing en nanosecondes
 */
void App::Run() {
    std::cout << "▶️  Lancement de la boucle principale" << std::endl;

    Uint64 lastTime = SDL_GetTicks();  // SDL3 : SDL_GetTicks() retourne Uint64 (millisecondes)

    while (running) {
        Uint64 currentTime = SDL_GetTicks();
        Uint64 deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        HandleEvents();
        Update(deltaTime);
        Render();

        // Limiter à ~60 FPS
        SDL_Delay(16);
    }

    std::cout << "⏹️  Boucle principale terminée" << std::endl;
}

/**
 * Arrête l'application
 */
void App::Quit() {
    running = false;
}

/**
 * Gère les événements
 * SDL3 : SDL_PollEvent retourne bool au lieu de int
 */
void App::HandleEvents() {
    SDL_Event event;

    // SDL3 : SDL_PollEvent retourne bool (true = événement disponible)
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_EVENT_QUIT:  // SDL3 : SDL_EVENT_QUIT au lieu de SDL_QUIT
                Quit();
                break;

            case SDL_EVENT_KEY_DOWN:  // SDL3 : SDL_EVENT_KEY_DOWN au lieu de SDL_KEYDOWN
                // SDL3 : event.key.key au lieu de event.key.keysym.sym
                switch (event.key.key) {
                    case SDLK_ESCAPE:
                        // ESC : quitter
                        Quit();
                        break;

                    case SDLK_R:
                        // R : reset
                        Reset();
                        break;

                    case SDLK_SPACE:
                        // ESPACE : étape suivante (mode manuel)
                        if (mode == GenerationMode::MANUAL) {
                            StepWFC();
                        }
                        break;

                    case SDLK_1:
                        // 1 : mode manuel
                        SetMode(GenerationMode::MANUAL);
                        break;

                    case SDLK_2:
                        // 2 : mode auto lent
                        SetMode(GenerationMode::AUTO_SLOW);
                        break;

                    case SDLK_3:
                        // 3 : mode auto rapide
                        SetMode(GenerationMode::AUTO_FAST);
                        break;

                    case SDLK_4:
                        // 4 : mode instant
                        SetMode(GenerationMode::INSTANT);
                        break;

                    default:
                        break;
                }
                break;

            default:
                break;
        }
    }
}

/**
 * Met à jour la logique
 */
void App::Update(Uint64 deltaTime) {
    Uint64 currentTime = SDL_GetTicks();

    // Génération automatique
    if (mode == GenerationMode::AUTO_SLOW || mode == GenerationMode::AUTO_FAST) {
        if (currentTime - lastStepTime >= stepDelay) {
            StepWFC();
            lastStepTime = currentTime;
        }
    }
    else if (mode == GenerationMode::INSTANT) {
        // Générer tout d'un coup
        if (!wfc->IsFinished()) {
            wfc->Run();
            SetMode(GenerationMode::MANUAL);  // Repasser en mode manuel après
        }
    }
}

/**
 * Effectue le rendu
 */
void App::Render() {
    renderer->Clear();
    renderer->DrawGrid(wfc->GetGrid());
    renderer->Present();
}

/**
 * Réinitialise la génération
 */
void App::Reset() {
    std::cout << "\n🔄 Réinitialisation..." << std::endl;
    wfc->Initialize();
    lastStepTime = SDL_GetTicks();
}

/**
 * Exécute une étape WFC
 */
void App::StepWFC() {
    if (wfc->IsFinished()) {
        if (wfc->IsSuccessful()) {
            std::cout << "✅ Génération terminée avec succès !" << std::endl;
        } else {
            std::cout << "❌ Contradiction détectée" << std::endl;
        }
        return;
    }

    wfc->Step();
}

/**
 * Change le mode de génération
 */
void App::SetMode(GenerationMode newMode) {
    mode = newMode;

    switch (mode) {
        case GenerationMode::MANUAL:
            std::cout << "📝 Mode: MANUEL (appuyez sur ESPACE)" << std::endl;
            break;

        case GenerationMode::AUTO_SLOW:
            std::cout << "🐢 Mode: AUTO LENT" << std::endl;
            stepDelay = 200;  // 200ms entre chaque étape
            lastStepTime = SDL_GetTicks();
            break;

        case GenerationMode::AUTO_FAST:
            std::cout << "🚀 Mode: AUTO RAPIDE" << std::endl;
            stepDelay = 50;   // 50ms entre chaque étape
            lastStepTime = SDL_GetTicks();
            break;

        case GenerationMode::INSTANT:
            std::cout << "⚡ Mode: INSTANT" << std::endl;
            break;
    }
}

/**
 * Affiche les contrôles
 */
void App::PrintControls() {
    std::cout << "\n════════════════════════════════════════" << std::endl;
    std::cout << "   🎮 CONTRÔLES" << std::endl;
    std::cout << "════════════════════════════════════════" << std::endl;
    std::cout << "  R         : Réinitialiser" << std::endl;
    std::cout << "  ESPACE    : Étape suivante (mode manuel)" << std::endl;
    std::cout << "  1         : Mode manuel" << std::endl;
    std::cout << "  2         : Mode auto lent" << std::endl;
    std::cout << "  3         : Mode auto rapide" << std::endl;
    std::cout << "  4         : Mode instant" << std::endl;
    std::cout << "  ESC       : Quitter" << std::endl;
    std::cout << "════════════════════════════════════════\n" << std::endl;
}

/**
 * NOTES SUR SDL3 vs SDL2 :
 * 
 * CHANGEMENTS DANS LES ÉVÉNEMENTS :
 * - SDL_QUIT          → SDL_EVENT_QUIT
 * - SDL_KEYDOWN       → SDL_EVENT_KEY_DOWN
 * - event.key.keysym.sym → event.key.key
 * - SDL_PollEvent() retourne bool au lieu de int
 * 
 * CHANGEMENTS DANS LE TIMING :
 * - SDL_GetTicks() retourne Uint64 au lieu de Uint32
 * - Nouveau : SDL_GetTicksNS() pour nanosecondes (plus précis)
 * 
 * MODES DE GÉNÉRATION :
 * - MANUAL     : L'utilisateur contrôle chaque étape (ESPACE)
 * - AUTO_SLOW  : Génération automatique avec visualisation (200ms/étape)
 * - AUTO_FAST  : Génération automatique rapide (50ms/étape)
 * - INSTANT    : Génération complète instantanée
 */