#!/bin/bash

# ============================================================
# Script de lancement pour Wave Function Collapse (Linux/macOS)
# ============================================================

echo ""
echo "============================================================"
echo "   Wave Function Collapse - Lancement"
echo "============================================================"
echo ""

# Vérifier si l'exécutable existe
if [ ! -f "build/WFC" ]; then
    echo "❌ [ERREUR] Exécutable non trouvé : build/WFC"
    echo ""
    echo "Veuillez compiler le projet avec :"
    echo "  python build.py"
    echo ""
    exit 1
fi

# Rendre l'exécutable exécutable (au cas où)
chmod +x build/WFC

# Lancer le programme
echo "🚀 Lancement de WFC..."
echo ""
cd build
./WFC
cd ..

echo ""
echo "============================================================"
echo "   Programme terminé"
echo "============================================================"
echo ""