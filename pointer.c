#include <stdio.h>
#include <stdlib.h>

// Typedef for a struct
typedef struct {
    char name[80];
    int age;
    float percentage;
} student;



typedef struct {
    int x;
    int y;
} point;

int main(void) {

	

//	point p1= {0,1};
//	printf("%p, %p\n", &p1.x, &p1.y);

// Pointer to memory of struct with members x and y
	point *p;
	
// Go to the memory at x and y and set them with different values

	p->x = 1;
	p->y = 2;
	printf("%d, %d\n", p->x, p->y); 	// The arrow operator exists to acsess the members of the strructure or the unions using pointers	

	
	int x = 7;
	printf("x is %d\n", x);

	x = 14;
	printf("x is %d after redeclaring variable\n", x);

	int *x_p = &x;				// Initialize pointer to address of x using &
	printf("The memory where x is stored, x_p, is %d\n", x_p);	

	printf("x is %d after dereferencing x_p with *\n", *x_p);	// Dereference x using *, i.e get value of x

	*x_p = 21; 					// Dereference x, change value to 21

	printf("x is %d, x_p is %d after dereferencing and redeclaring dereferenced variable x_p\n", x, *x_p);

	func(&x_p);
	func2(&x);
	
	return 0;
}

int func(int **x_p) {

	*x_p = 20;

	printf("%d\n", *x_p);

}

int func2( int *p) {

	*p = 5;
	printf("%d\n", *p);
}





