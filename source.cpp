#include <iostream>
#include "viewer.hpp"

int main(){
    Viewer viewer;
    viewer.init();

    while(viewer.update()){}

	return 0;
}
