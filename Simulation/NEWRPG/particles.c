#include "game.h"
void InitParticles(ParticleSystem* ps) { for (int i = 0; i < MAX_PARTICLES; i++) ps->particles[i].active = false; }
void SpawnExplosion(ParticleSystem* ps, Vector2 position, Color color) {
    int spawned = 0;
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (!ps->particles[i].active) {
            ps->particles[i].active = true; ps->particles[i].position = position; ps->particles[i].color = color;
            ps->particles[i].size = (float)GetRandomValue(3, 8); ps->particles[i].life = 1.0f;
            ps->particles[i].velocity = (Vector2){ (GetRandomValue(-100,100)/10.0f)*2.0f, (GetRandomValue(-100,100)/10.0f)*2.0f };
            if (++spawned >= 50) break;
        }
    }
}
void UpdateParticles(ParticleSystem* ps) {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (ps->particles[i].active) {
            ps->particles[i].position = Vector2Add(ps->particles[i].position, ps->particles[i].velocity);
            ps->particles[i].velocity = Vector2Scale(ps->particles[i].velocity, 0.95f);
            ps->particles[i].life -= 0.02f; if (ps->particles[i].size > 0.5f) ps->particles[i].size -= 0.1f;
            if (ps->particles[i].life <= 0) ps->particles[i].active = false;
        }
    }
}
void DrawParticles(ParticleSystem* ps) {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (ps->particles[i].active) {
            Color c = ps->particles[i].color; c.a = (unsigned char)(255 * ps->particles[i].life);
            DrawRectangleV(ps->particles[i].position, (Vector2){ps->particles[i].size, ps->particles[i].size}, c);
        }
    }
}