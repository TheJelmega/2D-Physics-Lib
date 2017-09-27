#include "TestEnvironment.h"
#include "Physics2D.h"

int main(int argc, char* argv[])
{
	(void)argc;
	(void)argv;

	P2D::BlockAllocator alloc;

	i32* i0 = (i32*)alloc.Allocate(sizeof(i32));
	alloc.Deallocate(i0, sizeof(i32));
	i0 = (i32*)alloc.Allocate(sizeof(i32));
	i32* i1 = (i32*)alloc.Allocate(sizeof(i32));
	alloc.Deallocate(i0, sizeof(i32));
	alloc.Deallocate(i1, sizeof(i32));


	TestEnvironment testEnvironment;
	testEnvironment.Run();
	return 0;
}
