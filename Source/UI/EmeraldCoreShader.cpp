#include "EmeraldCoreShader.h"

EmeraldCoreShader::EmeraldCoreShader()
{
    // Configure OpenGL context for continuous rendering (e.g. 60fps)
    openGLContext.setContinuousRepainting (true);
    
    // The standard JUCE vertex shader
    vertexShader = R"(
        attribute vec4 position;
        void main() {
            gl_Position = position;
        }
    )";

    // The Seraphic Fragment Shader: Raymarched Refractive Emerald
    // It creates an octahedral (diamond-like) SDF, applies an index of refraction based on Phi,
    // and adds a glowing internal core modulated by 'vitality'.
    fragmentShader = R"(
        uniform vec2 resolution;
        uniform float time;
        uniform float vitality;

        // Rotation matrix
        mat2 rot(float a) {
            float s = sin(a), c = cos(a);
            return mat2(c, -s, s, c);
        }

        // Octahedron SDF (Diamond shape)
        float sdOctahedron(vec3 p, float s) {
            p = abs(p);
            return (p.x + p.y + p.z - s) * 0.57735027; // 1/sqrt(3)
        }

        void main() {
            // Normalized pixel coordinates (from -1 to 1)
            vec2 uv = (gl_FragCoord.xy - 0.5 * resolution.xy) / resolution.y;
            
            // Ray origin and direction
            vec3 ro = vec3(0.0, 0.0, -3.0);
            vec3 rd = normalize(vec3(uv, 1.5)); // FOV
            
            // Background color (transparent/black)
            vec3 col = vec3(0.0);
            
            // Raymarching variables
            float t = 0.0;
            float max_d = 10.0;
            
            vec3 p = vec3(0.0);
            for(int i = 0; i < 64; ++i) {
                p = ro + rd * t;
                
                // Spin the core: base rotation + vitality burst
                p.xz *= rot(time * (0.2 + vitality * 2.0));
                p.xy *= rot(time * 0.15);
                
                float d = sdOctahedron(p, 1.2); // Core size
                
                if(d < 0.001 || t > max_d) break;
                t += d;
            }
            
            if(t < max_d) {
                // Surface found, calculate normals for lighting/refraction
                vec2 e = vec2(0.01, 0.0);
                vec3 n = normalize(vec3(
                    sdOctahedron(p + e.xyy, 1.2) - sdOctahedron(p - e.xyy, 1.2),
                    sdOctahedron(p + e.yxy, 1.2) - sdOctahedron(p - e.yxy, 1.2),
                    sdOctahedron(p + e.yyx, 1.2) - sdOctahedron(p - e.yyx, 1.2)
                ));
                
                // Seraphic Emerald Colors
                vec3 deepEmerald = vec3(0.0, 0.4, 0.2);
                vec3 brightEmerald = vec3(0.1, 0.9, 0.5);
                vec3 vitalityGlow = vec3(0.8, 1.0, 0.9); // White-cyan pure light
                
                // Lighting
                vec3 lightDir = normalize(vec3(1.0, 1.0, -1.0));
                float diff = max(dot(n, lightDir), 0.0);
                
                // Fresnel effect (edge glow based on Golden Ratio index of refraction ~1.618)
                float fresnel = pow(1.0 - max(dot(n, -rd), 0.0), 1.618);
                
                // Caustic reflections (faked with math)
                float caustic = sin(p.x * 10.0 + time) * cos(p.z * 10.0 + time);
                caustic = max(caustic, 0.0);
                
                // Blend materials
                col = mix(deepEmerald, brightEmerald, diff);
                col += fresnel * brightEmerald * 1.5;
                
                // Add Vitality Inner Glow
                col += caustic * vitalityGlow * vitality * 2.0;
                
                // Specular highlight
                vec3 ref = reflect(rd, n);
                float spec = pow(max(dot(ref, lightDir), 0.0), 32.0);
                col += vec3(1.0) * spec * (0.5 + vitality * 0.5);
            } else {
                // Background subtle glow (God-rays emanating from core)
                float dist = length(uv);
                float rays = sin(atan(uv.y, uv.x) * 8.0 + time * 0.5) * 0.5 + 0.5;
                col = vec3(0.0, 0.5, 0.2) * (1.0 - dist) * 0.2 * rays * vitality;
            }
            
            // Output to screen (with subtle bloom/gamma)
            col = pow(col, vec3(0.8)); // Gamma correction for pop
            gl_FragColor = vec4(col, 1.0);
        }
    )";
}

EmeraldCoreShader::~EmeraldCoreShader()
{
    shutdownOpenGL();
}

void EmeraldCoreShader::initialise()
{
    compileShader();
}

void EmeraldCoreShader::shutdown()
{
    shader.reset();
}

void EmeraldCoreShader::render()
{
    juce::OpenGLHelpers::clear (juce::Colours::transparentBlack);

    if (shader == nullptr) return;

    shader->use();

    // Set uniforms
    time += 0.016f; // Assuming ~60fps
    
    if (shader->getUniformIDFromName ("resolution") >= 0)
    {
        float w = static_cast<float> (getWidth());
        float h = static_cast<float> (getHeight());
        // Handle Retina/HiDPI scaling
        if (auto* display = juce::Desktop::getInstance().getDisplays().getPrimaryDisplay())
        {
            w *= static_cast<float>(display->scale);
            h *= static_cast<float>(display->scale);
        }
        shader->setUniform ("resolution", w, h);
    }

    if (shader->getUniformIDFromName ("time") >= 0)
        shader->setUniform ("time", time);

    if (shader->getUniformIDFromName ("vitality") >= 0)
        shader->setUniform ("vitality", vitality);

    // Draw a fullscreen quad to trigger the fragment shader
    const float vertices[] = {
        -1.0f, -1.0f,
         1.0f, -1.0f,
        -1.0f,  1.0f,
         1.0f,  1.0f
    };

    GLuint vbo;
    openGLContext.extensions.glGenBuffers (1, &vbo);
    openGLContext.extensions.glBindBuffer (juce::gl::GL_ARRAY_BUFFER, vbo);
    openGLContext.extensions.glBufferData (juce::gl::GL_ARRAY_BUFFER, sizeof (vertices), vertices, juce::gl::GL_STATIC_DRAW);

    GLint posAttrib = shader->getAttributeIDFromName ("position");
    if (posAttrib >= 0)
    {
        openGLContext.extensions.glVertexAttribPointer (static_cast<juce::GLuint> (posAttrib), 2, juce::gl::GL_FLOAT, juce::gl::GL_FALSE, 0, nullptr);
        openGLContext.extensions.glEnableVertexAttribArray (static_cast<juce::GLuint> (posAttrib));
    }

    glDrawArrays (juce::gl::GL_TRIANGLE_STRIP, 0, 4);

    if (posAttrib >= 0)
        openGLContext.extensions.glDisableVertexAttribArray (static_cast<juce::GLuint> (posAttrib));
        
    openGLContext.extensions.glBindBuffer (juce::gl::GL_ARRAY_BUFFER, 0);
    openGLContext.extensions.glDeleteBuffers (1, &vbo);
}

void EmeraldCoreShader::paint (juce::Graphics&)
{
    // The actual rendering is done in render() by OpenGL.
}

void EmeraldCoreShader::compileShader()
{
    std::unique_ptr<juce::OpenGLShaderProgram> newShader (new juce::OpenGLShaderProgram (openGLContext));

    if (newShader->addVertexShader (vertexShader)
     && newShader->addFragmentShader (fragmentShader)
     && newShader->link())
    {
        shader = std::move (newShader);
    }
    else
    {
        juce::Logger::writeToLog ("Shader compilation failed:\n" + newShader->getLastError());
    }
}
