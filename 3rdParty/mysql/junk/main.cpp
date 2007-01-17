#include "Database.h"


int
main(int argc, char *argv[])
{
	Database databaseTable("localhost","cpp_user","cpp_test","mysql_cpp_data");
	databaseTable.readTable("select * from stock");
	databaseTable.display();
}
