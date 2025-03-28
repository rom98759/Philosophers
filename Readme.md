# Philosophers

## Introduction

Le projet *Philosophers* de l'école 42 est une simulation du problème classique des philosophes mangeurs, un défi fondamental en programmation concurrente. L'objectif est d'implémenter une solution multi-threadée qui gère correctement la synchronisation entre plusieurs threads, évitant les conditions de compétition et les deadlocks.

## Objectifs

- Comprendre les bases des threads et du multithreading.
- Appliquer les mutex pour éviter les conditions de course.
- Gérer la synchronisation des threads pour prévenir les interblocages (deadlocks) et les famines.
- Respecter les contraintes du sujet, telles que le bon fonctionnement du programme avec différentes configurations de philosophes et de temps.

## Simulateur Visualiser

🌟 **Tu peux tester visuellement le problème des philosophes avec ce simulateur interactif !** 🧑‍💻🍴<br>
👉 [**Philosophers Visualizer**](https://rom98759.github.io/Philosophers-visualizer/) 👈 <br>
💡 **Super outil pour comprendre et expérimenter !** 🚀

```bash
➜  philo git:(main) ./philo 5 8 1 1 1
0 1 has taken left fork
0 1 has taken right fork
0 1 is eating
0 3 has taken left fork
0 3 has taken right fork
0 3 is eating
1 3 is sleeping
1 2 has taken right fork
1 2 has taken left fork
1 2 is eating
1 1 is sleeping
2 1 is thinking
2 1 has taken left fork
2 1 has taken right fork
2 1 is eating
2 3 is thinking
2 3 has taken left fork
2 2 is sleeping
2 3 has taken right fork
2 3 is eating
3 2 is thinking
3 3 is sleeping
3 2 has taken right fork
3 2 has taken left fork
3 2 is eating
3 5 has taken left fork
3 5 has taken right fork
3 5 is eating
3 1 is sleeping
3 4 has taken right fork
4 5 is sleeping
4 3 is thinking
4 4 has taken left fork
4 4 is eating
All philosophers have eaten the maximum number of meals
```

---

## Contexte et Explication

Le problème met en scène un nombre `N` de philosophes assis autour d'une table. Chaque philosophe suit un cycle de trois actions :
1. **Manger** : nécessite deux fourchettes (celle à gauche et celle à droite du philosophe).
2. **Dormir** : après avoir mangé, le philosophe se repose.
3. **Penser** : après le sommeil, le philosophe réfléchit avant de tenter de manger à nouveau.

Le défi réside dans le partage des fourchettes : chaque fourchette est placée entre deux philosophes et ne peut être utilisée que par un philosophe à la fois. Cela entraîne un risque de blocage mutuel si les philosophes prennent tous une seule fourchette et attendent indéfiniment la seconde.

![alt text](img/philos.png)

---

## Gestion des Conditions de Course et des Mutex

### Conditions de Course
Une **condition de course** se produit lorsque plusieurs threads accèdent simultanément à une même ressource et tentent de la modifier en même temps, menant à des résultats imprévisibles.

Exemple simplifié d'une condition de course avec un compteur partagé :
```c
#include <pthread.h>
#include <stdio.h>

int count = 0;

void *routine() {
    for (int i = 0; i < 1000000; i++) {
        count++;
    }
    return NULL;
}

int main() {
    pthread_t t1, t2;
    pthread_create(&t1, NULL, &routine, NULL);
    pthread_create(&t2, NULL, &routine, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    printf("Count: %d\n", count);
}
```
> Résultat : `count` ne sera pas toujours égal à `2,000,000` à cause d'accès simultanés non synchronisés.
>

Exemple de séquence d'exécution problématique :
```
Thread 1           | Thread 2
-------------------|-------------------
Lit count = 0      |
                   | Lit count = 0
Incrémente (1)     |
                   | Incrémente (1)
Écrit count = 1    |
                   | Écrit count = 1
```

### Utilisation des Mutex
Pour éviter ce problème, nous utilisons des **mutex** (verrous). Ils assurent qu'un seul thread accède à une ressource critique à la fois.

Correction avec un mutex :
```c
#include <pthread.h>
#include <stdio.h>

int count = 0;
pthread_mutex_t lock;

void *routine() {
    for (int i = 0; i < 1000000; i++) {
        pthread_mutex_lock(&lock);
        count++;
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

int main() {
    pthread_t t1, t2;
    pthread_mutex_init(&lock, NULL);
    pthread_create(&t1, NULL, &routine, NULL);
    pthread_create(&t2, NULL, &routine, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_mutex_destroy(&lock);
    printf("Count: %d\n", count);
}
```
> Ici, `pthread_mutex_lock` empêche plusieurs threads d'écrire en même temps, garantissant un comportement cohérent.


---

## Compilation et Exécution

Compilation avec `make` :
```sh
make
```

Exécution :
```sh
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
```

Exemple :
```sh
./philo 5 800 200 200 5
```

---

## Ressources Supplémentaires
- [Documentation Pthread](https://man7.org/linux/man-pages/man7/pthreads.7.html)
- [Vidéo sur les data Races](https://youtu.be/FY9livorrJI?si=13SkXfaNFl02xG53)
- [Vidéo sur les mutex](https://youtu.be/oq29KUy29iQ?si=J8sEUMkvHDNKZB3o)

---

