# Bezier curve drawer - Computer Graphics
My solution to Games101 HW4. The GUI interface is provided by the teaching team.

# My main tasks
1. Implement de Casteljau algorithm to draw a Bézier curve with any number of control points
2. Implement my Original anti-aliasing method to massively improve the smoothness of the Curve.

# Prerequisites
You need OpenCV to build this project. 
If you're on Ubuntu, run 
1. sudo apt install libopencv-dev

# Build
then, go into ./build/ directory or create one if it's not there and run
1. cmake ..
2. make

# Run (How to use the program?)
1. ./BezierCurve
2. Then, you use your mouse to click 4 control_points on the screen to generate a curve.

# Library used
OpenCV for loading images and creating images.

# Result
## Before
<img width="555" height="439" alt="without-antialiasing" src="https://github.com/user-attachments/assets/1766d3dc-953e-4b6c-93fd-c0fe5a8ce2f1" />

## After
<img width="555" height="439" alt="better_my_method" src="https://github.com/user-attachments/assets/44a2cd41-e04d-4a7e-8330-572fa3495752" />

# Explanation of my anti-aliasing method 
My anti-aliasing method:

1. First, find the 4 pixels that surround the point. They form a square. Let's the sides of the square "edges".
2. Then, compute the ratio t, which is the distance between the point and the left edge. Use this ratio t to split the total energy (1.0f) to allocate "energy" to the left. The amount is 1.0f * t. For the right pixels, total amount of 1.0f * (1-t) is allocated
3. Then, compute the ratio s, which is the distance between the point and the top edge. Use this ratio s to further split the energy to the top and the bottom pixel.

Benefits:
- The total "energy" ([0-1]) is preserved when you sum up the values of all the 4 points.
- When the point lies exactly on one side or one pixel, all the magnitude/energy will be allocated to that side/pixel, which matches our intuition and ensure the curve's shape is very accurate.
