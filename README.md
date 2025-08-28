Name: Sumit Ganesh Kothalkar
Roll No: 2025201076

q1:Seam Carving

input format: <image_path> <new_width> <new_height>
In image path you can enter path relative to q1.cpp or absolute path of the image.

run format: 1)g++ 2025201076_A1_Q1.cpp $(pkg-config --cflags --libs opencv4)
2)./a.out
after this you put the input according to input format.

a new file "output.jpeg" will be created you can open that file to see which seams are being removed .You can see the animation there.After the completion "output.jpeg" is the final output image with new width and height.

q2:Tries

input format:
first you enter: n q
n=number of words in the dictionary.
q=number of queries.

then next q lines of queries: a t
a=type of query
    a=1 means spell check;
    a=2 means autoComplete;
    a=3 means autoCorrect;

t=string

run format: 1)g++ 2025201076_A1_Q2.cpp
2)./a.out
then you enter input according to input format.

q3:Battle of Banners

input format: 
first you enter : q
q=number of queries.

then next q lines of queries: a 
a=type of query
    a=0 means update
    a=1 means print 
and if a=0 then we accept three more values 
    l,r and h
    l=left coordinate
    r=right coordinate
    h=height
and if a=0 then we print the silhouette
run format:1)2025201076_A1_Q3.cpp
2)./a.out
then put the input according to input format.
