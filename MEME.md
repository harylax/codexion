🔹 Processus (Process)

Un processus est un programme en cours d’exécution.

Il possède sa propre mémoire.
Il fonctionne de manière relativement indépendante des autres processus.
Exemple : quand tu ouvres Google Chrome, le système crée un ou plusieurs processus pour le faire fonctionner.

👉 À retenir : un processus = un programme qui s’exécute avec son propre espace mémoire.

🔹 Thread (fil d’exécution)

Un thread est une tâche d’exécution à l’intérieur d’un processus.

Un processus peut contenir plusieurs threads.
Les threads d’un même processus partagent la mémoire du processus.
Ils permettent de faire plusieurs tâches en parallèle ou de manière concurrente.

👉 À retenir : un thread = une unité d’exécution à l’intérieur d’un processus.

⚡ Différence en une phrase

Un processus est comme une maison, et les threads sont les personnes qui travaillent à l’intérieur de cette maison.

🔹 Concurrence

La concurrence signifie que plusieurs tâches peuvent progresser pendant la même période, en partageant le temps du processeur.

Dans le contexte des threads, cela signifie qu’un programme peut avoir plusieurs threads qui travaillent sur différentes tâches.

👉 À retenir : concurrence = plusieurs tâches qui avancent en même temps, même si elles ne s’exécutent pas forcément exactement au même instant.

🧠 Exemple simple

Imagine un programme avec 2 threads :

Thread 1 → télécharge un fichier
Thread 2 → affiche l’interface

Le processeur peut alterner rapidement entre les deux :

Thread 1 : ███  ███   ███

Thread 2 :   ███   ███   ███
             ↑
        alternance

Pour l'utilisateur, les deux semblent fonctionner en même temps.

⚠️ Concurrence ≠ parallélisme

C'est une distinction importante :

Concurrence → plusieurs tâches progressent ensemble.
Parallélisme → plusieurs tâches sont réellement exécutées simultanément, généralement grâce à plusieurs cœurs CPU.

Par exemple :

1 seul cœur :

Tâche A → Tâche B → Tâche A → Tâche B

➡️ Concurrence, mais pas de véritable exécution simultanée.

2 cœurs :

Cœur 1 : Tâche A ──────────

Cœur 2 : Tâche B ──────────

➡️ Concurrence et parallélisme.

🎯 Phrase à retenir

Les threads permettent de gérer plusieurs tâches de manière concurrente au sein d'un même processus.

Et si plusieurs cœurs sont disponibles, ces threads peuvent également être exécutés en parallèle.

🔹 Race condition

Une race condition (condition de course) arrive lorsque le résultat d'un programme dépend de l'ordre ou du timing d'exécution de plusieurs threads.

Autrement dit :

Le résultat dépend de "qui passe en premier".

Exemple

On a une variable :

compteur = 0

Deux threads veulent faire :

compteur = compteur + 1

On pourrait penser que le résultat sera :

0 + 1 + 1 = 2

Mais si les deux threads lisent 0 avant que l'un écrive son résultat :

Thread 1 : lit 0
Thread 2 : lit 0


Thread 1 : écrit 1
Thread 2 : écrit 1

Résultat :

compteur = 1 ❌

au lieu de :

compteur = 2 ✅

C'est une race condition.

🔹 Data race

Une data race est un cas plus spécifique.

Elle se produit lorsque :

plusieurs threads accèdent à la même donnée en mémoire ;
au moins un thread modifie cette donnée ;
les accès ne sont pas correctement synchronisés.

Exemple :

Thread 1 → écrit dans compteur
Thread 2 → lit compteur

Si ces accès peuvent se produire simultanément sans synchronisation, on a une data race.

🧠 Différence importante

On peut retenir :

Data race = problème d'accès concurrent à une même donnée.
Race condition = résultat dépendant du timing ou de l'ordre d'exécution.

Une data race peut provoquer une race condition, mais les deux termes ne sont pas exactement synonymes.

🎯 Exemple concret

Imagine deux personnes qui veulent modifier le même document :

Data race → elles écrivent sur la même partie du document en même temps.
Race condition → le résultat final dépend de qui écrit en premier.

Pour éviter ces problèmes, on utilise notamment des mécanismes de synchronisation comme les mutex, locks, sémaphores ou certaines opérations atomiques.

🔹 Opération atomique

Une opération atomique est une opération qui est considérée comme indivisible.

👉 Autrement dit :

Une opération atomique est effectuée entièrement, ou pas du tout : aucun autre thread ne peut observer son état intermédiaire.

🧠 Pourquoi c'est important ?

Reprenons :

compteur = compteur + 1

On pourrait croire que c'est une seule opération.

En réalité, le processeur doit généralement faire plusieurs étapes :

1. Lire compteur
2. Ajouter 1
3. Écrire compteur

Donc deux threads peuvent se marcher dessus :

Thread 1 : Lire 0
Thread 2 : Lire 0
Thread 1 : Ajouter 1
Thread 2 : Ajouter 1
Thread 1 : Écrire 1
Thread 2 : Écrire 1

Résultat → 1 au lieu de 2 ❌

🔒 Avec une opération atomique

On peut avoir une opération du genre :

compteur.incrementAtomiquement()

Le système garantit que l'incrément est traité comme une seule opération indivisible.

Thread 1 : increment → 1
Thread 2 : increment → 2

Résultat :

compteur = 2 ✅
⚠️ Atomique ≠ rapide

C'est une confusion fréquente.

Atomique ne veut pas dire :

« ça s'exécute très rapidement »

Ça veut dire :

« cette opération ne peut pas être observée à moitié et ne peut pas être interrompue de manière problématique par une autre opération concurrente, selon les garanties du mécanisme utilisé. »

🔗 Le lien entre tous les concepts

Tu peux voir la progression comme ça :

Processus

   ↓

contient des

   ↓

Threads

   ↓

peuvent s'exécuter en concurrence

   ↓

accèdent parfois aux mêmes données

   ↓

        ┌───────────────┐
        │ Data race     │
        └───────┬───────┘
                ↓
        problèmes possibles
                ↓
        Race condition
                ↓
        🔒 Synchronisation
          ↙           ↘
      Mutex/Lock    Opérations
                    atomiques

🎯 La phrase à retenir

Atomique = indivisible du point de vue de la concurrence.

Et surtout : une opération atomique est un outil permettant de contrôler les accès concurrents aux données, mais toute race condition ne se résout pas forcément avec une seule opération atomique.

🔹 Mutex

Mutex signifie Mutual Exclusion → exclusion mutuelle.

Un mutex est un mécanisme qui permet de dire :

« Une seule thread à la fois peut accéder à cette partie du code. »

On utilise donc un mutex pour protéger une ressource partagée.

🧠 Exemple simple

Imaginons deux threads qui veulent modifier :

compteur

On protège l'accès avec un mutex :

Thread 1 → 🔒 verrouille le mutex
           
		   ↓
        modifie compteur
           ↓
        🔓 déverrouille
           
Thread 2 → 🔒 verrouille
           
		   ↓
        modifie compteur
           ↓
        🔓 déverrouille

Si le Thread 1 possède le mutex, le Thread 2 doit attendre.

🔒 Le principe

On retrouve généralement trois étapes :

1. Lock      → prendre le mutex
2. Critical section → accéder à la ressource
3. Unlock    → libérer le mutex

La critical section (section critique) est la partie du code qui accède à la donnée partagée.

⚠️ Pourquoi utiliser un mutex ?

Pour éviter des problèmes comme :

Thread 1 ──┐
 
           ├──> compteur partagé
Thread 2 ──┘

Sans protection → data race possible ❌

Avec mutex :

Thread 1 → 🔒 → compteur → 🔓

Thread 2              ↳ attend
                  🔒 → compteur → 🔓

➡️ Les accès sont séquentiels, donc on évite que les deux threads modifient la donnée simultanément.

🆚 Mutex vs opération atomique

🎯 À retenir

Mutex = une clé qui permet à un seul thread d'entrer dans une section critique à la fois.

Donc, dans notre chaîne :

Threads → concurrence → données partagées → data race → mutex pour protéger l'accès.

Et atomique est une autre manière de garantir certaines opérations sûres vis-à-vis de la concurrence.

🔹 Condition variable (cond)

Une condition variable permet à un thread de dire :

« Je vais attendre qu'une certaine condition soit vraie avant de continuer. »

Elle est surtout utilisée avec un mutex.

🧠 Exemple simple

Imagine une file d'attente :

Thread producteur → ajoute un élément

Thread consommateur → récupère un élément

Si la file est vide, le consommateur ne doit pas essayer de récupérer quelque chose.

Il peut faire :

🔒 lock(mutex)


tant que la file est vide :
    cond_wait(cond, mutex)


récupérer l'élément


🔓 unlock(mutex)

Le thread consommateur dit en quelque sorte :

« Je dors jusqu'à ce que quelqu'un me prévienne qu'il y a un élément. »

Quand le producteur ajoute un élément :

🔒 lock(mutex)


ajouter élément


cond_signal(cond)


🔓 unlock(mutex)

cond_signal() réveille un thread qui attend sur cette condition.

🔄 Le mécanisme

          File vide ?
              │
             Oui
              ↓
       Thread consommateur
              │
              ↓
       cond_wait(cond, mutex)
              │
              💤
              │
              │  producteur ajoute
              │  un élément
              ↓
       cond_signal(cond)
              │
              ↓
          réveil 🔔
              │
              ↓
       récupère l'élément

🔑 Les principales fonctions

Selon le langage/API, tu retrouveras généralement :

cond_wait() → attendre qu'une condition soit satisfaite
cond_signal() → réveiller un thread qui attend
cond_broadcast() → réveiller tous les threads qui attendent

Et généralement :

mutex → protège les données
cond → permet aux threads de s'attendre et de se réveiller
🎯 Différence avec le mutex

C'est très important :

🔒 Mutex : « Qui a le droit d'accéder à la ressource ? »

💤 Condition variable : « Quand est-ce que je peux continuer ? »

Par exemple :

Mutex → protège la file
Cond  → indique quand la file n'est plus vide

Donc tu peux maintenant voir les rôles :

Mutex = protéger
Cond = attendre / réveiller
Atomique = rendre une opération indivisible

🔹 Deadlock

Un deadlock (interblocage) est une situation où plusieurs threads se bloquent mutuellement et attendent indéfiniment.

👉 En gros :

Chaque thread attend quelque chose que l'autre possède.

🧠 Exemple simple avec 2 mutex

On a :

Mutex A 🔒
Mutex B 🔒

Deux threads :

Thread 1 :

    lock(A)
    ↓
    attend B


Thread 2 :

    lock(B)
    ↓
    attend A

On obtient :

Thread 1 possède A

       ↓
 
   attend B

       ↑

Thread 2 possède B

       ↓

   attend A

💀 Personne ne peut avancer.

Thread 1 attend que Thread 2 libère B.
Thread 2 attend que Thread 1 libère A.
Aucun ne peut continuer pour libérer son mutex.

➡️ Deadlock.

🔄 Exemple concret

Imagine deux personnes :

Alice possède le stylo et attend le cahier.
Bob possède le cahier et attend le stylo.
Alice → possède stylo → attend cahier
Bob   → possède cahier → attend stylo

Ils attendent tous les deux.

Deadlock. 😄

⚠️ Pourquoi c'est lié aux mutex ?

Parce qu'un deadlock apparaît souvent lorsqu'on utilise plusieurs mutex et qu'on les prend dans un mauvais ordre.

Par exemple :

Thread 1 : lock(A) → lock(B)

Thread 2 : lock(B) → lock(A)

Pour éviter cela, on peut imposer un ordre fixe :

Toujours :
    lock(A)
    puis
    lock(B)

Ainsi, les threads demandent toujours les ressources dans le même ordre.

🎯 À retenir

Deadlock = des threads sont bloqués parce qu'ils attendent mutuellement des ressources détenues par les autres.

Et dans notre chaîne :

Threads

↓

Concurrence

↓

Données partagées

↓

Mutex

↓

Mauvaise gestion de plusieurs mutex

↓

💀 Deadlock

Une autre notion très proche à connaître ensuite est livelock : les threads ne sont pas bloqués, mais ils tournent sans réussir à progresser.

🔹 Starvation

La starvation (famine) est une situation où un thread attend très longtemps, voire indéfiniment, parce qu'il n'obtient jamais les ressources dont il a besoin.

👉 À retenir :

Le thread peut continuer à fonctionner, mais il n'a jamais vraiment sa chance d'accéder à la ressource.

🧠 Exemple avec un mutex

Imagine 3 threads qui veulent accéder à une ressource protégée par un mutex :

       🔒 Mutex
          │
   ┌──────┼──────┐
   ↓      ↓      ↓
Thread A Thread B Thread C

Supposons que le système donne constamment le mutex à A et B :

A → 🔒 → travaille → 🔓
B → 🔒 → travaille → 🔓
A → 🔒 → travaille → 🔓
B → 🔒 → travaille → 🔓
A → 🔒 → travaille → 🔓
...

Le thread C attend :

C → ⏳⏳⏳⏳⏳

Il n'est pas forcément bloqué par un deadlock : les autres threads continuent de travailler.

Mais C n'obtient jamais le mutex.

➡️ Starvation.

🆚 Starvation vs Deadlock

Deadlock 💀: 
- Les threads sont bloqués mutuellement
- Personne ne progresse
- Exemple : A attend B, B attend A
- Tout peut rester bloqué indéfiniment

Starvation 🍽️
- Un thread est constamment défavorisé
- Les autres peuvent continuer
- C attend toujours son tour
- Un seul thread peut être privé de ressource

🧩 Et avec la concurrence

On peut résumer les problèmes qu'on vient de voir :

Threads

   ↓

Concurrence

   ↓

Données partagées

   ↓

Synchronisation

   │

   ├── ❌ Data race
 
   │
 
   ├── 💀 Deadlock

   │
 
   └── 🍽️ Starvation

Deadlock : « Je t'attends, tu m'attends. »

Starvation : « Tout le monde passe avant moi. » 😄

1. pthread_cond_timedwait()
🔹 Concept : attente conditionnelle avec timeout

On a vu que :

pthread_cond_wait() permet à un thread d'attendre qu'une condition devienne vraie.

pthread_cond_timedwait() fait la même chose, mais avec une limite de temps.

Thread
  │
  ├── lock(mutex)
  │
  ├── condition pas vraie
  │
  ├── pthread_cond_timedwait()
  │          │
  │          💤 attend
  │          │
  │     ┌────┴────┐
  │     ↓         ↓
  │ condition   timeout
  │ vraie       atteint
  │     ↓         ↓
  └─────┴─────────┘

Par exemple :

« J'attends qu'un élément arrive dans la file, mais pas plus de 5 secondes. »

⚠️ Point important

pthread_cond_timedwait() s'utilise avec un mutex.

Typiquement :

pthread_mutex_lock(&mutex);


while (!condition) {
    pthread_cond_timedwait(&cond, &mutex, &timeout);
}


pthread_mutex_unlock(&mutex);

Pourquoi le while ?

Parce qu'après le réveil, le thread doit revérifier la condition. Un réveil ne signifie pas forcément que la condition est réellement satisfaite.

🎯 À retenir

pthread_cond_timedwait = « attends une condition, mais seulement jusqu'à une certaine limite de temps ».

2. pthread_cond_broadcast()
🔹 Concept : réveiller plusieurs threads en attente

On avait :

pthread_cond_signal()

qui sert généralement à réveiller un thread attendant sur une condition.

pthread_cond_broadcast() réveille tous les threads qui attendent sur cette condition.

Exemple

Imaginons 3 threads qui attendent :

Thread A → 💤
Thread B → 💤
Thread C → 💤


        condition variable
              │
              ↓
    pthread_cond_broadcast()
              │
       ┌──────┼──────┐
       ↓      ↓      ↓
       A      B      C
       🔔     🔔     🔔

Tous sont réveillés.

⚠️ Mais attention

broadcast ne signifie pas :

« Tous les threads peuvent maintenant exécuter leur code simultanément. »

Ils doivent toujours respecter le mutex.

Donc ils vont généralement se réveiller puis essayer d'obtenir le mutex.

🎯 À retenir

pthread_cond_broadcast = « réveille tous les threads qui attendent cette condition ».

3. gettimeofday()
🔹 Concept : obtenir l'heure actuelle

Cette fonction permet d'obtenir l'heure actuelle sous forme de :

secondes + microsecondes

Par exemple, conceptuellement :

temps = 172... secondes
        + 123456 microsecondes

Elle peut notamment servir à mesurer une durée :

t1 = gettimeofday()


... quelque chose se passe ...


t2 = gettimeofday()


durée = t2 - t1
⚠️ Attention

gettimeofday() donne l'heure civile actuelle, qui peut être modifiée par le système (synchronisation de l'horloge, changement manuel, etc.).

Pour mesurer précisément une durée, clock_gettime() avec une horloge monotone (CLOCK_MONOTONIC) est généralement préférable.

🎯 À retenir

gettimeofday = obtenir l'heure actuelle avec une résolution en microsecondes.

4. usleep()
🔹 Concept : mettre le thread en sommeil pendant une durée

usleep() signifie littéralement :

micro sleep

Par exemple :

usleep(100000);

➡️ Le thread dort environ 100 000 microsecondes = 100 ms.

Pendant ce temps, le thread ne consomme normalement pas le CPU comme s'il faisait une boucle active.

Thread
  │
  ├── travaille
  │
  ├── usleep(1 seconde)
  │        💤
  │        💤
  │        💤
  │
  └── reprend
⚠️ Différence avec une boucle d'attente

Mauvais :

while (!condition) {
    // tourner constamment
}

Le thread consomme potentiellement beaucoup de CPU.

Avec :

while (!condition) {
    usleep(1000);
}

il vérifie périodiquement et laisse du temps au CPU.

Mais pour la synchronisation entre threads, une condition variable est généralement préférable à une boucle avec usleep().

🧠 Maintenant, relions tout

Tu peux classer ces fonctions comme ceci :

                    THREADS
                       │
              ┌────────┴────────┐
              │                 │
       Synchronisation       Temps / attente
              │                 │
       ┌──────┴──────┐      ┌───┴──────────┐
       │             │      │              │
pthread_cond_   pthread_cond_ gettimeofday  usleep
timedwait       broadcast
       │             │          │              │
       ↓             ↓          ↓              ↓
 attendre avec   réveiller    obtenir       dormir
 timeout         plusieurs    l'heure       un thread
                 threads
🎯 Les quatre phrases à mémoriser
pthread_cond_timedwait → 💤 « J'attends une condition, avec une limite de temps. »
pthread_cond_broadcast → 🔔 « Réveille tous les threads qui attendent cette condition. »
gettimeofday → 🕐 « Donne-moi l'heure actuelle. »
usleep → 😴 « Fais dormir ce thread pendant un certain temps. »

Et surtout, dans un programme multithread, pthread_cond_timedwait + mutex + condition variable est le mécanisme de synchronisation, alors que usleep est simplement une temporisation.

## 3.1. Comprendre `timeval`, l'epoch et les unités de temps

Quand on utilise :

```c
struct timeval now;

gettimeofday(&now, NULL);
```

on obtient un instant représenté par deux valeurs :

```c
struct timeval {
    time_t      tv_sec;
    suseconds_t tv_usec;
};
```

### 🔹 `tv_sec`

`tv_sec` représente le nombre de **secondes écoulées depuis l'epoch Unix**.

L'epoch Unix correspond au :

```text
1er janvier 1970 à 00:00:00 UTC
```

Donc `tv_sec` est une valeur relativement grande.

Par exemple, conceptuellement :

```text
tv_sec = 1 780 000 000
```

signifie qu'environ 1,78 milliard de secondes se sont écoulées depuis l'epoch.

Cependant, lorsqu'on veut mesurer une durée, on ne s'intéresse généralement pas à cette grande valeur directement.

On fait plutôt :

```c
now.tv_sec - start.tv_sec
```

Ce qui donne le nombre de secondes écoulées entre deux instants.

---

### 🔹 `tv_usec`

`tv_usec` représente les **microsecondes supplémentaires à l'intérieur de la seconde courante**.

Il ne représente donc PAS le nombre total de microsecondes depuis l'epoch.

Par exemple :

```text
tv_sec  = 100
tv_usec = 250000
```

signifie :

```text
100 secondes + 250000 microsecondes
```

soit :

```text
100,25 secondes
```

On peut donc visualiser `timeval` comme :

```text
                    instant
                       │
             ┌─────────┴─────────┐
             │                   │
         secondes          fraction de seconde
         tv_sec                tv_usec
             │                   │
        100 secondes        250000 µs
             └─────────┬─────────┘
                       │
                  100,25 secondes
```

`tv_usec` est donc toujours la partie fractionnaire de la seconde, généralement comprise entre :

```text
0 et 999999 µs
```

---

### 🔹 Pourquoi `* 1000` puis `/ 1000` ?

Dans notre programme, on souhaite généralement travailler en **millisecondes**.

Les unités sont liées ainsi :

```text
1 seconde      = 1000 millisecondes
1 milliseconde = 1000 microsecondes
1 seconde      = 1000000 microsecondes
```

Pour convertir les secondes en millisecondes :

```c
seconds * 1000
```

Pour convertir les microsecondes en millisecondes :

```c
microseconds / 1000
```

Ainsi :

```c
elapsed = (now.tv_sec - start.tv_sec) * 1000;
elapsed += (now.tv_usec - start.tv_usec) / 1000;
```

signifie :

```text
partie en secondes
        ↓
      × 1000
        ↓
millisecondes

        +

partie en microsecondes
        ↓
      ÷ 1000
        ↓
millisecondes
```

On additionne ensuite les deux parties.

---

### 🧠 Exemple

Supposons :

```text
start:
    tv_sec  = 100
    tv_usec = 200000

now:
    tv_sec  = 103
    tv_usec = 700000
```

La différence est :

```text
3 secondes + 500000 microsecondes
```

Conversion :

```text
3 × 1000 = 3000 ms

500000 / 1000 = 500 ms
```

Donc :

```text
elapsed = 3000 + 500
        = 3500 ms
```

Il s'est donc écoulé :

```text
3,5 secondes
```

---

### ⚠️ `tv_usec` ne contient pas les secondes converties en microsecondes

C'est une confusion importante.

On pourrait imaginer :

```text
tv_sec = 10
tv_usec = 10000000
```

pour représenter 10 secondes en microsecondes.

Ce n'est pas ce que fait `timeval`.

La représentation correcte est :

```text
tv_sec  = 10
tv_usec = 0
```

pour exactement 10 secondes.

Et :

```text
tv_sec  = 10
tv_usec = 250000
```

pour 10,25 secondes.

Autrement dit :

```text
tv_sec  → partie entière en secondes
tv_usec → partie fractionnaire de cette seconde
```

---

### 🔹 Pourquoi `tv_sec` peut être une grande valeur sans problème ?

`tv_sec` utilise le type `time_t`, qui est conçu pour représenter des timestamps.

Sur les systèmes modernes 64 bits, `time_t` est généralement suffisamment grand pour représenter des timestamps sur une très grande période.

Le fait que `tv_sec` soit une grande valeur n'est donc pas un problème en pratique.

De plus, pour mesurer une durée, on soustrait deux timestamps :

```c
now.tv_sec - start.tv_sec
```

Par exemple :

```text
now.tv_sec   = 1786000123
start.tv_sec = 1786000100
```

donne simplement :

```text
23 secondes
```

On ne travaille donc pas directement avec la valeur absolue du timestamp, mais avec la différence entre deux timestamps.

---

### 🔹 Timestamp vs durée

Il faut bien distinguer les deux concepts.

Un **timestamp** représente un instant précis :

```text
"Quelle heure est-il ?"
```

Par exemple :

```text
1786000123 secondes depuis l'epoch
```

Une **durée** représente le temps entre deux instants :

```text
"Combien de temps s'est écoulé ?"
```

On l'obtient en faisant :

```text
timestamp_fin - timestamp_début
```

C'est exactement ce que fait notre fonction :

```c
get_timestamp_ms(sim)
```

Elle transforme la différence entre deux instants en une durée exprimée en millisecondes.

---

### 🔹 `gettimeofday()` et `usleep()` n'utilisent pas la même unité

`gettimeofday()` fournit :

```text
tv_sec  → secondes
tv_usec → microsecondes
```

Alors que :

```c
usleep(...)
```

attend une durée exprimée en **microsecondes**.

Si notre programme manipule une durée en millisecondes :

```c
long duration_ms = 100;
```

il faut la convertir avant d'utiliser `usleep()` :

```c
usleep(duration_ms * 1000);
```

car :

```text
100 ms × 1000 = 100000 µs
```

Donc :

```c
usleep(100000);
```

correspond à environ :

```text
100 ms
```

On peut donc garder une convention simple dans le programme :

```text
Mesure interne       → millisecondes
get_timestamp_ms()   → retourne des millisecondes
usleep()             → conversion ms → µs uniquement lors de l'appel
```

---

### 🎯 À retenir

```text
Epoch
  ↓
1er janvier 1970

gettimeofday()
  ↓
struct timeval
  ├── tv_sec  → secondes depuis l'epoch
  └── tv_usec → microsecondes dans la seconde courante

Pour mesurer une durée :
  ↓
fin - début

Pour obtenir des millisecondes :
  ↓
secondes × 1000
microsecondes ÷ 1000

usleep()
  ↓
prend des microsecondes
```

La règle mentale la plus importante est :

> **`tv_sec` donne la partie entière du temps en secondes, tandis que `tv_usec` donne la fraction de cette seconde en microsecondes.**
