// Alycia Riese
// main.cpp

// includes
#include <fstream>
#include <iostream>
#include <cmath>
#include <string>
#include <sstream>

#define VERTICAL 0
#define HORIZONTAL 1

// using statements
using std::string;
using std::cout;
using std::istringstream;

// Function prototypes
void getEnergy(int** pgm, int** energy, int x, int y);
void findCumulativeEnergy(int** p_energy, int** c_energy, int x, int y);
void removeSeam(int** pgm, int** c_energy, int x, int y);


// main function
int main(int argc, char* argv[])
{
    if (argc < 4)
    {
        std::cout << "Error with format, should be: a.exe image.pgm [vertical] [horizontal]\n";
        return 0;
    }

    // get the data from the command
    string image_str = argv[1];
    int vertical = std::stoi(argv[2]);
    int horizontal = std::stoi(argv[3]);

    // open the image
    std::ifstream image;
    image.open(image_str);

    string p2;
    string comment;
    string dimension;
    string gray;

    // separate out the p2, comment, dimension, and grayscale lines
    getline(image, p2);
    getline(image, comment);
    getline(image, dimension);
    getline(image, gray);

    int x;
    int y;

    istringstream iss(dimension);
    iss >> x >> y;
    //int array[x][y];

    //string data;
    //getline(image, data);
    //cout << data;

    // make array of integer ptrs for data from pgm
    int **pgm = new int* [x];
    for(int i = 0; i < x; i++)
    {
        pgm[i] = new int[y];
    }

    // make array of integer ptrs for pixel energy
    int **energy = new int* [x];
    for(int i = 0; i < x; i++)
    {
        energy[i] = new int[y];
    }

    // make array of integer ptrs for cumulative energy
    int **c_energy = new int* [x];
    for(int i = 0; i < x; i++)
    {
        c_energy[i] = new int[y];
    }


    // read the file, put it into the pgm array
    //for(int i = 0; i < x; i++)
    //{
    //    for(int j = 0; j < y; j++)
    //    {
    //        image >> pgm[i][j];
    //    }
    //}

    int t = 0;
    int temp_array[x*y];

    while(!image.eof())
    {   
        string temp_str;
        getline(image, temp_str);
        //cout << temp_str << std::endl;
        istringstream iss(temp_str);
        do
        {
            string subs;
            iss >> subs;
            //cout << subs << std::endl;
            if (subs.length() > 0)
            {
                temp_array[t] = std::stoi(subs);
                t++;

            }
            
        } while (iss);
        
    }

    int k = 0;
    // read the file, put it into the pgm array
    for(int i = 0; i < x; i++)
    {
        for(int j = 0; j < y; j++)
        {
            pgm[i][j] = temp_array[k];
            k++;
        }
    }

    //cout << *temp_array << std::endl;

    //cout << "hello\n" << std::flush; 


/*
    for(int i = 0; i < x; i++)
    {
        for(int j = 0; j < y; j++)
        {
            cout << pgm[i][j] << std::endl;
            
        }
    }
    */
    //cout << **pgm << std::endl;

    // calculate energy
    getEnergy(pgm, energy, x, y);
    findCumulativeEnergy(energy, c_energy, x, y);


    //cout << "hello\n" << std::flush; 
    //cout << **energy << std::endl;


    ////////////////////////////////////////////////////////////////////////

    //remove vertical seams
        for(int p=0; p < atoi(argv[2]); p++){
            
             
            removeSeam(pgm, c_energy, x, y);
           
            y-=1; // there is now one less column
            getEnergy(pgm, energy, x, y);
            findCumulativeEnergy(energy, c_energy, x, y);
        }
 

        
        
        
        
        // make a matrix to turn image into
        int **tempPgm = new int* [y];
        for (int i = 0; i < y; ++i)
            tempPgm[i] = new int[x];
        
        int **tempEnergy = new int* [y];
        for (int i = 0; i < y; ++i)
            tempEnergy[i] = new int[x];
        
        int **tempCum = new int* [y];
        for (int i = 0; i < y; ++i)
            tempCum[i] = new int[x];
        
        
        //turn image 90 degrees
        for (int i=0; i<y ; i++){
            for(int j=0; j<x; j++){
                tempPgm[i][j] = pgm[j][i];
            }
        }
        
        //fill the temp arrays
        getEnergy(tempPgm, tempEnergy, y, x);
        findCumulativeEnergy(tempEnergy, tempCum, y, x);
        
        
        
        // remove horizontal seams
        for(int p=0; p < atoi(argv[3]); p++){
            removeSeam(tempPgm, tempCum, y, x);
            x--; // there is now one less row
            getEnergy(tempPgm, tempEnergy, y, x);
            findCumulativeEnergy(tempEnergy, tempCum, y, x);
        }
        
        for(int i=0; i<x; i++){
            for(int j=0; j<y; j++){
                pgm[i][j] = tempPgm[j][i];
            }
        }






    return 0;
}

// calculates energy of each pixel of the image using 
// e(i,j) = |v(i,j)-v(i-1,j)|+ |v(i,j)-v(i+1,j)|+ |v(i,j)-v(i,j-1)|+ |v(i,j)-v(i,j+1)|
void getEnergy(int** pgm, int** energy, int x, int y)
{
    // loop through all pixels in image
    for(int i = 0; i < x; i++)
    {
        for(int j = 0; j < y; j++)
        {
            // we're at the top edge of image
            if (i == 0)
            {
                // we're at the top left corner
                if (j == 0)
                {
                    energy[i][j] = abs(pgm[i][j] - pgm[i+1][j]) + abs(pgm[i][j] - pgm[i][j+1]);
                }

                // at top right corner
                else if (j == y-1)
                {
                    energy[i][j] = abs(pgm[i][j] - pgm[i+1][j]) + abs(pgm[i][j] - pgm[i][j-1]);
                }

                // normal
                else
                {
                    energy[i][j] = abs(pgm[i][j] - pgm[i+1][j]) + abs(pgm[i][j] - pgm[i][j-1]) + abs(pgm[i][j] - pgm[i][j+1]);
                }
            }

            // bottom edge
            else if (i == x-1)
            {
                // at bottom left corner
                if (j == 0)
                {
                    energy[i][j] = abs(pgm[i][j] - pgm[i-1][j]) + abs(pgm[i][j] - pgm[i][j+1]);
                }

                // at bottom right corner
                else if (j == y-1)
                {
                    energy[i][j] = abs(pgm[i][j] - pgm[i-1][j]) + abs(pgm[i][j] - pgm[i][j-1]);
                }

                // normal
                else 
                {
                    energy[i][j] = abs(pgm[i][j] - pgm[i-1][j]) + abs(pgm[i][j] - pgm[i][j-1]) + abs(pgm[i][j] - pgm[i][j+1]);
                }
            }

            // middle of image
            else
            {
                // left edge
                if (j == 0)
                {
                    energy[i][j] = abs(pgm[i][j] - pgm[i-1][j]) + abs(pgm[i][j] - pgm[i+1][j]) + abs(pgm[i][j] - pgm[i][j+1]);
                }

                //right edge
                else if (j == y-1)
                {
                    energy[i][j] = abs(pgm[i][j] - pgm[i-1][j]) + abs(pgm[i][j] - pgm[i+1][j]) + abs(pgm[i][j] - pgm[i][j-1]);
                }

                // normal
                else
                {
                    energy[i][j] = abs(pgm[i][j] - pgm[i-1][j]) + abs(pgm[i][j] - pgm[i+1][j]) + abs(pgm[i][j] - pgm[i][j-1]) + abs(pgm[i][j] - pgm[i][j+1]);
                }
            }
        }
    }
}


// find the vertical cumulative energy
void findCumulativeEnergy(int** p_energy, int** c_energy, int x, int y)
{
    // loop through all pixels
    for (int i = 0; i < x; i++)
    {
        for (int j = 0; j < y; j++)
        {
            // at top
            if (i == 0)
            {
                c_energy[i][j] = p_energy[i][j];
            }

            // not top
            else
            {
                // left side
                if (j == 0)
                {
                    c_energy[i][j] = p_energy[i][j] + std::min(c_energy[i-1][j], c_energy[i-1][j+1]);
                }

                // right side
                else if (j == y-1)
                {
                    c_energy[i][j] = p_energy[i][j] + std::min(c_energy[i-1][j], c_energy[i-1][j-1]);
                }

                // everything else
                else
                {
                    int compare = std::min(c_energy[i-1][j-1], c_energy[i-1][j]);
                    c_energy[i][j] = p_energy[i][j] + std::min(compare, c_energy[i-1][j+1]);
                    
                }
            }
        }
    }
}


// removes a seam
void removeSeam(int** pgm, int** c_energy, int x, int y)
{
    // find the least energy
    int min = c_energy[0][0];
    int min_pos = 0;
    int temp;
    
    for (int j = 0; j < y; j++)
    {
        // we found new min at j
        if (c_energy[x-1][j] < min)
        {
            min = c_energy[x-1][j];
            min_pos = j;
        }
    }

    
    
    //remove vertical seams
    for(int i = x-1; i >= 0; i--)
    {
        for(int j = 0; j < y-min_pos; j++)
        {
            // shift rest of image
            pgm[i][min_pos+j] = pgm[i][min_pos+j+1];
        }
        
        // check for more values to remove
        if(i > 0)
        {
            if(min_pos == 0)
            {
                temp = std::min(c_energy[i-1][min_pos], c_energy[i-1][min_pos+1]);
                if(temp == c_energy[i-1][min_pos+1])
                {
                    min_pos+=1;
                }
            }

            else if(min_pos == y-1)
            {
                temp = std::min(c_energy[i-1][min_pos], c_energy[i-1][min_pos-1]);
                if(temp == c_energy[i-1][min_pos-1])
                {
                    min_pos-=1;
                }
            }

            else
            {
                int compare = std::min(c_energy[i-1][min_pos-1], c_energy[i-1][min_pos]);
                temp = std::min(compare, c_energy[i-1][min_pos+1]);
                if(temp == c_energy[i-1][min_pos-1])
                {
                    min_pos-=1;
                }
                    
                else if(temp == c_energy[i-1][min_pos+1])
                {
                    min_pos+=1;
                }
            }
        }
    }
}
