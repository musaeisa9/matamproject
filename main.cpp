/*
 * main.cpp
 *
 *  Created on: Aug 5, 2020
 *      Author: JLAL
 */
#include"gcalc.h"
int main(int argc, char** argv){
    if (!(argc == 1 || argc == 3)){
        cerr<<"bad arguments to Gcalc";
        return 0;
    }
    if(argc == 1){
        calculator(cout,cin,"Gcalc> ");
    }
    if (argc == 3){
        std::filebuf ft;
        ft.open (argv[1],std::ios::in);
        istream is(&ft);
        std::filebuf fb;
        fb.open (argv[2],std::ios::out);
        ostream os(&fb);
        calculator(os,is,"");
        ft.close();
        fb.close();
    }
	return 0;
}
