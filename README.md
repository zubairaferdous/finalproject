The created program consists of 3 main components. Namingly,
operations.c, image.c, gui.c

main.c kicks of the program by accessing the graphical interface.

gui.c:
handles the graphical user interface. also contains the local variable in which the pixel values are hosted
takes input through iup's dialogue box. 

image.c and image.h:
 ->contains the struct data types declaration(image and pixel) in which the image is kept.
 ->pixel is a 3byte struct which stores rgb value of a single pixel
 ->image keeps track of the height,width and contains an array of pixel(which we treat as an 2d array with the help of pointer arythmatics)
 ->they also contain header and infoheader data type to read and store the first 54 Header bytes of bmp
 ->they have the save,copy(to undo) and freeing functions declared inside them. 
 ->we skip over the offset bytes and take the entire image in top to bottom styel(unlike how it's originally saved
  inside the bmp file) ,in rgb format, inside an array(pointer) of pixel struct. which is then later passed to different
  functions as necessary

operations.c:
this is the core of the entire project. the functions are:

==create blank image- allocates memory according to the given parameters, essentially creates a blank space. 
                       is needed for crop,rotate,blur and sharpenning.
]
==clamp- clamps the brightness value if its more than 255 or less than 0

==grayscale- takes the image data type as the parameter, grays the rgb value of a pixel,and replaces it in the original image

==brightness- takes image and brigness values, changes them with the help of clamp as safety measure, replaces it in the original image

==invert- same as the last 2, subtracts 255 from the rgb values and replaces the pixels

==vertical and horizontal flip- flips the pixels respectively for horizontal and vertical flip. one keeps the middle element of the column
                              as anchor,other keeps the middle element of the row.

==rotate- creates blank space with new height and width, populates the blank space with according pixels, returns the new rotated image

==blur- creates blank space,traverses the entire image,picks a pixel, checks the 3x3 neighbourhood,applies blur by avging the 
         values from those 9 pixel,populates the blank space with the new blur pixel value. blank space is needed to make sure a blurred
         pixel doesnt effect those around it. finally, the completed blurred image replaces the original.
         this also accounts for edge and corner cases while averaging for blur

==sharpen - same as blur,just take weighted avg as the kernel values provided

==crop - creates a blank space following the newly taken height and width, takes the pixel position from which the crop will begin,
          populates the blank space with the according pixels from the original image, returns the cropped image



before every action,a copy of the image is created to provide the undo feature. 
in the end, the program frees all the memories before closing.

<img width="1822" height="1028" alt="Screenshot (47)" src="https://github.com/user-attachments/assets/55056c58-5183-45be-9f18-5fa5bfc44301" />



