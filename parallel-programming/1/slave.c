#include <pvm3.h>
#include <stdio.h>

int main() {
	printf("i'm %d\n", pvm_mytid());
	pvm_initsend(PvmDataDefault);
	pvm_recv(pvm_parent(),13);
	pvm_barrier("slaves",BLADES);
	printf("Received message from parent %d\n", pvm_parent());
	pvm_exit();
}
