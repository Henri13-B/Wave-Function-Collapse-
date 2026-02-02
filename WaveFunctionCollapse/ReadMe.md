# 🌊 Wave Function Collapse

**Générateur procédural de terrains 2D** utilisant l'algorithme Wave Function Collapse, implémenté en C++ avec SDL3.

![Language](https://img.shields.io/badge/C%2B%2B-17-blue)
![SDL](https://img.shields.io/badge/SDL-3-green)
![License](https://img.shields.io/badge/license-MIT-brightgreen)

---

## 📖 Table des matières

- [À propos](#à-propos)
- [Fonctionnalités](#fonctionnalités)
- [Prérequis](#prérequis)
- [Installation](#installation)
- [Compilation](#compilation)
- [Utilisation](#utilisation)
- [Architecture](#architecture)
- [Algorithme](#algorithme)
- [Personnalisation](#personnalisation)
- [Contributeurs](#contributeurs)
- [License](#license)

---

## 🎯 À propos

Ce projet implémente l'algorithme **Wave Function Collapse (WFC)**, inspiré de la mécanique quantique, pour générer des terrains procéduraux cohérents. L'algorithme garantit que chaque tuile respecte les règles de voisinage définies, créant ainsi des paysages réalistes sans intervention manuelle.

### Qu'est-ce que le Wave Function Collapse ?

Le WFC est un algorithme de génération procédurale qui fonctionne en trois étapes :

1. **Superposition** : Chaque cellule peut être dans plusieurs états simultanément
2. **Observation** : Choix aléatoire d'un état pour la cellule la plus contrainte
3. **Propagation** : Mise à jour des cellules voisines selon les règles de compatibilité

---

## ✨ Fonctionnalités

- ✅ **Génération procédurale** de terrains 2D cohérents
- ✅ **5 types de tuiles** : Eau, Sable, Herbe, Forêt, Montagne
- ✅ **4 modes de génération** : Manuel, Auto lent, Auto rapide, Instant
- ✅ **Visualisation en temps réel** avec SDL3
- ✅ **Transitions naturelles** entre les biomes
- ✅ **Interface clavier intuitive**
- ✅ **Code bien commenté** et documenté

---

## 🔧 Prérequis

### Logiciels nécessaires

| Outil | Version minimum | Installation |
|-------|-----------------|--------------|
| **C++ Compiler** | C++17 | clang++ ou g++ |
| **SDL3** | 3.0+ | [Installation SDL3](#installation-de-sdl3) |
| **Python** | 3.7+ | Pour le script de build |
| **Git** | 2.0+ | (optionnel) Pour cloner le projet |

### Installation de SDL3

#### Windows (MSYS2)
```bash
pacman -S mingw-w64-x86_64-SDL3
```

#### Linux (Ubuntu/Debian)
```bash
# Compiler depuis les sources
git clone https://github.com/libsdl-org/SDL.git -b main
cd SDL
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make
sudo make install
```

#### macOS (Homebrew)
```bash
brew install sdl3
```

---

## 📦 Installation

### 1. Cloner le projet
```bash
git clone https://github.com/votre-nom/WaveFunctionCollapse.git
cd WaveFunctionCollapse
```

### 2. Vérifier la structure
```
WaveFunctionCollapse/
├── src/
│   ├── main.cpp
│   ├── App.hpp / App.cpp
│   ├── Renderer.hpp / Renderer.cpp
│   ├── Core/
│   │   ├── Tile.hpp
│   │   ├── Cell.hpp
│   │   ├── Grid.hpp
│   │   ├── WFC.hpp / WFC.cpp
│   │   └── Propagation.hpp
│   └── Utils/
│       └── Random.hpp
├── build.py
├── run.bat
├── run.sh
└── README.md
```

---

## 🔨 Compilation

### Méthode 1 : Script Python (recommandé)

```bash
# Compilation standard (mode debug)
python build.py

# Compilation optimisée (mode release)
python build.py --release

# Nettoyer avant compilation
python build.py --clean

# Afficher les détails de compilation
python build.py --verbose
```

### Méthode 2 : Compilation manuelle

#### Windows (MSYS2)
```bash
clang++ src/main.cpp src/App.cpp src/Renderer.cpp src/Core/WFC.cpp \
  -o build/WFC.exe \
  -std=c++17 \
  -Isrc -Isrc/Core -Isrc/Utils \
  -lmingw32 -lSDL3 \
  -Wall -O2
```

#### Linux
```bash
clang++ src/main.cpp src/App.cpp src/Renderer.cpp src/Core/WFC.cpp \
  -o build/WFC \
  -std=c++17 \
  -Isrc -Isrc/Core -Isrc/Utils \
  -lSDL3 -lm \
  -Wall -O2
```

#### macOS
```bash
clang++ src/main.cpp src/App.cpp src/Renderer.cpp src/Core/WFC.cpp \
  -o build/WFC \
  -std=c++17 \
  -Isrc -Isrc/Core -Isrc/Utils \
  -lSDL3 -framework Cocoa \
  -Wall -O2
```

---

## 🚀 Utilisation

### Lancer le programme

#### Windows
```bash
run.bat
# ou
.\build\WFC.exe
```

#### Linux/macOS
```bash
./run.sh
# ou
./build/WFC
```

### Contrôles clavier

| Touche | Action |
|--------|--------|
| **R** | Réinitialiser la grille |
| **ESPACE** | Étape suivante (mode manuel) |
| **1** | Mode manuel |
| **2** | Mode auto lent (200ms/étape) |
| **3** | Mode auto rapide (50ms/étape) |
| **4** | Mode instant (génération complète) |
| **ESC** | Quitter l'application |

### Modes de génération

- **Manuel (1)** : Vous contrôlez chaque étape avec ESPACE
- **Auto lent (2)** : Génération automatique, idéale pour observer l'algorithme
- **Auto rapide (3)** : Génération rapide mais visible
- **Instant (4)** : Génération complète immédiate

---

## 🏗️ Architecture

### Structure du code

```
┌─────────────────────────────────────────────┐
│              main.cpp                        │
│         (Point d'entrée)                     │
└──────────────────┬──────────────────────────┘
                   │
         ┌─────────▼─────────┐
         │       App         │
         │  (Boucle de jeu)  │
         └───┬───────────┬───┘
             │           │
      ┌──────▼─────┐ ┌──▼──────────┐
      │  Renderer  │ │     WFC     │
      │   (SDL3)   │ │ (Algorithme)│
      └────────────┘ └──┬──────────┘
                        │
           ┌────────────┼────────────┐
           │            │            │
      ┌────▼───┐   ┌───▼────┐  ┌───▼────────┐
      │  Grid  │   │TileSet │  │Propagator  │
      └────┬───┘   └────────┘  └────────────┘
           │
      ┌────▼───┐
      │  Cell  │
      └────────┘
```

### Modules principaux

| Module | Fichiers | Responsabilité |
|--------|----------|----------------|
| **Core** | `WFC.hpp/cpp` | Algorithme principal |
| **Data** | `Grid.hpp`, `Cell.hpp`, `Tile.hpp` | Structures de données |
| **Logic** | `Propagation.hpp` | Propagation des contraintes |
| **Render** | `Renderer.hpp/cpp` | Affichage SDL3 |
| **App** | `App.hpp/cpp` | Boucle de jeu et événements |
| **Utils** | `Random.hpp` | Générateur aléatoire |

---

## 🧮 Algorithme

### Pseudocode

```python
fonction WFC():
    # 1. Initialisation
    pour chaque cellule dans grille:
        cellule.états_possibles = TOUS_LES_TYPES
    
    # 2. Boucle principale
    tant que grille_non_complète():
        # a. Trouver la cellule avec l'entropie minimale
        cellule = trouver_min_entropie()
        
        # b. Observer (collapse)
        état = choisir_aléatoirement(cellule.états_possibles)
        cellule.collapse(état)
        
        # c. Propager les contraintes
        pour voisin dans cellule.voisins:
            voisin.contraindre_selon_règles(cellule)
            si voisin.changé:
                ajouter_à_file(voisin)
        
        # d. Propager récursivement
        tant que file_non_vide():
            cell = file.défiler()
            pour voisin dans cell.voisins:
                voisin.contraindre_selon_règles(cell)
                si voisin.changé:
                    ajouter_à_file(voisin)
```

### Règles de compatibilité

Les tuiles respectent ces transitions naturelles :

```
EAU ←→ SABLE ←→ HERBE ←→ FORÊT ←→ MONTAGNE
```

Détails :
- **Eau** : Peut être adjacente à Eau ou Sable
- **Sable** : Peut être adjacente à Eau, Sable ou Herbe
- **Herbe** : Peut être adjacente à Sable, Herbe ou Forêt
- **Forêt** : Peut être adjacente à Herbe, Forêt ou Montagne
- **Montagne** : Peut être adjacente à Forêt ou Montagne

---

## 🎨 Personnalisation

### Changer la taille de la grille

Modifier dans `src/main.cpp` :

```cpp
const size_t GRID_WIDTH = 40;   // Largeur (ex: 20, 40, 100)
const size_t GRID_HEIGHT = 40;  // Hauteur
```

### Ajouter de nouveaux types de tuiles

1. **Ajouter dans `src/Core/Tile.hpp`** :
```cpp
enum class TileType {
    WATER = 0,
    SAND = 1,
    GRASS = 2,
    FOREST = 3,
    MOUNTAIN = 4,
    SNOW = 5,      // Nouveau type
    COUNT
};
```

2. **Définir les règles dans `TileSet::InitializeTiles()`** :
```cpp
// SNOW peut être adjacent à MOUNTAIN ou SNOW
for (int d = 0; d < static_cast<int>(Direction::COUNT); ++d) {
    tiles[5].AddNeighbor(static_cast<Direction>(d), TileType::MOUNTAIN);
    tiles[5].AddNeighbor(static_cast<Direction>(d), TileType::SNOW);
}
```

3. **Ajouter une couleur dans `src/Renderer.cpp`** :
```cpp
Color colorSnow(240, 240, 255);  // Blanc neige
```

### Modifier les couleurs

Dans `src/Renderer.cpp`, modifiez le constructeur :

```cpp
Renderer::Renderer(...)
    : colorWater(0, 105, 148)        // Bleu océan
    , colorSand(238, 214, 175)       // Beige
    , colorGrass(34, 139, 34)        // Vert
    , colorForest(0, 100, 0)         // Vert foncé
    , colorMountain(105, 105, 105)   // Gris
```

---

## 🐛 Dépannage

### Erreur : "SDL3/SDL.h not found"

**Solution** : SDL3 n'est pas installé ou pas dans le PATH
```bash
# Vérifier l'installation
ls /mingw64/include/SDL3/  # Windows MSYS2
ls /usr/local/include/SDL3/  # Linux

# Réinstaller si nécessaire
pacman -S mingw-w64-x86_64-SDL3  # Windows
```

### Erreur : "undefined reference to SDL_Init"

**Solution** : Problème de linkage
```bash
# Vérifier que -lSDL3 est bien dans la commande de compilation
python build.py --verbose
```

### Contradiction détectée lors de la génération

**Cause** : Les règles sont trop restrictives pour la taille de grille

**Solutions** :
- Réduire la taille de la grille
- Assouplir les règles de compatibilité
- Appuyer sur `R` pour réessayer avec une seed différente

### Le programme ne se lance pas sous Windows

**Solution** : Copier SDL3.dll à côté de l'exécutable
```bash
cp /mingw64/bin/SDL3.dll build/
```

---

## 📚 Ressources

### Liens utiles

- [SDL3 Documentation](https://wiki.libsdl.org/SDL3)
- [Wave Function Collapse (article original)](https://github.com/mxgmn/WaveFunctionCollapse)
- [Explication vidéo de WFC](https://www.youtube.com/watch?v=2SuvO4Gi7uY)

### Articles et tutoriels

- [The Wavefunction Collapse Algorithm explained very clearly](https://robertheaton.com/2018/12/17/wavefunction-collapse-algorithm/)
- [Procedural Generation with Wave Function Collapse](https://www.boristhebrave.com/2020/04/13/wave-function-collapse-explained/)

---

## 👥 Contributeurs

- **Votre Nom** - Développeur principal

---

## 📄 License

Ce projet est sous licence MIT. Voir le fichier `LICENSE` pour plus de détails.

```
MIT License

Copyright (c) 2025 Votre Nom

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction...
```

---

## 🙏 Remerciements

- **Maxim Gumin** - Créateur de l'algorithme WFC original
- **SDL Team** - Pour la bibliothèque SDL3
- **Communauté C++** - Pour les ressources et la documentation

---

## 📞 Contact

Pour toute question ou suggestion :

- **Email** : henribelinga99@gmail.com
- **GitHub** : [Henri13-B](https://github.com/henri13-B)

---

<div align="center">

**⭐ Si ce projet vous a plu, n'hésitez pas à lui donner une étoile ! ⭐**

Made with ❤️ and C++

</div>