# ChessCode

ChessCode est un moteur d'échecs écrit en C++ qui permet de jouer contre un bot ou contre un autre joueur. Il utilise un algorithme minimax avec élagage alpha-bêta et Zobrist hashing pour l'optimisation de la recherche de coups.

## Prérequis

- **CMake** (version 3.29 ou supérieure) : Utilisé pour la configuration du projet.
- **Compilateur C++** (supportant C++20 minimum) : Le code utilise des fonctionnalités de C++20, assurez-vous que votre compilateur prend en charge cette version.
- **GoogleTest** : Utilisé pour les tests unitaires (normalement intégré au fichier de config cmake du projet)

## Compilation

### Étapes pour compiler le projet

1. Clonez le dépôt :
   ```bash
   git clone https://github.com/Thibautau/ChessCode.git
   cd ChessCode
   ```

2. Créez un répertoire de construction (build) :
   ```bash
   mkdir build
   cd build
   ```

3. Exécutez CMake pour configurer le projet :
   ```bash
   cmake ..
   ```

4. Enregistrez et Compilez le projet :
   ```bash
   cmake --build .
   ```

### Exécution

Une fois le projet compilé, vous pouvez exécuter le programme principal ou les tests :

- **Exécuter le jeu d'échecs** :
  ```bash
  ./ChessCode
  ```

- **Exécuter les tests** :
  ```bash
  ./runTests
  ```

- **Exécuter les tests de performance** :
  ```bash
  ./runPerf
  ```

## Configuration pour UCI (Universal Chess Interface)

Par défaut, le jeu est jouable depuis la console. Cependant, pour le relier à un logiciel d'interface graphique (GUI) utilisant le protocole UCI, vous devez suivre les étapes suivantes :

1. Dans le fichier `main.cpp`, décommentez la ligne suivante :
   ```cpp
   /*UCI* uci = new UCI();
   uci->uciCommunication();*/
   ```
   et commentez la ligne suivante :
   ```cpp
   game->playTurn();
   ```

2. Compilez le programme :
   ```bash
   cmake --build .
   ```

3. Une fois la compilation terminée, renseignez l'exécutable ChessCode du programme dans votre logiciel GUI, en vous assurant que le protocole UCI est bien activé pour l'échange de données.

Le bot devrait désormais être opérationnel dans votre interface graphique.