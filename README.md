Cloud Renderer
=============

Real-time 3D cloud simulation and rendering (work in progress).

Cloud Renderer is an OpenGL implementation of methods for simulating and rendering clouds. Simulation is based on cellular automata described by Nagel and extended by Dobashi et al. Rendering is based on volume ray casting. Single scattering of light is approximated by casting shadow rays at sample points.

![Clouds rendered with Cloud Renderer](/screenshot.jpg)

### Libraries

Cloud renderer uses GLFW, GLEW, GLM and SOIL libraries. GLFW and GLEW need to be installed in order to run the program.
