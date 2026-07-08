# invaderz

# What is this project about?

This project aims at creating a small clone of the famouns [Space Invaders](https://en.wikipedia.org/wiki/Space_Invaders) game.

# Installation

First make sure to install dependencies locally:
* [SDL3](https://wiki.libsdl.org/SDL3/FrontPage)
* [Eigen3](https://libeigen.gitlab.io/eigen/docs-5.0/GettingStarted.html)

The repository can then be cloned:

```bash
git clone git@github.com:triplezstudio/invaderz.git
```

A `setup` target is provided in the [Makefile](Makefile): this needs to be ran once when setting up the project.

```bash
make setup
```

You can build and run the project with the dedicated target:

```bash
make run
```
