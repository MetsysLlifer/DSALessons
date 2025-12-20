#include "game.h"

void InitParticles(ParticleSystem* ps) {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        ps->particles[i].active = false;
    }
}

void SpawnExplosion(ParticleSystem* ps, Vector2 position, Color color) {
    int particlesToSpawn = 50; 
    int spawnedCount = 0;
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (!ps->particles[i].active) {
            Particle* p = &ps->particles[i];
            p->active = true;
            p->position = position;
            p->color = color;
            p->size = (float)GetRandomValue(3, 8);
            p->life = 1.0f;
            float speedX = (GetRandomValue(-100, 100) / 10.0f);
            float speedY = (GetRandomValue(-100, 100) / 10.0f);
            p->velocity = (Vector2){ speedX * 2.0f, speedY * 2.0f };
            spawnedCount++;
            if (spawnedCount >= particlesToSpawn) break;
        }
    }
}

void UpdateParticles(ParticleSystem* ps) {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (ps->particles[i].active) {
            Particle* p = &ps->particles[i];
            p->position = Vector2Add(p->position, p->velocity);
            p->velocity = Vector2Scale(p->velocity, 0.95f);
            p->life -= 0.02f; 
            if (p->size > 0.5f) p->size -= 0.1f;
            if (p->life <= 0) p->active = false;
        }
    }
}

void DrawParticles(ParticleSystem* ps) {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (ps->particles[i].active) {
            Color c = ps->particles[i].color;
            c.a = (unsigned char)(255 * ps->particles[i].life);
            DrawRectangleV(ps->particles[i].position, (Vector2){ps->particles[i].size, ps->particles[i].size}, c);
        }
    }
}