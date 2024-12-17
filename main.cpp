#include <iostream>
#include <string>
using namespace std;

class display{
    private:
        int rows;
        int cols;
    public:
        display();
        int get_rows();
        int get_cols();
};

int main(){
    display* displayOne = new display();

}

int display::get_rows()
{
}

int display::get_cols()
{
}

display::display()
{
    rows = 0;
    cols = 0;
}
