#include <iostream>

/**
    \file Hello.cc
    An overly extended example of how to use breathe
*/

/*!
    With a little bit of a elaboration, should you feel it necessary.
*/
class Hello
{
public:
	//! Hello Constructor
	Hello(){};
	~Hello(){};

	/*! Say whatever you want
	 
 	    \param message - the message to say
	*/
	SayIt(){};
	SayBye(){};

	//! state variable
	int state;

	char * message;



};


/*! \brief Brief description.
 *         Brief description continued.
 *
 *  Detailed description starts here.
 */

int main(void){


	std::cout << "HELLO WORLD" << std::endl;
	return 0;

}
