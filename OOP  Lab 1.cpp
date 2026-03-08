// OOP  Lab 1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "rect.h"
#include "barrel.h"
#include "matrix.h"
#include "my_string.h"
#include "text_wrapper.h"

using namespace std;

int main()
{
   
    // Task 1.2 - Implicit constructors and destructors
    //
    // When the block below runs, the compiler-generated default constructor
    // and destructor are called for 'rect'.
    // The implicit default constructor does nothing for built-in types
    // (fields contain garbage), but calls the default constructor for any
    // field that is itself a class.
    // The implicit destructor mirrors this: it does nothing for built-in fields
    // and calls the destructor of any field that is a class.
    
    {
        Rect rect;
        // destructor fires here, when 'rect' goes out of scope
    }

    
    // Task 1.3 - Explicit constructors
    //
    // Once we added Rect(int, int, int, int), the compiler stopped generating
    // a default constructor automatically.  That would have made `Rect rect;`
    // above a compile error, so we explicitly added the no-argument constructor.
    
    {
        Rect rect(1, 2, 3, 4);
        // destructor fires here
    }

    
    // Task 1.4 - Copy constructor
    //
    // Both lines below call the copy constructor (we wrote it explicitly in
    // Task 1.6 - before that the compiler generated one).
    // The implicit copy constructor copies every field one-by-one.
    // For Rect this is safe because we own no heap memory.
    //
    //   Rect rect2 = rect1;  -- copy-initialisation syntax
    //   Rect rect3(rect1);   -- direct-initialisation syntax
    //                           (same result, different syntax)
    
    {
        Rect rect1(1, 2, 3, 4);
        Rect rect2 = rect1;   // copy constructor
        Rect rect3(rect1);    // copy constructor (alternative syntax)
    }

    
    // Task 1.5 - Destructor
    //
    // A destructor is a special method called automatically when an object is
    // destroyed.  It has no return type and its name is ~ClassName().
    //
    //   - Stack objects: destructor runs when execution leaves the enclosing { }
    //   - Heap objects:  destructor runs only when you call `delete`
    //
    // Rect's destructor does nothing useful (no heap memory to free), but we
    // added a debug print so we can observe when it is called.
    

    
    // Task 1.6 - Counting constructor and destructor calls
    //
    //   Rect r1;              -> default ctor  r1                (total: 1 ctor)
    //   new Rect(1,2,1,2)     -> 4-param ctor  *pR               (total: 2 ctor)
    //   Rect r2(r1)           -> copy ctor     r2                (total: 3 ctor)
    //   Rect arRect[2]        -> default ctor  x2                (total: 5 ctor)
    //   Loop 3 iterations:
    //     static r3           -> 4-param ctor  once (i==0 only)  (total: 6 ctor)
    //     r4(*pR)             -> copy ctor     x3                (total: 9 ctor)
    //     r5(i,i,i,i)         -> 4-param ctor  x3                (total: 12 ctor)
    //     r4, r5 destroyed at end of each iteration -> 6 dtors   (total: 6 dtor)
    //   End of inner block: ~r2, ~arRect[1], ~arRect[0]          (total: 9 dtor)
    //   delete pR             -> ~*pR                            (total: 10 dtor)
    //   End of outer block:   -> ~r1                             (total: 11 dtor)
    //   Program exit:         -> ~r3 (static lifetime)           (total: 12 dtor)
    //
    // Grand total: 12 constructors, 12 destructors.
    
    {
        Rect r1;                          // default ctor: r1
        Rect* pR = new Rect(1, 2, 1, 2); // 4-param ctor: *pR
        {
            Rect r2(r1);                  // copy ctor: r2
            Rect arRect[2];               // default ctor x2: arRect[0], arRect[1]
            for (int i = 0; i < 3; i++)
            {
                static Rect r3(i, i, i, i); // 4-param ctor ONCE (i==0), then skipped
                Rect r4(*pR);               // copy ctor on every iteration
                Rect r5(i, i, i, i);        // 4-param ctor on every iteration
                // r5 and r4 are destroyed here (end of iteration)
            }
            // r2, arRect[1], arRect[0] are destroyed here (end of inner block)
        }
        delete pR;   // destructor for *pR
        // r1 is destroyed here (end of outer block)
    }
    // r3 (static) is destroyed here when the program exits


    
    // Task 1.7 - Private fields
    //
    // With all fields declared private, the line below would not compile:
    //     std::cout << r1.x;  // error: 'x' is private within this context
    //
    // Direct access to private members is forbidden outside the class.
    // This is intentionally left commented out to show what the error would be.
    
    // {
    //     Rect r1;
    //     cout << r1.x << std::endl;  // compile error: x is private
    // }


   
    // Task 1.8 - Encapsulation and accessor methods
    //
    // A member function (method) differs from a plain function in two ways:
    //   It belongs to a class and is called via an object: r.get_left()
    //   It has an implicit 'this' pointer giving it access to private fields
    // 
    // A plain function has no such implicit access; data must be passed explicitly.
   
    {
        Rect r;
        cout << r.get_left() << "; " << r.get_right() << '\n';
        r.set_all(1, 2, 3, 4);
        cout << r.get_top() << "; " << r.get_bottom() << '\n';
    }

    

    // Task 1.10 - Passing objects to and from functions
    //
    // bounding_rect(r1, r2): r1 and r2 are passed BY VALUE, so the copy
    // constructor is called for each parameter.
    // print_rect(r3): r3 is passed BY REFERENCE, so no copy is made.
    

    {
        Rect r1(0, 5, 0, 3);
        Rect r2(2, 8, -1, 4);
        Rect r3 = bounding_rect(r1, r2);
        print_rect(r3);
    }

    

    // Task 1.11 - Computed properties: width, height, area
   

    {
        Rect r(0, 10, 0, 5);
        cout << "width=" << r.get_width()
            << " height=" << r.get_height()
            << " area=" << r.get_square() << '\n';

        // set_width /set_height preserve the bottom-left corner
        r.set_width(20);
        r.set_height(10);
        cout << "after resize: ";
        print_rect(r);
    }

   

    // Task 2.1 - Barrel: object as a domain entity
    //
    // A 10-litre barrel of 96% alcohol and a 10-litre barrel of pure water.
    // Each iteration: scoop 1L from the water barrel into the alcohol barrel,
    // then scoop 1L from the alcohol barrel into the water barrel.
    // Find the iteration at which the alcohol concentration drops below 50%.
    

    {
        cout << "\n--- Task 2.1: barrels ---\n";

        Barrel alch(10.0, 0.96);  // 10 L, 96% alcohol
        Barrel water(10.0, 0.0);  // 10 L, pure water

        int iteration = 0;
        while (alch.get_concentration() > 0.5) {
            iteration++;
            alch.pour_from(water, 1.0);   // scoop from water into alcohol barrel
            water.pour_from(alch, 1.0);   // scoop from alcohol barrel into water barrel
        }
        cout << "Alcohol concentration dropped below 50% at iteration: "
            << iteration << '\n';
    }

    

    // Task 2.2 - Matrix: object as a mathematical entity
    //
    // Fibonacci numbers via matrix exponentiation:
    //
    //   [ F(n+1) ]   [ 1 1 ]^n   [ F(1) ]
    //   [ F(n)   ] = [ 1 0 ]   * [ F(0) ]
    //
    // Starting from the identity matrix and multiplying by the base matrix
    // n times gives us F(n) at position [1][0].
    

    {
        cout << "\n--- Task 2.2: Fibonacci numbers via matrix multiplication ---\n";

        // Base matrix [[1,1],[1,0]]
        Matrix base(2, 2, 0.0);
        base.set(0, 0, 1); base.set(0, 1, 1);
        base.set(1, 0, 1); base.set(1, 1, 0);

        // Compute F(10)
        int n = 10;
        Matrix result(2);       // start with 2x2 identity
        Matrix cur(base);
        for (int i = 0; i < n; i++)
            result = result.multiply(cur);

        cout << "F(" << n << ") = " << (int)result.get(1, 0) << '\n';

        // Print the first 10 Fibonacci numbers
        cout << "First 10 Fibonacci numbers: ";
        Matrix acc(2);          // identity
        Matrix b(base);
        for (int i = 1; i <= 10; i++) {
            acc = acc.multiply(b);
            cout << (int)acc.get(1, 0);
            if (i < 10) cout << ", ";
        }
        cout << '\n';
    }

    
    // Task 2.3 - MyString: object as a resource owner
    
    {
        cout << "\n--- Task 2.3: MyString ---\n";

        MyString s1;                     // empty string
        s1.print(); cout << " (empty)\n";

        MyString s2("I am Bat Man!");
        s2.print(); cout << '\n';

        MyString s3(s2);                 // deep copy - s3 owns its own memory
        s3.set(0, 'h');                  // modifying s3 must not affect s2
        s3.print(); cout << '\n';
        s2.print(); cout << " (unchanged)\n";

        s1.set_new_string("new content"); // frees old buffer, allocates new one
        s1.print(); cout << '\n';
    }

    // Verify that copy semantics work correctly in various scenarios
    {
        MyString s1;
        MyString s2 = s1;                // copy of empty string
        MyString s3("This is my string");
        MyString s4 = s3;                // copy of non-empty string
        // All four destructors must fire cleanly here with no double-free
    }

    
    // Task 2.4 - TextWrapper: object as an algorithm
    // (interactive read_line() is commented out so automated tests can run)
    
    {
        cout << "\n--- Task 2.4: TextWrapper ---\n";
        int line_width = 40;
        MyString input("Hello world this is Bat Man");
        TextWrapper wrapper(input, line_width);
        wrapper.print_wrapped();
    }

    return 0;
}

