#include <iostream>
#include <vector>
#include<string>
#include "rapidcsv.h"
#include "directoryServer.h"

using namespace std;
int main()
{
	directoryServer ds;
	string username = "ahmed";
	string password = "aaa";
	string username2 = "wael";
	string password2 = "www";
	string imagname1 = "myImage1";
	string imagname2 = "myImage2";
	string imagname3 = "myImage3";



	ds.signup(username, password, &ds);
	ds.signup(username, password, &ds);
	ds.signup(username2, password2, &ds);
	ds.login(username, password,&ds);
	ds.logout(username, &ds);
	ds.login(username, password, &ds);
	ds.login(username2, password2, &ds);
	ds.logout(username, &ds);
	ds.uploadimage(username, imagname1, &ds);
	ds.uploadimage(username, imagname2, &ds);
	ds.uploadimage(username, imagname3, &ds);
	ds.uploadimage(username2, imagname3, &ds);

	return 0;
}
