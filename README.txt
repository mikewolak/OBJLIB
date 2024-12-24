3D Engine Library V1.1 - User Manual

Overview:
This lightweight 3D engine library provides basic functionality for rendering 3D wireframe models. It's designed to be backend-agnostic, allowing integration with various graphics systems.

Key Features:
1. Wireframe rendering of 3D meshes
2. Mesh rotation and zooming
3. Automatic mesh centering and scaling
4. Support for custom color definitions
5. Thick line rendering option

Integration Steps:
1. Include the 3d_engine.h header in your project.
2. Implement a pixel-drawing function compatible with your graphics backend.
3. Link against the compiled 3d_engine library.

Core Functions:
- init_3d_engine(width, height): Initialize the engine with screen dimensions.
- set_put_pixel_callback(callback): Set the function used for drawing pixels.
- render_mesh(mesh, rotation_x, rotation_y, rotation_z, zoom, thick_lines): Render a 3D mesh.
- center_mesh(mesh): Center a mesh at the origin.
- fit_mesh_to_screen(mesh, width, height): Calculate optimal zoom for fitting mesh to screen.
- create_cube(mesh): Create a simple cube mesh (for testing).
- free_mesh(mesh): Free memory allocated for a mesh.

Usage Example:
1. Initialize your graphics backend.
2. Call init_3d_engine() with your screen dimensions.
3. Implement a put_pixel function for your backend.
4. Call set_put_pixel_callback() with your function.
5. Load or create a mesh.
6. In your render loop:
   - Clear the screen (using your backend).
   - Call render_mesh() with your mesh and desired parameters.
   - Update the screen (using your backend).

Color Definitions:
The library includes 16 predefined colors (e.g., COLOR_RED, COLOR_GREEN). 
Use these or define custom colors as 24-bit RGB values.

Limitations:
- Wireframe rendering only (no solid fills).
- No texture support.
- Basic perspective projection.
- No lighting calculations.

Performance Considerations:
- Rendering speed depends on the efficiency of your put_pixel implementation.
- Large meshes may impact performance on slower systems.

This library provides a simple, flexible foundation for 3D wireframe rendering that can be easily integrated with various graphics backends.
