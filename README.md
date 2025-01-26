# Explication des paramètres modifiables

Le code contient plusieurs variables et paramètres qui peuvent être modifiés afin d'ajuster le comportement du robot. Ce fichier explique en détail ce que fait chaque paramètre et comment il influence le fonctionnement global.

## 1. `axis` - Axe du Gyroscope
### Description
La variable `axis` définit l'axe du gyroscope que le robot va utiliser pour mesurer l'orientation. Le gyroscope peut mesurer l'angle de rotation sur trois axes : X, Y et Z. Cette variable détermine donc quel axe sera utilisé pour la correction d'angle.

### Valeurs possibles
- `0x01`: L'axe X est sélectionné. Le robot mesurera la rotation sur l'axe horizontal.
- `0x02`: L'axe Y est sélectionné. Le robot mesurera la rotation sur l'axe vertical.
- `0x03`: L'axe Z est sélectionné. Le robot mesurera la rotation autour de l'axe de profondeur.

### Effet
Changer cette valeur affecte directement la façon dont le robot interprète son orientation dans l'espace. Le choix de l'axe dépend de la configuration du robot et de la manière dont il doit interagir avec son environnement.

---

## 2. `direction` - Direction du mouvement
### Description
La variable `direction` détermine la direction dans laquelle le robot va se déplacer. Elle est utilisée pour contrôler le sens du mouvement, que ce soit en avant ou en arrière.

### Valeurs possibles
- `0x01`: Le robot va avancer en avant.
- `-0x01`: Le robot va avancer en arrière.

### Effet
Ce paramètre change la direction dans laquelle les moteurs du robot tournent. Un changement de direction influence le calcul de la vitesse des moteurs dans la fonction `correctDependingOnAngle`.

---

## 3. `baseSpeed` - Vitesse de base
### Description
La variable `baseSpeed` définit la vitesse de base du robot. Cette vitesse est utilisée comme référence pour les ajustements dans les fonctions de correction de l'angle.

### Valeurs possibles
- `V ∈ [-0x64 : 0x64]`: Cette plage indique la plage de vitesses possibles. Les valeurs négatives permettent un mouvement en arrière, tandis que les valeurs positives permettent un mouvement en avant.
- **Recommandé**: `V ∈ [-0x20 : 0x40]`: Ces valeurs sont recommandées pour éviter que le robot ne se déplace trop vite ou trop lentement.

### Effet
Le paramètre `baseSpeed` est multiplié par la direction et ajusté en fonction de l'angle de rotation mesuré par le gyroscope. Cela permet de modifier la vitesse du robot en fonction de la correction d'angle.

---

## 4. `cooldown` - Temps d'attente entre les corrections
### Description
La variable `cooldown` définit le temps d'attente entre chaque correction effectuée par le robot. Ce paramètre est particulièrement utile pour éviter des corrections trop fréquentes qui pourraient rendre les mouvements du robot instables.

### Valeurs possibles
- **En mode réel**: `T ∈ [0x00 : 0x0a]`: Cette plage est adaptée à une utilisation réelle où les corrections doivent être espacées de manière raisonnable pour maintenir la stabilité du robot.
- **En mode test**: `V ∈ [0x1e : 0x32]`: Ces valeurs sont utilisées lors de tests pour obtenir des corrections plus rapides et observer plus facilement les effets.

### Effet
Le paramètre `cooldown` permet de gérer le rythme des corrections effectuées sur le robot. Un temps de cooldown trop court peut entraîner un comportement erratique, tandis qu'un temps trop long pourrait rendre le robot trop lent à réagir.

---

## 5. `divisionFactor` - Facteur de division pour la sensibilité aux angles
### Description
La variable `divisionFactor` est utilisée pour ajuster la sensibilité du robot aux variations d'angle mesurées par le gyroscope. Un facteur de division plus élevé rendra le robot moins sensible aux petites variations d'angle, tandis qu'un facteur plus faible augmentera la réactivité du robot aux changements d'angle.

### Valeurs possibles
- `X ∈ [-0x02 : 0x0a]`: Cette plage de valeurs permet de choisir un facteur de division adapté à la situation. Un facteur trop faible peut rendre le robot trop sensible, tandis qu'un facteur trop élevé peut réduire sa capacité à réagir rapidement.

### Effet
Le `divisionFactor` est utilisé pour ajuster le calcul de la vitesse des moteurs en fonction de l'angle mesuré. Cela permet de limiter les corrections excessives lorsque l'angle mesuré est faible ou de rendre le robot plus réactif aux grands changements d'angle.

---

## 6. `offset` - Décalage des angles mesurés
### Description
L'`offset` est une valeur utilisée pour corriger les angles mesurés par le gyroscope. Il est calculé comme la différence entre l'angle mesuré au démarrage (`initialAngle`) et un angle de référence (ici, 0). Cette correction permet d'ajuster les mesures d'angle pour que le robot puisse prendre en compte son orientation initiale avant de commencer à se déplacer.

### Calcul
Le calcul de l'offset se fait dans la fonction `setup()` comme suit :
```cpp
offset = 0 - initialAngle;
