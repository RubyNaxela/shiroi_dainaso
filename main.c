#include <SFML/Graphics.h>
#include "game_context.h"
#include "scene.h"
#include "util.h"

///
/// \brief Application entry point
///
_static int main() {

    const sfVideoMode windowConfig = {g.config.window.size.x, g.config.window.size.y, 24};
    sfRenderWindow* const window = _new(sfRenderWindow)(windowConfig, "Shiroi Dainaso", sfTitlebar + sfClose, null);
    sfRenderWindow_setVerticalSyncEnabled(window, true);

    g.assets = _new(dsAssetsBundle)();
    dsAssetsBundle_register(g.assets, "font.pixelboy", ds_createFont("assets/pixel_boy.ttf"));
    dsAssetsBundle_register(g.assets, "sound.dainaso.hit", ds_createSound("assets/game_over_sound.ogg"));
    dsAssetsBundle_register(g.assets, "sound.dainaso.jump", ds_createSound("assets/jump_sound.ogg"));
    dsAssetsBundle_register(g.assets, "sound.milestone", ds_createSound("assets/milestone_sound.ogg"));
    dsAssetsBundle_register(g.assets, "texture.cactus.large", ds_createTexture("assets/cactus_large.png"));
    dsAssetsBundle_register(g.assets, "texture.cactus.small", ds_createTexture("assets/cactus_small.png"));
    dsAssetsBundle_register(g.assets, "texture.cloud", ds_createTexture("assets/cloud.png"));
    dsAssetsBundle_register(g.assets, "texture.dune", ds_createTexture("assets/dune.png"));
    dsAssetsBundle_register(g.assets, "texture.dainaso.hit", ds_createTexture("assets/dainaso_hit.png"));
    dsAssetsBundle_register(g.assets, "texture.dainaso.idle", ds_createTexture("assets/dainaso_idle.png"));
    dsAssetsBundle_register(g.assets, "texture.dainaso.duck1", ds_createTexture("assets/dainaso_duck1.png"));
    dsAssetsBundle_register(g.assets, "texture.dainaso.duck2", ds_createTexture("assets/dainaso_duck2.png"));
    dsAssetsBundle_register(g.assets, "texture.dainaso.step1", ds_createTexture("assets/dainaso_step1.png"));
    dsAssetsBundle_register(g.assets, "texture.dainaso.step2", ds_createTexture("assets/dainaso_step2.png"));
    dsAssetsBundle_register(g.assets, "texture.pterodactyl.flap1", ds_createTexture("assets/pterodactyl_flap1.png"));
    dsAssetsBundle_register(g.assets, "texture.pterodactyl.flap2", ds_createTexture("assets/pterodactyl_flap2.png"));

    sfEvent event;
    dsScene* scene = _new(dsScene)();
    sfClock* const frameClock = _new(sfClock)();

    while (sfRenderWindow_isOpen(window)) {
        while (sfRenderWindow_pollEvent(window, &event)) if (event.type == sfEvtClosed) sfRenderWindow_close(window);
        sfRenderWindow_clear(window, scene->background);
        if (scene->playing) {
            dsScene_iteration(scene, sfTime_asSeconds(sfClock_restart(frameClock)));
            dsScene_draw(scene, window);
            sfRenderWindow_display(window);
        } else if (sfKeyboard_isKeyPressed(sfKeySpace)) {
            free(scene);
            scene = _new(dsScene)();
            dsScene_ignoreFirstJump(scene);
            sfClock_restart(frameClock);
        }
    }

    return 0;
}