# Compilation Linux

Pour la compilation sous linux, un Makefile est inclus
dans la répertoire `TPIMN401/`.

## Dépendances

Il est important de s'assurer que votre GPU supporte la version 4.6
d'OpenGL pour faire rouler le pipeline.

Pour vérifier quelle version d'OpenGL est supporté par votre architecture,
utilisez les commandes suivantes:

```bash
sudo apt update && sudo apt install -y mesa-utils
```

```bash
glxinfo | grep "OpenGL version"
```

### Debian / Ubuntu

Uniquement Debian 11 est testé pour l'installation puisque c'est la distribution
Linux sur mon portable. Cela dit, les packages nécéssaires sont nommés de la
même façon sur les répertoires Ubuntu, alors le fonctionnement est probablement identique.

Les dépendances pour compiler le programme peuvent être installées
avec la commande suivante:

```bash
sudo apt update && sudo apt install -y libgl-dev libglfw3-dev
```

## Utilisation

Pour compiler le programme, `cd` dans le répertoire `TPIMN401/`, puis
utilisez la commande suivante:

```bash
make
```

Pour exécuter le programme après l'avoir compilé:

```bash
./main
```
