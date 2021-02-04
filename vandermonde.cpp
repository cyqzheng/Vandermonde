//code for assignment 0
//written by Catherine Zheng


#include <iostream>

using namespace std;

bool dataSet(double *v[], double ymat[], int rows, int columns){

    for (int i=0; i<rows; i++){
        cout << "Input x" << i << ": "; //user prompts
        cin >> v[i][1]; //store value in second column of v

        cout << "Input y" << i << ": "; //user prompt for y
        cin >> ymat[i]; //store y values in a separate matrix (y-matrix -> ymat)
        cout<<endl; //format

        for (int k=0; k<i; k++){ //checking loop for the same inputs to prevent a discriminant of 0 (which means inverse cannot be found)
            if (v[i][1]== v[k][1]){ //if there are two of the same xs
                if (ymat[i]== ymat[k]){ //if there are two of the same ys as well
                    cout << "Same input as before, please restart"; //the two inputs are the same
                    return false; //program ends
                }
            }
        }
    }

    cout<<endl<<endl; //format

     return true; //no problems
}

void makeV(double *v[], int rows, int columns){ //function that will fill in the rest of matrix v
    for (int i=0; i<rows; i++){ //for every row
        for(int j=2; j<columns; j++){ //and for all columns other than columns 0 and 1 which have already been filled

            v[i][j] = v[i][j-1]*v[i][1]; //they are found by multiplying the column before it with column 1, where the original x value is
                                         //e.g column 6 will contain the x^5 value, so it can be found by multiplying
                                         //column 5(x^4) with column 1 (x) since (x^4)*(x)= x^5

        }
    }

}

void makeVtV(double*vtv[], double*v[], int rows, int columns){ //function that multiplies Vt with V
    for (int j=0; j<columns; j++){ //for the each row of Vt (which is the column of V)
        for(int k=0; k<columns; k++){ //for each column of V which corresponds with the row of Vt

            double temp=0; //declare a temporary value for convenience

            for (int m=0; m<rows; m++){ //for every row in the column
                temp += v[m][j]*v[m][k]; //multiply the corresponding values and add them to temp
                                         //this creates the addition of the products

            }

            vtv[j][k] = temp; //temp is equal to vtv

        }
    }
}

void makeInverse(double*inverse[], int columns){ //begins the inverse matrix by initially creating an identity matrix
    for(int i=0; i<columns; i++){ //for all rows
        for (int j=0; j<columns; j++){ //for all columns
            if (i==j){ //when the row# and column# are equal
                inverse[i][j]=1; //the value is 1, creating a diagonal of 1s from the top left to bottom right
            }
            else{
                inverse[i][j]=0; //everything else in the matrix is 0
            }
        }
    }
}

void reduce(double*vtv[], double*inverse[], int rows, int columns){ //reduces vtv to an identity matrix and inverse to the inverse matrix

    for (int k=0; k<columns; k++){ //the column index that will divide each number in the row
        for (int i=k; i<rows; i++){ //the row that is going to be divided
            double temp = vtv[i][k]; //set a temporary variable that is vtv[row][column index]

            for(int j=0; j<columns;j++){ //goes through each number in the row
                vtv[i][j] /= temp; //divide all numbers in the row by temp, creating one column where all values are 1
                inverse[i][j] /= temp; //divide all numbers in the same row of the identity matrix by temp

            }
        }
        for (int i=k+1; i<rows; i++){ //for all rows below the one where row = column index
            //(e.g if we are dividing all numbers to make column 0 = 1 we subtract row 0 from all rows below it
            for(int j=0; j<columns;j++){ //for each number in the row

                vtv[i][j] -= vtv[k][j]; //subtract each number in the rows below by each number in the row where row#=column#
                inverse[i][j] -= inverse[k][j]; //same thing but in identity matrix to correspond

            }//move onto next row until end
        } //finish subtracting and increase column index by 1
    }

    //the above part creates a upright triangle of 0s on the bottom left and a diagonal of zeros from the top left to bottom right in the vtv matrix
    //which we are trying to make an identity matrix

    //e.g we have
    //1-3-4-6-3
    //0-1-4-2-6
    //0-0-1-9-1
    //0-0-0-1-3
    //0-0-0-0-1
    //as a matrix
    //note that the bottommost row is in its identity matrix state

    //next we need to retain the diagonal of 1s while turning all of the values not already zero, into zero
    //creating an inverted triangle of zeros from the top right


    //reduce from bottom to top, same as before but in a different direction to leave on diagonal of 1s
    //note that we already have a diagonal of zeros and we only want to get rid of the extraneous values on the right
    //so we start from the bottom that has already been reduced to its identity matrix state and reduce upwards
    //first turning the second last row to 0-0-0...-0-1-0 then the 3rd last to 0-0-...-1-0-0 and so on

    for (int i=rows-1; i>0; i--){ //starting from the bottom most row
        for (int j=i-1; j>-1; j--){ //going through columns from the left

            double temp = vtv[j][i]; //set a temporary variable that is equal to the value that needs to be subtracted

            for (int k=rows-1; k>-1; k--){
                vtv[j][k]-=temp*vtv[i][k]; //multiplying one by any number equals that number. By subtracting it, it becomes 0.
                                           //subtracting 0-0-0...-0-1 from every thing above after multiplying
                                           //so everything above the one becomes a 0
                inverse[j][k]-=temp*inverse[i][k]; //must do the same thing to inverse
            }
        }
    }

    //after this vtv has become an identity matrix and the inverse is found by doing the same operations to the initial identity matrix
}

int main() //main
{
    int rows, columns; //declare variables that will store the sizes rows and columns

    cout << "Degree of polynomial?" << endl;
    cin >> columns;
    columns += 1; //to add the constant

    cout << "How many DIFFERENT entries? (must be greater than degree of polynomial)" << endl;
    cin >> rows;
    if (rows < columns){
        cout << "Not enough entries, please restart" <<endl; //if entries aren't adequate
        return 0; //quit program
    }

    int rowsVTY = rows; //declare a secondary row variable which will later be use only for Vt*Y
    double ymat [rows]; //declare size of the y matrix



    double *v[rows];  //declaring v matrix and its row size
    for (int i=0;i<rows; i++){ //create the 2D array's second dimension of size columns
        v[i]= new double[columns];
    }
    for (int j=0; j<rows; j++){
        v[j][0]=1; //fill the first column with 1s
    }

    //inputting dataset
    if (dataSet(v, ymat, rows, columns)== false) //if values are invalid
        return 0; //quite program

    //When values are valid, program continues


    //making v matrix from dataset
    makeV(v, rows, columns);


    //declare Vt*V matrix
    double *vtv[columns]; //new row value is equal to the number of columns of v
    for (int i=0;i<columns; i++){
        vtv[i]= new double[columns];
    }

    makeVtV(vtv, v, rows, columns);  //make vtv from v



    double *inverse[columns];    //declare inverse (identity matrix)
    for (int i=0;i<columns; i++){
        inverse[i]= new double[columns];
    }
    makeInverse(inverse, columns); //make inverse matrix


    //display results

    cout<<"Input data"<<endl;
        for (int i=0; i<rows; i++){
            cout<<"("<<v[i][1]<<", "<<ymat[i]<<")"<<endl;
        }
    cout << endl << endl;

    cout<<"Vandermonde matrix"<<endl;  //display v
    for (int i=0; i<rows; i++){
        for (int j=0; j<columns; j++){
            cout<< v[i][j] << "   ";
        }
        cout << endl;
    }

    cout << endl << endl;

    cout<<"Transposed Vandermonde matrix"<<endl;  //display Vt
    for (int i=0; i<columns; i++){
        for (int j=0; j<rows; j++){
            cout<< v[j][i] << "   ";
        }
        cout << endl;
    }

    cout << endl << endl;


    cout<<"VtV matrix"<<endl;       //display vtv
    for (int i=0; i<columns; i++){
        for (int j=0; j<columns; j++){
            cout<< vtv[i][j] << "   ";
        }
        cout << endl;
    }

    cout << endl << endl;


    //continue to find a values
    rows = columns; //to simplify, initial rows value retained in rowsVTY
    reduce(vtv, inverse, rows, columns); //reduce end row to one, triangle from bottom left of zero, then reduce top right triangle

    for (int i=0; i<columns; i++){
        for (int j=0; j<columns; j++){
            cout<< inverse[i][j] << "   ";
        }
        cout << endl;
    }

    cout<<endl<<endl;


    for (int i=0; i<columns; i++){
        for (int j=0; j<columns; j++){
            cout<< vtv[i][j] << "   ";
        }
        cout << endl;
    }

    //make vt*y matrix
    double vty[rowsVTY]; //size of initial row size

    for (int i=0; i<rowsVTY; i++){   //multiply Vt with y matrix
        for (int j=0; j<rowsVTY; j++){

            vty[i] += v[j][i]*ymat[j];

        }
    }


    //find a by multiplying inverse and Vty
    double finalA[rows];

    for (int i=0; i<rows; i++){
        for (int j=0; j<columns; j++){
            finalA[i] += inverse[i][j]*vty[j];

        }
    }


    //results for a

    cout << "the A values (coeffiecient values) are(ordered from lowest to highest polynomial degree):" <<endl<<endl;  //display a values

    for (int i=0; i<rows; i++){
        cout << finalA[i] << endl;
    }
    cout << endl ;

    cout << "the polynomial is:" << endl;
    for (int i=0; i<rows; i++){
        if (i==0){
            cout << finalA[i] << " + ";
        }
        else if (i==1){
            cout << finalA[i] << "x" << " + ";
        }
        else if (i==(rows-1)){
            cout << finalA[i] << "x^" << i ;
        }
        else
            cout << finalA[i] << "x^" << i << " + ";
    }
    cout << endl<<endl;
    cout << "END :)";


    // delete to prevent memory leaks
    for (int i=0; i<rowsVTY; i++){
        delete [] v[i];

    }
    for (int i=0; i<rows; i++){
        delete [] inverse[i];
        delete [] vtv[i];
    }


    return 0;
}

