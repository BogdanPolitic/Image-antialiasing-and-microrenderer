# Image-antialiasing-and-microrenderer
There are two little programs.
The first one applies the resize of an image by doing pixel antialiasing.
The second one takes a line equation and renders a black line, of a 3% * (image width) thickness, which respects the given equation.
(the "output.pgm" image is an example of a program output, a line whose equation is −1 ∗ x + 2 ∗ y + 200 = 0, on a 400x400 white image)

Running tasks:
--> make

./homework calls the resize & antialiasing program.
./homework1 calls the line renderer program.

--> ./homework [inputImageName] [outputImageName] [resizeFactor] [numberOfThreads]

The [resizeFactor] is the number of times that the image dimensions are reduced. It should be an integer.
The [numberOfThreads] specifies the number of parallel threads running during the execution. Until a certain threshold, the more threads you choose, the more time-lapse performance you will benefit of.

--> ./homework1 [outputImageName] [width] [a-coefficient] [b-coefficient] [c-coefficient] [numberOfThreads]

The [width] is the output image resized width.
The [a-coefficient], [b-coefficient], [c-coefficient] are the liniar equation coefficients a, b, c respectively. The liniar equation is expressed like this:
a * x + b * y + c = 0.
In the "output.pgm" example image, a = -1, b = 2, c = 200. So the program running command for the 400x400 output image was the following: "./homework1 output.pgm 400 4 -1 2 200".


Full information about the programs functioning are found in the "Task.pdf".
