import os
import sys
import subprocess
import platform
import shutil
from pathlib import Path

class Colors:
    """Codes ANSI pour colorer la sortie terminal"""
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKCYAN = '\033[96m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'

def print_colored(message, color):
    """Affiche un message coloré"""
    print(f"{color}{message}{Colors.ENDC}")

def print_header(message):
    """Affiche un header"""
    print_colored(f"\n{'='*60}", Colors.HEADER)
    print_colored(f"  {message}", Colors.HEADER)
    print_colored(f"{'='*60}\n", Colors.HEADER)

def clean_build():
    """Nettoie les fichiers de compilation"""
    print_header("🧹 Nettoyage des fichiers de build")
    
    build_dir = Path("build")
    
    if build_dir.exists():
        for file in build_dir.iterdir():
            if file.is_file():
                print(f"  ❌ Suppression: {file.name}")
                file.unlink()
        print_colored("✅ Nettoyage terminé", Colors.OKGREEN)
    else:
        print_colored("⚠️  Dossier build introuvable", Colors.WARNING)

def detect_compiler():
    """Détecte le compilateur disponible"""
    print_header("🔍 Détection du compilateur")
    
    # Essayer clang++ en premier
    try:
        result = subprocess.run(
            ["clang++", "--version"],
            capture_output=True,
            text=True,
            check=True
        )
        print_colored("✅ Compilateur détecté: clang++", Colors.OKGREEN)
        print(f"  Version: {result.stdout.split()[2]}")
        return "clang++"
    except (subprocess.CalledProcessError, FileNotFoundError):
        pass
    
    # Essayer g++
    try:
        result = subprocess.run(
            ["g++", "--version"],
            capture_output=True,
            text=True,
            check=True
        )
        print_colored("✅ Compilateur détecté: g++", Colors.OKGREEN)
        print(f"  Version: {result.stdout.split()[3]}")
        return "g++"
    except (subprocess.CalledProcessError, FileNotFoundError):
        pass
    
    print_colored("❌ Aucun compilateur trouvé (clang++ ou g++)", Colors.FAIL)
    print("   Installez clang++ ou g++ pour compiler le projet")
    return None

def compile_project(compiler, release=False, verbose=False):
    """Compile le projet WFC"""
    print_header("🔨 Compilation du projet WaveFunctionCollapse")
    
    # Créer le dossier build s'il n'existe pas
    os.makedirs("build", exist_ok=True)
    
    # Détecter le système d'exploitation
    system = platform.system()
    
    # Sources à compiler
    sources = [
        "src/main.cpp",
        "src/App.cpp",
        "src/Renderer.cpp",
        "src/Core/WFC.cpp"
    ]
    
    # Nom de l'exécutable
    output = "build/WFC.exe" if system == "Windows" else "build/WFC"
    
    # Options de compilation
    flags = [
        "-std=c++17",
        "-Isrc",
        "-Isrc/Core",
        "-Isrc/Utils",
        "-Wall",
        "-Wextra"
    ]
    
    # Mode Release ou Debug
    if release:
        flags.extend(["-O3", "-DNDEBUG"])
        print_colored("  📦 Mode: RELEASE (optimisé)", Colors.OKCYAN)
    else:
        flags.extend(["-g", "-O0"])
        print_colored("  🐛 Mode: DEBUG", Colors.OKCYAN)
    
    # Bibliothèques à lier
    libs = []
    
    # Détecter SDL3
    if system == "Windows":
        # Windows (MSYS2/MinGW)
        libs = ["-lmingw32", "-lSDL3", "-mwindows"]
    elif system == "Darwin":
        # macOS
        libs = ["-lSDL3", "-framework", "Cocoa"]
    else:
        # Linux
        libs = ["-lSDL3", "-lm"]
    
    # Construire la commande complète
    cmd = [compiler] + sources + ["-o", output] + flags + libs
    
    # Afficher la commande si verbose
    if verbose:
        print_colored("\n📝 Commande de compilation:", Colors.OKBLUE)
        print(f"  {' '.join(cmd)}\n")
    
    print(f"  Compilateur: {compiler}")
    print(f"  Sources: {len(sources)} fichiers")
    print(f"  Output: {output}")
    print()
    
    # Compiler
    try:
        result = subprocess.run(
            cmd,
            check=True,
            capture_output=True,
            text=True
        )
        
        print_colored("✅ Compilation réussie !", Colors.OKGREEN)
        print_colored(f"📦 Exécutable créé: {output}", Colors.OKGREEN)
        
        # Afficher les warnings s'il y en a
        if result.stderr:
            print_colored("\n⚠️  Warnings:", Colors.WARNING)
            print(result.stderr)
        
        return True
        
    except subprocess.CalledProcessError as e:
        print_colored("❌ Erreur de compilation", Colors.FAIL)
        print_colored("\n📋 Sortie d'erreur:", Colors.FAIL)
        print(e.stderr)
        return False

def print_usage():
    """Affiche les instructions d'utilisation"""
    print_header("🚀 Compilation terminée avec succès !")
    
    system = platform.system()
    
    print("▶️  Pour lancer le programme:\n")
    
    if system == "Windows":
        print_colored("  run.bat", Colors.OKGREEN)
        print("  ou")
        print_colored("  .\\build\\WFC.exe", Colors.OKGREEN)
    else:
        print_colored("  ./run.sh", Colors.OKGREEN)
        print("  ou")
        print_colored("  ./build/WFC", Colors.OKGREEN)
    
    print("\n🎮 Contrôles dans le programme:")
    print("  R       : Réinitialiser")
    print("  ESPACE  : Étape suivante")
    print("  1-4     : Changer de mode")
    print("  ESC     : Quitter")
    print()

def main():
    """Point d'entrée principal"""
    # Parser les arguments
    args = sys.argv[1:]
    clean = "--clean" in args
    release = "--release" in args
    verbose = "--verbose" in args
    
    print_colored("""
╔══════════════════════════════════════════════════════╗
║  🌊 Wave Function Collapse - Build System           ║
╚══════════════════════════════════════════════════════╝
    """, Colors.HEADER)
    
    # Nettoyage si demandé
    if clean:
        clean_build()
        if len(args) == 1:  # Si seulement --clean
            return
    
    # Détecter le compilateur
    compiler = detect_compiler()
    if not compiler:
        sys.exit(1)
    
    # Compiler
    success = compile_project(compiler, release, verbose)
    
    if success:
        print_usage()
        sys.exit(0)
    else:
        print_colored("\n💥 La compilation a échoué", Colors.FAIL)
        sys.exit(1)

if __name__ == "__main__":
    main()