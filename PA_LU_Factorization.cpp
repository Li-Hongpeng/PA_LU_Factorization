#include <iostream>
#include<iomanip>
using namespace std;
double Abs(double n) //对输入数字返回其绝对值
{
    if (n >= 0) return n;
    else return -1 * n;
}
void printMatrix(double** A,int* p,int size) //输出矩阵最后运算结果，P，L，U矩阵的值。
{
    cout << "The matrix after factorization is:" << endl;  //依据p中记录的行顺序输出矩阵
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++) 
            cout << setw(10) << A[p[i]][j];
        cout << endl;
    }
    cout << "Row interchange p is:" << endl;  //输出P矩阵
    for (int i = 0; i < size; i++)
    {
        cout <<setw(10)<<p[i]+1 << endl;
    }
    cout << "L=" << endl;  //输出L矩阵
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
            if (i == j) cout << setw(10) << 1;
            else if (i > j) cout << setw(10) << A[p[i]][j];
            else cout << setw(10)<<0;
        cout << endl;
    }
    cout << "U=" << endl;  //输出U矩阵
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
            if (i == j) cout << setw(10) << A[p[i]][j];
            else if (i > j) cout << setw(10) << 0;
            else cout << setw(10) << A[p[i]][j];
        cout << endl;
    }

}
void pivoting(double** A,int* p, int row, int column,int size)  //partial pivoting，对矩阵A中特定行列元素进行比较，交换最大元素所在行到第一行，记录在p矩阵中
{
    int pivot_row = p[row];                                     //假设pivot在第一行
    double pivot = Abs(A[pivot_row][column]);                   //取pivot绝对值
    int max_index_in_p = row;                                   //记录pivot所在的当前行
    for (int i = row+1; i < size; i++)
    {
        if (Abs(A[p[i]][column]) > pivot)                       //逐行比较，若存在比原pivot值大的某行，记录其元素值和所在行数
        { 
            max_index_in_p = i;
            pivot_row = p[i];
            pivot = Abs(A[p[i]][column]);
        }
    }
    if (pivot_row != p[row])                                    //交换第一行和pivot所在行，记录在p矩阵中
    {
        int temp = p[row];
        p[row] = pivot_row;
        p[max_index_in_p] = temp;
    }
}
void typeIII(double** A,int* p,int row,int column,int size)    //typeIII行变换操作，同时将系数记录在下三角元素中
{
    double coefficient = 0;                                    //初始化系数为0
    for (int i = row + 1 ; i < size; i++)                      //从第二行开始，对每一行进行typeIII操作
    {
        if(A[p[row]][column]!=0) coefficient = A[p[i]][column] / double(A[p[row]][column]);//若pivot不等于0，更新系数值
        A[p[i]][column] = coefficient;                         //记录系数值于下三角元素中
        for (int j = column + 1; j < size; j++)                //每一行剩下的元素进行typeIII操作
        {
            A[p[i]][j] = A[p[i]][j] - coefficient * A[p[row]][j];
        }
    }

}
bool isNonsingular(double** A, int* p, int size)               //用对角线乘积判断矩阵是否非奇异，是返true，否返回false
{
    double product = 1;
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
            if (i == j) product *= A[p[i]][j];
    }
    if (product == 0) return false;
    else return true;
}
int main()
{
    //输出程序信息，数据示例
    cout << "Author:LiHongpeng StudentID:202028002829003" << endl<<endl;
    cout << "This program can do LU factorization with row interchange,namely PA LU factorization.\n";
    cout << "It takes two inputs, the size of matrix and entries of matrix by rows.\nFor example: a 4*4 matrix:\n";
    cout << setw(4) << 1 << setw(4) << 2 << setw(4) << 4 << setw(4) << 17<<endl
        << setw(4) << 3 << setw(4) << 6 << setw(4) << -12 << setw(4) << 3 << endl
        << setw(4) << 2 << setw(4) << 3 << setw(4) << -3 << setw(4) << 2 << endl
        <<setw(4) << 0 << setw(4) << 2 << setw(4) << -2 << setw(4) << 6 << endl;
    cout << "First input size of the matrix 4 and press Enter." << endl << "Then by row input data 1 2 4 17 and press Enter to input next row..."<< endl;
    for (int i = 0; i < 80; i++) cout << "-";
    cout << endl;

    //程序入口
    int quit = 2;
    while (quit==2)
    {
        int matrix_size = 0;  //输入矩阵大小，非法数值会无效并提示重新输入
        while (true)
        {
            cout << "Input the size of your matrix:" << endl;
            cin >> matrix_size;
            if (matrix_size > 0)
            {
                break;
            }
            else
            {
                cout << "Invalid data,please input correct size" << endl;
            }
        }
        //初始化P矩阵，初始值为按顺序的0,1,2,3,...，长度等于矩阵行数
        int* p = new int[matrix_size];
        for (int i = 0; i < matrix_size; i++)
            p[i] = i;
        //初始化矩阵，按行输入矩阵数据
        double** A = new double* [matrix_size];
        for (int i = 0; i < matrix_size; i++)
        {
            cout << "Input the " << i + 1 << " row of matrix,divided by space." << endl;
            A[i] = new double[matrix_size];
            double value = 0;
            int j = 0;
            while (j<matrix_size and cin >> value)
            {
                A[i][j] = value;
                j++;
            }
        }
        //共进行matrix_size-1步计算操作，得到最终结果
        int step = 0;
        while (step < matrix_size - 1)
        {
            pivoting(A, p, step, step, matrix_size);
            typeIII(A, p, step, step, matrix_size);
            step++;
        }
        if (!isNonsingular(A, p, matrix_size))  //判断最后结果是否为非奇异矩阵
            cout << "It is not a nonsingular matrix, cannot perform LU factorization." << endl;
        else 
            printMatrix(A, p, matrix_size);    //输出结果
        cout << "Input number to continue:\n1.Exit  2.Restart" << endl;
        cin >> quit;
    }
    return 0;
}