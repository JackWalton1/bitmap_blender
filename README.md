# bitmap_blender
Blends bitmap images via command line.

try running:

gcc blendimages.c -o blendimages

./blendimages wolf.bmp lion.bmp 0.5 lionWolf.bmp (./program [inBmp1] [inBmp2] [Ratio(0-1.0)] [bmpOutFile])


Does not work on images of different resolutions (Does not interpolate)
