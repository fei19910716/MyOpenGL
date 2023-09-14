
#pragma once

#include "random_texture.h"

#define MAX_PARTICLES 1000
#define PARTICLE_LIFETIME 10.0f

#define PARTICLE_TYPE_LAUNCHER 0.0f
#define PARTICLE_TYPE_SHELL 1.0f
#define PARTICLE_TYPE_SECONDARY_SHELL 2.0f

struct Particle
{
    float Type;    
    glm::vec3 Pos;
    glm::vec3 Vel;    
    float LifetimeMillis;    
};


class ParticleSystem
{
public:
    ParticleSystem();
    
    ~ParticleSystem();
    
    bool InitParticleSystem(const glm::vec3& Pos);
    
    void Render(int DeltaTimeMillis, const glm::mat4& VP, const glm::vec3& CameraPos);
    
private:
    
    void UpdateParticles(int DeltaTimeMillis);
    void RenderParticles(const glm::mat4& VP, const glm::vec3& CameraPos);
    
    bool m_isFirst;
    unsigned int m_currVB;
    unsigned int m_currTFB;
    GLuint m_particleBuffer[2];
    GLuint m_transformFeedback[2];
    RandomTexture m_randomTexure;

    Shader particle_shader;
    Shader billboad_shader;

    Texture2D m_pTexture;
    int m_time;
};



ParticleSystem::ParticleSystem()
{
    m_currVB = 0;
    m_currTFB = 1;
    m_isFirst = true;
    m_time = 0;           
    
    memset(m_transformFeedback,0,sizeof(m_transformFeedback));
    memset(m_particleBuffer,0,sizeof(m_particleBuffer));
}


ParticleSystem::~ParticleSystem()
{
    if (m_transformFeedback[0] != 0) {
        glDeleteTransformFeedbacks(2, m_transformFeedback);
    }
    
    if (m_particleBuffer[0] != 0) {
        glDeleteBuffers(2, m_particleBuffer);
    }
}


bool ParticleSystem::InitParticleSystem(const glm::vec3& Pos)
{   
    Particle Particles[MAX_PARTICLES];
    memset(Particles,0,sizeof(Particles));

    Particles[0].Type = PARTICLE_TYPE_LAUNCHER;
    Particles[0].Pos = Pos;
    Particles[0].Vel = glm::vec3(0.0f, 0.0001f, 0.0f);
    Particles[0].LifetimeMillis = 0.0f;

    glGenTransformFeedbacks(2, m_transformFeedback);    
    glGenBuffers(2, m_particleBuffer);
    
    for (unsigned int i = 0; i < 2 ; i++) {
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_transformFeedback[i]);
        glBindBuffer(GL_ARRAY_BUFFER, m_particleBuffer[i]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Particles), Particles, GL_DYNAMIC_DRAW);
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, m_particleBuffer[i]);        
    }
                      

    
    particle_shader.use();
    
    // particle_shader.SetRandomTextureUnit(RANDOM_TEXTURE_UNIT_INDEX);
    // particle_shader.SetLauncherLifetime(100.0f);
    // particle_shader.SetShellLifetime(10000.0f);
    // particle_shader.SetSecondaryShellLifetime(2500.0f);
    
    
    billboad_shader.use();

    // m_billboardTechnique.SetColorTextureUnit(COLOR_TEXTURE_UNIT_INDEX);

    // m_billboardTechnique.SetBillboardSize(0.01f);
    
    m_pTexture.FromImage("../Content/fireworks_red.jpg",false);
    
    return true;
}


void ParticleSystem::Render(int DeltaTimeMillis, const glm::mat4& VP, const glm::vec3& CameraPos)
{
    m_time += DeltaTimeMillis;
    
    UpdateParticles(DeltaTimeMillis);

    RenderParticles(VP, CameraPos);

    m_currVB = m_currTFB;
    m_currTFB = (m_currTFB + 1) & 0x1;
}


void ParticleSystem::UpdateParticles(int DeltaTimeMillis)
{
    particle_shader.use();
    m_updateTechnique.SetTime(m_time);
    m_updateTechnique.SetDeltaTimeMillis(DeltaTimeMillis);
   
    m_randomTexture.Bind(RANDOM_TEXTURE_UNIT);
    
    glEnable(GL_RASTERIZER_DISCARD);
    
    glBindBuffer(GL_ARRAY_BUFFER, m_particleBuffer[m_currVB]);    
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_transformFeedback[m_currTFB]);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), 0);                          // type
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)4);         // position
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)16);        // velocity
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)28);          // lifetime
    
    glBeginTransformFeedback(GL_POINTS);

    if (m_isFirst) {
        glDrawArrays(GL_POINTS, 0, 1);

        m_isFirst = false;
    }
    else {
        glDrawTransformFeedback(GL_POINTS, m_transformFeedback[m_currVB]);
    }            
    
    glEndTransformFeedback();

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
}
    

void ParticleSystem::RenderParticles(const glm::mat4& VP, const glm::vec3& CameraPos)
{
    billboad_shader.use();
    billboad_shader.SetUniform<glm::vec3>(CameraPos);
    billboad_shader.SetUniform<glm::mat4>(VP);
    
    m_pTexture.Bind(GL_TEXTURE0);
    
    glDisable(GL_RASTERIZER_DISCARD);

    glBindBuffer(GL_ARRAY_BUFFER, m_particleBuffer[m_currTFB]);    

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)4);  // position

    glDrawTransformFeedback(GL_POINTS, m_transformFeedback[m_currTFB]);

    glDisableVertexAttribArray(0);
}