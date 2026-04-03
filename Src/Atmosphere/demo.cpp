/**
 * Copyright (c) 2017 Eric Bruneton
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holders nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

 /*<h2>atmosphere/demo/demo.cc</h2>

 <p>This file, together with the shader <a href="demo.glsl.html">demo.glsl</a>,
 shows how the API provided in <a href="../model.h.html">model.h</a> can be used
 in practice. It implements the <code>Demo</code> class whose header is defined
 in <a href="demo.h.html">demo.h</a> (note that most of the following code is
 independent of our atmosphere model. The only part which is related to it is the
 <code>InitModel</code> method).
 */

#include "demo.h"

#include <gl/glew.h>

#include <algorithm>
#include <cmath>
#include <map>
#include <stdexcept>
#include <sstream>
#include <string>
#include <vector>

#include "MyWin32.h"

namespace atmosphere {
    namespace demo {

        /*
        <p>Our demo application renders a simple scene made of a purely spherical planet
        and a large sphere (of 1km radius) lying on it. This scene is displayed by
        rendering a full screen quad, with a fragment shader computing the color of each
        pixel by "ray tracing" (which is very simple here because the scene consists of
        only two spheres). The vertex shader is thus very simple, and is provided in the
        following constant. The fragment shader is more complex, and is defined in the
        separate file <a href="demo.glsl.html">demo.glsl</a> (which is included here as
        a string literal via the generated file <code>demo.glsl.inc</code>):
        */

        namespace {

            constexpr double kPi = 3.1415926;
            constexpr double kSunAngularRadius = 0.0225;
            constexpr double kSunSolidAngle = kPi * kSunAngularRadius * kSunAngularRadius;
            constexpr double kLengthUnitInMeters = 100.0;


         //   static std::map<int, Demo*> INSTANCES;

        }  // anonymous namespace

        /*
        <p>The class constructor is straightforward and completely independent of our
        atmosphere model (which is initialized in the separate method
        <code>InitModel</code>). It's main role is to create the demo window, to set up
        the event handler callbacks (it does so in such a way that several Demo
        instances can be created at the same time, using the <code>INSTANCES</code>
        global variable), and to create the vertex buffers and the text renderer that
        will be used to render the scene and the help messages:
        */

        Demo::Demo(int viewport_width, int viewport_height) :
            use_constant_solar_spectrum_(true),
            use_ozone_(true),
            use_combined_textures_(true),
            use_half_precision_(true),
            use_luminance_(NONE),
            do_white_balance_(false),
            show_help_(true),
            program_(0),
            view_distance_meters_(9000.0),
            view_zenith_angle_radians_(1.47),
            view_azimuth_angle_radians_(-0.1),
            sun_zenith_angle_radians_(1.3),
            sun_azimuth_angle_radians_(2.9),
            exposure_(10.0) {


            glGenVertexArrays(1, &full_screen_quad_vao_);
            glBindVertexArray(full_screen_quad_vao_);
            glGenBuffers(1, &full_screen_quad_vbo_);
            glBindBuffer(GL_ARRAY_BUFFER, full_screen_quad_vbo_);
            const GLfloat vertices[] = {
              -1.0, -1.0, 0.0, 1.0,
              +1.0, -1.0, 0.0, 1.0,
              -1.0, +1.0, 0.0, 1.0,
              +1.0, +1.0, 0.0, 1.0,
            };
            glBufferData(GL_ARRAY_BUFFER, sizeof vertices, vertices, GL_STATIC_DRAW);
            constexpr GLuint kAttribIndex = 0;
            constexpr int kCoordsPerVertex = 4;
            glVertexAttribPointer(kAttribIndex, kCoordsPerVertex, GL_FLOAT, false, 0, 0);
            glEnableVertexAttribArray(kAttribIndex);
            glBindVertexArray(0);

          //  text_renderer_.reset(new TextRenderer);

            InitModel();
        }

        /*
        <p>The destructor is even simpler:
        */

        Demo::~Demo() {
            glDeleteShader(vertex_shader_);
            glDeleteShader(fragment_shader_);
            glDeleteProgram(program_);
            glDeleteBuffers(1, &full_screen_quad_vbo_);
            glDeleteVertexArrays(1, &full_screen_quad_vao_);
          //  INSTANCES.erase(window_id_);
        }

        /*
        <p>The "real" initialization work, which is specific to our atmosphere model,
        is done in the following method. It starts with the creation of an atmosphere
        <code>Model</code> instance, with parameters corresponding to the Earth
        atmosphere:
        */

        void Demo::InitModel() {
            // Values from "Reference Solar Spectral Irradiance: ASTM G-173", ETR column
            // (see http://rredc.nrel.gov/solar/spectra/am1.5/ASTMG173/ASTMG173.html),
            // summed and averaged in each bin (e.g. the value for 360nm is the average
            // of the ASTM G-173 values for all wavelengths between 360 and 370nm).
            // Values in W.m^-2.
            constexpr int kLambdaMin = 360;
            constexpr int kLambdaMax = 830;
            constexpr double kSolarIrradiance[48] = {
              1.11776, 1.14259, 1.01249, 1.14716, 1.72765, 1.73054, 1.6887, 1.61253,
              1.91198, 2.03474, 2.02042, 2.02212, 1.93377, 1.95809, 1.91686, 1.8298,
              1.8685, 1.8931, 1.85149, 1.8504, 1.8341, 1.8345, 1.8147, 1.78158, 1.7533,
              1.6965, 1.68194, 1.64654, 1.6048, 1.52143, 1.55622, 1.5113, 1.474, 1.4482,
              1.41018, 1.36775, 1.34188, 1.31429, 1.28303, 1.26758, 1.2367, 1.2082,
              1.18737, 1.14683, 1.12362, 1.1058, 1.07124, 1.04992
            };
            // Values from http://www.iup.uni-bremen.de/gruppen/molspec/databases/
            // referencespectra/o3spectra2011/index.html for 233K, summed and averaged in
            // each bin (e.g. the value for 360nm is the average of the original values
            // for all wavelengths between 360 and 370nm). Values in m^2.
            constexpr double kOzoneCrossSection[48] = {
              1.18e-27, 2.182e-28, 2.818e-28, 6.636e-28, 1.527e-27, 2.763e-27, 5.52e-27,
              8.451e-27, 1.582e-26, 2.316e-26, 3.669e-26, 4.924e-26, 7.752e-26, 9.016e-26,
              1.48e-25, 1.602e-25, 2.139e-25, 2.755e-25, 3.091e-25, 3.5e-25, 4.266e-25,
              4.672e-25, 4.398e-25, 4.701e-25, 5.019e-25, 4.305e-25, 3.74e-25, 3.215e-25,
              2.662e-25, 2.238e-25, 1.852e-25, 1.473e-25, 1.209e-25, 9.423e-26, 7.455e-26,
              6.566e-26, 5.105e-26, 4.15e-26, 4.228e-26, 3.237e-26, 2.451e-26, 2.801e-26,
              2.534e-26, 1.624e-26, 1.465e-26, 2.078e-26, 1.383e-26, 7.105e-27
            };
            // From https://en.wikipedia.org/wiki/Dobson_unit, in molecules.m^-2.
            constexpr double kDobsonUnit = 2.687e20;
            // Maximum number density of ozone molecules, in m^-3 (computed so at to get
            // 300 Dobson units of ozone - for this we divide 300 DU by the integral of
            // the ozone density profile defined below, which is equal to 15km).
            constexpr double kMaxOzoneNumberDensity = 300.0 * kDobsonUnit / 15000.0;
            // Wavelength independent solar irradiance "spectrum" (not physically
            // realistic, but was used in the original implementation).
            constexpr double kConstantSolarIrradiance = 1.5;
            constexpr double kBottomRadius = 6360000.0;
            constexpr double kTopRadius = 6620000.0;//Original-> 6420000.0;
            constexpr double kRayleigh = 1.24062e-6;
            constexpr double kRayleighScaleHeight = 8000.0;// Original-> 8000.0;
            constexpr double kMieScaleHeight = 1200.0;
            constexpr double kMieAngstromAlpha = 0.0;
            constexpr double kMieAngstromBeta = 5.328e-3;
            constexpr double kMieSingleScatteringAlbedo = 0.9;
            constexpr double kMiePhaseFunctionG = 0.8;
            constexpr double kGroundAlbedo = 0.1;
            const double max_sun_zenith_angle =
                (use_half_precision_ ? 102.0 : 120.0) / 180.0 * kPi;

            DensityProfileLayer
                rayleigh_layer(0.0, 1.0, -1.0 / kRayleighScaleHeight, 0.0, 0.0);
            DensityProfileLayer mie_layer(0.0, 1.0, -1.0 / kMieScaleHeight, 0.0, 0.0);
            // Density profile increasing linearly from 0 to 1 between 10 and 25km, and
            // decreasing linearly from 1 to 0 between 25 and 40km. This is an approximate
            // profile from http://www.kln.ac.lk/science/Chemistry/Teaching_Resources/
            // Documents/Introduction%20to%20atmospheric%20chemistry.pdf (page 10).
            std::vector<DensityProfileLayer> ozone_density;
            ozone_density.push_back(
                DensityProfileLayer(25000.0, 0.0, 0.0, 1.0 / 15000.0, -2.0 / 3.0));
            ozone_density.push_back(
                DensityProfileLayer(0.0, 0.0, 0.0, -1.0 / 15000.0, 8.0 / 3.0));

            std::vector<double> wavelengths;
            std::vector<double> solar_irradiance;
            std::vector<double> rayleigh_scattering;
            std::vector<double> mie_scattering;
            std::vector<double> mie_extinction;
            std::vector<double> absorption_extinction;
            std::vector<double> ground_albedo;
            for (int l = kLambdaMin; l <= kLambdaMax; l += 10) {
                double lambda = static_cast<double>(l) * 1e-3;  // micro-meters
                double mie =
                    kMieAngstromBeta / kMieScaleHeight * pow(lambda, -kMieAngstromAlpha);
                wavelengths.push_back(l);
                if (use_constant_solar_spectrum_) {
                    solar_irradiance.push_back(kConstantSolarIrradiance);
                }
                else {
                    solar_irradiance.push_back(kSolarIrradiance[(l - kLambdaMin) / 10]);
                }
                rayleigh_scattering.push_back(kRayleigh * pow(lambda, -4));
                mie_scattering.push_back(mie * kMieSingleScatteringAlbedo);
                mie_extinction.push_back(mie);
                absorption_extinction.push_back(use_ozone_ ?
                    kMaxOzoneNumberDensity * kOzoneCrossSection[(l - kLambdaMin) / 10] :
                    0.0);
                ground_albedo.push_back(kGroundAlbedo);
            }

            model_.reset(new Model(wavelengths, solar_irradiance, kSunAngularRadius,
                kBottomRadius, kTopRadius, { rayleigh_layer }, rayleigh_scattering,
                { mie_layer }, mie_scattering, mie_extinction, kMiePhaseFunctionG,
                ozone_density, absorption_extinction, ground_albedo, max_sun_zenith_angle,
                kLengthUnitInMeters, use_luminance_ == PRECOMPUTED ? 15 : 3,
                use_combined_textures_, use_half_precision_));
            model_->Init();

        }

        GLuint Demo::GetTempTexture(void)
        {
            return model_.get()->GetTexture();
        }


    }  // namespace demo
}  // namespace atmosphere
