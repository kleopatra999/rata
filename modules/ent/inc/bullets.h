#ifndef HAVE_ENT_BULLETS_H
#define HAVE_ENT_BULLETS_H

#include "shell/inc/main.h"
#include "util/inc/organization.h"
#include "util/inc/honestly.h"
#include "vis/inc/common.h"
namespace geo { struct Object; }

namespace ent {
    struct Bullet;
    extern util::Links<Bullet> bullets;
    struct Bullet : util::Link<Bullet>, vis::Drawn<vis::Overlay> {
        static CE size_t MAX_PTS = 4;
        geo::Object* owner = NULL;
         // For ricocheting motion.  Front to back.  Some may be undefined.
        util::Vec pts [MAX_PTS];
        util::Vec vel = util::Vec(0, 0);

        bool done = false;

        Bullet () { pts[0] = util::Vec(NAN, NAN); Link<Bullet>::link(bullets); }
         // For convenience
        Bullet (geo::Object* owner, util::Vec pos, util::Vec vel) : owner(owner), vel(vel) {
            pts[0] = pos;
            pts[1] = util::Vec(NAN, NAN);
            Link<Bullet>::link(bullets);
            appear();
        }
        ~Bullet () { Link<Bullet>::unlink(); }
        void finish () { appear(); }

         // Do a raycast in the b2World to find if we hit anything.
         // We might disappear, do damage, ricochet, set owner to NULL
        void update ();
        void Drawn_draw (vis::Overlay) override;
    };

    void shoot_sound (float volume = 1.0);
    void ricochet_sound (float volume = 0.4);
     // Calls shoot_sound and makes new Bullet
    Bullet* shoot_bullet (geo::Object* owner, util::Vec pos, util::Vec vel);

     // 名前通り。Call this after before_move I guess.
    void update_bullets ();
}

#endif
