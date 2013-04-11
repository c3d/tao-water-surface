/**
 * @~english
 * @taomoduledescription{WaterSurface, Water surface}
 *
 * <tt>import WaterSurface</tt> - Defines a sort of dynamic water surface.\n
 *
 * With this module, you can add nice and dynamic water surfaces in your presentations,
 * as shown by the following example :
 * @include water_surface.ddd
 *
 * @image html water_surface.png "Ripple effect"
 *
 * @endtaomoduledescription{WaterSurface}
 * @~french
 * @taomoduledescription{WaterSurface, Surface d'eau}
 *
 * <tt>import WaterSurface</tt> - Définit une sorte de surface d'eau dynamique.\n
 *
 * Ce module permet d'intégrer des surfaces d'eau dynamiques à vos présentations,
 * comme le montre l'exemple suivant :
 * @include water_surface.ddd
 *
 * @image html water_surface.png "Exemple de surface d'eau"
 *
 * @endtaomoduledescription{WaterSurface}
 *
 * @~
 * @{
 */


/**
 * @~english
 * Detail of the water surface.
 *
 * Increase this number can improve the rendering
 * to the expense of the performances.
 * @~french
 * Détail de la surface d'eau.
 *
 * Augmenter ce nombre peut améliorer la qualité du rendu au détriment
 * des performances.
 */
integer WATER_DETAIL = 150;


/**
 * @~english
 * Strength of the water displacement.
 *
 * The greater @ref WATER_STRENGTH will be, The more the water surface will be influenced
 * by the action of external forces (drops, waves, etc.).
 *
 * @~french
 * Force du déplacement d'eau.
 *
 * Plus @ref WATER_STRENGTH sera grand, plus la surface d'eau sera influencée
 * par l'action des forces extérieures (gouttes, vagues, etc.).
 */
real WATER_STRENGTH = 1.0;


/**
 * @~english
 * Creates a dynamic water surface.
 *
 * Defines a basic water surface of size (@p w, @p h) named @p name.\n
 * To obtain a realistic surface, a texture have to be binded previously
 * on the texture unit 1 in order to define correctly the underwater environment.
 *
 * @note An other texture showing the abovewater environment can also be binded on the texture unit 2
 * in order to improve slightly the rendering.
 *
 * For instance, the following example creates a basic water surface named @e water :
@code
// Our underwater texture
texture_unit 1
texture "under.jpg"
// Our abovewater texture (optionnal)
texture_unit 2
texture "above.jpg"
// Create water surface
water_surface 500, 500
@endcode
 *
 * @note This function is a shorcut to :
@code
color 0.25, 1.0, 1.0, 1.0
colored_water_surface w, h
@endcode
 *
 * @~french
 * Construit une surface d'eau dynamique.
 *
 * Définit une surface d'eau dynamique de taille (@p w, @p h) avec le nom @p name.\n
 * Pour obtenir une surface d'eau réaliste, une texture doit nécesserairement être appliquée
 * sur l'unité de texture 1 afin de décrire l'environnement situé au fond de l'eau.
 *
 * @note Une autre texture décrivant l'environnement situé au dessus de l'eau peut également être appliqué sur
 * l'unité de texture 2 afin d'améliorer légérement le rendu obtenu.
 *
 * Par exemple, le code suivant créé une simple surface d'eau nommée @e eau:
@code
// La texture du fond
texture_unit 1
texture "fond.jpg"
// La texture du dessus (optionnelle)
texture_unit 2
texture "dessus.jpg"
// Création de la surface d'eau
water_surface 500, 500
@endcode
 *
 * @note Cette fonction est un raccourci à :
@code
color 0.25, 1.0, 1.0, 1.0
colored_water_surface w, h
@endcode
 */
water_surface (name:text, w:real, h:real);


/**
 * @~english
 * Creates a colored water surface.
 *
 * Defines a dynamic water surface of size (@p w, @p h) named @p name.\n
 * To obtain a realistic surface, a texture have to
 * be binded previously on the texture unit 1 in order to define correctly the underwater environment.\n
 * The color of this water surface is defined by all color primitives.
 *
 * @note An other texture showing the abovewater environment can also be binded on the texture unit 2
 * in order to improve slightly the rendering.
 *
 * For instance, the following example create a white water surface named @e water:
@code
// Water color
color "white"
// Our under texture
texture_unit 1
texture "under.jpg"
// Our above texture (optional)
texture_unit 2
texture "above.jpg"
// Create water surface named "water"
colored_water_surface "water", 500, 500
@endcode
 *
 * @~french
 * Construit une surface d'eau colorée.
 *
 * Définit une surface d'eau dynamique de taille (@p w, @p h) avec le nom @p name.\n
 * Pour obtenir une surface d'eau réaliste, une texture doit nécesserairement être
 * appliquée sur l'unité de texture 1 afin de décrire l'environnement situé au fond de l'eau.\n
 * La couleur de cette surface d'eau est en revanche définit par les primitives de couleur.
 *
 * @note Une autre texture décrivant l'environnement situé au dessus de l'eau peut également être appliqué sur
 * l'unité de texture 2 afin d'améliorer légérement le rendu obtenu.
 *
 * Par exemple, le code suivant créé une surface d'eau blanche nommée @e eau :
@code
// Couleur de l'eau
color "white"
// La texture du fond
texture_unit 1
texture "fond.jpg"
// La texture du dessus (optionnelle)
texture_unit 2
texture "dessus.jpg"
// Création de la surface d'eau nommée "eau"
water_surface "eau", 500, 500
@endcode
 */
colored_water_surface (name:text, w:real, h:real);


/**
 * @~english
 * Keeps only one water surface.
 *
 * Delete all water surfaces except the one specified. If @p name is an empty string, all
 * surfaces are deleted.
@code
// Keep only the water
// surface named "water"
water_only "water"
@endcode
 *
 * @~french
 * Conserve une seule surface d'eau.
 *
 * Toutes les surfaces d'eau sont détruites, sauf celle dont le nom est précisé. Si
 * @p name est une chaine vide, toutes les surfaces d'eau sont détruites.
 *
@code
// Conservation uniquement de la
//  surface d'eau appelée "eau"
water_only "eau"
@endcode
 * @~
 * @see water_remove.
 */
water_only (name:text);


/**
 * @~english
 * Deletes a water surface.
 *
 * Remove the water surface named @p name.
 * @~french
 * Détruit une surface d'eau.
 *
 * Supprime la surface d'eau nommée @p name.
 * @~
 * @see water_only.
 */
water_remove(name:text);


/**
 * @~english
 * Modify extenuation ratio of a water surface.
 *
 * Specify the extenuation ratio @p r to the water surface named @p name.\n
 * The greater the value will be, The more the waves on the water surface will last.\n
 * The maximum value is 1.0.
@code
water_extenuation "water", 0.92
@endcode
 *
 * @~french
 * Modifie le paramètre d'atténuation d'une surface d'eau.
 *
 * Applique le paramètre d'atténuation @p r à la surface d'eau nommée @p name.\n
 * Plus cette valeur sera grande, plus les vagues sur la surface d'eau dureront longtemps.\n
 * La valeur maximum est 1.0.
 *
@code
water_extenuation "eau", 0.92
@endcode
 */
water_extenuation (name:text, r:real);


/**
 * @~english
 * Add a drop to the surface of a water.
 *
 * Put a drop on the water surface named @p name, which leads to the creations of some waves
 * disappearing more or less quickly according to the value of @ref water_extenuation.\n
 * The strength of these waves depend mainly of the value of @ref WATER_STRENGTH.
 *
 * @param name name of the water surface.
 * @param x    x-coordinate of the drop. This value is comprised between 0.0 and 1.0 (0.0 is the right edge of the surface and 1.0, the left one).
 * @param y    y-coordinate of the drop. This value is comprised between 0.0 and 1.0 (0.0 is the bottom edge of the surface and 1.0, the top one).
 * @param r    radius of the drop.
 * @param s    strength of the drop. An important value leads to more waves.
 *
 * For instance, the following code allows to use mouse to put some drops
 * on the water surface named @e water.
@code
add_drop "water", mouse_x / 500, mouse_y / 500, 1.0, 1.0
@endcode
 *
 * @~french
 * Dépose une goutte à la surface de l'eau.
 *
 * Ajoute une goutte sur la surface d'eau dont le nom est @p name, ce qui conduit à la création
 * de vagues disparaissant plus ou moins rapidement selon la valeur de @ref water_extenuation.\n
 * La force de ces vagues dépend principalement de la valeur de @ref WATER_STRENGTH.
 *
 * @param name Nom de la surface d'eau.
 * @param x    Abscisse de la goutte. Cette valeur est comprise entre 0.0 et 1.0 (0.0 correspond au bord droit de la surface et 1.0 au gauche).
 * @param y    Ordonnée de la goutte. Cette valeur est comprise entre 0.0 et 1.0 (0.0 correspond au bord inférieur de la surface et 1.0 au supérieur).
 * @param r    Rayon de la goutte.
 * @param s    Force de la goutte.
 *
 * Par exemple le code suivant permet d'utiliser la souris pour ajouter des gouttes
 * sur la surface d'eau nommée @e eau.
@code
add_drop "eau", mouse_x / 500, mouse_y / 500, 1.0, 1.0
@endcode
 */
add_drop(name:text, x:real, y:real, r:real, s:real);


/**
 * @~english
 * Add some random drops to a water surface.
 *
 * Put @p n random drops on the water surface named @p name.
 * as it could be done with @ref add_drop.
 *
@code
add_random_drops "water", 20
@endcode
 *
 * @~french
 * Dépose plusieurs gouttes de manière aléatoire sur une surface d'eau.
 *
 * Ajoute @p n gouttes aléatoires sur la surface d'eau dont le nom est @p name comme
 * cela pourrait être fait avec @ref add_drop.
 *
@code
add_random_drops "eau", 20
@endcode
 */
add_random_drops(name:text, n:integer);


/**
 * @}
 */
