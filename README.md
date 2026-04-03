# Project_PaleBlueDot
Pale Blue Dot — Advanced OpenGL Rendering Demo  
A real-time OpenGL-based Earth visualization inspired by the Pale Blue Dot concept, focused on achieving cinematic planetary rendering through physically grounded techniques and modern GPU pipelines. The project emphasizes large-scale realism, atmospheric depth, and high-quality reconstruction.

## Core Rendering & Graphics Techniques

### Temporal Super Resolution (TSR)
Implemented temporal accumulation and reprojection to achieve high-quality upscaling and anti-aliasing, improving visual stability and detail reconstruction across frames.

### Physically Based Shading (PBR)
Utilized energy-conserving BRDF models for accurate material response, ensuring realistic interaction between sunlight, atmosphere, and terrain.

### Volumetric Clouds (SIGGRAPH 2015 & 2017 Inspired)
Implemented physically motivated volumetric cloud rendering using 3D noise, light scattering, and density sampling to simulate realistic cloud formations and lighting.

### Atmospheric Scattering
Modeled Rayleigh and Mie scattering to simulate Earth’s atmosphere, producing realistic horizon glow, color gradients, and light falloff.

### Beer Shadow Maps & Atmospheric Occlusion
Applied Beer–Lambert law-based light attenuation for cloud shadows and atmospheric light absorption, enhancing depth and realism in large-scale lighting.

### SRTM Digital Elevation Mapping
Integrated Shuttle Radar Topography Mission (SRTM) data to generate accurate terrain elevation, improving planetary surface fidelity.

### Exponential Height Fog
Implemented height-based fog for additional depth cues and smooth blending between terrain and atmosphere.

### Deferred Rendering Pipeline
Designed a G-buffer-based deferred shading system to efficiently handle multiple lighting interactions and complex shading scenarios.
