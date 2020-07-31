#include <iostream>
using namespace std;

//Error type for grid operations
struct Error {
    bool border_error = false;
    bool conflict_error = false;
    bool success = true;
};

//Cell type represent one cell of Grid
//Every operator member have pointer to their center
struct Cell {
    bool is_center;
    char type;
    int size;
    int row;
    int col;
    Cell* center;
};

//Grid wraps all methods and data to execute instructions
class Grid {
    Cell** matrix;
    int num_of_rows;
    int num_of_colums;
    void Print_border_error(char type, int row, int column, int size);
    void Print_conflict_error(char type, int row, int column, int size);
    void Print_operation_success(char type, int row, int column, int size);
    void Print_move_succes(char type, int row_1, int col_1, int row_2, int col_2);
    Error Check_border_confict(char type, int row, int column, int size);
public:
    Grid(int rows,int columns); //Constructor
    Grid(const Grid& arg); //Copy constructor Move method deep copies grid to perform it's task
    ~Grid(); //destructor to deallocate matrix memory

    //Operations 
    Grid& operator=(const Grid& arg);
    friend ostream& operator<<(ostream& os, const Grid& grid) {
        for (int i = 0; i< grid.num_of_rows; i++){
            for(int j = 0; j < grid.num_of_colums; j++){
                os<<grid.matrix[i][j].type<<" ";
            }
            os<<endl;
        };
        return os;
    }

    //These add methods can be called to Remove or inside move function
    //if remove or called_by_movie are true errors are not printed
    Error Add_Remove_minus(char type, int row, int column, int size, bool Remove, bool called_by_move);
    Error Add_Remove_plus(char type, int row, int column, int size, bool Remove, bool called_by_move);
    Error Add_Remove_slash(char type, int row, int column, int size, bool Remove, bool called_by_move);
    Error Add_Remove_cross(char type, int row, int column, int size, bool Remove, bool called_by_move);

    //This method can move any type by using Add_Remove_types
    void Move(char move_type,int row, int column, int size); 
};

/**************************** Constructors ****************************/
Grid::Grid(int rows,int columns){
    num_of_colums = columns;
    num_of_rows = rows;
    matrix = new Cell*[num_of_rows];
    for(int i = 0; i < num_of_rows; ++i){
        matrix[i] = new Cell[num_of_colums];
    }
    for (int i = 0; i< num_of_rows; i++){
        for(int j = 0; j < num_of_colums; j++){
            matrix[i][j].is_center = false;
            matrix[i][j].row = i;
            matrix[i][j].col = j;
            matrix[i][j].type = '0';
            matrix[i][j].size = 0;
            matrix[i][j].center = NULL;
        }
    }
    cout<<"A grid is created:"<<num_of_rows<<" "<<num_of_colums<<endl;
}

//Copy constructor
Grid::Grid(const Grid& arg){
    num_of_rows = arg.num_of_rows;
    num_of_colums = arg.num_of_colums;
    matrix = new Cell*[num_of_rows];
    for(int i = 0; i < num_of_rows; ++i){
        matrix[i] = new Cell[num_of_colums];
    }
    for (int i = 0; i< num_of_rows; i++){
        for(int j = 0; j < num_of_colums; j++){
            matrix[i][j].is_center = arg.matrix[i][j].is_center;
            matrix[i][j].row = i;
            matrix[i][j].col = j;
            matrix[i][j].type =  arg.matrix[i][j].type;
            matrix[i][j].size = arg.matrix[i][j].size;
            matrix[i][j].center = arg.matrix[i][j].center;
        }
    }
}
//Copy assignment
Grid& Grid::operator=(const Grid& arg){
    num_of_rows = arg.num_of_rows;
    num_of_colums = arg.num_of_colums;
    Cell** m = new Cell*[num_of_rows];
    for(int i = 0; i < num_of_rows; ++i){
        m[i] = new Cell[num_of_colums];
    }
    for (int i = 0; i< num_of_rows; i++){
        for(int j = 0; j < num_of_colums; j++){
            m[i][j].is_center = arg.matrix[i][j].is_center;
            m[i][j].row = i;
            m[i][j].col = j;
            m[i][j].type =  arg.matrix[i][j].type;
            m[i][j].size = arg.matrix[i][j].size;
            m[i][j].center = arg.matrix[i][j].center;
        }
    }
    //Free old memory
    for(int i = 0; i < num_of_rows ; i++){
        delete[] matrix[i];
    }
    matrix = m;
    return *this;
}

Grid::~Grid(){
    for(int i = 0; i < num_of_rows ; i++){
        delete[] matrix[i];
    }
}


/**************************** Add methods ****************************/
Error Grid::Add_Remove_minus(char type, int row, int column, int size, bool Remove = false, bool called_by_move = false){
    Error err = Check_border_confict(type,row,column,size);
    if(Remove || err.success){
        row--;column--;
        //Create center cell
        matrix[row][column].is_center = true;
        matrix[row][column].type = (Remove ? '0' : type);
        matrix[row][column].row = row;
        matrix[row][column].col = column;
        matrix[row][column].size = size;
        matrix[row][column].center = NULL;
        //Create other cells
        for(int i = column - size; i <= column + size; i++){
            if (i == column)
                continue;
            matrix[row][i].is_center = false;
            matrix[row][i].type = (Remove ? '0' : type);
            matrix[row][i].row = row;
            matrix[row][i].col = i;
            matrix[row][i].size = size;
            matrix[row][i].center = &matrix[row][column];
        }
        if ((!Remove) && !called_by_move) {
            Print_operation_success(type,row + 1,column + 1,size);
        }
        return err;
    }
    if (called_by_move || Remove) {
        return err;
    }
    if (err.border_error) {
        Print_border_error(type, row, column, size);
    }
    if (err.conflict_error) {
        Print_conflict_error(type, row, column, size);
    }
    return err;
}


Error Grid::Add_Remove_plus(char type, int row, int column, int size, bool Remove = false, bool called_by_move = false) {
    Error err = Check_border_confict(type, row, column, size);
    if(Remove || err.success){
        row--;column--;
        //Create center cell
        matrix[row][column].is_center = true;
        matrix[row][column].type = (Remove ? '0' : type);
        matrix[row][column].row = row;
        matrix[row][column].col = column;
        matrix[row][column].size = size;
        matrix[row][column].center = NULL;
        for(int i = row - size; i <= row + size; i++){
            if (i == row)
                continue;
            matrix[i][column].is_center = false;
            matrix[i][column].type = (Remove ? '0' : type);
            matrix[i][column].size = size;
            matrix[i][column].row = i;
            matrix[i][column].col = column;
            matrix[i][column].center = &matrix[row][column];
        }
        for(int i = column - size; i <= column + size; i++){
            if (i == column)
                continue;
            matrix[row][i].is_center = false;
            matrix[row][i].type = (Remove ? '0' : type);
            matrix[row][i].size = size;
            matrix[row][i].row = row;
            matrix[row][i].col = i;
            matrix[row][i].center = &matrix[row][column];
        }
        if ((!Remove) && !called_by_move) {
            Print_operation_success(type,row + 1,column + 1,size);
        }
        return err;
    }
    if (called_by_move || Remove) {
        return err;
    }
    if (err.border_error) {
        Print_border_error(type, row, column, size);
    }
    if (err.conflict_error) {
        Print_conflict_error(type, row, column, size);
    }
    return err;     
}

Error Grid::Add_Remove_slash(char type, int row, int column, int size, bool Remove = false, bool called_by_move = false) {
    Error err = Check_border_confict(type, row, column, size);
    if(Remove || err.success){
        row--;column--;
        //Create center cell
        matrix[row][column].is_center = true;
        matrix[row][column].type = (Remove ? '0' : type);
        matrix[row][column].row = row;
        matrix[row][column].col = column;
        matrix[row][column].size = size;
        matrix[row][column].center = NULL;
        
        for (int i = row - size, j =  column + size; i <= row+size ; i++, j--){
            if (row == i)
                continue;
            matrix[i][j].is_center = false;
            matrix[i][j].type = (Remove ? '0' : type);
            matrix[i][j].row = i;
            matrix[i][j].col = j;
            matrix[i][j].size = size;
            matrix[i][j].center = &matrix[row][column];
        }
        if ((!Remove) && !called_by_move) {
            Print_operation_success(type,row + 1,column + 1,size);
        }
        return err;
    }
    if (called_by_move || Remove) {
        return err;
    }
    if (err.border_error) {
        Print_border_error(type, row, column, size);
    }
    if (err.conflict_error) {
        Print_conflict_error(type, row, column, size);
    }
    return err;
}

Error Grid::Add_Remove_cross(char type, int row, int column, int size, bool Remove = false, bool called_by_move = false){
    Error err = Check_border_confict(type, row, column, size);
    if(Remove || err.success){
        row--;column--;
        //Create center cell
        matrix[row][column].is_center = true;
        matrix[row][column].type = (Remove ? '0' : type);
        matrix[row][column].row = row;
        matrix[row][column].col = column;
        matrix[row][column].size = size;
        matrix[row][column].center = NULL;

        for (int i = row - size,j =  column + size; i <= row+size ; i++, j--){
            if (row == i)
                continue;

            matrix[i][j].is_center = false;
            matrix[i][j].type = (Remove ? '0' : type);
            matrix[i][j].size = size;
            matrix[i][j].row = i;
            matrix[i][j].col = j;
            matrix[i][j].center = &matrix[row][column];
        }
        for(int i = row - size, j = column - size; i <= row + size ; i++, j++){
            if (row == i)
                continue;
            matrix[i][j].is_center = false;
            matrix[i][j].type = (Remove ? '0' : type);
            matrix[i][j].row = i;
            matrix[i][j].col = j;
            matrix[i][j].size = size;
            matrix[i][j].center = &matrix[row][column];
        }
        if ((!Remove) && !called_by_move) {
            Print_operation_success(type,row + 1 ,column + 1,size);
        }
        return err;
    }  
    if (called_by_move || Remove) {
        return err;
    }
    if (err.border_error) {
        Print_border_error(type, row, column, size);
    }
    if (err.conflict_error) {
        Print_conflict_error(type, row, column, size);
    }
    return err;   
}

/**************************** Move method ****************************/

void Grid::Move(char move_type, int row, int column, int size){
    //every cell has pointer to their center
    //detecting center
    row--;column--;
    
    //Detect type and center of symbol
    char type = matrix[row][column].type;
    int center_row = ((matrix[row][column].is_center) ? (row) : (matrix[row][column].center->row));
    int center_col = (matrix[row][column].is_center) ? (column) : (matrix[row][column].center->col);
    int size_of_op = matrix[row][column].size;
    if(type == '0'){
        return;
    }

    //shifting logic
    int shift_row = 0;
    int shift_column = 0;
    switch(move_type){
        case 'u':
            shift_row -= size;
            break;
        case 'd':
            shift_row += size;
            break;
        case 'l':
            shift_column -= size;
            break;
        case 'r':
            shift_column += size;
            break;
        default:
            cout<<"Invalid move_type"<<endl;
    }
    int dest_center_row = shift_row + center_row;
    int dest_center_col = shift_column + center_col;

    //clone this grid and try to perform move
    Grid clone_grid = *this;
    Error err;
    switch(type){
        case '+':
            //Remove detected symbol
            clone_grid.Add_Remove_plus('+',center_row + 1,center_col + 1,size_of_op,true);
            //Try to add moved symbol
            err = clone_grid.Add_Remove_plus('+',dest_center_row + 1 ,dest_center_col + 1,size_of_op,false,true);
            break;
        case '-':
            //Remove detected symbol
            clone_grid.Add_Remove_minus('-',center_row + 1,center_col +1 ,size_of_op,true);
            //Try to add moved symbol
            err = clone_grid.Add_Remove_minus('-',dest_center_row + 1,dest_center_col + 1,size_of_op,false,true);
            break;
        case 'x':
            clone_grid.Add_Remove_cross('x',center_row + 1,center_col + 1,size_of_op,true);
            err = clone_grid.Add_Remove_cross('x',dest_center_row + 1,dest_center_col + 1,size_of_op,false,true);
            break;
        case '/':
            clone_grid.Add_Remove_slash('/',center_row + 1,center_col + 1,size_of_op,true);
            err = clone_grid.Add_Remove_slash('/',dest_center_row + 1,dest_center_col + 1,size_of_op,false,true);
            break;
    }
    if (err.success) {
        *this = clone_grid;
        Print_move_succes(type,center_row + 1,center_col + 1,dest_center_row + 1,dest_center_col + 1);
        return;
    }
    if (err.border_error) {
        cout<<"BORDER ERROR: "<<type<<" can not be moved from "
            <<"("<<center_row+1<<","<<center_col+1<<")"<<" to "
            <<"("<<dest_center_row+1<<","<<dest_center_col+1<<")."<<endl;
    }
    if (err.conflict_error) {
        cout<<"CONFLICT ERROR: "<<type<<" can not be moved from "
            <<"("<<center_row+1<<","<<center_col+1<<")"<<" to "
            <<"("<<dest_center_row+1<<","<<dest_center_col+1<<")."<<endl;

    }
}

/**************************** Private methods ****************************/
void Grid::Print_move_succes(char type, int row_1, int col_1, int row_2, int col_2) {
    cout<<"SUCCESS: "<<type<<" moved from "<<"("<<row_1<<","<<col_1<<")"
        <<" to "<<"("<<row_2<<","<<col_2<<")."<<endl;
}
void Grid::Print_operation_success(char type, int row, int column, int size){
    cout<<"SUCCESS: Operator "<<type<<" with size "<<size
        <<" is placed on "<<"("<<row<<","<<column<<")."<<endl;
}

void Grid::Print_border_error(char type, int row, int column, int size) {
    cout<<"BORDER ERROR: Operator "<<type<<" with size "
        <<size<<" can not be placed on ("<<row<<","<<column<<")."<<endl;
}

void Grid::Print_conflict_error(char type, int row, int column, int size) {
    cout<<"CONFLICT ERROR: Operator "<<type<<" with size "
        <<size<<" can not be placed on ("<<row<<","<<column<<")."<<endl;
}


Error Grid::Check_border_confict(char type, int row, int column, int size){
    Error err;
    --row; --column; //inner representation indices start from 0
    switch(type){
        case '+':
            //check for conflict and border
            for(int i = row - size; i <= row + size; i++){
                if((i < num_of_rows && column < num_of_colums) && (i >= 0 && column >= 0)) {
                    if (matrix[i][column].type != '0')  {
                        err.conflict_error = true;
                        break;
                    }
                } else {
                    err.border_error = true;
                }
            }
            //check for conflict and border
            for(int i = column - size; i <= column + size; i++){
                if((row < num_of_rows && i < num_of_colums) && (row >= 0 && i >= 0)) {
                    if (matrix[row][i].type != '0') {
                        err.conflict_error = true;
                        break;
                    }
                } else {
                    err.border_error = true;
                } 
            }
            break;
        case '-':
            //check for conflict and border
            for(int i = column - size; i <= column + size; i++){
                if((row < num_of_rows && i < num_of_colums) && (row >= 0 && i >= 0)) {
                    if (matrix[row][i].type != '0') {
                        err.conflict_error = true;
                        break;
                    }
                } else {
                    err.border_error = true;
                }
            }
            break;
        case '/':
            //check for conflict and border
            for (int i = row - size, j =  column + size; i <= row+size ; i++, j--){
                if((i < num_of_rows && j < num_of_colums) && (i >= 0 && j >= 0)) {
                    if (matrix[i][j].type != '0') {
                        err.conflict_error = true;
                        break;
                    }
                } else {
                    err.border_error = true;
                }
            }
            break;
        case 'x':
            //check for conflict and border
            for (int i = row - size,j =  column + size; i <= row+size ; i++, j--){
                if((i < num_of_rows && j < num_of_colums) && (i >= 0 && j >= 0)){
                    if (matrix[i][j].type != '0') {
                        err.conflict_error = true;
                        break;
                    }
                } else {
                    err.border_error = true;
                }   
            }
            //check for conflict and border
            for(int i = row - size, j = column - size; i <= row + size ; i++, j++){
                if((i < num_of_rows && j < num_of_colums) && (i >= 0 && j >= 0)){
                    if (matrix[i][j].type != '0') {
                        err.conflict_error = true;
                        break;
                    }
                }else {
                    err.border_error = true;
                }  
            }
            break;
    }
    if (err.conflict_error == true ||err.border_error == true){
        err.success = false;
    }
    return err;
}