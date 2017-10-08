//неудачная попытка написать часть ядра на C

void clear();
void curToZero();
void print(char* msg);
void pause();

void main(void) {
	clear();
	curToZero();
	print("TEST!");
	pause();
}

void pause() {
	asm(
		"mov $0x10, %ah\n\t"
		"int $0x16\n\t"
	);
}

void clear() {
	asm (
		"mov $0x0002, %ax\n\t"
		"int $0x10\n\t"
	);
}

void curToZero() {
	asm (
		"xor %dx, %dx\n\t"
		"mov $0x02, %ah\n\t"
		"xor %bh, %bh\n\t"
		"int $0x10\n\t"
	);
}

void print(char* msg) {
	asm (
		"mov %0, %%ebp\n\t"
		"mov $0x0005, %%cx\n\t"
		"mov $0x02, %%bl\n\t"
		"mov $0x1301, %%ax\n\t"
		"int $0x10\n\t"
		:		
		:"r"(msg)
	);
}
