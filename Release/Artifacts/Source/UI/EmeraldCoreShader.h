#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_opengl/juce_opengl.h>

/**
 * Hyper-realistic 3D Refractive Emerald Core.
 * Uses a custom GLSL fragment shader to raymarch a spinning, glowing gemstone.
 */
class EmeraldCoreShader : public juce::OpenGLAppComponent
{
public:
    EmeraldCoreShader();
    ~EmeraldCoreShader() override;

    void initialise() override;
    void shutdown() override;
    void render() override;

    void paint (juce::Graphics& g) override;

    /**
     * Updates the vitality amount to drive the shader's internal glow and rotation speed.
     */
    void setVitality (float newVitality) { vitality = newVitality; }

private:
    void compileShader();

    juce::String vertexShader;
    juce::String fragmentShader;
    std::unique_ptr<juce::OpenGLShaderProgram> shader;

    float vitality = 0.0f;
    float time = 0.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EmeraldCoreShader)
};
