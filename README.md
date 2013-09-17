Cloud Renderer
==

Real-time 3D cloud simulation and rendering (work in progress).

Cloud Renderer is an OpenGL implementation of methods for simulating and rendering clouds. Simulation is based on cellular automata described by Nagel and extended by Dobashi et al. Rendering is based on volume ray casting. Single scattering of light is approximated by casting shadow rays at sample points.

![Clouds rendered with Cloud Renderer](https://raw.github.com/LukaCiko/CloudRenderer/master/Screenshot.png)

##Dependencies
Cloud renderer uses GLFW, GLEW, GLM and SOIL libraries. GLFW and GLEW need to be installed in order to run the program.

##Licence
CloudRenderer is made available under the terms of the MIT License. See the LICENSE file that accompanies this distribution for the full text of the license.
