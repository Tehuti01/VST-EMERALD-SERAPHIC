<!DOCTYPE html>
<html lang="en" class="scroll-smooth">
<head>
    <meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Seraphic Genesis | Interactive Blueprint</title>
<script src="https://cdn.tailwindcss.com"></script>
    <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
    <link href="https://fonts.googleapis.com/css2?family=Cinzel:wght@400;600;800&family=Inter:wght@300;400;600&display=swap" rel="stylesheet">
<script>
    tailwind.config = {
        theme: {
            extend: {
                fontFamily: {
                    serif: ['Cinzel', 'serif'],
                    sans: ['Inter', 'sans-serif'],
                },
                colors: {
                    obsidian: '#050505',
                    obsidianLight: '#111111',
                    platinum: '#E5E4E2',
                    gold: '#D4AF37',
                    cyanGlow: '#00F0FF',
                    purpleGlow: '#8B5CF6',
                },
                backgroundImage: {
                    'iridescent': 'linear-gradient(to right, #8B5CF6, #00F0FF, #D4AF37)',
                    'platinum-brushed': 'linear-gradient(135deg, #E5E4E2 0%, #B0B0B0 50%, #E5E4E2 100%)',
                },
                boxShadow: {
                    'glow-cyan': '0 0 20px rgba(0, 240, 255, 0.4)',
                    'glow-gold': '0 0 30px rgba(212, 175, 55, 0.5)',
                    'glow-gem': '0 0 50px rgba(139, 92, 246, 0.6), inset 0 0 20px rgba(0, 240, 255, 0.8)',
                }
            }
        }
    }
    </script>
    <style>
    body {
    background-color: #050505;
    color: #E5E4E2;
    overflow-x: hidden;
}

/* Custom Scrollbar */
::-webkit-scrollbar { width: 8px; }
::-webkit-scrollbar-track { background: #050505; }
::-webkit-scrollbar-thumb { background: #333; border-radius: 4px; }
::-webkit-scrollbar-thumb:hover { background: #D4AF37; }

.text-iridescent {
    background: linear-gradient(to right, #8B5CF6, #00F0FF, #D4AF37);
    -webkit-background-clip: text;
    -webkit-text-fill-color: transparent;
    background-clip: text;
}

.chart-container {
    position: relative;
    width: 100%;
    max-width: 800px;
    margin-left: auto;
    margin-right: auto;
    height: 35vh;
    max-height: 400px;
    border-radius: 8px;
    overflow: hidden;
    background: rgba(10, 10, 10, 0.8);
    border: 1px solid rgba(212, 175, 55, 0.2);
    box-shadow: inset 0 0 30px rgba(0, 0, 0, 0.9);
}

/* The Diamond Core CSS Shape (NO SVG) */
.diamond-core {
    width: 80px;
    height: 80px;
    background: linear-gradient(135deg, #E5E4E2, #8B5CF6, #00F0FF);
    clip-path: polygon(50% 0%, 100% 50%, 50% 100%, 0% 50%);
    transition: all 0.5s cubic-bezier(0.25, 0.46, 0.45, 0.94);
    cursor: pointer;
    position: relative;
    z-index: 10;
    box-shadow: 0 0 20px rgba(0, 240, 255, 0.2);
}

.diamond-core::after {
    content: '';
    position: absolute;
    top: 5px; left: 5px; right: 5px; bottom: 5px;
    background: linear-gradient(135deg, rgba(255,255,255,0.8), transparent);
    clip-path: polygon(50% 0%, 100% 50%, 50% 100%, 0% 50%);
}

.diamond-core.active {
    transform: scale(1.1) rotate(180deg);
    background: linear-gradient(135deg, #D4AF37, #00F0FF, #D4AF37);
    box-shadow: 0 0 50px rgba(212, 175, 55, 0.8), 0 0 100px rgba(0, 240, 255, 0.6);
    animation: pulse-gem 2s infinite alternate;
}

@keyframes pulse-gem {
    0% { filter: brightness(1) drop-shadow(0 0 20px #8B5CF6); }
    100% { filter: brightness(1.5) drop-shadow(0 0 40px #00F0FF); }
}

/* VST View Perspectives */
.vst-wrapper {
    transition: all 1s cubic-bezier(0.16, 1, 0.3, 1);
    transform-style: preserve-3d;
}
.view-hero { transform: perspective(1000px) rotateY(0deg) rotateX(0deg) scale(1); }
.view-angled { transform: perspective(1500px) rotateY(-25deg) rotateX(15deg) scale(0.9); box-shadow: 30px 30px 50px rgba(0,0,0,0.8), -1px -1px 2px rgba(255,255,255,0.2); }
.view-environment { transform: perspective(2000px) rotateY(10deg) rotateX(5deg) scale(0.85); box-shadow: 0 0 100px rgba(139, 92, 246, 0.3); }

/* Environment Background */
#env-canvas {
    position: absolute;
    top: 0; left: 0; width: 100%; height: 100%;
    z-index: 0;
    opacity: 0;
    transition: opacity 1s ease;
    pointer-events: none;
}
.view-environment-active #env-canvas { opacity: 1; }

/* Knobs */
.knob {
    width: 48px; height: 48px;
    border-radius: 50%;
    background: radial-gradient(circle at 30% 30%, #444, #111);
    border: 2px solid #333;
    position: relative;
    box-shadow: 0 4px 6px rgba(0,0,0,0.6), inset 0 1px 2px rgba(255,255,255,0.1);
    transition: transform 0.2s;
}
.knob-indicator {
    position: absolute; top: 4px; left: 50%; transform: translateX(-50%);
    width: 2px; height: 12px; background: #00F0FF; border-radius: 2px;
    box-shadow: 0 0 5px #00F0FF;
}
.knob-gold .knob-indicator { background: #D4AF37; box-shadow: 0 0 5px #D4AF37; }

.phi-proportions {
    aspect-ratio: 1.618 / 1;
}
</style>
</head>
<body class="antialiased selection:bg-purpleGlow selection:text-white">

    <!-- Chosen Palette: Obsidian Base with Purple/Cyan/Gold Iridescent Accents -->
<!-- Application Structure Plan: The SPA is architected to mirror the requested specification for the "Seraphic Genesis" plugin. It uses a dashboard approach tailored to stakeholders and developers.
- Section 1 introduces the conceptual background.
- Section 2 is an interactive HTML/Canvas prototype of the VST interface, fulfilling the request for 3 specific mock-up axes (Hero, 3/4 Angled, Environment) via CSS 3D transforms.
- Section 3 breaks down the DSP architecture and features using interactive grids and text, updating based on the "Vitality" state. This structure allows the user to both read the spec and physically experience the "Golden Ratio" logic. -->
<!-- Visualization & Content Choices:
    - Goal: Demonstrate the Phi-Eternal Engine -> Viz: Chart.js Line Chart -> Interaction: Toggling "Vitality" changes the math formula of the wave to include Golden Ratio (Phi) and Pi modulations in real-time -> Justification: Shows the actual DSP concept mathematically without requiring audio.
- Goal: Show the 3 requested mockups -> Viz: CSS 3D transformed HTML container -> Interaction: Tab buttons alter perspective -> Justification: Fulfills the image mockup requirement dynamically in code without external assets.
- Confirming NO SVG/Mermaid: All elements use HTML/CSS shapes (clip-path for the diamond), unicode, and pure HTML Canvas. -->
<!-- CONFIRMATION: NO SVG graphics used. NO Mermaid JS used. -->

<!-- Navigation -->
<nav class="fixed top-0 w-full z-50 bg-obsidian/90 backdrop-blur-md border-b border-white/5">
<div class="max-w-7xl mx-auto px-6 py-4 flex justify-between items-center">
<div class="font-serif text-2xl font-bold tracking-widest text-platinum flex items-center gap-2">
<span class="text-gold text-3xl">&#10022;</span> SERAPHIC <span class="font-light text-gray-500">|</span> GENESIS
</div>
<div class="hidden md:flex space-x-8 text-sm font-sans tracking-wider text-gray-400">
<a href="#prototype" class="hover:text-cyanGlow transition-colors">THE PROTOTYPE</a>
<a href="#architecture" class="hover:text-purpleGlow transition-colors">DSP ARCHITECTURE</a>
<a href="#editions" class="hover:text-gold transition-colors">EDITIONS</a>
    </div>
    </div>
    </nav>

    <!-- Hero Section -->
    <header class="pt-32 pb-20 px-6 relative overflow-hidden">
<div class="absolute inset-0 bg-[radial-gradient(ellipse_at_center,_var(--tw-gradient-stops))] from-purpleGlow/10 via-obsidian to-obsidian -z-10"></div>
    <div class="max-w-4xl mx-auto text-center space-y-8">
<div class="inline-block px-4 py-1 rounded-full border border-gold/30 bg-gold/10 text-gold text-xs font-bold tracking-widest mb-4">
    PLUGIN #1 / 12 : THE SYNTH
</div>
<h1 class="text-5xl md:text-7xl font-serif font-bold text-platinum leading-tight">
    Infinite Evolutionary <br> <span class="text-iridescent">Wavetable Synth</span>
</h1>
<p class="text-xl md:text-2xl text-gray-400 font-light italic">
    "From the first breath of light — sound that evolves forever."
    </p>
    <p class="text-sm md:text-base text-gray-500 max-w-2xl mx-auto font-sans leading-relaxed">
    A pristine, hyper-detailed architectural blueprint bridging a pure C++ JUCE foundation with the proprietary Seraphic Vitality algorithm. Built strictly upon the mathematical constants of <strong class="text-gold">Phi (φ)</strong> and <strong class="text-cyanGlow">Pi (π)</strong>.
</p>
</div>
</header>

<!-- Main Application Content -->
<main class="max-w-7xl mx-auto px-6 pb-32 space-y-24">

    <!-- Interactive Prototype Section -->
<section id="prototype" class="space-y-8">
<div class="text-center">
<h2 class="text-3xl font-serif font-bold text-platinum mb-4 border-b border-white/10 pb-4 inline-block">The Divine Interface</h2>
<p class="text-gray-400 max-w-3xl mx-auto text-sm">
    This interactive module demonstrates the locked <strong>golden ratio (1.618:1)</strong> layout, hyper-realistic material concepts, and the three requested viewing axes. Interact with the <span class="text-gold">Diamond Core</span> to witness the Phi-Eternal Engine mathematical visualization.
</p>
</div>

<!-- View Controls -->
<div class="flex flex-wrap justify-center gap-4 mb-12">
<button onclick="changeView('hero')" id="btn-hero" class="px-6 py-2 border border-cyanGlow/50 bg-cyanGlow/10 text-cyanGlow rounded text-sm tracking-widest font-bold transition-all hover:bg-cyanGlow/20">MOCKUP 1: HERO</button>
<button onclick="changeView('angled')" id="btn-angled" class="px-6 py-2 border border-white/10 text-gray-400 rounded text-sm tracking-widest font-bold transition-all hover:text-white hover:border-white/30">MOCKUP 2: 3/4 ANGLE</button>
<button onclick="changeView('environment')" id="btn-env" class="px-6 py-2 border border-white/10 text-gray-400 rounded text-sm tracking-widest font-bold transition-all hover:text-white hover:border-white/30">MOCKUP 3: EXOTIC ENVIRONMENT</button>
</div>

<!-- The Virtual VST Plugin -->
<div id="env-container" class="relative w-full py-20 flex justify-center items-center overflow-hidden rounded-xl bg-obsidianLight border border-white/5 shadow-2xl">
    <!-- Environment Particles Canvas -->
<canvas id="env-canvas"></canvas>

    <!-- VST Frame -->
    <div id="vst-ui" class="vst-wrapper view-hero relative w-full max-w-5xl phi-proportions bg-gradient-to-br from-[#1a1a1a] to-[#050505] rounded-lg border-2 border-[#E5E4E2]/20 shadow-[inset_0_0_50px_rgba(0,0,0,1)] p-8 flex flex-col justify-between z-10">

    <!-- Decorative Engravings -->
<div class="absolute top-0 left-0 w-full h-full pointer-events-none rounded-lg border border-gold/10" style="background-image: repeating-linear-gradient(45deg, transparent, transparent 10px, rgba(212,175,55,0.02) 10px, rgba(212,175,55,0.02) 20px);"></div>

    <!-- Top Bar -->
    <div class="flex justify-between items-center z-10 relative">
<div class="font-serif text-xl tracking-widest text-platinum flex items-center gap-2">
<div class="w-2 h-2 rounded-full bg-cyanGlow shadow-[0_0_10px_#00F0FF]"></div>
GENESIS
</div>
<div class="flex gap-4">
<div class="text-xs text-gray-500 font-sans border border-gray-700 px-3 py-1 rounded bg-black/50">PRESET: <span class="text-white">CELESTIAL CHOIR</span></div>
<div class="text-xs text-gray-500 font-sans border border-gray-700 px-3 py-1 rounded bg-black/50">POLY: 8</div>
</div>
</div>

<!-- Center Area: Oscillator & Diamond Core -->
<div class="flex-1 flex items-center justify-between z-10 relative mt-6">
    <!-- Left Panel (Controls) -->
<div class="w-1/4 grid grid-cols-2 gap-y-8 gap-x-4 place-items-center">
<div class="flex flex-col items-center gap-2">
<div class="knob" style="transform: rotate(-45deg)"><div class="knob-indicator"></div></div>
<span class="text-[10px] text-gray-400 tracking-wider">CUTOFF</span>
    </div>
    <div class="flex flex-col items-center gap-2">
<div class="knob" style="transform: rotate(20deg)"><div class="knob-indicator"></div></div>
<span class="text-[10px] text-gray-400 tracking-wider">RESO</span>
    </div>
    <div class="flex flex-col items-center gap-2">
<div class="knob knob-gold" style="transform: rotate(-120deg)"><div class="knob-indicator"></div></div>
<span class="text-[10px] text-gold tracking-wider">φ DETUNE</span>
</div>
<div class="flex flex-col items-center gap-2">
<div class="knob" style="transform: rotate(60deg)"><div class="knob-indicator"></div></div>
<span class="text-[10px] text-gray-400 tracking-wider">MORPH</span>
    </div>
    </div>

    <!-- Center: Diamond Core & Display -->
    <div class="w-2/4 flex flex-col items-center justify-center relative h-full">

    <!-- The Chart Container for the Wavetable Visualizer -->
<div class="chart-container w-full">
<canvas id="waveformChart"></canvas>
    <!-- Interactive Tooltip/Overlay info -->
    <div id="engine-status" class="absolute top-2 left-3 text-[10px] text-cyanGlow font-sans tracking-widest bg-black/50 px-2 py-1 rounded border border-cyanGlow/20">
    ENGINE: STANDARD SINE
</div>
</div>

<!-- The Diamond Core Action Button -->
<div class="absolute -bottom-10 flex flex-col items-center gap-4">
<div id="diamond-core" class="diamond-core" onclick="toggleVitality()"></div>
    <div class="text-[10px] font-bold tracking-[0.2em] text-gray-400 uppercase" id="vitality-label">
    Engage Vitality
</div>
</div>
</div>

<!-- Right Panel (Envelopes/LFOs) -->
<div class="w-1/4 grid grid-cols-2 gap-y-8 gap-x-4 place-items-center">
<div class="flex flex-col items-center gap-2">
<div class="knob" style="transform: rotate(-100deg)"><div class="knob-indicator"></div></div>
<span class="text-[10px] text-gray-400 tracking-wider">ATTACK</span>
    </div>
    <div class="flex flex-col items-center gap-2">
<div class="knob" style="transform: rotate(10deg)"><div class="knob-indicator"></div></div>
<span class="text-[10px] text-gray-400 tracking-wider">DECAY</span>
    </div>
    <div class="flex flex-col items-center gap-2">
<div class="knob knob-gold" style="transform: rotate(130deg)"><div class="knob-indicator"></div></div>
<span class="text-[10px] text-gold tracking-wider">π LFO RATE</span>
</div>
<div class="flex flex-col items-center gap-2">
<div class="knob" style="transform: rotate(-45deg)"><div class="knob-indicator"></div></div>
<span class="text-[10px] text-gray-400 tracking-wider">RELEASE</span>
    </div>
    </div>
    </div>

    <!-- Bottom Bar -->
    <div class="flex justify-between items-end z-10 relative mt-16 border-t border-white/5 pt-4">
<div class="text-[10px] text-gray-600 tracking-widest font-serif">
    OPEN-SOURCE DSP FOUNDATION: MIT LICENSED CORE
</div>
<div class="flex gap-1">
    <!-- Macro visualizers -->
<div class="w-8 h-1 bg-cyanGlow/20 rounded overflow-hidden"><div class="w-1/2 h-full bg-cyanGlow"></div></div>
<div class="w-8 h-1 bg-purpleGlow/20 rounded overflow-hidden"><div class="w-[80%] h-full bg-purpleGlow"></div></div>
<div class="w-8 h-1 bg-gold/20 rounded overflow-hidden"><div class="w-[30%] h-full bg-gold"></div></div>
    </div>
    </div>
    </div>
    </div>
    </section>

    <!-- DSP Architecture Breakdown -->
    <section id="architecture" class="space-y-12 border-t border-white/10 pt-16">
<div>
    <h2 class="text-3xl font-serif font-bold text-platinum mb-4 border-b border-white/10 pb-4 inline-block">Seraphic DSP Architecture</h2>
<p class="text-gray-400 max-w-3xl text-sm leading-relaxed">
    Genesis combines an incredibly clean, MIT-licensed open-source C++ core (derived from JUCE synth templates and DaisySP) with a proprietary, highly sophisticated mathematical modulation layer. Below is the technical breakdown of the audio engine and how the Golden Ratio (<strong class="text-gold">1.618</strong>) dictates acoustic evolution.
    </p>
    </div>

    <div class="grid grid-cols-1 md:grid-cols-3 gap-8">
    <!-- Feature 1 -->
<div class="bg-obsidianLight p-8 rounded border border-white/5 hover:border-cyanGlow/30 transition-colors shadow-lg">
<div class="text-cyanGlow text-2xl mb-4">&#10023;</div>
<h3 class="text-xl font-bold text-platinum mb-3 font-serif">Dynamic Wavetable Core</h3>
<p class="text-sm text-gray-400 leading-relaxed mb-4">
    Four simultaneous oscillators starting with ultra-clean 2048-point arrays (sine, saw, square, custom 'seraph').
</p>
<ul class="text-xs text-gray-500 space-y-2 border-t border-white/5 pt-4">
<li><strong class="text-gray-300">Unison:</strong> 8-voice built-in</li>
<li><strong class="text-gray-300">Filter:</strong> 24 dB/oct Low-pass ladder</li>
<li><strong class="text-gray-300">Drift:</strong> Mathematically perfect analog variance</li>
</ul>
</div>

<!-- Feature 2 -->
<div class="bg-obsidianLight p-8 rounded border border-white/5 hover:border-gold/30 transition-colors shadow-lg relative overflow-hidden">
<div class="absolute top-0 right-0 w-32 h-32 bg-gold/5 rounded-full blur-3xl"></div>
    <div class="text-gold text-2xl mb-4">&#10044;</div>
<h3 class="text-xl font-bold text-platinum mb-3 font-serif">Seraphic Vitality Engine</h3>
<p class="text-sm text-gray-400 leading-relaxed mb-4">
    Activated via the Diamond Core. Injects <strong class="text-gold">φ (1.618) phase offsets</strong> and <strong class="text-gold">π harmonic stretching</strong> so waveforms mutate organically.
</p>
<ul class="text-xs text-gray-500 space-y-2 border-t border-white/5 pt-4">
<li><strong class="text-gray-300">Evolution:</strong> 8-64 bar cycles</li>
<li><strong class="text-gray-300">Modulation:</strong> All parameters spiral outward</li>
<li><strong class="text-gray-300">Result:</strong> Generative, non-repeating blooms</li>
</ul>
</div>

<!-- Feature 3 -->
<div class="bg-obsidianLight p-8 rounded border border-white/5 hover:border-purpleGlow/30 transition-colors shadow-lg">
<div class="text-purpleGlow text-2xl mb-4">&#10052;</div>
<h3 class="text-xl font-bold text-platinum mb-3 font-serif">Irrational Modulation</h3>
<p class="text-sm text-gray-400 leading-relaxed mb-4">
    4 LFOs and 4 Envelopes strictly tied to tempo-synced irrational numbers rather than standard rhythmic subdivisions.
</p>
<ul class="text-xs text-gray-500 space-y-2 border-t border-white/5 pt-4">
<li><strong class="text-gray-300">Rhythm:</strong> Hypnotic, non-repeating</li>
<li><strong class="text-gray-300">LFO Shapes:</strong> Spectral morphed</li>
<li><strong class="text-gray-300">Sync:</strong> Locked to φ or π ratios</li>
</ul>
</div>
</div>
</section>

<!-- Monetization / Editions Table -->
<section id="editions" class="space-y-8 border-t border-white/10 pt-16">
<div class="text-center mb-12">
<h2 class="text-3xl font-serif font-bold text-platinum mb-4 border-b border-white/10 pb-4 inline-block">Deployment Strategy</h2>
<p class="text-gray-400 max-w-3xl mx-auto text-sm">
    Monetization follows a strictly tiered approach. No bundles initially, ensuring the premium exclusivity of single assets. "Seraphic Pairs" (e.g., Genesis + Elysium) may be introduced later.
</p>
</div>

<div class="max-w-4xl mx-auto grid grid-cols-1 md:grid-cols-2 gap-6">
    <!-- Lite Edition -->
<div class="p-8 rounded-lg border border-white/10 bg-obsidian flex flex-col justify-between">
<div>
    <h3 class="text-2xl font-serif font-bold text-gray-300 mb-2">Genesis Lite</h3>
<p class="text-3xl font-sans font-light text-white mb-6">Free / $29 <span class="text-sm text-gray-500">One-Time</span></p>
<ul class="space-y-3 text-sm text-gray-400">
<li class="flex items-center gap-3"><span class="text-cyanGlow">&#10003;</span> 4 Core Wavetables</li>
<li class="flex items-center gap-3"><span class="text-cyanGlow">&#10003;</span> Basic Vitality (8-bar cycles)</li>
<li class="flex items-center gap-3"><span class="text-cyanGlow">&#10003;</span> standard 4 LFOs / 4 Envelopes</li>
<li class="flex items-center gap-3"><span class="text-gray-600">&#10007;</span> <span class="text-gray-600 line-through">User Wavetable Import</span></li>
</ul>
</div>
<button class="mt-8 w-full py-3 border border-gray-600 text-gray-300 rounded hover:bg-white/5 transition-colors font-bold tracking-widest text-xs">SPECIFY LITE</button>
</div>

<!-- Max Edition -->
<div class="p-8 rounded-lg border border-gold/40 bg-gradient-to-b from-gold/10 to-obsidian relative overflow-hidden shadow-[0_0_30px_rgba(212,175,55,0.1)] flex flex-col justify-between">
<div class="absolute top-4 right-4 text-xs font-bold bg-gold text-obsidian px-2 py-1 rounded">MAXIMUM VITALITY</div>
<div>
<h3 class="text-2xl font-serif font-bold text-gold mb-2">Genesis Max</h3>
<p class="text-3xl font-sans font-light text-white mb-6">$79 - $129 <span class="text-sm text-gray-500">One-Time</span></p>
<ul class="space-y-3 text-sm text-platinum">
<li class="flex items-center gap-3"><span class="text-gold">&#10003;</span> 32 Pristine Wavetables + User Import</li>
<li class="flex items-center gap-3"><span class="text-gold">&#10003;</span> Full Phi-Eternal Engine (Infinite cycles)</li>
<li class="flex items-center gap-3"><span class="text-gold">&#10003;</span> Macro Morphing & MPE Support</li>
<li class="flex items-center gap-3"><span class="text-gold">&#10003;</span> Exportable "Living Presets"</li>
</ul>
</div>
<button class="mt-8 w-full py-3 bg-gold text-obsidian rounded hover:bg-yellow-400 transition-colors font-bold tracking-widest text-xs shadow-[0_0_15px_rgba(212,175,55,0.4)]">DEPLOY MAX</button>
</div>
</div>
</section>

</main>

<!-- Footer -->
<footer class="border-t border-white/5 bg-black py-8 text-center text-xs text-gray-600 tracking-widest font-sans">
    <p>&copy; 2026 SERAPHIC PLUGINS. JUCE 8+ FRAMEWORK. RUST/C++ HYBRID ARCHITECTURE.</p>
</footer>

<!-- Application Logic -->
<script>
// ---------------------------------------------------
// 1. Chart.js Oscilloscope (Wavetable Visualizer)
// ---------------------------------------------------
const ctx = document.getElementById('waveformChart').getContext('2d');
let time = 0;
let isVitalityActive = false;
const PHI = 1.6180339887;
const PI = Math.PI;

// Initialize array data
const dataPoints = 200;
const labels = Array.from({length: dataPoints}, (_, i) => i);
const initialData = Array(dataPoints).fill(0);

// Chart configuration (Responsive container rules applied)
const waveChart = new Chart(ctx, {
    type: 'line',
    data: {
        labels: labels,
        datasets: [{
            label: 'DSP Waveform',
            data: initialData,
            borderColor: '#00F0FF',
            borderWidth: 2,
            pointRadius: 0,
            tension: 0.4, // Smooth curve
            fill: true,
            backgroundColor: 'rgba(0, 240, 255, 0.05)',
        }]
    },
    options: {
        responsive: true,
        maintainAspectRatio: false,
        animation: false, // Turn off built-in animation for manual requestAnimationFrame looping
        plugins: {
            legend: { display: false },
            tooltip: { enabled: false }
        },
        scales: {
            x: { display: false },
            y: {
                display: false,
                min: -2.5,
                max: 2.5
            }
        },
        layout: { padding: 0 }
    }
});

// Animation Loop for the DSP Waveform
function animateWaveform() {
    const data = waveChart.data.datasets[0].data;
    time += 0.05;

    for (let i = 0; i < dataPoints; i++) {
        const x = i * 0.1;

        if (!isVitalityActive) {
            // Standard Sine Wave (Clean, airy)
            data[i] = Math.sin(x - time * 2);
        } else {
            // Phi-Eternal Engine: Golden Ratio phase offsets and Pi harmonic stretching
            // This creates the "mutating, organic, non-repeating" visual
            const baseWave = Math.sin(x - time);
            const phiLayer = 0.8 * Math.sin(PHI * (x - time * PHI));
            const piLayer = 0.4 * Math.sin(PI * (x + time / PHI));
            data[i] = baseWave + phiLayer + piLayer;
        }
    }

    waveChart.update();
    requestAnimationFrame(animateWaveform);
}

// Start waveform animation
animateWaveform();


// ---------------------------------------------------
// 2. Interaction: Diamond Core & Vitality
// ---------------------------------------------------
function toggleVitality() {
    isVitalityActive = !isVitalityActive;
    const core = document.getElementById('diamond-core');
    const label = document.getElementById('vitality-label');
    const statusBox = document.getElementById('engine-status');
    const dataset = waveChart.data.datasets[0];

    if (isVitalityActive) {
        // Activate UI State
        core.classList.add('active');
        label.textContent = "Vitality Engaged";
        label.classList.replace('text-gray-400', 'text-gold');

        // Update Chart Colors to Iridescent/Gold vibe
        dataset.borderColor = '#D4AF37';
        dataset.backgroundColor = 'rgba(212, 175, 55, 0.1)';
        statusBox.textContent = "ENGINE: PHI-ETERNAL (φ MODULATION)";
        statusBox.classList.replace('text-cyanGlow', 'text-gold');
        statusBox.classList.replace('border-cyanGlow/20', 'border-gold/50');

    } else {
        // Deactivate UI State
        core.classList.remove('active');
        label.textContent = "Engage Vitality";
        label.classList.replace('text-gold', 'text-gray-400');

        // Revert Chart Colors
        dataset.borderColor = '#00F0FF';
        dataset.backgroundColor = 'rgba(0, 240, 255, 0.05)';
        statusBox.textContent = "ENGINE: STANDARD SINE";
        statusBox.classList.replace('text-gold', 'text-cyanGlow');
        statusBox.classList.replace('border-gold/50', 'border-cyanGlow/20');
    }
}


// ---------------------------------------------------
// 3. 3D View Manipulation (The 3 Mockup Axes)
// ---------------------------------------------------
function changeView(viewType) {
    const wrapper = document.getElementById('vst-ui');
    const envContainer = document.getElementById('env-container');
    const btnHero = document.getElementById('btn-hero');
    const btnAngled = document.getElementById('btn-angled');
    const btnEnv = document.getElementById('btn-env');

    // Reset Button Styles
    [btnHero, btnAngled, btnEnv].forEach(btn => {
        btn.className = "px-6 py-2 border border-white/10 text-gray-400 rounded text-sm tracking-widest font-bold transition-all hover:text-white hover:border-white/30";
    });

    // Reset Wrapper Classes
    wrapper.className = "vst-wrapper relative w-full max-w-5xl phi-proportions bg-gradient-to-br from-[#1a1a1a] to-[#050505] rounded-lg border-2 border-[#E5E4E2]/20 p-8 flex flex-col justify-between z-10";
    envContainer.classList.remove('view-environment-active');

    if (viewType === 'hero') {
        wrapper.classList.add('view-hero');
        wrapper.style.boxShadow = "inset 0 0 50px rgba(0,0,0,1)";
        btnHero.className = "px-6 py-2 border border-cyanGlow/50 bg-cyanGlow/10 text-cyanGlow rounded text-sm tracking-widest font-bold transition-all hover:bg-cyanGlow/20";
    }
    else if (viewType === 'angled') {
        wrapper.classList.add('view-angled');
        btnAngled.className = "px-6 py-2 border border-cyanGlow/50 bg-cyanGlow/10 text-cyanGlow rounded text-sm tracking-widest font-bold transition-all hover:bg-cyanGlow/20";
    }
    else if (viewType === 'environment') {
        wrapper.classList.add('view-environment');
        envContainer.classList.add('view-environment-active');
        btnEnv.className = "px-6 py-2 border border-cyanGlow/50 bg-cyanGlow/10 text-cyanGlow rounded text-sm tracking-widest font-bold transition-all hover:bg-cyanGlow/20";
    }
}

// ---------------------------------------------------
// 4. Exotic Environment Particles (HTML Canvas)
// ---------------------------------------------------
const envCtx = document.getElementById('env-canvas').getContext('2d');
let particles = [];
const numParticles = 80;

function initParticles() {
    const canvas = document.getElementById('env-canvas');
    canvas.width = canvas.parentElement.offsetWidth;
    canvas.height = canvas.parentElement.offsetHeight;
    particles = [];
    for (let i = 0; i < numParticles; i++) {
        particles.push({
            x: Math.random() * canvas.width,
            y: Math.random() * canvas.height,
            radius: Math.random() * 2 + 0.5,
            angle: Math.random() * Math.PI * 2,
            speed: (Math.random() * 0.5 + 0.1) * (Math.random() > 0.5 ? 1 : -1),
            orbitCenter: { x: canvas.width / 2, y: canvas.height / 2 },
            distance: Math.random() * (canvas.width / 1.5)
        });
    }
}

function animateParticles() {
    const canvas = document.getElementById('env-canvas');
    // Only draw if the environment is active to save performance
    if (document.getElementById('env-container').classList.contains('view-environment-active')) {
        envCtx.clearRect(0, 0, canvas.width, canvas.height);

        // Draw nebulae gradient
        const grad = envCtx.createRadialGradient(canvas.width/2, canvas.height/2, 0, canvas.width/2, canvas.height/2, canvas.width/1.5);
        grad.addColorStop(0, 'rgba(139, 92, 246, 0.15)'); // Purple core
        grad.addColorStop(0.5, 'rgba(0, 240, 255, 0.05)'); // Cyan mid
        grad.addColorStop(1, 'transparent');
        envCtx.fillStyle = grad;
        envCtx.fillRect(0, 0, canvas.width, canvas.height);

        // Draw floating light particles in phi-like spirals
        particles.forEach(p => {
            p.angle += p.speed * 0.01;
            // Introduce slight spiral logic
            p.x = p.orbitCenter.x + Math.cos(p.angle) * p.distance;
            p.y = p.orbitCenter.y + Math.sin(p.angle) * p.distance * 0.6; // Flatten orbit for 3d feel

            envCtx.beginPath();
            envCtx.arc(p.x, p.y, p.radius, 0, Math.PI * 2);
            envCtx.fillStyle = `rgba(255, 255, 255, ${Math.random() * 0.5 + 0.3})`;
            envCtx.shadowBlur = 10;
            envCtx.shadowColor = p.radius > 1.5 ? '#00F0FF' : '#D4AF37';
            envCtx.fill();
        });
    }
    requestAnimationFrame(animateParticles);
}

window.addEventListener('resize', initParticles);
initParticles();
animateParticles();

</script>
</body>
</html>