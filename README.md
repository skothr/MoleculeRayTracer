# MoleculeRayTracer

Currently only works on Windows due to the GUI.

To port to non-windows OS:
  * Get rid of all GUI headers, and don't make RayTracer inherit from Control
  * RayTracer draw function pretty much just needs access to a Camera object (which should work on any OS --> or you can just pass existing matrices/position)
  * Window, keystrokes, and mouse functionality need to be ported to whatever framework you're using
  * Main loop needs to be updated to your framework
